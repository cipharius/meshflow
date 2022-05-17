#include "ReadFileNode.h"

REGISTER_NODE(ReadFileNode, "Read file");

ReadFileNode::ReadFileNode() {
  addInputPin<RuntimeType::String>("File path");
  addOutputPin<RuntimeType::String>("Content");
}

void ReadFileNode::update() {
  this->wait_for_input();
  auto filePath = readPin<RuntimeType::String>(0);

  if (auto value = filePath) {
    std::filesystem::path path(*value);

    if (std::filesystem::is_regular_file(path)) {
      std::ifstream file (std::filesystem::absolute(path), std::ifstream::in);
      std::stringstream ss;

      ss << file.rdbuf();
      file.close();

      writePin<RuntimeType::String>(0, std::make_shared<std::string>(std::move(ss.str())));
    } else {
      resetPin<RuntimeType::String>(0);
    }
  } else {
    resetPin<RuntimeType::String>(0);
  }
}
