#ifndef PIN_H
#define PIN_H

#include <string>
#include <imgui_node_editor.h>

#include "DataType.h"

namespace NodeEditor = ax::NodeEditor;

class Pin {
  public:
    Pin(std::string name, DataType type, NodeEditor::PinKind kind);
    Pin(const Pin&) = delete;
    Pin(Pin&& other);

    static Pin* from(NodeEditor::PinId pinId);

    NodeEditor::PinId id();
    NodeEditor::PinKind kind();
    DataType type();
    void render();

  private:
    NodeEditor::PinId _id;
    std::string _name;
    DataType _type;
    NodeEditor::PinKind _kind;
};

#endif
