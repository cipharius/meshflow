#include "NodeRegistry.h"

std::set<const char*> NodeRegistry::_node_types;
std::unordered_map<const char*, Node*(*)(const char*)> NodeRegistry::_node_constructors;
