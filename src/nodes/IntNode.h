#pragma once

#include "imgui.h"

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class IntNode : public Node {
  public:
    IntNode();
    void update() override;
    void render_widget() override;

  private:
    char _label[32];
    ImS32 _value;
};
