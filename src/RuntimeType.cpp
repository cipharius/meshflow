#include "RuntimeType.h"

RuntimeType::~RuntimeType() {}

template <typename T, const char* N>
RuntimeType::Type<T, N>::Type(T value) : value(std::make_shared<T>(value)) {}

template <typename T, const char* N>
bool RuntimeType::Type<T, N>::has_value() {
  return (bool)value;
}

template <typename T, const char* N>
std::string RuntimeType::Type<T, N>::to_string() {
  if (value) {
    std::stringstream ss;
    ss << *value;
    return ss.str();
  } else {
    return "";
  }
}


INSTANTIATE_TYPE (String, std::string)
INSTANTIATE_TYPE (Int, int)
INSTANTIATE_TYPE (Float, float)
