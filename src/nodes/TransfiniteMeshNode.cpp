#include "TransfiniteMeshNode.h"

QuadMesh transfiniteInterpolate(Block const & block, const size_t, const size_t ny);

REGISTER_NODE(TransfiniteMeshNode, "Transfinite mesh");

TransfiniteMeshNode::TransfiniteMeshNode() {
  this->addInputPin<RuntimeType::Block>("Block");
  this->addInputPin<RuntimeType::Int>("NX");
  this->addInputPin<RuntimeType::Int>("NY");

  this->addOutputPin<RuntimeType::QuadMesh>("Mesh");
}

void TransfiniteMeshNode::update() {
  this->wait_for_input();
  auto block = this->readPin<RuntimeType::Block>(0);
  auto NX = this->readPin<RuntimeType::Int>(1);
  auto NY = this->readPin<RuntimeType::Int>(2);

  if (block && NX && NY && *NX > 0 && *NY > 0) {
    QuadMesh mesh = transfiniteInterpolate(*block, *NX, *NY);
    this->writePin<RuntimeType::QuadMesh>(0, std::make_shared<QuadMesh>(std::move(mesh)));
  } else {
    this->resetPin<RuntimeType::QuadMesh>(0);
  }
}

QuadMesh transfiniteInterpolate(Block const & block, const size_t nx, const size_t ny) {
  QuadMesh result;

  result.vertices.reserve((nx + 1) * (ny + 1));
  result.faces.reserve(nx * ny);

  for (size_t y = 0; y < (ny + 1); y++) {
    const float v = (float)y / (float)ny;

    for (size_t x = 0; x < (nx + 1); x++) {
      const float u = (float)x / (float)nx;
      Vec3 position = (
        (1 - v) * block.bottom().lerp(u) + v * block.top().lerp(u) +
        (1 - u) * block.left().lerp(v) + u * block.right().lerp(v) - (
          (1 - u) * (1 - v) * block.bottom_left() +
          u * v * block.top_right() +
          u * (1 - v) * block.bottom_right() +
          (1 - u) * v * block.top_left()
        )
      );

      result.vertices.push_back(position);
    }
  }

  // Face order: Bottom to top, from left to right
  for (size_t y = 0; y < ny; y++) {
    for (size_t x = 0; x < nx; x++) {
      // Clockwise starting from top left corner
      std::array<size_t,4> face;
      face[0] = (nx + 1) * (y + 1) + x;
      face[1] = (nx + 1) * (y + 1) + (x + 1);
      face[2] = (nx + 1) * y       + (x + 1);
      face[3] = (nx + 1) * y       + x;
      result.faces.push_back(std::move(face));
    }
  }

  return result;
}
