#pragma once

#include <cstddef>

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class TransfiniteMeshNode : public Node {
  public:
    TransfiniteMeshNode();
    void update() override;
};
