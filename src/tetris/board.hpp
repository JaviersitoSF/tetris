#pragma once

#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <cstddef>

#include "colors.hpp"

template <size_t width, size_t height>
class Board {
public:
  Board();

private:
  std::array<Color, width * height> grid;
};

#endif // BOARD_HPP
