#include "Node.h"

Node::Node() : _firstRender(true) {
  _id = NodeEditor::NodeId(this);
  (void)_inputPins;
  (void)_outputPins;
}

Node::~Node() {}

Node* Node::from(NodeEditor::NodeId& nodeId) {
  return reinterpret_cast<Node*>(nodeId.AsPointer());
}

NodeEditor::NodeId Node::id() {
  return _id;
}

std::vector<std::shared_ptr<Link>> Node::inbound_links() {
  auto result = std::vector<std::shared_ptr<Link>>();
  result.reserve(_inputPins.size());

  for (auto& inputPin : _inputPins) {
    if (auto&& link = inputPin->link()) {
      result.push_back(std::move(link));
    }
  }

  result.shrink_to_fit();
  return result;
}

std::vector<std::shared_ptr<Link>> Node::outbound_links() {
  auto result = std::vector<std::shared_ptr<Link>>();
  result.reserve(_outputPins.size());

  for (auto& outputPin : _outputPins) {
    if (auto&& link = outputPin->link()) {
      result.push_back(std::move(link));
    }
  }

  result.shrink_to_fit();
  return result;
}

void Node::render() {
  ImGuiIO& io = ImGui::GetIO();
  auto alpha = ImGui::GetStyle().Alpha;

  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255 * alpha));
  NodeEditor::PushStyleVar(NodeEditor::StyleVar_NodeRounding, 0);
  NodeEditor::PushStyleVar(NodeEditor::StyleVar_LinkStrength, 200);
  NodeEditor::PushStyleColor(NodeEditor::StyleColor_NodeBg, ImVec4(1.0f, 1.0f, 1.0f, alpha));
  NodeEditor::PushStyleColor(NodeEditor::StyleColor_NodeBorder, ImVec4(0.0f, 0.0f, 0.0f, alpha));

  NodeEditor::BeginNode(_id);
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::Text("%s", this->node_name().c_str());
    ImGui::PopFont();
    ImGui::Spacing();
    ImVec2 headerEnd = ImGui::GetCursorScreenPos();

    ImGui::Spacing();

    ImGui::BeginGroup();
    for (auto& pin : _inputPins)
      pin->render();
    ImGui::EndGroup();

    ImGui::SameLine(0, 50);

    ImGui::PushItemWidth(-FLT_MIN);
    ImGui::BeginGroup();
    float maxSize = 0;
    for (auto& pin : _outputPins)
      maxSize = std::max(maxSize, pin->calc_size().x);

    for (auto& pin : _outputPins) {
      int offset = maxSize - pin->calc_size().x;
      pin->render(offset);
    }
    ImGui::EndGroup();
    ImGui::PopItemWidth();

  NodeEditor::EndNode();

  if (ImGui::IsItemVisible()) {
    auto drawList = NodeEditor::GetNodeBackgroundDrawList(_id);
    ImVec2 minPos = ImGui::GetItemRectMin();
    ImVec2 maxPos = ImGui::GetItemRectMax();
    const float borderWidth = NodeEditor::GetStyle().NodeBorderWidth - 0.3f;

    auto headerColor = IM_COL32(255 * 0.7f, 255 * 0.7f, 255 * 0.7f, 255 * alpha);

    drawList->AddRectFilled(
      ImVec2((float)minPos.x + borderWidth, (float)minPos.y + borderWidth),
      ImVec2((float)maxPos.x - borderWidth, (float)headerEnd.y),
      headerColor
    );
  }

  NodeEditor::PopStyleColor(2);
  NodeEditor::PopStyleVar(2);
  ImGui::PopStyleColor();

  _firstRender = false;
}
