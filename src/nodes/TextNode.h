#ifndef TEXTNODE_H
#define TEXTNODE_H

#include <iostream>

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
    // static int input_text_callback(ImGuiInputTextCallbackData* data);

    char _text[1024 * 16];
    bool _updated;
};

#endif
