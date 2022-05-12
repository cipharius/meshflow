#include <stdio.h>
#include <vector>
#include <unordered_set>

#include <iostream>

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
  for (int i=0; i<10; i++) {
    Node* node = new ReadFileNode();
    nodes.insert(node);
  }

  std::unordered_set<Link*> links;

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

    if (NodeEditor::BeginCreate()) {
      NodeEditor::PinId inputPinId, outputPinId;

      if (NodeEditor::QueryNewLink(&inputPinId, &outputPinId)) {
        if (inputPinId && outputPinId) {
          Pin* inputPin = Pin::from(inputPinId);
          Pin* outputPin = Pin::from(outputPinId);

          if (inputPin->kind() != outputPin->kind() && inputPin->type()->type_name() == outputPin->type()->type_name()) {
            if (NodeEditor::AcceptNewItem()) {
              links.insert(new Link(inputPinId, outputPinId));
            }
          } else {
            NodeEditor::RejectNewItem();
          }
        }
      }
    }
    NodeEditor::EndCreate();


    if (NodeEditor::BeginDelete()) {
      NodeEditor::NodeId deletedNodeId;
      while (NodeEditor::QueryDeletedNode(&deletedNodeId)) {
        nodes.erase(Node::from(deletedNodeId));
      }

      NodeEditor::LinkId deletedLinkId;
      while (NodeEditor::QueryDeletedLink(&deletedLinkId))
        links.erase(Link::from(deletedLinkId));
    }
    NodeEditor::EndDelete();

    for (Node* node : nodes)
      node->render();

    for (Link* link : links)
      link->render();

    NodeEditor::End();

    if (NodeEditor::PinId hoveredPinId = NodeEditor::GetHoveredPin()) {
      RuntimeType* value = Pin::from(hoveredPinId)->type();
      ImGui::SetTooltip("Type: %s\nValue: %s", value->type_name(), value->to_string().c_str());
    }

    if (NodeEditor::NodeId hoveredNodeId = NodeEditor::GetHoveredNode()) {
      if (ImGui::IsKeyPressed(ImGuiKey_Space)) {
        Node* value = Node::from(hoveredNodeId);
        value->update();
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

  for (Node* node : nodes)
    delete node;

  for (Link* link : links)
    delete link;

  NodeEditor::DestroyEditor(m_Context);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
