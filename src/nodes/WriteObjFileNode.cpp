#include "WriteObjFileNode.h"

REGISTER_NODE(WriteObjFileNode, "Write .obj file");

WriteObjFileNode::WriteObjFileNode() {
  addInputPin<RuntimeType::String>("File path");
  addInputPin<RuntimeType::QuadMesh>("Mesh");
}

void WriteObjFileNode::update() {
  this->wait_for_input();
  auto filePath = readPin<RuntimeType::String>(0);
  auto mesh = readPin<RuntimeType::QuadMesh>(1);
  if (!filePath || !mesh) return;

  std::error_code convert_failure;
  std::filesystem::path path(*filePath);
  path = std::filesystem::absolute(path, convert_failure);
  if (convert_failure) return;

  std::ofstream file (path, std::ofstream::out);
  if (!file.is_open()) return;

  for (auto const & vertex : mesh->vertices) {
    file << "v " << vertex.x() << ' ' << vertex.y() << ' ' << vertex.z() << std::endl;
  }

  for (auto const & face : mesh->faces) {
    file << "f "
         << face[0] + 1 << ' '
         << face[1] + 1 << ' '
         << face[2] + 1 << ' '
         << face[3] + 1 << std::endl;
  }

  file.close();
}
