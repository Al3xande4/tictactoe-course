#include "board.hpp"
#include "cstring"

namespace ttt::my_player
{
    Board::Board()
    {
        memset(m_oboard, 0, BITMAP_SIZE);
        memset(m_xboard, 0, BITMAP_SIZE);
    }

    Board::Board(const State &state)
    {
        memset(m_oboard, 0, BITMAP_SIZE);
        memset(m_xboard, 0, BITMAP_SIZE);

        uint64_t current_x = 0;
        uint64_t current_o = 0;
        int index = 0;
        int bit_idx = 0;

        for (int y = 0; y < BoardOps::HEIGHT; y++)
        {
            for (int x = 0; x < BoardOps::WIDTH; x++)
            {
                if (bit_idx == BITMAP_SIZE)
                {
                    m_xboard[index] = current_x;
                    m_oboard[index] = current_o;

                    ++index;
                    current_o = current_x = bit_idx = 0;
                }

                Sign cell = state.get_value(x, y);

                if (cell == Sign::X || cell == Sign::WALL)
                    current_x |= 1 << bit_idx;
                if (cell == Sign::O || cell == Sign::WALL)
                    current_o |= 1 << bit_idx;
                bit_idx++;
            }
            if (bit_idx != 0)
            {
                m_xboard[index] = current_x;
                m_oboard[index] = current_o;
            }
        }
    }

    Sign Board::get(int x, int y) const
    {
        if (x < 0 || x >= BoardOps::WIDTH || y < 0 || y >= BoardOps::HEIGHT)
            return Sign::NONE;

        int index = y * BoardOps::WIDTH + x;
        int byte_idx = index / 64;
        int bit_idx = index & 63;
        int64_t mask = 1ULL << bit_idx;

        bool x_cell = m_xboard[byte_idx] & mask;
        bool o_cell = m_oboard[byte_idx] & mask;

        return (Sign)(x_cell + o_cell * 2);
    }

    void Board::set(int x, int y, Sign sign)
    {
        if (x < 0 || x >= BoardOps::WIDTH || y < 0 || y >= BoardOps::HEIGHT)
            throw "Index is out of boundaries: can not set sign here";

        int index = y * BoardOps::WIDTH + x;
        int byte_idx = index / 64;
        int bit_idx = index & 63;
        int64_t mask = 1ULL << bit_idx;

        if (sign == Sign::X)
        {
            m_xboard[byte_idx] |= mask;
            m_oboard[byte_idx] &= ~mask;
        }
        else if (sign == Sign::O)
        {
            m_oboard[byte_idx] |= mask;
            m_xboard[byte_idx] &= ~mask;
        }
        else if (sign == Sign::WALL)
        {
            m_xboard[byte_idx] |= mask;
            m_oboard[byte_idx] |= mask;
        }
        else
        {
            m_xboard[byte_idx] &= ~mask;
            m_oboard[byte_idx] &= ~mask;
        }
    }
}