#ifndef NODEREGISTRY_H
#define NODEREGISTRY_H

#include <set>
#include <unordered_map>

#include "Node.h"

#define REGISTER_NODE(NODE, NAME) \
RegisterNode<NODE> Register##NODE(NAME);

class NodeRegistry {
  public:
    static const std::set<const char*> node_types() {
      return _node_types;
    }

    static void register_node(const char* name, Node* (*constructor)(const char*)) {
      _node_types.emplace(name);
      _node_constructors.emplace(name, constructor);
    }

    static Node* create_node(const char* name) {
      return _node_constructors[name](name);
    }

  private:
    static std::set<const char*> _node_types;
    static std::unordered_map<const char*, Node*(*)(const char*)> _node_constructors;
};

template <class T>
class RegisterNode {
  public:
    RegisterNode(const char* type) : _name(type) {
      NodeRegistry::register_node(
        type,
        [](const char* name){
          T* obj = new T();
          obj->assign_name(name);
          return static_cast<Node*>(obj);
        }
      );
    }

    constexpr const char* node_name() const {
      return _name;
    }

  private:
    const char* _name;
};

#endif
