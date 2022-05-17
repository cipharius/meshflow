#include "BlockToPointsNode.h"

REGISTER_NODE(BlockToPointsNode, "Block to points");

BlockToPointsNode::BlockToPointsNode() {
  this->addInputPin<RuntimeType::Block>("Block");
  this->addOutputPin<RuntimeType::PointVec>("Points");
}

void BlockToPointsNode::update() {
  this->wait_for_input();
  auto block = this->readPin<RuntimeType::Block>(0);

  if (block) {
    auto points = std::shared_ptr<std::vector<std::array<float, 3>>>(new std::vector<std::array<float, 3>>());

    for (auto point : block->north) points->push_back(point);
    for (auto point : block->east) points->push_back(point);
    for (auto point : block->south) points->push_back(point);
    for (auto point : block->west) points->push_back(point);

    this->writePin<RuntimeType::PointVec>(0, points);
  } else {
    this->resetPin<RuntimeType::PointVec>(0);
  }
}
