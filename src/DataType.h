#ifndef DATATYPE_H
#define DATATYPE_H

#include <string>
#include <optional>

class DataType {
  public:
    enum Type : uint8_t {
      String,
      Integer,
      Double
    };

    DataType() = default;
    constexpr DataType(Type type) : _type(type) {}

    constexpr operator Type() const { return _type; }
    explicit operator bool() = delete;
    constexpr bool operator ==(DataType other) const { return _type == other._type; }
    constexpr bool operator !=(DataType other) const { return _type != other._type; }

    const char* toString();

  private:
    Type _type;
};

#endif
