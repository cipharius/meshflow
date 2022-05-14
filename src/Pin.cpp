#include "Pin.h"

GenericPin::GenericPin(NodeEditor::NodeId parentId, std::string name, RuntimeType* type, NodeEditor::PinKind kind)
: _parentId(parentId), _name(name), _kind(kind), _type(type) {
}

GenericPin::~GenericPin() {}

NodeEditor::PinId GenericPin::id() {
  return _id;
}

NodeEditor::NodeId GenericPin::parent_id() {
  return _parentId;
}

NodeEditor::PinKind GenericPin::kind() {
  return _kind;
}

std::string GenericPin::name() {
  return _name;
}

RuntimeType* GenericPin::type() {
  return _type;
}

std::shared_ptr<Link> GenericPin::link() {
  return _link;
}

GenericPin* GenericPin::from(NodeEditor::PinId pinId) {
  return reinterpret_cast<GenericPin*>(pinId.AsPointer());
}

ImVec2 GenericPin::calc_size() {
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

void GenericPin::render(int offset) {
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
