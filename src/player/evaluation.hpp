#pragma once
#include "board.hpp"
#include "array"
#include "config.hpp"
#include "iostream"

namespace ttt::my_player
{
    using config::BoardOps;

    static constexpr int directions[][2] = {{0, 1}, {1, 0}, {1, 1}, {-1, 1}};
    inline bool is_promising_point(const Board &board, int x, int y)
    {
        for (const auto &dir : directions)
        {
            int dx = dir[0], dy = dir[1];

            for (int k = -4; k <= 4; k++)
            {
                if (k == 0)
                    continue;
                Sign cell = board.get(x + k * dx, y + k * dy);
                if (cell == Sign::X || cell == Sign::O)
                    return true;
            }
        }
        return false;
    }

    inline void build_line(const Board &board, int x, int y, Sign player, const int dir[], int line[])
    {
        int dx = dir[0], dy = dir[1];
        line[4] = 1;
        for (int k = -4; k <= 4; k++)
        {
            if (k == 0)
                continue;
            Sign cell = board.get(x + k * dx, y + k * dy);
            if (cell == player)
                line[k + 4] = 1;
            else if (cell == Sign::NONE)
                line[k + 4] = 0;
            else
                line[k + 4] = 2;
        }
    }

    // enum Score
    // {
    //     FIVE = 100000000,
    //     FOUR = 100000,
    //     OPEN_THREE = 100000,
    //     CLOSED_THREE = 10000,
    //     OPEN_TWO = 10000,
    //     CLOSED_TWO = 1000,
    //     ONE = 1,
    //     FORK = 10000000

    // };

    // inline constexpr int encode(const int line[], const int l)
    // {
    //     int code = 0, power = 1;
    //     for (int i = 0; i < l; i++)
    //     {
    //         code += line[i] * power;
    //         power *= 3;
    //     }
    //     return code;
    // }

    // inline constexpr std::array<int, 243> build_basic_pattern_table()
    // {
    //     std::array<int, 243> codes{0};

    //     for (int i = 0; i < 243; i++)
    //     {
    //         int code = i;
    //         int line[5]{0};

    //         int stones = 0;
    //         int blocked = 0;
    //         for (int j = 0; j < 5; j++)
    //         {
    //             line[j] = code % 3;
    //             if (line[j] == 1)
    //                 stones++;
    //             else if (line[j] == 2)
    //             {
    //                 blocked++;
    //                 break;
    //             }
    //             code /= 3;
    //         }
    //         if (blocked > 0)
    //         {
    //             codes[i] = 0;
    //             continue;
    //         }

    //         bool open = line[0] == 0 && line[4] == 0;
    //         if (stones == 5)
    //             codes[i] = Score::FIVE;
    //         else if (stones == 4)
    //             codes[i] = Score::FOUR;
    //         else if (stones == 3)
    //         {
    //             if (open)
    //                 codes[i] = Score::OPEN_THREE;
    //             else
    //                 codes[i] = Score::CLOSED_THREE;
    //         }
    //         else if (stones == 2)
    //         {
    //             if (open)
    //                 codes[i] = Score::OPEN_TWO;
    //             else
    //                 codes[i] = Score::CLOSED_TWO;
    //         }
    //         else if (stones == 1)
    //             codes[i] = Score::ONE;
    //         else
    //             codes[i] = 0;
    //     }

    //     return codes;
    // }

    // static constexpr std::array<int, 243> basic_pattern_table = build_basic_pattern_table();

    static constexpr float DECAY_FACTOR = 0.9f;

    enum PatterScore
    {
        FIVE = 10000000,
        OPEN_FOUR = 1000000,
        CLOSED_FOUR = 20000,
        OPEN_THREE = 100000,
        CLOSED_THREE = 5000,
        OPEN_TWO = 10000,
        CLOSED_TWO = 1000,
        OPEN_ONE = 10,
        CLOSED_ONE = 10
    };

    constexpr inline int encode_line(int line[])
    {
        int res = 0, power = 1;

        for (int i = 0; i < 9; i++)
        {
            res += line[i] * power;
            power *= 3;
        }
        return res;
    }

    constexpr inline void decode_line(int code, int line[])
    {
        for (int i = 0; i < 9; i++)
        {
            line[i] = code % 3;
            code /= 3;
        }
    }

    constexpr inline int process_line(int line[])
    {
        int total_score = 0;
        for (int i = 0; i <= 4; i++)
        {
            int player_figures = 0;
            bool opponent_figures = 0;

            int longest_player_streak = 0;
            int current_player_streak = 0;

            for (int j = i; j <= i + 4; j++)
            {
                if (line[j] == 1)
                {
                    player_figures++;
                    current_player_streak++;
                    if (current_player_streak > longest_player_streak)
                        longest_player_streak = current_player_streak;
                    continue;
                }
                else if (line[j] == 2)
                {
                    opponent_figures = 1;
                    break;
                }

                current_player_streak = 0;
            }

            if (opponent_figures)
                continue;

            bool open_left = i > 0 && line[i - 1] == 0;
            bool open_right = i < 4 && line[i + 5] == 0;

            bool closed = !open_left || !open_right;
            bool decayed = false;

            int window_score = 0;
            if (player_figures == 5)
                window_score = PatterScore::FIVE;
            else if (player_figures == 4)
            {
                if (closed)
                {
                    if (decayed)
                        window_score = PatterScore::CLOSED_FOUR * DECAY_FACTOR;
                    else
                        window_score = PatterScore::CLOSED_FOUR;
                }
                else
                {
                    if (decayed)
                        window_score = PatterScore::OPEN_FOUR * DECAY_FACTOR;
                    else
                        window_score = PatterScore::OPEN_FOUR;
                }
            }
            else if (player_figures == 3)
            {
                if (closed)
                {
                    if (decayed)
                        window_score = PatterScore::CLOSED_THREE * DECAY_FACTOR;
                    else
                        window_score = PatterScore::CLOSED_THREE;
                }
                else
                {
                    if (decayed)
                        window_score = PatterScore::OPEN_THREE * DECAY_FACTOR;
                    else
                        window_score = PatterScore::OPEN_THREE;
                }
            }
            else if (player_figures == 2)
            {
                if (closed)
                {
                    if (decayed)
                        window_score = PatterScore::CLOSED_TWO * DECAY_FACTOR;
                    else
                        window_score = PatterScore::CLOSED_TWO;
                }
                else
                {
                    if (decayed)
                        window_score = PatterScore::OPEN_TWO * DECAY_FACTOR;
                    else
                        window_score = PatterScore::OPEN_TWO;
                }
            }
            else if (player_figures == 1)
            {
                if (closed)
                    window_score = PatterScore::CLOSED_ONE;
                else
                    window_score = PatterScore::OPEN_ONE;
            }
            total_score += window_score;
        }
        return total_score;
    }

    constexpr std::array<int64_t, 19683> build_pattern_table()
    {
        std::array<int64_t, 19683> table{0};

        for (int i = 0; i < 19683; i++)
        {
            int line[9]{};
            decode_line(i, line);
            table[i] = process_line(line);
        }
        return table;
    }

    constexpr std::array<int64_t, 19683> pattern_table = build_pattern_table();

    inline int attack_score(const Board &board, int x, int y, Sign player)
    {
        int score = 0;
        int threats_count = 0;
        for (auto &dir : directions)
        {
            int line[9];
            build_line(board, x, y, player, dir, line);

            int s = pattern_table[encode_line(line)];
            if (s >= OPEN_THREE)
                threats_count++;
            score += s;
        }

        if (threats_count >= 2)
            score *= 10;
        return score;
    }

    inline int evaluate_board(const Board &board, Sign player)
    {
        int my_score = 0;
        int opp_score = 0;
        Sign opponent = (player == Sign::X) ? Sign::O : Sign::X;

        for (int x = 0; x < BoardOps::WIDTH; x++)
        {
            for (int y = 0; y < BoardOps::HEIGHT; y++)
            {
                if (board.get(x, y) == Sign::NONE && is_promising_point(board, x, y))
                {
                    my_score += attack_score(board, x, y, player);
                    opp_score += attack_score(board, x, y, player);
                }
            }
        }
        return my_score - (int)(opp_score * 0.9);
    }

    inline bool check_win(const Board &fb, int x, int y, Sign player)
    {
        if (x < 0 || y < 0)
            return false;
        for (auto &dir : directions)
        {
            int count = 1;
            for (int i = 1; i <= 4; i++)
            {
                if (fb.get(x + i * dir[0], y + i * dir[1]) == player)
                    count++;
                else
                    break;
            }

            for (int i = 1; i <= 4; i++)
            {
                if (fb.get(x - i * dir[0], y - i * dir[1]) == player)
                    count++;
                else
                    break;
            }
            if (count >= 5)
                return true;
        }
        return false;
    }
}
