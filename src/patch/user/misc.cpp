#include "game/game.hpp"
#include "game/signatures.hpp"
#include "game/struct/net/gameupdatepacket.hpp"
#include "game/struct/netavatar/netavatar.hpp"
#include "patch/patch.hpp"

REGISTER_GAME_FUNCTION(NetAvatarSetCharacterState, "_ZN9NetAvatar17SetCharacterStateEP16GameUpdatePacket", void,
                       NetAvatar*, GameUpdatePacket*);
REGISTER_GAME_FUNCTION(OnStoreBuyConfirm, "_Z17OnStoreBuyConfirmP11VariantList", void, VariantList*);
REGISTER_GAME_FUNCTION(GetChosenStoreItemEntity, "_Z24GetChosenStoreItemEntityv", Entity*);

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

class SkipStoreConfirm : public patch::BasePatch
{
  public:
    void apply() const override
    {
        // This patch skips the server confirmation screen ("Please wait...") when buying packs from the gem store. This
        // negates low-ping advantage when buying bulk packs at expense of not seeing what you bought in the GUI.
        // You still have to click both "Buy" and "Confirm", it just won't wait for the server side confirmation
        // anymore.
        auto& game = game::GameHarness::get();
        game.hookFunctionPattern<OnStoreBuyConfirm_t>(pattern::OnStoreBuyConfirm, OnStoreBuyConfirm,
                                                            &real::OnStoreBuyConfirm);
        RESOLVE_SYMBOL(GetChosenStoreItemEntity);

        auto& events = game::EventsAPI::get();
        events.m_sig_loadFromMem.connect(&loadFromMemCallback);

        auto& optionsMgr = game::OptionsManager::get();
        optionsMgr.addCheckboxOption("qol", "Input", "osgt_qol_skip_store_confirm",
                                     "Skip store purchase confirmation for packs", &OnCheckboxCallback);
    }

    static void loadFromMemCallback()
    {
        // Default it to disabled.
        Variant* pVariant = real::GetApp()->GetVar("osgt_qol_skip_store_confirm");
        if (pVariant->GetType() == Variant::TYPE_UNUSED)
            pVariant->Set(0U);
        m_bSkipConfirm = pVariant->GetUINT32() == 1;
    }

    static void OnCheckboxCallback(VariantList* pVariant)
    {
        Entity* pCheckbox = pVariant->Get(1).GetEntity();
        bool bChecked = pCheckbox->GetVar("checked")->GetUINT32() != 0;
        real::GetApp()->GetVar("osgt_qol_skip_store_confirm")->Set(uint32_t(bChecked));
        m_bSkipConfirm = bChecked;
    }

    static void OnStoreBuyConfirm(VariantList* pVL)
    {
        if (real::GetApp()->m_serverProtocol >= 92 || !m_bSkipConfirm)
        {
            real::OnStoreBuyConfirm(pVL);
            return;
        }
        Entity* pChosen = real::GetChosenStoreItemEntity();
        if (pChosen)
            real::SendPacket(2, "action|buy\nitem|" + pChosen->GetName(), real::GetApp()->m_pClient->m_pEnetPeer);
    }

  private:
    static bool m_bSkipConfirm;
};
bool SkipStoreConfirm::m_bSkipConfirm = false;
REGISTER_USER_GAME_PATCH(SkipStoreConfirm, skip_store_confirm);