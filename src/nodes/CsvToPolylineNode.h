#pragma once

#include <vector>
#include <array>
#include <sstream>

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class CsvToPolylineNode : public Node {
  public:
    CsvToPolylineNode();
    void update() override;
};
