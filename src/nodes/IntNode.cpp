#include "IntNode.h"

REGISTER_NODE(IntNode, "Int");

IntNode::IntNode() : _value(0) {
  this->addOutputPin<RuntimeType::Int>("Int");

  snprintf(_label, IM_ARRAYSIZE(_label), "##IntNode(%p)", this);
}

void IntNode::update() {
  this->writePin<RuntimeType::Int>(0, _value);
  this->pause_update_loop();
}

void IntNode::render_widget() {
  ImGui::InputScalar(_label, ImGuiDataType_S32, &_value, NULL, NULL, "%d");
  if (ImGui::IsItemDeactivatedAfterEdit()) {
    this->resume_update_loop();
  }
}
