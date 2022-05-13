#ifndef LINK_H
#define LINK_H

#include <imgui_node_editor.h>

namespace NodeEditor = ax::NodeEditor;

class Link {
  public:
    static Link* from(NodeEditor::LinkId& linkId);

    Link(NodeEditor::PinId inputPinId, NodeEditor::PinId outputPinId);
    Link(const Link&) = delete;


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
