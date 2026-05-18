#pragma once

#include "core/game.hpp"
#include "cstdint"
#include "config.hpp"

namespace ttt::my_player
{
    using config::BoardOps;
    using game::Sign;
    using game::State;

    class Board
    {
    private:
        static constexpr int BITMAP_SIZE = (BoardOps::TOTAL_CELLS * 2 + 7) / 8;

        char m_board[BITMAP_SIZE];
        // Sign m_array[400];

    public:
        Board();
        Board(const State &);

        Sign get(int x, int y) const;
        void set(int, int, Sign);
    };
}