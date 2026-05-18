#pragma once

#include "core/game.hpp"
#include "evaluation.hpp"
#include "board.hpp"
#include "vector"
#include "config.hpp"
#include "algorithm"

namespace ttt::my_player
{
    using config::BoardOps;
    using game::Sign;
    using std::vector;

    struct RatedMove
    {
        int x, y;
        int priority;

        inline bool operator<(const RatedMove &other) { return priority > other.priority; }
    };

    inline int negamax(Board &fb, int depth, int alpha, int beta,
                       Sign current_sign, Sign bot_sign, int last_x, int last_y)
    {
        Sign opponent = (current_sign == Sign::X) ? Sign::O : Sign::X;

        if (check_win(fb, last_x, last_y, opponent))
            return -PatterScore::FIVE + (depth * 1000);

        if (depth == 0)
            return evaluate_board(fb, current_sign);

        std::vector<RatedMove> moves;
        for (int x = 0; x < BoardOps::WIDTH; x++)
        {
            for (int y = 0; y < BoardOps::HEIGHT; y++)
            {
                if (fb.get(x, y) != Sign::NONE || !is_promising_point(fb, x, y))
                    continue;

                int a_score = attack_score(fb, x, y, current_sign);
                int d_score = attack_score(fb, x, y, opponent);

                if (a_score >= PatterScore::OPEN_FOUR)
                    return PatterScore::FIVE;

                moves.push_back({x, y, a_score + d_score});
            }
        }

        if (moves.empty())
            return 0;

        std::sort(moves.begin(), moves.end());
        if (moves.size() > 8)
            moves.resize(8);

        int max_score = -2000000000;
        for (const auto &m : moves)
        {
            fb.set(m.x, m.y, current_sign);
            int score = -negamax(fb, depth - 1, -alpha - 1, -alpha, opponent, bot_sign, m.x, m.y);
            if (score > alpha && score < beta)
            {
                score = -negamax(fb, depth - 1, -beta, -alpha, opponent, bot_sign, m.x, m.y);
            }

            fb.set(m.x, m.y, Sign::NONE);

            if (score > max_score)
                max_score = score;
            if (score > alpha)
                alpha = score;
            if (alpha >= beta)
                break;
        }

        return max_score;
    }
}