#ifndef LINK_H
#define LINK_H

#include <vector>

#include <imgui_node_editor.h>

namespace NodeEditor = ax::NodeEditor;

class Link {
  public:
    static Link* from(NodeEditor::LinkId& linkId);
    static void play_animations();

    Link(NodeEditor::PinId inputPinId, NodeEditor::PinId outputPinId);
    Link(const Link&) = delete;

    NodeEditor::LinkId id();
    NodeEditor::PinId inputPinId();
    NodeEditor::PinId outputPinId();
    void render();
    void activity_animation();

  private:
    static std::vector<Link*> _animationQueue;
    NodeEditor::LinkId _id;
    NodeEditor::PinId _inputPinId;
    NodeEditor::PinId _outputPinId;
};

#endif
