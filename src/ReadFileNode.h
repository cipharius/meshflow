#ifndef READFILENODE_H
#define READFILENODE_H

#include <string>
#include <imgui_node_editor.h>

#include "Node.h"
#include "RuntimeType.h"

namespace NodeEditor = ax::NodeEditor;

class ReadFileNode : public Node {
  public:
    ReadFileNode();

    std::string node_name();
    void update();
};


#endif
