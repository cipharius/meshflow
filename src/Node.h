#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>

#include <imgui.h>
#include <imgui_node_editor.h>

#include "Pin.h"
#include "RuntimeType.h"

namespace NodeEditor = ax::NodeEditor;

class Node {
  public:
    Node();
    Node(const Node&) = delete;
    virtual ~Node() = 0;

    static Node* from(NodeEditor::NodeId& nodeId);

    virtual std::string node_name() = 0;
    virtual void update() = 0;
    NodeEditor::NodeId id();
    void render();

  protected:
    template <class RT>
    constexpr void addInputPin(std::string name) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      _inputPins.emplace_back(name, new RT(), NodeEditor::PinKind::Input);
    }

    template <class RT, class T>
    constexpr void addInputPin(std::string name, T value) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      _inputPins.emplace_back(name, new RT(value), NodeEditor::PinKind::Input);
    }

    template <class RT>
    constexpr void addOutputPin(std::string name){
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      _outputPins.emplace_back(name, new RT(), NodeEditor::PinKind::Output);
    }

    template <class RT, class T>
    constexpr void addOutputPin(std::string name, T value){
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      _outputPins.emplace_back(name, new RT(value), NodeEditor::PinKind::Output);
    }

    template <class RT, class T>
    constexpr std::shared_ptr<T> readPin(long unsigned int i) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      if (i >= _inputPins.size()) return std::shared_ptr<T>();

      if (auto* handle = RT::cast(_inputPins[i].type())) {
        return handle->value;
      }

      return std::shared_ptr<T>();
    }

    template <class RT, class T>
    constexpr void writePin(long unsigned int i, T value) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      static_assert(std::is_same<T, typename RT::type>::value, "Value doesn't match the RT type");
      if (i >= _outputPins.size()) return;

      if (auto* handle = RT::cast(_outputPins[i].type())) {
        handle->value = std::make_shared<T>(value);
      }
    }

    std::vector<Pin> _inputPins;
    std::vector<Pin> _outputPins;

  private:
    NodeEditor::NodeId _id;
};

#endif
