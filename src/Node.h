#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_node_editor.h>

#include "Pin.h"
#include "DataType.h"

namespace NodeEditor = ax::NodeEditor;

class Node {
  public:
    Node();
    Node(const Node&) = delete;
    virtual ~Node() = 0;

    static Node* from(NodeEditor::NodeId& nodeId);

    virtual std::string node_name() = 0;
    NodeEditor::NodeId id();
    void render();
    virtual void update();

  protected:
    void addInputPin(std::string name, DataType type);
    void addOutputPin(std::string name, DataType type);

    std::vector<Pin> _inputPins;
    std::vector<Pin> _outputPins;

  private:
    NodeEditor::NodeId _id;
};

#endif
