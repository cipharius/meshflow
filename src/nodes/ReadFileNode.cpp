#include "ReadFileNode.h"

#include <iostream>

ReadFileNode::ReadFileNode() {
  addInputPin<RuntimeType::String>("File path", "Test");
  addInputPin<RuntimeType::Int>("Crap");

  addOutputPin<RuntimeType::String>("Content", "Boop");
  addOutputPin<RuntimeType::Float>("Stuff");
}

std::string ReadFileNode::node_name() {
  return "Read file";
}

void ReadFileNode::update() {
  auto filePath = readPin<RuntimeType::String, std::string>(0);

  if (auto value = filePath) {
    std::cout << "File path: " << *value << std::endl;
    writePin<RuntimeType::String>(0, *value);
  } else {
    std::cout << "No file path" << std::endl;
  }
}
