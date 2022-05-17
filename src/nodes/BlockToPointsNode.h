#ifndef BLOCKTOPOINTSNODE_H
#define BLOCKTOPOINTSNODE_H

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class BlockToPointsNode : public Node {
  public:
    BlockToPointsNode();
    void update() override;
};

#endif
