#ifndef READFILENODE_H
#define READFILENODE_H

#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

#include <imgui_node_editor.h>

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class ReadFileNode : public Node {
  public:
    ReadFileNode();
    void update() override;
    void render_widget() override;

  private:
    std::string _message;
    bool _hasError;
};


#endif
