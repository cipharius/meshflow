#include "Node.h"

Node::Node() :
_name(nullptr), _firstRender(true), _renderWidget(true),
_isUpdateLoopStopping(false), _isFirstUpdate(true) {
  _id = NodeEditor::NodeId(this);
  (void)_inputPins;
  (void)_outputPins;
}

Node::~Node() {}

void Node::update_loop(Node *node) {
  while (!node->is_stopping()) {
    node->update();
    node->_isFirstUpdate = false;
    std::this_thread::yield();
  }
}

Node* Node::from(NodeEditor::NodeId& nodeId) {
  return reinterpret_cast<Node*>(nodeId.AsPointer());
}

void Node::render_widget() {
  _renderWidget = false;
}

NodeEditor::NodeId Node::id() {
  return _id;
}

void Node::assign_name(const char* name) {
  if (_name != nullptr) return;
  _name = name;
}

std::vector<std::shared_ptr<Link>> Node::inbound_links() {
  auto result = std::vector<std::shared_ptr<Link>>();
  result.reserve(_inputPins.size());

  for (auto& inputPin : _inputPins) {
    auto links = inputPin->links();
    if (links.size() > 0) {
      result.push_back(std::move(links[0]));
    }
  }

  result.shrink_to_fit();
  return result;
}

std::vector<std::shared_ptr<Link>> Node::outbound_links() {
  auto result = std::vector<std::shared_ptr<Link>>();
  result.reserve(_outputPins.size());

  for (auto& outputPin : _outputPins) {
    for (auto&& link : outputPin->links()) {
      result.push_back(std::move(link));
    }
  }

  result.shrink_to_fit();
  return result;
}

const std::vector<std::shared_ptr<GenericPin>> Node::input_pins() {
  return std::vector<std::shared_ptr<GenericPin>>(_inputPins);
}

const std::vector<std::shared_ptr<GenericPin>> Node::output_pins() {
  return std::vector<std::shared_ptr<GenericPin>>(_outputPins);
}


void Node::render() {
  ImGuiIO& io = ImGui::GetIO();
  auto alpha = ImGui::GetStyle().Alpha;

  ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 0, 0, 255 * alpha));
  NodeEditor::PushStyleVar(NodeEditor::StyleVar_NodeRounding, 0);
  NodeEditor::PushStyleVar(NodeEditor::StyleVar_LinkStrength, 200);
  NodeEditor::PushStyleColor(NodeEditor::StyleColor_NodeBg, ImVec4(1.0f, 1.0f, 1.0f, alpha));
  NodeEditor::PushStyleColor(NodeEditor::StyleColor_NodeBorder, ImVec4(0.0f, 0.0f, 0.0f, alpha));
  ImGui::PushItemWidth(200);

  NodeEditor::BeginNode(_id);
    ImGui::PushFont(io.Fonts->Fonts[1]);
    ImGui::Text("%s", this->node_name());
    ImGui::PopFont();
    ImGui::Spacing();
    ImVec2 headerEnd = ImGui::GetCursorScreenPos();

    ImGui::Spacing();

    // Left pins
    ImGui::BeginGroup();
    for (auto& pin : _inputPins)
      pin->render();
    ImGui::EndGroup();

    ImGui::SameLine(ImGui::CalcItemWidth() / 2, 50);

    // Right pins
    ImGui::BeginGroup();
    float maxSize = 0;
    for (auto& pin : _outputPins) {
      maxSize = std::max(maxSize, pin->calc_size().x);
    }

    for (auto& pin : _outputPins) {
      int offset = maxSize - pin->calc_size().x;
      pin->render(offset);
    }
    ImGui::EndGroup();

    ImVec2 pinsEnd;
    if (_renderWidget) {
      ImGui::Spacing();
       pinsEnd = ImGui::GetCursorScreenPos();
      ImGui::Spacing();
    }

    _renderWidget = true;
    this->render_widget();

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

    if (_renderWidget) {
      drawList->AddLine(
        ImVec2((float)minPos.x + (borderWidth - 0.4), pinsEnd.y),
        ImVec2((float)maxPos.x - (borderWidth + 0.5), pinsEnd.y),
        headerColor
      );
    }
  }

  ImGui::PopItemWidth();
  NodeEditor::PopStyleColor(2);
  NodeEditor::PopStyleVar(2);
  ImGui::PopStyleColor();

  _firstRender = false;
}

void Node::start_update_loop() {
  _thread = std::thread(Node::update_loop, this);
}

void Node::stop_update_loop() {
   _isUpdateLoopStopping = true;

  for (auto& pin : _inputPins) {
    pin->close();
  }

  _onInput.notify_all();
  _onResume.notify_all();

  _thread.join();
}

void Node::pause_update_loop() {
  if (_isUpdateLoopStopping) return;
  std::unique_lock lock(_pauseMutex);
  _onResume.wait(lock);
}

void Node::resume_update_loop() {
  _onResume.notify_all();
}

void Node::wait_for_input() {
  if (_isUpdateLoopStopping) return;
  if (_isFirstUpdate) return;
  std::unique_lock lock(_pauseMutex);
  _onInput.wait(lock);
}

bool Node::is_stopping() {
  return _isUpdateLoopStopping;
}
