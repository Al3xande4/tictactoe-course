#include "board.hpp"
#include "cstring"

namespace ttt::my_player
{
    Board::Board()
    {
        memset(m_board, 0, BITMAP_SIZE);
    }

    Board::Board(const State &state)
    {
        memset(m_board, 0, BITMAP_SIZE);

        for (int y = 0; y < BoardOps::HEIGHT; y++)
        {
            for (int x = 0; x < BoardOps::WIDTH; x++)
            {
                set(x, y, state.get_value(x, y));
            }
        }
    }

    Sign Board::get(int x, int y) const
    {
        if (x < 0 || x >= BoardOps::WIDTH || y < 0 || y >= BoardOps::HEIGHT)
            return Sign::WALL;

        // return m_array[x + y * 20];

        const int bit_no = (x + y * BoardOps::WIDTH) * 2;
        const int byte_no = bit_no / 8;
        const char value = (m_board[byte_no] >> (bit_no % 8)) & 0b11;
        return static_cast<Sign>(value);
    }

    void Board::set(int x, int y, Sign sign)
    {
        if (x < 0 || x >= BoardOps::WIDTH || y < 0 || y >= BoardOps::HEIGHT)
            return;
        // m_array[x + y * 20] = sign;

        const int bit_no = (x + y * BoardOps::WIDTH) * 2;
        const int offset = bit_no % 8;
        char &byte = m_board[bit_no / 8];
        byte &= ~(0b11 << offset);
        const char value = static_cast<char>(sign);
        byte |= value << offset;
    }
}