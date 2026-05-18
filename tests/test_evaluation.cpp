#include "player/evaluation.hpp"
#include "cassert"

int main(int argc, const char **argv)
{

    auto board = ttt::my_player::Board();
    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            board.set(x, y, ttt::game::Sign::NONE);
        }
    }

    for (int i = 0; i < 5; i++)
        board.set(i, 0, ttt::my_player::Sign::X);

    for (int i = 0; i < 5; i++)
        assert(ttt::my_player::check_win(board, i, 0, ttt::game::Sign::X));

    assert(ttt::my_player::check_win(board, 0, 1, ttt::game::Sign::X) == false);

    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            assert(ttt::my_player::check_win(board, x, y, ttt::game::Sign::O) == false);
        }
    }

    // pattern_table
    const auto &table = ttt::my_player::pattern_table;

    assert(table[120] >= ttt::my_player::PatterScore::OPEN_FOUR);
    assert(table[147] == ttt::my_player::PatterScore::CLOSED_ONE);

    //

    for (int x = 0; x < 20; x++)
    {
        for (int y = 0; y < 20; y++)
        {
            board.set(x, y, ttt::game::Sign::NONE);
        }
    }
    board.set(5, 5, ttt::game::Sign::X);
    board.set(6, 6, ttt::game::Sign::X);
    board.set(7, 7, ttt::game::Sign::X);
    int score = ttt::my_player::attack_score(board, 8, 8, ttt::game::Sign::X);

    assert(score >= ttt::my_player::PatterScore::OPEN_FOUR);

    return 0;
}