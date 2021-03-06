#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <thread>
#include <atomic>

#include <imgui.h>
#include <imgui_node_editor.h>

#include "Pin.h"
#include "RuntimeType.h"

namespace NodeEditor = ax::NodeEditor;

class Node {
  public:
    static void update_loop(Node* node);

    Node();
    Node(const Node&) = delete;
    virtual ~Node() = 0;

    static Node* from(NodeEditor::NodeId& nodeId);

    virtual void update() = 0;
    virtual void render_widget();

    constexpr bool is_first_render() { return _firstRender; }
    constexpr const char* node_name() { return _name; }

    NodeEditor::NodeId id();
    void assign_name(const char* name);
    std::vector<std::shared_ptr<Link>> inbound_links();
    std::vector<std::shared_ptr<Link>> outbound_links();
    const std::vector<std::shared_ptr<GenericPin>> input_pins();
    const std::vector<std::shared_ptr<GenericPin>> output_pins();
    void render();
    void start_update_loop();
    void stop_update_loop();
    void pause_update_loop();
    void resume_update_loop();
    void wait_for_input();
    bool is_stopping();

  protected:
    template <class RT>
    constexpr void addInputPin(std::string name) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(), NodeEditor::PinKind::Input, &_onInput);
      _inputPins.emplace_back(pin);
    }

    template <class RT, class T>
    constexpr void addInputPin(std::string name, T value) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(value), NodeEditor::PinKind::Input, &_onInput);
      _inputPins.emplace_back(pin);
    }

    template <class RT>
    constexpr void addOutputPin(std::string name){
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(), NodeEditor::PinKind::Output, nullptr);
      _outputPins.emplace_back(pin);
    }

    template <class RT, class T>
    constexpr void addOutputPin(std::string name, T value){
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      auto* pin = new Pin<RT>(_id, name, new RT(value), NodeEditor::PinKind::Output, nullptr);
      _outputPins.emplace_back(pin);
    }

    template <class RT>
    constexpr std::shared_ptr<typename RT::type> readPin(long unsigned int i) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      if (i >= _inputPins.size()) return std::shared_ptr<typename RT::type>();

      if (auto& genericPin = _inputPins[i]) {
        if (auto* pin = Pin<RT>::cast( &(*genericPin) )) {
          return pin->read();
        }
      }

      return std::shared_ptr<typename RT::type>();
    }

    template <class RT>
    constexpr void writePin(long unsigned int i, typename RT::type value) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      if (i >= _outputPins.size()) return;

      if (auto& genericPin = _outputPins[i]) {
        if (auto* pin = Pin<RT>::cast( &(*genericPin) )) {
          pin->write(std::make_shared<typename RT::type>(value));
        }
      }
    }

    template <class RT>
    constexpr void writePin(long unsigned int i, std::shared_ptr<typename RT::type> value) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      if (i >= _outputPins.size()) return;

      if (auto& genericPin = _outputPins[i]) {
        if (auto* pin = Pin<RT>::cast( &(*genericPin) )) {
          pin->write(value);
        }
      }
    }

    template <class RT>
    constexpr void resetPin(long unsigned int i) {
      static_assert(std::is_base_of<RuntimeType, RT>::value, "RT not derived from RuntimeType");
      if (i >= _outputPins.size()) return;

      if (auto& genericPin = _outputPins[i]) {
        if (auto* pin = Pin<RT>::cast( &(*genericPin) )) {
          pin->write(std::shared_ptr<typename RT::type>());
        }
      }
    }

    std::vector<std::shared_ptr<GenericPin>> _inputPins;
    std::vector<std::shared_ptr<GenericPin>> _outputPins;

  private:
    const char* _name;
    NodeEditor::NodeId _id;
    std::thread _thread;
    std::mutex _pauseMutex;
    std::condition_variable _onResume;
    std::condition_variable _onInput;
    bool _firstRender;
    bool _renderWidget;
    bool _isUpdateLoopStopping;
    bool _isFirstUpdate;
};

#endif
