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
INSTANTIATE_TYPE (Point, (std::array<float, 3>))
INSTANTIATE_TYPE (PointVec, (std::vector<std::array<float, 3>>))
