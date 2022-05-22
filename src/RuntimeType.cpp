#include "RuntimeType.h"

RuntimeType::~RuntimeType() {}

template <typename T, const char* N>
RuntimeType::Type<T, N>::Type(T value) : value(std::make_shared<T>(value)) {}

template <typename T, const char* N>
bool RuntimeType::Type<T, N>::has_value() {
  return (bool)value;
}

INSTANTIATE_TYPE (String, std::string)
INSTANTIATE_TYPE (Int, int)
INSTANTIATE_TYPE (Float, float)
INSTANTIATE_TYPE (Point, Vec3)
INSTANTIATE_TYPE (PointVec, std::vector<Vec3>)
INSTANTIATE_TYPE (Polyline, Polyline)
INSTANTIATE_TYPE (Block, Block)
INSTANTIATE_TYPE (QuadMesh, QuadMesh)
