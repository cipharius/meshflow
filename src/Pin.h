#ifndef PIN_H
#define PIN_H

#include <string>
#include <imgui_node_editor.h>

#include "RuntimeType.h"

namespace NodeEditor = ax::NodeEditor;

class Pin {
  public:
    Pin(std::string name, RuntimeType* type, NodeEditor::PinKind kind);
    Pin(const Pin&) = delete;
    Pin(Pin&& other);

    static Pin* from(NodeEditor::PinId pinId);

    NodeEditor::PinId id();
    NodeEditor::PinKind kind();
    RuntimeType* type();
    ImVec2 calc_size();
    void render(int offset = 0);

  private:
    NodeEditor::PinId _id;
    std::string _name;
    RuntimeType* _type;
    NodeEditor::PinKind _kind;
};

#endif
