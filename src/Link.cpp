#include "Link.h"

Link::Link(NodeEditor::PinId inputPinId, NodeEditor::PinId outputPinId)
: _inputPinId(inputPinId), _outputPinId(outputPinId) {
  _id = NodeEditor::LinkId(this);
}

Link* Link::from(NodeEditor::LinkId& linkId) {
  return reinterpret_cast<Link*>(linkId.AsPointer());
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
