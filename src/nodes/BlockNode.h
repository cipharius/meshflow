#ifndef BLOCKNODE_H
#define BLOCKNODE_H

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include <imgui_node_editor.h>

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class BlockNode : public Node {
  public:
    BlockNode();
    void update() override;
};


#endif

