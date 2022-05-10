#include "ReadFileNode.h"

#include <iostream>

ReadFileNode::ReadFileNode() {
  this->addInputPin("File path", DataType::String);

  this->addOutputPin("Content", DataType::String);
}

std::string ReadFileNode::node_name() {
  return std::string("Read file");
}

void ReadFileNode::update() {
}
