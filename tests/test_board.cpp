#include "player/board.hpp"
#include "core/state.hpp"
#include "core/field.hpp"
#include "cassert"
#include "iostream"

int main(int argc, const char **argv)
{
    ttt::game::State::Opts opts;
    opts.rows = opts.cols = 20;
    opts.win_len = 5;
    opts.max_moves = 0;

    auto random_fi = ttt::game::RandomObstaclesFI(0.75, 50, 1);
    auto state = ttt::game::State(opts, &random_fi);
    auto board = ttt::my_player::Board(state);

    for (int y = 0; y < opts.rows; y++)
    {
        for (int x = 0; x < opts.cols; x++)
        {
            assert(state.get_value(x, y) == board.get(x, y));
        }
    }

    board.set(5, 5, ttt::game::Sign::NONE);
    assert(board.get(5, 5) == ttt::game::Sign::NONE);

    board.set(1, 9, ttt::game::Sign::X);
    assert(board.get(1, 9) == ttt::game::Sign::X);

    board.set(19, 15, ttt::game::Sign::O);
    assert(board.get(19, 15) == ttt::game::Sign::O);

    board.set(15, 3, ttt::game::Sign::WALL);
    assert(board.get(15, 3) == ttt::game::Sign::WALL);

    return 0;
}