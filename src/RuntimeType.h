#ifndef RUNTIMETYPE_H
#define RUNTIMETYPE_H

#include <string>
#include <sstream>
#include <memory>

#define DECLARE_TYPE(NAME, TYPE) \
constexpr static const char type_##NAME[] = #NAME; \
using NAME = Type<TYPE, type_##NAME>;

#define INSTANTIATE_TYPE(NAME, TYPE) \
template class RuntimeType::Type<TYPE, RuntimeType::type_##NAME>;

class RuntimeType {
  public:
    template<typename T, const char* N> class Type;
    virtual ~RuntimeType() = 0;
    virtual const char* type_name() = 0;
    virtual bool has_value() = 0;
    virtual std::string to_string() = 0;

    DECLARE_TYPE (String, std::string)
    DECLARE_TYPE (Int, int)
    DECLARE_TYPE (Float, float)
};

template<typename T, const char* N>
class RuntimeType::Type final : public RuntimeType {
  public:
    std::shared_ptr<T> value;

    Type() = default;
    explicit Type(T value);

    bool has_value();
    std::string to_string();

    constexpr static RuntimeType::Type<T, N>* cast(RuntimeType* abstract) {
      if (abstract->type_name() == N) {
        return static_cast<RuntimeType::Type<T, N>*>(abstract);
      } else {
        return nullptr;
      }
    }

    constexpr const char* type_name() {
      return N;
    }
};

#endif
