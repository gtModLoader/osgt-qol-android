#include "game/signatures.hpp"
#include "dlfcn.h"
#include "game.hpp"


REGISTER_GAME_FUNCTION(GetApp, "_Z6GetAppv", App*);
REGISTER_GAME_FUNCTION(VariantDBPrint, "_ZN9VariantDB5PrintEv", void, VariantDB*);
REGISTER_GAME_FUNCTION(VariantDBGetFunction, "_ZN9VariantDB11GetFunctionERKSs", FunctionObject*, VariantDB*,
                       std::string const& key);
REGISTER_GAME_FUNCTION(VariantDBGetFunctionIfExists, "_ZN9VariantDB19GetFunctionIfExistsERKSs", FunctionObject*,
                       VariantDB*, std::string const& key);
REGISTER_GAME_FUNCTION(VariantDBCallFunctionIfExists, "_ZN9VariantDB20CallFunctionIfExistsERKSsP11VariantList", void,
                       VariantDB*, std::string const& key, VariantList* pVList);
REGISTER_GAME_FUNCTION(VariantDBGetVarIfExists, "_ZNK9VariantDB14GetVarIfExistsERKSs", Variant*, VariantDB*,
                       std::string const&);
REGISTER_GAME_FUNCTION(VariantDBGetVarWithDefault, "_ZN9VariantDB17GetVarWithDefaultERKSsRK7Variant", Variant*,
                       VariantDB*, std::string const& key, const Variant& vDefault);
REGISTER_GAME_FUNCTION(VariantDBGetVar, "_ZN9VariantDB6GetVarERKSs", Variant*, VariantDB*, std::string const& key);
REGISTER_GAME_FUNCTION(VariantDBDeleteVar, "_ZN9VariantDB9DeleteVarERKSs", void, VariantDB*, std::string const& key);
REGISTER_GAME_FUNCTION(VariantDBDeleteAll, "_ZN9VariantDB9DeleteAllEv", void, VariantDB*);

REGISTER_GAME_FUNCTION(GetFontAndScaleToFitThisLinesPerScreenY, "_Z39GetFontAndScaleToFitThisLinesPerScreenYP5eFontPff",
                       void, uint32_t&, float&, float);
REGISTER_GAME_FUNCTION(CreateTextLabelEntity, "_Z21CreateTextLabelEntityP6EntitySsffSs", Entity*, Entity*,
                       std::string const&, float, float, std::string const&);
REGISTER_GAME_FUNCTION(SetupTextEntity, "_Z15SetupTextEntityP6Entity5eFontf", Entity*, Entity*, uint32_t, float);
REGISTER_GAME_FUNCTION(SetTextEntity, "_Z13SetTextEntityP6EntityRKSs", void, Entity*, std::string const&);
REGISTER_GAME_FUNCTION(CreateInputTextEntity, "_Z21CreateInputTextEntityP6EntitySsffSsffSsbSsS0_", Entity*, Entity*,
                       std::string const&, float, float, std::string const&, float, float, std::string const&, bool,
                       std::string const&, Entity*);
REGISTER_GAME_FUNCTION(SlideScreen, "_Z11SlideScreenP6Entitybii", Entity*, Entity*, bool, int, int);
REGISTER_GAME_FUNCTION(LogToConsole, "_Z12LogToConsolePKcz", void, const char*...);
REGISTER_GAME_FUNCTION(FadeInEntity, "_Z12FadeInEntityP6Entitybiif13eTimingSystem", void, Entity*, bool, int, int,
                       float, int);
REGISTER_GAME_FUNCTION(ResizeScrollBounds, "_Z18ResizeScrollBoundsP11VariantList", void, VariantList*);

#define RESOLVE_DIR_SYMBOL(name) real::name = (name##_t)dlsym(handle, pattern::name.c_str());
namespace game
{
void GameHarness::resolveSharedSigs()
{
    RESOLVE_DIR_SYMBOL(GetApp);
    RESOLVE_DIR_SYMBOL(VariantDBPrint);

    RESOLVE_DIR_SYMBOL(VariantDBGetFunction);
    RESOLVE_DIR_SYMBOL(VariantDBGetFunctionIfExists);
    RESOLVE_DIR_SYMBOL(VariantDBCallFunctionIfExists);
    RESOLVE_DIR_SYMBOL(VariantDBGetVarIfExists);
    RESOLVE_DIR_SYMBOL(VariantDBGetVarWithDefault);
    RESOLVE_DIR_SYMBOL(VariantDBGetVar);
    RESOLVE_DIR_SYMBOL(VariantDBDeleteVar);
    RESOLVE_DIR_SYMBOL(VariantDBDeleteAll);

    RESOLVE_DIR_SYMBOL(GetFontAndScaleToFitThisLinesPerScreenY);
    RESOLVE_DIR_SYMBOL(CreateTextLabelEntity);
    RESOLVE_DIR_SYMBOL(SetupTextEntity);
    RESOLVE_DIR_SYMBOL(SetTextEntity);
    RESOLVE_DIR_SYMBOL(CreateInputTextEntity);
    RESOLVE_DIR_SYMBOL(SlideScreen);
    RESOLVE_DIR_SYMBOL(LogToConsole);
    RESOLVE_DIR_SYMBOL(FadeInEntity);
    RESOLVE_DIR_SYMBOL(ResizeScrollBounds);
}
} // namespace game