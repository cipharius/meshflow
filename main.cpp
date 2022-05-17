#include <stdio.h>
#include <vector>
#include <unordered_set>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <imgui_node_editor.h>

#include "Node.h"
#include "Link.h"

#include "ReadFileNode.h"

namespace NodeEditor = ax::NodeEditor;

static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**) {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit())
    return 1;

  const char* glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SCALE_TO_MONITOR, GL_TRUE);

  GLFWwindow* window = glfwCreateWindow(1920, 1080, "Meshflow", NULL, NULL);
  if (window == NULL)
    return 1;

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

  ImGui::StyleColorsClassic();

  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  const ImWchar glyph_range[] = { 0x20, 0xFFFF, 0 };
  io.Fonts->AddFontFromFileTTF("data/dejavu/ttf/DejaVuSans.ttf", 18, NULL, glyph_range);
  io.Fonts->AddFontFromFileTTF("data/dejavu/ttf/DejaVuSans-Bold.ttf", 18, NULL, glyph_range);

  for (auto& config : io.Fonts->ConfigData) {
    config.OversampleH = 3;
    config.OversampleV = 3;
  }

  NodeEditor::Config config;
  config.SettingsFile = "meshflow.json";
  NodeEditor::EditorContext* m_Context = NodeEditor::CreateEditor(&config);

  std::unordered_set<Node*> nodes;

  NodeEditor::NodeId contextNodeId = 0;
  GenericPin* contextPin = 0;
  ImVec2 openPopupPos;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(io.DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

    ImGui::Begin("Content", nullptr,
      ImGuiWindowFlags_NoTitleBar |
      ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoScrollWithMouse |
      ImGuiWindowFlags_NoSavedSettings |
      ImGuiWindowFlags_NoBringToFrontOnFocus);


    NodeEditor::SetCurrentEditor(m_Context);
    NodeEditor::Begin("Mesh composer", ImVec2(0.0f, 0.0f));

    NodeEditor::EnableShortcuts(!io.WantTextInput);

    if (NodeEditor::BeginCreate()) {
      NodeEditor::PinId startPinId, endPinId;

      if (NodeEditor::QueryNewLink(&startPinId, &endPinId)) {
        if (startPinId && endPinId) {
          GenericPin* startPin = GenericPin::from(startPinId);
          GenericPin* endPin = GenericPin::from(endPinId);

          if (startPin->can_bind(endPin)) {
            if (NodeEditor::AcceptNewItem()) {
              startPin->bind(endPin);
            }
          } else {
            NodeEditor::RejectNewItem();
          }
        }
      }

      NodeEditor::PinId pinId = 0;
      if (NodeEditor::QueryNewNode(&pinId)) {
        GenericPin* pin = GenericPin::from(pinId);
        if (pin) {
          ImGui::SetTooltip("New node");

          if (NodeEditor::AcceptNewItem()) {
            contextPin = pin;
            NodeEditor::Suspend();
            openPopupPos = ImGui::GetMousePos();
            ImGui::OpenPopup("CreateFromPin");
            NodeEditor::Resume();
          }
        } else {
          NodeEditor::RejectNewItem();
        }
      }
    }
    NodeEditor::EndCreate();


    if (NodeEditor::BeginDelete()) {
      NodeEditor::NodeId deletedNodeId;
      while (NodeEditor::QueryDeletedNode(&deletedNodeId)) {
        if (Node* node = Node::from(deletedNodeId)) {
          nodes.erase(node);
          node->stop_update_loop();
          delete node;
        }
      }

      NodeEditor::LinkId deletedLinkId;
      while (NodeEditor::QueryDeletedLink(&deletedLinkId)) {
        if (auto* link = Link::from(deletedLinkId)) {
          if (auto* pin = GenericPin::from(link->inputPinId())) {
            pin->unbind();
          }
        }
      }
    }
    NodeEditor::EndDelete();

    for (Node* node : nodes) {
      if (node->is_first_render()) {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0);
        node->render();
        ImGui::PopStyleVar();

        ImVec2 nodeSize = NodeEditor::GetNodeSize(node->id());
        ImVec2 nodePos = NodeEditor::GetNodePosition(node->id());
        ImVec2 nodeNewPos = ImVec2(nodePos.x - nodeSize.x/2, nodePos.y - nodeSize.y/2);
        NodeEditor::SetNodePosition(node->id(), nodeNewPos);
      }

      node->render();
    }

    for (Node* node : nodes) {
      for (auto& link : node->outbound_links()) {
        link->render();
      }
    }

    NodeEditor::Suspend();
    if (NodeEditor::ShowNodeContextMenu(&contextNodeId)) {
      openPopupPos = ImGui::GetMousePos();
      ImGui::OpenPopup("NodeContext");
    }
    if (NodeEditor::ShowBackgroundContextMenu()) {
      openPopupPos = ImGui::GetMousePos();
      ImGui::OpenPopup("BackgroundContext");
    }
    NodeEditor::Resume();

    NodeEditor::Suspend();
    if (ImGui::BeginPopup("NodeContext", ImGuiWindowFlags_NoMove)) {
      if (auto* node = Node::from(contextNodeId)) {
        if (ImGui::MenuItem("Delete")) {
          nodes.erase(node);
          node->stop_update_loop();
          delete node;
        }
      }

      ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("BackgroundContext", ImGuiWindowFlags_NoMove)) {
      ImGui::TextUnformatted("Create new node");
      ImGui::Separator();

      Node* node = nullptr;
      for (const char* node_type : NodeRegistry::node_types()) {
        if (ImGui::MenuItem(node_type)) {
          node = NodeRegistry::create_node(node_type);
        }
      }

      if (node) {
        ImVec2 newNodePos = NodeEditor::ScreenToCanvas(openPopupPos);

        node->start_update_loop();
        nodes.insert(node);
        NodeEditor::SetNodePosition(node->id(), newNodePos);
      }

      ImGui::EndPopup();
    }

    if (ImGui::BeginPopup("CreateFromPin", ImGuiWindowFlags_NoMove)) {
      ImGui::TextUnformatted("Create new node");
      ImGui::Separator();

      Node* node = nullptr;
      for (const char* node_type : NodeRegistry::node_types()) {
        if (ImGui::MenuItem(node_type)) {
          node = NodeRegistry::create_node(node_type);
          node->start_update_loop();
        }
      }

      if (node) {
        ImVec2 newNodePos = NodeEditor::ScreenToCanvas(openPopupPos);

        nodes.insert(node);
        NodeEditor::SetNodePosition(node->id(), newNodePos);

        if (contextPin->kind() == NodeEditor::PinKind::Input) {
          for (auto& pin : node->output_pins()) {
            if (pin->type()->type_name() == contextPin->type()->type_name()) {
              pin->bind(contextPin);
              break;
            }
          }
        } else {
          for (auto& pin : node->input_pins()) {
            if (pin->type()->type_name() == contextPin->type()->type_name()) {
              pin->bind(contextPin);
              break;
            }
          }
        }
      }

      ImGui::EndPopup();
    }
    NodeEditor::Resume();

    NodeEditor::End();

    if (NodeEditor::PinId hoveredPinId = NodeEditor::GetHoveredPin()) {
      if (auto* pin = GenericPin::from(hoveredPinId)) {
        ImGui::SetTooltip("Type: %s", pin->type_name());
      }
    }

    NodeEditor::SetCurrentEditor(nullptr);

    ImGui::End();
    ImGui::PopStyleVar(2);

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }

  for (Node* node : nodes) {
    node->stop_update_loop();
    delete node;
  }

  NodeEditor::DestroyEditor(m_Context);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
