#ifndef TEXTNODE_H
#define TEXTNODE_H

#include <iostream>
#include <condition_variable>

#include "imgui.h"

#include "RuntimeType.h"
#include "Node.h"
#include "NodeRegistry.h"


class TextNode : public Node {
  public:
    TextNode();
    void update();
    void render_widget() override;

  private:
    char _text[1024];
    char _label[32];
};

#endif
