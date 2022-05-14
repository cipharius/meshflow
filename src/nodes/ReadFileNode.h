#ifndef READFILENODE_H
#define READFILENODE_H

#include <string>
#include <imgui_node_editor.h>

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class ReadFileNode : public Node {
  public:
    ReadFileNode();
    void update();
};


#endif
