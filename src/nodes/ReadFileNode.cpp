#include "ReadFileNode.h"

REGISTER_NODE(ReadFileNode, "Read file");

ReadFileNode::ReadFileNode() {
  addInputPin<RuntimeType::String>("File path");
  addOutputPin<RuntimeType::String>("Content");
}

void ReadFileNode::update() {
  this->wait_for_input();
  auto filePath = readPin<RuntimeType::String>(0);

  if (!filePath) {
    _hasError = true;
    _message = "No input";
    resetPin<RuntimeType::String>(0);
    return;
  }

  std::filesystem::path path(*filePath);

  if (!std::filesystem::is_regular_file(path)) {
    _hasError = true;
    _message = "Invalid file path";
    resetPin<RuntimeType::String>(0);
    return;
  }

  std::ifstream file (std::filesystem::absolute(path), std::ifstream::in);
  std::stringstream ss;

  ss << file.rdbuf();
  file.close();

  _hasError = false;
  _message = path.filename();

  writePin<RuntimeType::String>(0, std::make_shared<std::string>(std::move(ss.str())));
}

void ReadFileNode::render_widget() {
  ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + 500);
  if (_hasError) {
    ImGui::TextColored(ImVec4(1, 0, 0, 1), "%s", _message.c_str());
  } else {
    ImGui::TextUnformatted(_message.c_str());
  }
  ImGui::PopTextWrapPos();
}
