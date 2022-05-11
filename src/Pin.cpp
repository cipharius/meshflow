#include "Pin.h"

Pin::Pin(std::string name, RuntimeType* type, NodeEditor::PinKind kind)
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

RuntimeType* Pin::type() {
  return _type;
}

ImVec2 Pin::calc_size() {
  std::stringstream ss;
  switch (_kind) {
    case NodeEditor::PinKind::Input: {
      ss << u8"⊛ ";
      ss << _name;
      return ImGui::CalcTextSize(ss.str().c_str());
    }
    case NodeEditor::PinKind::Output: {
      ss << _name;
      ss << u8" ⊛";
      return ImGui::CalcTextSize(ss.str().c_str());
    }
    default: return ImVec2(0, 0);
  }
}

void Pin::render(int offset) {
  NodeEditor::BeginPin(_id, _kind);

  switch (_kind) {
    case NodeEditor::PinKind::Input: {
      ImGui::Text(u8"⊛ %s", _name.c_str());
      break;
    }
    case NodeEditor::PinKind::Output: {
      float cursorPosX = ImGui::GetCursorPosX();
      ImGui::SetCursorPosX(cursorPosX + offset);
      ImGui::Text(u8"%s ⊛", _name.c_str());
      break;
    }
  }

  NodeEditor::EndPin();
}
