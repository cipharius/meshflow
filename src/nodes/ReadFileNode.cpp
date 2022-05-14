#include "ReadFileNode.h"

REGISTER_NODE(ReadFileNode, "Read file");

ReadFileNode::ReadFileNode() {
  addInputPin<RuntimeType::String>("File path", "Test");

  addOutputPin<RuntimeType::String>("Content", "Boop");
}

void ReadFileNode::update() {
  auto filePath = readPin<RuntimeType::String>(0);

  if (auto value = filePath) {
    std::cout << "File path: " << *value << std::endl;
    writePin<RuntimeType::String>(0, *value);
  } else {
    std::cout << "No file path" << std::endl;
  }
}
