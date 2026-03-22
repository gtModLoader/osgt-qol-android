#include "game/signatures.hpp"
#include "dlfcn.h"
#include "game.hpp"

REGISTER_GAME_FUNCTION(GetApp, "_Z6GetAppv", App*);
REGISTER_GAME_FUNCTION(EntityAddComponent,
                       "_ZN6Entity12AddComponentESt10unique_ptrI15EntityComponentSt14default_deleteIS1_EE",
                       EntityComponent*, Entity*, std::unique_ptr<EntityComponent>);
REGISTER_GAME_FUNCTION(EntityRemoveComponentByName, "_ZN6Entity21RemoveComponentByNameERKSs", bool, Entity*,
                       std::string const&);
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
REGISTER_GAME_FUNCTION(VariantSetStr, "_ZN7Variant3SetERKSs", void, Variant*, std::string const& key);

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
REGISTER_GAME_FUNCTION(SurfaceCtor, "_ZN7SurfaceC1Ev", void*, void*);
REGISTER_GAME_FUNCTION(SurfaceDtor, "_ZN7SurfaceD1Ev", void, void*);
REGISTER_GAME_FUNCTION(SurfaceLoadFile, "_ZN7Surface8LoadFileESsb", bool, void*, std::string, bool);
REGISTER_GAME_FUNCTION(SurfaceBlitScaled, "_ZN7Surface10BlitScaledEff7CL_Vec2IfE10eAlignmentjfP13RenderBatcherbb", void,
                       void* pSurf, float x, float y, CL_Vec2f& vScale, int alignment, unsigned int rgba,
                       float rotation, void* pRenderBatcher, bool flipX, bool flipY);
REGISTER_GAME_FUNCTION(SurfaceKill, "_ZN7Surface4KillEv", void, void*);
REGISTER_GAME_FUNCTION(SurfaceAnimCtor, "_ZN11SurfaceAnimC1Ev", void*, void*);
REGISTER_GAME_FUNCTION(SurfaceAnimDtor, "_ZN11SurfaceAnimD1Ev", void, void*);
REGISTER_GAME_FUNCTION(SurfaceAnimBlitScaledAnim,
                       "_ZN11SurfaceAnim14BlitScaledAnimEffii7CL_Vec2IfE10eAlignmentjfS1_bbP13RenderBatcheri", void,
                       void* pAnimSurf, float x, float y, int frameX, int frameY, CL_Vec2f* vScale, int alignment,
                       unsigned int rgba, float rotation, CL_Vec2f* vRotationPt, bool flipX, bool flipY, void* pBatcher,
                       int padding);
REGISTER_GAME_FUNCTION(DrawFilledRect, "_Z14DrawFilledRectRK8CL_Rectfjf7CL_Vec2IfE", void, CL_Rectf const&, uint32_t,
                       float, CL_Vec2f*);
REGISTER_GAME_FUNCTION(SendPacket, "_Z10SendPacket15eNetMessageTypeRKSsP9_ENetPeer", void, int, std::string, void*);
REGISTER_GAME_FUNCTION(GetSavePath, "_Z11GetSavePathv", std::string);
REGISTER_GAME_FUNCTION(RenderBatcherFlush, "_ZN13RenderBatcher5FlushENS_10eFlushModeEm", void, void*, unsigned int,
                       int64_t);
REGISTER_GAME_GLOBAL_VAR(g_globalBatcher, "g_globalBatcher", void*);

#define RESOLVE_DIR_SYMBOL(name) real::name = (name##_t)dlsym(handle, pattern::name.c_str());
namespace game
{
void GameHarness::resolveSharedSigs()
{
    RESOLVE_DIR_SYMBOL(GetApp);
    RESOLVE_DIR_SYMBOL(EntityAddComponent);
    RESOLVE_DIR_SYMBOL(EntityRemoveComponentByName);
    RESOLVE_DIR_SYMBOL(VariantDBPrint);

    RESOLVE_DIR_SYMBOL(VariantDBGetFunction);
    RESOLVE_DIR_SYMBOL(VariantDBGetFunctionIfExists);
    RESOLVE_DIR_SYMBOL(VariantDBCallFunctionIfExists);
    RESOLVE_DIR_SYMBOL(VariantDBGetVarIfExists);
    RESOLVE_DIR_SYMBOL(VariantDBGetVarWithDefault);
    RESOLVE_DIR_SYMBOL(VariantDBGetVar);
    RESOLVE_DIR_SYMBOL(VariantDBDeleteVar);
    RESOLVE_DIR_SYMBOL(VariantDBDeleteAll);
    RESOLVE_DIR_SYMBOL(VariantSetStr);

    RESOLVE_DIR_SYMBOL(GetFontAndScaleToFitThisLinesPerScreenY);
    RESOLVE_DIR_SYMBOL(CreateTextLabelEntity);
    RESOLVE_DIR_SYMBOL(SetupTextEntity);
    RESOLVE_DIR_SYMBOL(SetTextEntity);
    RESOLVE_DIR_SYMBOL(CreateInputTextEntity);
    RESOLVE_DIR_SYMBOL(SlideScreen);
    RESOLVE_DIR_SYMBOL(LogToConsole);
    RESOLVE_DIR_SYMBOL(FadeInEntity);
    RESOLVE_DIR_SYMBOL(ResizeScrollBounds);

    RESOLVE_DIR_SYMBOL(SurfaceCtor);
    RESOLVE_DIR_SYMBOL(SurfaceDtor);
    RESOLVE_DIR_SYMBOL(SurfaceLoadFile);
    RESOLVE_DIR_SYMBOL(SurfaceBlitScaled);
    RESOLVE_DIR_SYMBOL(SurfaceKill);
    RESOLVE_DIR_SYMBOL(SurfaceAnimCtor);
    RESOLVE_DIR_SYMBOL(SurfaceAnimDtor);
    RESOLVE_DIR_SYMBOL(SurfaceAnimBlitScaledAnim);

    RESOLVE_DIR_SYMBOL(DrawFilledRect);
    RESOLVE_DIR_SYMBOL(SendPacket);
    RESOLVE_DIR_SYMBOL(GetSavePath);

    RESOLVE_DIR_SYMBOL(RenderBatcherFlush);
    real::g_globalBatcher = (void*)dlsym(handle, pattern::g_globalBatcher.c_str());
}
} // namespace game