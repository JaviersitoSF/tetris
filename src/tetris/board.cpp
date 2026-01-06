#include "board.hpp"

template <size_t width, size_t height>
Board<width, height>::Board() {
    for (auto& cell : grid) {
        cell = Color::Background;
    }
}
