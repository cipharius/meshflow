#ifndef RUNTIMETYPE_H
#define RUNTIMETYPE_H

#include <string>
#include <vector>
#include <sstream>
#include <memory>

#include "GeometricTypes.h"

namespace {
  // Escapes () around complex types
  template<typename T> struct unwrap;
  template<typename T, typename U> struct unwrap<T(U)> { typedef U type; };
}

#define DECLARE_TYPE(NAME, TYPE) \
constexpr static const char type_##NAME[] = #NAME; \
using NAME = Type<typename unwrap<void(TYPE)>::type, type_##NAME>;

#define INSTANTIATE_TYPE(NAME, TYPE) \
template class RuntimeType::Type<typename unwrap<void(TYPE)>::type, RuntimeType::type_##NAME>;

class RuntimeType {
  public:
    template<typename T, const char* N> class Type;
    virtual ~RuntimeType() = 0;
    virtual const char* type_name() = 0;
    virtual bool has_value() = 0;

    DECLARE_TYPE (String, std::string)
    DECLARE_TYPE (Int, int)
    DECLARE_TYPE (Float, float)
    DECLARE_TYPE (Point, Vec3)
    DECLARE_TYPE (PointVec, std::vector<Vec3>)
    DECLARE_TYPE (Polyline, Polyline)
    DECLARE_TYPE (Block, Block)
    DECLARE_TYPE (QuadMesh, QuadMesh)
};

template<typename T, const char* N>
class RuntimeType::Type final : public RuntimeType {
  public:
    constexpr static const char* static_type_name() { return N; }

    using type = T;
    std::shared_ptr<T> value;

    Type() = default;
    explicit Type(T value);

    bool has_value();

    constexpr static RuntimeType::Type<T, N>* cast(RuntimeType* abstract) {
      if (abstract->type_name() != N) return nullptr;

      return static_cast<RuntimeType::Type<T, N>*>(abstract);
    }

    constexpr const char* type_name() { return N; }
};

#endif
