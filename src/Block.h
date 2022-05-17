#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <array>

struct Block {
  std::vector<std::array<float, 3>> north;
  std::vector<std::array<float, 3>> east;
  std::vector<std::array<float, 3>> south;
  std::vector<std::array<float, 3>> west;
};

#endif
