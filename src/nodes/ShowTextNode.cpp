#include "ShowTextNode.h"

REGISTER_NODE(ShowTextNode, "Show text");

ShowTextNode::ShowTextNode() {
  addInputPin<RuntimeType::String>("Text");
}

void ShowTextNode::update() {
  this->wait_for_input();
  auto text = readPin<RuntimeType::String>(0);

  if (text) {
    _textBoxState = text;
  } else {
    _textBoxState.reset();
  }
}

void ShowTextNode::render_widget() {
  ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 500);
  if (_textBoxState) {
    ImGui::TextUnformatted(_textBoxState->c_str());
  } else {
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "No input");
  }
  ImGui::PopTextWrapPos();
}
