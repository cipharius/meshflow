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

    virtual void update() = 0;

    constexpr bool is_first_render() { return _firstRender; }
    constexpr const char* node_name() { return _name; }

    NodeEditor::NodeId id();
    void assign_name(const char* name);
    std::vector<std::shared_ptr<Link>> inbound_links();
    std::vector<std::shared_ptr<Link>> outbound_links();
    void render();

  protected:
    template <class RT>
    constexpr void addInputPin(std::string name) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(), NodeEditor::PinKind::Input);
      _inputPins.emplace_back(pin);
    }

    template <class RT, class T>
    constexpr void addInputPin(std::string name, T value) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(value), NodeEditor::PinKind::Input);
      _inputPins.emplace_back(pin);
    }

    template <class RT>
    constexpr void addOutputPin(std::string name){
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(), NodeEditor::PinKind::Output);
      _outputPins.emplace_back(pin);
    }

    template <class RT, class T>
    constexpr void addOutputPin(std::string name, T value){
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(value), NodeEditor::PinKind::Output);
      _outputPins.emplace_back(pin);
    }

    template <class RT, class T>
    constexpr std::shared_ptr<T> readPin(long unsigned int i) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      if (i >= _inputPins.size()) return std::shared_ptr<T>();

      if (auto& pin = _inputPins[i]) {
        if (auto* runtimeType = RT::cast(pin->type())) {
          return runtimeType->value;
        }
      }

      return std::shared_ptr<T>();
    }

    template <class RT, class T>
    constexpr void writePin(long unsigned int i, T value) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      static_assert(std::is_same<T, typename RT::type>::value, "Value doesn't match the RT type");
      if (i >= _outputPins.size()) return;

      if (auto& pin = _outputPins[i]) {
        if (auto* runtimeType = RT::cast(pin->type())) {
          runtimeType->value = std::make_shared<T>(value);
        }
      }
    }

    std::vector<std::unique_ptr<GenericPin>> _inputPins;
    std::vector<std::unique_ptr<GenericPin>> _outputPins;

  private:
    const char* _name;
    bool _firstRender;
    NodeEditor::NodeId _id;
};

#endif
