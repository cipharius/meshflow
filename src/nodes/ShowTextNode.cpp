#include "ShowTextNode.h"

REGISTER_NODE(ShowTextNode, "Show text");

ShowTextNode::ShowTextNode() {
  addInputPin<RuntimeType::String>("Text");
}

void ShowTextNode::update() {
  auto text = readPin<RuntimeType::String, std::string>(0);

  if (auto value = text) {
    std::cout << "Show text: " << text << std::endl;
  } else {
    std::cout << "Show text - No text" << std::endl;
  }
}
