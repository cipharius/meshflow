#include "ShowTextNode.h"

REGISTER_NODE(ShowTextNode, "Show text");

ShowTextNode::ShowTextNode() {
  addInputPin<RuntimeType::String>("Text");
}

void ShowTextNode::update() {
  auto text = readPin<RuntimeType::String>(0);

  std::cout << "Show text update" << std::endl;

  if (text) {
    _textBoxState = text;
  } else {
    _textBoxState.reset();
  }
}

void ShowTextNode::render_widget() {
  if (_textBoxState) {
    ImGui::TextUnformatted(_textBoxState->c_str());
  } else {
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "No input");
  }
}
