#ifndef PIN_H
#define PIN_H

#include <string>
#include <imgui_node_editor.h>

#include "RuntimeType.h"
#include "Channel.h"
#include "Link.h"

#include <iostream>

template <class T>
using ChannelSharedPtr = std::shared_ptr<Channel<T>>;

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
    virtual void bind(GenericPin* otherGeneric) = 0;
    virtual void unbind() = 0;

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
    : GenericPin(parentId, name, type, kind) {
      _id = NodeEditor::PinId(this);
    }

    ~Pin() {
      this->unbind();
    }

    constexpr RT* type() { return static_cast<RT*>( GenericPin::type() ); }
    constexpr const char* type_name() { return GenericPin::type()->type_name(); }

    void bind(GenericPin* otherGeneric) override {
      if (this->parent_id() == otherGeneric->parent_id()) return;

      Pin<RT>* other = Pin<RT>::cast(otherGeneric);
      if (!other || this->kind() == other->kind()) return;

      NodeEditor::PinId inputPinId, outputPinId = 0;

      if (this->kind() == NodeEditor::PinKind::Output) {
        if (_link && _link->inputPinId() == other->id()) return;

        outputPinId = this->id();
        inputPinId = other->id();
      } else {
        if (_link && _link->outputPinId() == other->id()) return;

        outputPinId = other->id();
        inputPinId = this->id();
      }
      this->unbind();

      _channel = ChannelSharedPtr<typename RT::type>(new Channel<typename RT::type>());
      _link = std::shared_ptr<Link>(new Link(inputPinId, outputPinId));
      other->_channel = _channel;
      other->_link = _link;

      if (this->kind() == NodeEditor::PinKind::Output) {
        _channel->push(this->type()->value);
      } else {
        _channel->push(other->type()->value);
      }
    }

    void unbind() override {
      if (!_link) return;

      GenericPin* otherGeneric;
      if (this->id() == _link->outputPinId()) {
        otherGeneric = GenericPin::from(_link->inputPinId());
      } else {
        otherGeneric = GenericPin::from(_link->outputPinId());
      }

      if (auto* other = Pin<RT>::cast(otherGeneric)) {
        other->_channel.reset();
        other->_link.reset();
      }

      _channel.reset();
      _link.reset();
    }

    void write(std::shared_ptr<typename RT::type> value) {
      this->type()->value = value;
      if (!_channel) return;
      _channel->push(value);
    }

    std::shared_ptr<typename RT::type> read() {
      if (!_channel) return this->type()->value;
      return _channel->pull();
    }

    Pin(const Pin<RT>&) = delete;

  protected:
    ChannelSharedPtr<typename RT::type> _channel;
};

#endif
