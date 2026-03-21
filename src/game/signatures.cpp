#include "game/signatures.hpp"
#include "game.hpp"
#include "game/struct/ubistring.hpp"
#include "dlfcn.h"

REGISTER_GAME_FUNCTION(GetApp, "_Z6GetAppv", App*);
REGISTER_GAME_FUNCTION(VariantDBPrint, "_ZN9VariantDB5PrintEv", void, VariantDB*);
REGISTER_GAME_FUNCTION(VariantDBGetFunction, "_ZN9VariantDB11GetFunctionERKSs", FunctionObject*, VariantDB*,
                       void* key);
REGISTER_GAME_FUNCTION(VariantDBGetFunctionIfExists, "_ZN9VariantDB19GetFunctionIfExistsERKSs", FunctionObject*,
                       VariantDB*, void* key);
REGISTER_GAME_FUNCTION(VariantDBCallFunctionIfExists, "_ZN9VariantDB20CallFunctionIfExistsERKSsP11VariantList", void,
                       VariantDB*, void* key, VariantList* pVList);
REGISTER_GAME_FUNCTION(VariantDBGetVarIfExists, "_ZNK9VariantDB14GetVarIfExistsERKSs", Variant*, VariantDB*,
                       void*);
REGISTER_GAME_FUNCTION(VariantDBGetVarWithDefault, "_ZN9VariantDB17GetVarWithDefaultERKSsRK7Variant", Variant*,
                       VariantDB*, void* key, const Variant& vDefault);
REGISTER_GAME_FUNCTION(VariantDBGetVar, "_ZN9VariantDB6GetVarERKSs", Variant*, VariantDB*, void* key);
REGISTER_GAME_FUNCTION(VariantDBDeleteVar, "_ZN9VariantDB9DeleteVarERKSs", void, VariantDB*, void* key);
REGISTER_GAME_FUNCTION(VariantDBDeleteAll, "_ZN9VariantDB9DeleteAllEv", void, VariantDB*);

REGISTER_GAME_FUNCTION(GetFontAndScaleToFitThisLinesPerScreenY, "_Z39GetFontAndScaleToFitThisLinesPerScreenYP5eFontPff",
                       void, uint32_t&, float&, float);
REGISTER_GAME_FUNCTION(CreateTextLabelEntity, "_Z21CreateTextLabelEntityP6EntitySsffSs", Entity*, Entity*, void*, float,
                       float, void*);
REGISTER_GAME_FUNCTION(SetupTextEntity, "_Z15SetupTextEntityP6Entity5eFontf", Entity*, Entity*, uint32_t, float);
REGISTER_GAME_FUNCTION(SetTextEntity, "_Z13SetTextEntityP6EntityRKSs", void, Entity*, void*);
REGISTER_GAME_FUNCTION(CreateInputTextEntity, "_Z21CreateInputTextEntityP6EntitySsffSsffSsbSsS0_", Entity*, Entity*,
                       const UbiString&, float, float, const UbiString&, float, float, const UbiString&, bool,
                       const UbiString&, Entity*);
REGISTER_GAME_FUNCTION(SlideScreen, "_Z11SlideScreenP6Entitybii", Entity*, Entity*, bool, int, int);
REGISTER_GAME_FUNCTION(LogToConsole, "_Z12LogToConsolePKcz", void, const char*...);
REGISTER_GAME_FUNCTION(iPhoneMapX, "_Z10iPhoneMapXf", float, float);
REGISTER_GAME_FUNCTION(FadeInEntity, "_Z12FadeInEntityP6Entitybiif13eTimingSystem", void, Entity*, bool, int, int, float, int);
REGISTER_GAME_FUNCTION(ResizeScrollBounds, "_Z18ResizeScrollBoundsP11VariantList", void, VariantList*);

UbiAllocCtor_t g_ubi_alloc_ctor;
UbiStrCtor_t g_ubi_str_ctor;
UbiStrDtor_t g_ubi_str_dtor;

#define RESOLVE_DIR_SYMBOL(name) real::name = (name##_t)dlsym(handle, pattern::name.c_str());
namespace game
{
void GameHarness::resolveSharedSigs()
{
    g_ubi_alloc_ctor = (UbiAllocCtor_t)dlsym(handle, "_ZN11ubiservices18ContainerAllocatorIcEC1Ev");
    g_ubi_str_ctor = (UbiStrCtor_t)dlsym(handle, "_ZN11ubiservices11BasicStringIcEC2EPKcRKNS_18ContainerAllocatorIcEE");
    g_ubi_str_dtor =
        (UbiStrDtor_t)dlsym(handle, "_ZNSbIcSt11char_traitsIcEN11ubiservices18ContainerAllocatorIcEEED1Ev");
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
    RESOLVE_DIR_SYMBOL(iPhoneMapX);
    RESOLVE_DIR_SYMBOL(FadeInEntity);
    RESOLVE_DIR_SYMBOL(ResizeScrollBounds);
}
} // namespace game