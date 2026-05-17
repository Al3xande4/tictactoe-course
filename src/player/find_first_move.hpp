#pragma once

#include "core/game.hpp"
#include "config.hpp"
#include "evaluation.hpp"

namespace ttt::my_player
{
    using config::BoardOps;
    using game::Point;
    using game::State;

    inline Point find_start_move(const State &state)
    {
        Point p = {0, 0};
        int best_score = -1;

        Board board = Board(state);
        for (int x = 0; x < BoardOps::WIDTH; x++)
        {
            for (int y = 0; y < BoardOps::HEIGHT; y++)
            {
                if (state.get_value(x, y) != Sign::NONE)
                    continue;

                int score = attack_score(board, x, y, state.get_current_player());
                if (score > best_score)
                {
                    best_score = score;
                    p = {x, y};
                }
            }
        }
        return p;
    }
}