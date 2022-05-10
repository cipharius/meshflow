#include "DataType.h"

const char* DataType::toString() {
  switch (_type) {
    case DataType::String: return "String";
    case DataType::Integer: return "Integer";
    case DataType::Double: return "Double";
    default: return "";
  }
}

