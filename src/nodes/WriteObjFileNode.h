#ifndef WRITEOBJFILENODE_H
#define WRITEOBJFILENODE_H

#include <fstream>
#include <filesystem>

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class WriteObjFileNode : public Node {
  public:
    WriteObjFileNode();
    void update() override;
};

#endif
