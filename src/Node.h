#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_node_editor.h>

#include "Pin.h"
#include "RuntimeType.h"

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

  protected:
    template<class RT>
    constexpr void addInputPin(std::string name) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      _inputPins.emplace_back(name, new RT(), NodeEditor::PinKind::Input);
    }

    template<class RT>
    constexpr void addOutputPin(std::string name){
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      _outputPins.emplace_back(name, new RT(), NodeEditor::PinKind::Output);
    }

    std::vector<Pin> _inputPins;
    std::vector<Pin> _outputPins;

  private:
    NodeEditor::NodeId _id;
};

#endif
