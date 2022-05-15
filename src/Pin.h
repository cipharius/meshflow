#ifndef PIN_H
#define PIN_H

#include <string>
#include <mutex>
#include <condition_variable>

#include <imgui_node_editor.h>

#include "RuntimeType.h"
#include "Link.h"

namespace NodeEditor = ax::NodeEditor;

class GenericPin {
  public:
    static GenericPin* from(NodeEditor::PinId pinId);

    virtual ~GenericPin() = 0;

    NodeEditor::PinId id();
    NodeEditor::NodeId parent_id();
    NodeEditor::PinKind kind();
    std::string name();
    RuntimeType* type();
    std::shared_ptr<Link> link();
    virtual const char* type_name() = 0;
    virtual bool can_bind(GenericPin* otherGeneric) = 0;
    virtual void bind(GenericPin* otherGeneric) = 0;
    virtual void unbind() = 0;
    virtual void close() = 0;

    ImVec2 calc_size();
    void render(int offset = 0);

  protected:
    GenericPin(NodeEditor::NodeId parentId, std::string name, RuntimeType* type, NodeEditor::PinKind kind);

    NodeEditor::NodeId _parentId;
    std::string _name;
    NodeEditor::PinKind _kind;
    NodeEditor::PinId _id;
    std::shared_ptr<Link> _link;

  private:
    RuntimeType* _type;
};

template <class RT>
class Pin : public GenericPin {
  public:
    constexpr static Pin<RT>* cast(GenericPin* generic) {
      if (generic->type_name() != RT::static_type_name()) return nullptr;
      return static_cast<Pin<RT>*>(generic);
    }

    constexpr Pin(NodeEditor::NodeId parentId, std::string name, RT* type, NodeEditor::PinKind kind)
    : GenericPin(parentId, name, type, kind), _connectedPin(nullptr), _hasChanged(false), _isClosed(false) {
      _id = NodeEditor::PinId(this);
    }

    ~Pin() {
      this->unbind();
    }

    constexpr RT* type() { return static_cast<RT*>( GenericPin::type() ); }
    constexpr const char* type_name() { return GenericPin::type()->type_name(); }

    bool can_bind(GenericPin* otherGeneric) override {
      if (_isClosed) return false;
      if (!otherGeneric) return false;
      if (_connectedPin == otherGeneric) return false;
      if (this->parent_id() == otherGeneric->parent_id()) return false;
      if (this->kind() == otherGeneric->kind()) return false;
      if (this->type_name() != otherGeneric->type_name()) return false;
      return true;
    }

    void bind(GenericPin* otherGeneric) override {
      if (!this->can_bind(otherGeneric)) return;

      Pin<RT>* other = Pin<RT>::cast(otherGeneric);
      if (!other) return;

      NodeEditor::PinId inputPinId, outputPinId = 0;

      if (this->kind() == NodeEditor::PinKind::Output) {
        outputPinId = this->id();
        inputPinId = other->id();
      } else {
        outputPinId = other->id();
        inputPinId = this->id();
      }

      this->unbind();
      other->unbind();

      std::unique_lock lock(_mutex);
      std::unique_lock other_lock(other->_mutex);

      _connectedPin = other;

      _link = std::shared_ptr<Link>(new Link(inputPinId, outputPinId));
      other->_link = _link;
      other->_connectedPin = this;

      if (this->kind() == NodeEditor::PinKind::Input) {
        this->_hasChanged = true;
        this->_onUpdate.notify_all();
      } else {
        other->_hasChanged = true;
        other->_onUpdate.notify_all();
      }
    }

    void unbind() override {
      std::unique_lock lock(_mutex);
      if (!_connectedPin) return;
      std::unique_lock other_lock(_connectedPin->_mutex);

      _connectedPin->_link.reset();
      _link.reset();

      auto* other = _connectedPin;
      _connectedPin->_connectedPin = nullptr;
      _connectedPin = nullptr;

      if (this->kind() == NodeEditor::PinKind::Input) {
        _hasChanged = true;
        _onUpdate.notify_all();
      } else {
        other->_hasChanged = true;
        other->_onUpdate.notify_all();
      }
    }

    void close() override {
      _isClosed = true;
      _hasChanged = true;
      this->type()->value.reset();
      _onUpdate.notify_all();
    }

    void write(std::shared_ptr<typename RT::type> value) {
      if (_isClosed) return;
      if (_kind == NodeEditor::PinKind::Input) return;

      std::unique_lock lock(_mutex);
      if (value && this->type()->value && *value == *(this->type()->value)) return;
      this->type()->value = value;

      if (_connectedPin) {
        std::unique_lock other_lock(_connectedPin->_mutex);
        _connectedPin->_hasChanged = true;
        _connectedPin->_onUpdate.notify_all();
      }
    }

    std::shared_ptr<typename RT::type> read() {
      if (_isClosed) return std::shared_ptr<typename RT::type>();
      if (_kind == NodeEditor::PinKind::Output) return std::shared_ptr<typename RT::type>();

      std::unique_lock lock(_mutex);
      if (!_hasChanged) {
        _onUpdate.wait(lock);
      }

      if (!_connectedPin) {
        _hasChanged = false;
        return std::shared_ptr<typename RT::type>();
      }

      std::unique_lock other_lock(_connectedPin->_mutex);
      _hasChanged = false;
      return _connectedPin->type()->value;
    }

    Pin(const Pin<RT>&) = delete;

  protected:
    Pin<RT>* _connectedPin;
    std::mutex _mutex;
    std::condition_variable _onUpdate;
    bool _hasChanged;
    bool _isClosed;
};

#endif
