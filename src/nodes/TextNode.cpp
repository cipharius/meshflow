#include "TextNode.h"

REGISTER_NODE(TextNode, "Text");

TextNode::TextNode() : _text("") {
  addOutputPin<RuntimeType::String>("Text");
  snprintf(_label, IM_ARRAYSIZE(_label), "##TextNode(%p)", this);
}

void TextNode::update() {
  writePin<RuntimeType::String>(0, std::string(_text));
  this->pause_update_loop();
}

void TextNode::render_widget() {
  ImGui::PushItemWidth(200);
  if (ImGui::InputText(_label, _text, IM_ARRAYSIZE(_text), ImGuiInputTextFlags_EnterReturnsTrue)) {
    this->resume_update_loop();
  }
  ImGui::PopItemWidth();
}
