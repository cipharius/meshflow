#ifndef LINK_H
#define LINK_H

#include <imgui_node_editor.h>

namespace NodeEditor = ax::NodeEditor;

class Link {
  public:
    Link(NodeEditor::PinId inputPinId, NodeEditor::PinId outputPinId);
    Link(const Link&) = delete;

    static Link* from(NodeEditor::LinkId& linkId);

    NodeEditor::LinkId id();
    NodeEditor::PinId inputPinId();
    NodeEditor::PinId outputPinId();
    void render();

  private:
    NodeEditor::LinkId _id;
    NodeEditor::PinId _inputPinId;
    NodeEditor::PinId _outputPinId;
};

#endif
