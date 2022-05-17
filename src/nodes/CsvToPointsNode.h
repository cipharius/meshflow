#ifndef CSVTOPOINTSNODE_H
#define CSVTOPOINTSNODE_H

#include <vector>
#include <array>
#include <sstream>

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class CsvToPointsNode : public Node {
  public:
    CsvToPointsNode();
    void update() override;
};

#endif
