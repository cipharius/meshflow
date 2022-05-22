#include "ShowPolylineNode.h"

REGISTER_NODE(ShowPolylineNode, "Show polyline");

ShowPolylineNode::ShowPolylineNode() {
  this->addInputPin<RuntimeType::Polyline>("Polyline");
}

void ShowPolylineNode::update() {
  this->wait_for_input();
  auto polyline = this->readPin<RuntimeType::Polyline>(0);

  if (polyline) {
    std::vector<Vec3> const & points = polyline->points();
    std::stringstream ss;

    for (size_t i = 0; i < points.size(); i++) {
      ss << '[' << i << ']' << ": " << points[i] << std::endl;
    }

    _textBoxState = std::make_shared<std::string>(ss.str());
  } else {
    _textBoxState.reset();
  }
}

void ShowPolylineNode::render_widget() {
  ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 500);
  if (_textBoxState) {
    ImGui::TextUnformatted(_textBoxState->c_str());
  } else {
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "No input");
  }
  ImGui::PopTextWrapPos();
}
