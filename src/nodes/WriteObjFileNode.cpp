#include "WriteObjFileNode.h"
#include <iostream>

REGISTER_NODE(WriteObjFileNode, "Write .obj file");

WriteObjFileNode::WriteObjFileNode() {
  addInputPin<RuntimeType::String>("File path");
  addInputPin<RuntimeType::PointVec>("Points");
}

void WriteObjFileNode::update() {
  this->wait_for_input();
  auto filePath = readPin<RuntimeType::String>(0);
  auto points = readPin<RuntimeType::PointVec>(1);

  if (filePath && points) {
    std::filesystem::path path(*filePath);
    std::ofstream file (std::filesystem::absolute(path), std::ofstream::out);

    for (auto& point : *points) {
      file << "v " << point[0] << ' ' << point[1] << ' ' << point[2] << std::endl;
    }

    file.close();
  }
}
