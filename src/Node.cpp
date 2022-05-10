#include "Node.h"

Node::Node() {
  _id = NodeEditor::NodeId(this);
  (void)_inputPins;
  (void)_outputPins;
}

Node::~Node() {}

NodeEditor::NodeId Node::id() {
  return _id;
}

Node* Node::from(NodeEditor::NodeId& nodeId) {
  return reinterpret_cast<Node*>(nodeId.AsPointer());
}

void Node::render() {
  ImGuiIO& io = ImGui::GetIO();

  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255));
  NodeEditor::PushStyleVar(NodeEditor::StyleVar_NodeRounding, 0);
  NodeEditor::PushStyleVar(NodeEditor::StyleVar_LinkStrength, 200);
  NodeEditor::PushStyleColor(NodeEditor::StyleColor_NodeBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
  NodeEditor::PushStyleColor(NodeEditor::StyleColor_NodeBorder, ImVec4(0.0f, 0.0f, 0.0f, 1.0f));

  NodeEditor::BeginNode(_id);
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::Text("%s", this->node_name().c_str());
    ImGui::PopFont();
    ImGui::Spacing();
    ImVec2 headerEnd = ImGui::GetCursorScreenPos();

    ImGui::Spacing();

    for (auto& pin : _inputPins)
      pin.render();

    ImGui::SameLine(0, 50);

    for (auto& pin : _outputPins)
      pin.render();

  NodeEditor::EndNode();

  if (ImGui::IsItemVisible()) {
    auto drawList = NodeEditor::GetNodeBackgroundDrawList(_id);
    ImVec2 minPos = ImGui::GetItemRectMin();
    ImVec2 maxPos = ImGui::GetItemRectMax();
    const float borderWidth = NodeEditor::GetStyle().NodeBorderWidth - 0.3f;

    auto headerColor = IM_COL32(255 * 0.7f, 255 * 0.7f, 255 * 0.7f, 255);

    drawList->AddRectFilled(
      ImVec2((float)minPos.x + borderWidth, (float)minPos.y + borderWidth),
      ImVec2((float)maxPos.x - borderWidth, (float)headerEnd.y),
      headerColor
    );
  }

  NodeEditor::PopStyleColor(2);
  NodeEditor::PopStyleVar(2);
  ImGui::PopStyleColor();
}

void Node::addInputPin(std::string name, DataType type) {
  _inputPins.emplace_back(name, type, NodeEditor::PinKind::Input);
}

void Node::addOutputPin(std::string name, DataType type) {
  _outputPins.emplace_back(name, type, NodeEditor::PinKind::Output);
}
