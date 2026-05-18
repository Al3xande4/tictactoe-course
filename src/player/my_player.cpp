#include "my_player.hpp"
#include <cstdlib>
#include "find_first_move.hpp"
#include "negamax.hpp"
#include "config.hpp"

namespace ttt::my_player
{
    using config::BoardOps;

    void MyPlayer::set_sign(Sign sign) { m_sign = sign; }
    const char *MyPlayer::get_name() const { return m_name; }

    Point MyPlayer::make_move(const State &state)
    {
        if (state.get_move_no() == 0)
            return find_start_move(state);

        Board board = Board(state);

        Sign opponent = (m_sign == Sign::X) ? Sign::O : Sign::X;
        Point best_move;

        std::vector<RatedMove> root_moves;
        for (int x = 0; x < BoardOps::WIDTH; x++)
        {
            for (int y = 0; y < BoardOps::HEIGHT; y++)
            {
                if (board.get(x, y) != Sign::NONE || !is_promising_point(board, x, y))
                    continue;

                int a_score = attack_score(board, x, y, m_sign);
                int d_score = attack_score(board, x, y, opponent);

                if (a_score >= PatterScore::FIVE)
                {
                    best_move.x = x;
                    best_move.y = y;
                    return best_move;
                }

                root_moves.push_back({x, y, a_score + (d_score)});
            }
        }

        std::sort(root_moves.begin(), root_moves.end());
        if (root_moves.size() > 8)
            root_moves.resize(8);

        if (!root_moves.empty())
        {
            best_move.x = root_moves[0].x;
            best_move.y = root_moves[0].y;
        }

        int alpha = -2000000000;
        int beta = 2000000000;
        int max_score = -2000000000;

        for (const auto &m : root_moves)
        {
            board.set(m.x, m.y, m_sign);
            int score = -negamax(board, 5, -beta, -alpha, opponent, m_sign, m.x, m.y);
            board.set(m.x, m.y, Sign::NONE);

            if (score > max_score)
            {
                max_score = score;
                best_move.x = m.x;
                best_move.y = m.y;
            }
            if (max_score > alpha)
            {
                alpha = max_score;
            }
        }

        return best_move;
    }
}; // namespace ttt::my_player
