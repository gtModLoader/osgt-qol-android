#include "game/game.hpp"
#include "game/struct/net/gameupdatepacket.hpp"
#include "game/struct/netavatar/netavatar.hpp"
#include "patch/patch.hpp"

REGISTER_GAME_FUNCTION(NetAvatarSetCharacterState, "_ZN9NetAvatar17SetCharacterStateEP16GameUpdatePacket", void,
                       NetAvatar*, GameUpdatePacket*);

class DoubleJumpStateFixer : public patch::BasePatch
{
  public:
    void apply() const override
    {
        // By default, the game will force your double jump available state to be off whenever you receive a state
        // packet. This was likely done to prevent some weird high-jump + double-jump combos back in the day, but the
        // way this is implemented is inherently flawed and also obsolete by 2016-2017 state of the game. We will
        // workaround this by restoring double jump available flag if double jump effect was never removed in first
        // place and user still had a free jump remaining.
        auto& game = game::GameHarness::get();
        game.hookFunctionPattern<NetAvatarSetCharacterState_t>(
            pattern::NetAvatarSetCharacterState, NetAvatarSetCharacterState, &real::NetAvatarSetCharacterState);
    }

    static void NetAvatarSetCharacterState(NetAvatar* this_, GameUpdatePacket* packet)
    {
        // Save our current state for use later.
        bool bCurrentJumpState = this_->m_bDoubleJumpAvailable;
        // Let game process characater state as usual, it'll reset the double jump available var, but that's fine since
        // we just cached it's prior value.
        real::NetAvatarSetCharacterState(this_, packet);
        // Double jump flag wasn't removed and we had a double jump available? Restore it.
        if (bCurrentJumpState && (this_->m_flags & 0x02))
            this_->m_bDoubleJumpAvailable = true;
    }
};
REGISTER_USER_GAME_PATCH(DoubleJumpStateFixer, double_jump_state_fixer);