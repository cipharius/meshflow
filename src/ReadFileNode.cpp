#include "ReadFileNode.h"

ReadFileNode::ReadFileNode() {
  addInputPin<RuntimeType::String>("File path");
  addOutputPin<RuntimeType::String>("Content");
}

std::string ReadFileNode::node_name() {
  return "Read file";
}

