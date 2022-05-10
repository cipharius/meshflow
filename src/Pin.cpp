#include "Pin.h"

Pin::Pin(std::string name, DataType type, NodeEditor::PinKind kind)
: _name(name), _type(type), _kind(kind) {
  _id = NodeEditor::PinId(this);
}

Pin::Pin(Pin&& other)
: _name(other._name), _type(other._type), _kind(other._kind) {
  _id = NodeEditor::PinId(this);
}

Pin* Pin::from(NodeEditor::PinId pinId) {
  return reinterpret_cast<Pin*>(pinId.AsPointer());
}

NodeEditor::PinId Pin::id() {
  return _id;
}

NodeEditor::PinKind Pin::kind() {
  return _kind;
}

DataType Pin::type() {
  return _type;
}

void Pin::render() {
  NodeEditor::BeginPin(_id, _kind);

  ImGui::AlignTextToFramePadding();
  switch (_kind) {
    case NodeEditor::PinKind::Input: {
      ImGui::Text(u8"⊛ %s", _name.c_str());
      break;
    }
    case NodeEditor::PinKind::Output: {
      ImGui::Text(u8"%s ⊛", _name.c_str());
      break;
    }
  }

  NodeEditor::EndPin();
}
