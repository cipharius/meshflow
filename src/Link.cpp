#include "Link.h"

std::vector<Link*> Link::_animationQueue = std::vector<Link*>();

Link* Link::from(NodeEditor::LinkId& linkId) {
  return reinterpret_cast<Link*>(linkId.AsPointer());
}

void Link::play_animations() {
  for (auto* link : _animationQueue) {
    NodeEditor::Flow(link->id());
  }
  _animationQueue.clear();
}

Link::Link(NodeEditor::PinId inputPinId, NodeEditor::PinId outputPinId)
: _inputPinId(inputPinId), _outputPinId(outputPinId) {
  _id = NodeEditor::LinkId(this);
}

NodeEditor::LinkId Link::id() {
  return _id;
}

NodeEditor::PinId Link::inputPinId() {
  return _inputPinId;
}

NodeEditor::PinId Link::outputPinId() {
  return _outputPinId;
}

void Link::render() {
  NodeEditor::Link(_id, _inputPinId, _outputPinId, ImVec4(1, 1, 1, 1), 2.0f);
}

void Link::activity_animation() {
  Link::_animationQueue.push_back(this);
}
