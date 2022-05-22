#pragma once

#include <sstream>

#include "imgui.h"

#include "Node.h"
#include "NodeRegistry.h"
#include "RuntimeType.h"

class ShowPolylineNode : public Node {
  public:
    ShowPolylineNode();
    void update() override;
    void render_widget();

  private:
    std::shared_ptr<std::string> _textBoxState;
};
