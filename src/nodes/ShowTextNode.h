#ifndef SHOWTEXTNODE_H
#define SHOWTEXTNODE_H

#include <iostream>

#include "RuntimeType.h"
#include "Node.h"
#include "NodeRegistry.h"

class ShowTextNode : public Node {
  public:
    ShowTextNode();
    void update();
};

#endif
