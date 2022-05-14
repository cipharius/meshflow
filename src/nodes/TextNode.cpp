#include "TextNode.h"

REGISTER_NODE(TextNode, "Text");

TextNode::TextNode() : _text(""), _updated(true) {
  addOutputPin<RuntimeType::String>("Text");
  snprintf(_label, IM_ARRAYSIZE(_label), "##TextNode(%p)", this);
}

void TextNode::update() {
  writePin<RuntimeType::String>(0, std::string(_text));
}

void TextNode::render_widget() {
  ImGui::PushItemWidth(200);
  ImGui::InputText(_label, _text, IM_ARRAYSIZE(_text));
  ImGui::PopItemWidth();
}

// int TextNode::input_text_callback(ImGuiInputTextCallbackData* data) {
//   if (data->EventFlag != ImGuiInputTextFlags_CallbackEdit) return 0;

//   this->_updated = true;
// }

