#ifndef SHOWTEXTNODE_H
#define SHOWTEXTNODE_H

#include <iostream>

#include "imgui.h"

#include "RuntimeType.h"
#include "Node.h"
#include "NodeRegistry.h"


class ShowTextNode : public Node {
  public:
    ShowTextNode();
    void update();
    void render_widget() override;

  private:
    std::shared_ptr<std::string> _textBoxState;
};

#endif
