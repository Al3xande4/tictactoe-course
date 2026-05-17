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

        int index = 0;
        int bit_idx = 0;

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
            return Sign::NONE;

        const int bit_no = (x + y * BoardOps::WIDTH) * 2;
        const int byte_no = bit_no / 64;
        const uint64_t value = (m_board[byte_no] >> (bit_no % 64)) & 0b11ULL;
        return static_cast<Sign>(value);
    }

    void Board::set(int x, int y, Sign sign)
    {
        if (x < 0 || x >= BoardOps::WIDTH || y < 0 || y >= BoardOps::HEIGHT)
            throw "Index is out of boundaries: can not set sign here";

        const int bit_no = (x + y * BoardOps::WIDTH) * 2;
        const int offset = bit_no % 64;
        uint64_t &byte = m_board[bit_no / 64];
        byte &= ~(0b11ULL << offset);
        const uint64_t value = static_cast<uint64_t>(sign);
        byte |= value << offset;
    }
}