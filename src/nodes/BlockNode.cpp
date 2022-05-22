#include "BlockNode.h"

REGISTER_NODE(BlockNode, "Block");

BlockNode::BlockNode() {
  this->addInputPin<RuntimeType::Polyline>("Top");
  this->addInputPin<RuntimeType::Polyline>("Right");
  this->addInputPin<RuntimeType::Polyline>("Bottom");
  this->addInputPin<RuntimeType::Polyline>("Left");

  this->addOutputPin<RuntimeType::Block>("Block");
}

void BlockNode::update() {
  this->wait_for_input();
  auto top = this->readPin<RuntimeType::Polyline>(0);
  auto right = this->readPin<RuntimeType::Polyline>(1);
  auto bottom = this->readPin<RuntimeType::Polyline>(2);
  auto left = this->readPin<RuntimeType::Polyline>(3);

  if (top && right && bottom && left) {
    std::shared_ptr<Block> block(new Block(
      *top,
      *right,
      *bottom,
      *left
    ));

    writePin<RuntimeType::Block>(0, block);
  } else {
    resetPin<RuntimeType::Block>(0);
  }
}
