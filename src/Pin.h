#ifndef PIN_H
#define PIN_H

#include <string>
#include <vector>
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
    std::vector<std::shared_ptr<Link>> links();
    virtual const char* type_name() = 0;
    virtual bool can_bind(GenericPin* otherGeneric) = 0;
    virtual void bind(GenericPin* otherGeneric) = 0;
    virtual void unbind(GenericPin* otherGeneric) = 0;
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
    std::vector<std::shared_ptr<Link>> _links;

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

    constexpr Pin(NodeEditor::NodeId parentId, std::string name, RT* type, NodeEditor::PinKind kind, std::condition_variable* onInput)
    : GenericPin(parentId, name, type, kind), _onInput(onInput), _isClosed(false) {
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
      if (this->parent_id() == otherGeneric->parent_id()) return false;
      if (this->kind() == otherGeneric->kind()) return false;
      if (this->type_name() != otherGeneric->type_name()) return false;

      for (auto* connectedPin : _connectedPins) {
        if (connectedPin == otherGeneric) return false;
      }

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

        other->unbind();
      } else {
        outputPinId = other->id();
        inputPinId = this->id();

        this->unbind();
      }

      std::unique_lock lock(_mutex);
      std::unique_lock other_lock(other->_mutex);

      auto link = std::shared_ptr<Link>(new Link(outputPinId, inputPinId));
      _links.push_back(link);
      _connectedPins.push_back(other);

      other->_links.push_back(link);
      other->_connectedPins.push_back(this);

      if (this->kind() == NodeEditor::PinKind::Input) {
        this->_onInput->notify_all();
      } else {
        other->_onInput->notify_all();
      }
      link->activity_animation();
    }

    void unbind(GenericPin* otherGeneric) override {
      std::unique_lock lock(_mutex);

      int pinIndex = -1;
      for (unsigned long int i = 0; i < _connectedPins.size(); i++) {
        if (_connectedPins[i] == otherGeneric) {
          pinIndex = i;
          break;
        }
      }
      if (pinIndex == -1) return;

      Pin<RT>* other = Pin<RT>::cast(otherGeneric);
      if (!other) return;

      std::unique_lock other_lock(other->_mutex);

      std::shared_ptr<Link> link = _links[pinIndex];
      for (unsigned long int i = 0; i < other->_links.size(); i++) {
        if (link == other->_links[i]) {
          other->_links.erase(other->_links.begin() + i);
          other->_connectedPins.erase(other->_connectedPins.begin() + i);
          break;
        }
      }
      _links.erase(_links.begin() + pinIndex);
      _connectedPins.erase(_connectedPins.begin() + pinIndex);
      link.reset();

      if (this->kind() == NodeEditor::PinKind::Input) {
        _onInput->notify_all();
      } else {
        other->_onInput->notify_all();
      }
    }

    void unbind() override {
      auto connectedPinsCopy = _connectedPins;
      for (auto* connectedPin : connectedPinsCopy) {
        this->unbind(connectedPin);
      }
    }

    void close() override {
      _isClosed = true;
      this->type()->value.reset();
      _onInput->notify_all();
    }

    void write(std::shared_ptr<typename RT::type> value) {
      if (_isClosed) return;
      if (_kind == NodeEditor::PinKind::Input) return;

      std::unique_lock lock(_mutex);
      if (value && this->type()->value && value == this->type()->value) return;
      this->type()->value = value;

      for (unsigned long int i = 0; i < _connectedPins.size(); i++) {
        _connectedPins[i]->_onInput->notify_all();
        _links[i]->activity_animation();
      }
    }

    std::shared_ptr<typename RT::type> read() {
      if (_isClosed) return std::shared_ptr<typename RT::type>();
      if (_kind == NodeEditor::PinKind::Output) return std::shared_ptr<typename RT::type>();

      std::unique_lock lock(_mutex);
      if (_connectedPins.empty()) {
        return std::shared_ptr<typename RT::type>();
      }

      auto* connectedPin = _connectedPins[0];
      std::unique_lock other_lock(connectedPin->_mutex);
      return connectedPin->type()->value;
    }

    Pin(const Pin<RT>&) = delete;

  protected:
    std::vector<Pin<RT>*> _connectedPins;
    std::mutex _mutex;
    std::condition_variable* _onInput;
    bool _isClosed;
};

#endif
