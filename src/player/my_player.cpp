#include "my_player.hpp"
#include <cstdlib>
#include "find_first_move.hpp"

namespace ttt::my_player
{

    void MyPlayer::set_sign(Sign sign) { m_sign = sign; }
    const char *MyPlayer::get_name() const { return m_name; }

    Point MyPlayer::make_move(const State &state)
    {
        if (state.get_move_no() == 0)
            return find_start_move(state);
        return {0, 0};
    }
}; // namespace ttt::my_player
