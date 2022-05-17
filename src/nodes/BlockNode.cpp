#include "BlockNode.h"

REGISTER_NODE(BlockNode, "Block");

BlockNode::BlockNode() {
  this->addInputPin<RuntimeType::PointVec>("North");
  this->addInputPin<RuntimeType::PointVec>("East");
  this->addInputPin<RuntimeType::PointVec>("South");
  this->addInputPin<RuntimeType::PointVec>("West");
  this->addOutputPin<RuntimeType::Block>("Block");
}

void BlockNode::update() {
  this->wait_for_input();
  auto north = this->readPin<RuntimeType::PointVec>(0);
  auto east = this->readPin<RuntimeType::PointVec>(1);
  auto south = this->readPin<RuntimeType::PointVec>(2);
  auto west = this->readPin<RuntimeType::PointVec>(3);

  if (north && east && south && west) {
    std::shared_ptr<Block> block(new Block());
    block->north = *north;
    block->east = *east;
    block->south = *south;
    block->west = *west;

    writePin<RuntimeType::Block>(0, block);
  } else {
    resetPin<RuntimeType::Block>(0);
  }
}
