#pragma once
#include "game/struct/app.hpp"

#define DEFINE_GAME_FUNCTION(name, ret, ...)                                                                           \
    using name##_t = ret (*)(__VA_ARGS__);                                                                             \
    namespace real                                                                                                     \
    {                                                                                                                  \
    extern name##_t name;                                                                                              \
    }
#define DEFINE_GAME_GLOBAL_VAR(name, ret)                                                                              \
    namespace real                                                                                                     \
    {                                                                                                                  \
    extern ret name;                                                                                                   \
    }

// Defined in options.cpp
DEFINE_GAME_FUNCTION(GetScreenSizeXf, float);
DEFINE_GAME_FUNCTION(GetScreenSizeYf, float);
DEFINE_GAME_FUNCTION(iPadMapX, float, float);
DEFINE_GAME_FUNCTION(iPadMapY, float, float);
DEFINE_GAME_FUNCTION(iPhoneMapX, float, float);
DEFINE_GAME_FUNCTION(iPhoneMapY, float, float);
DEFINE_GAME_FUNCTION(AddFocusIfNeeded, void, Entity*, bool, int);
DEFINE_GAME_FUNCTION(TouchHandlerComponent, EntityComponent*, void*);
DEFINE_GAME_FUNCTION(TouchHandlerArcadeComponent, EntityComponent*, void*);
DEFINE_GAME_FUNCTION(FilterInputComponent, EntityComponent*, void*);
DEFINE_GAME_FUNCTION(MessageManagerCallEntityFunction, void, void*, Entity*, int, std::string const&, VariantList*,
                     int);
DEFINE_GAME_FUNCTION(GetMessageManager, void*);
DEFINE_GAME_FUNCTION(DisableAllButtonsEntity, void, Entity*, bool);
DEFINE_GAME_FUNCTION(OptionsMenuCreate, void, Entity*, bool);
DEFINE_GAME_FUNCTION(AddHotkeyToButton, void, Entity*, int);
DEFINE_GAME_FUNCTION(SetButtonStyleEntity, void, Entity*, int);
// Defined in signatures.cpp
DEFINE_GAME_FUNCTION(GetApp, App*);
DEFINE_GAME_FUNCTION(EntityAddComponent, EntityComponent*, Entity*, std::unique_ptr<EntityComponent>);
DEFINE_GAME_FUNCTION(EntityRemoveComponentByName, bool, Entity*, std::string const&);
DEFINE_GAME_FUNCTION(VariantDBPrint, void, VariantDB*);
DEFINE_GAME_FUNCTION(VariantDBGetFunction, FunctionObject*, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBGetFunctionIfExists, FunctionObject*, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBCallFunctionIfExists, void, VariantDB*, std::string const& key, VariantList* pVList);
DEFINE_GAME_FUNCTION(VariantDBGetVarIfExists, Variant*, VariantDB*, std::string const&);
DEFINE_GAME_FUNCTION(VariantDBGetVarWithDefault, Variant*, VariantDB*, std::string const& key, const Variant& vDefault);
DEFINE_GAME_FUNCTION(VariantDBGetVar, Variant*, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBDeleteVar, void, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBDeleteAll, void, VariantDB*);
DEFINE_GAME_FUNCTION(VariantSetStr, void, Variant*, std::string const& key);
DEFINE_GAME_FUNCTION(GetFontAndScaleToFitThisLinesPerScreenY, void, uint32_t&, float&, float);
DEFINE_GAME_FUNCTION(CreateTextLabelEntity, Entity*, Entity*, std::string const&, float, float, std::string const&);
DEFINE_GAME_FUNCTION(SetupTextEntity, Entity*, Entity*, uint32_t, float);
DEFINE_GAME_FUNCTION(SetTextEntity, void, Entity*, std::string const&);
DEFINE_GAME_FUNCTION(CreateInputTextEntity, Entity*, Entity*, std::string const&, float, float, std::string const&,
                     float, float, std::string const&, bool, std::string const&, Entity*);
DEFINE_GAME_FUNCTION(SlideScreen, Entity*, Entity*, bool, int, int);
DEFINE_GAME_FUNCTION(LogToConsole, void, const char*...);
DEFINE_GAME_FUNCTION(FadeInEntity, void, Entity*, bool, int, int, float, int);
DEFINE_GAME_FUNCTION(ResizeScrollBounds, void, VariantList*);
DEFINE_GAME_FUNCTION(SurfaceCtor, void*, void*);
DEFINE_GAME_FUNCTION(SurfaceDtor, void, void*);
DEFINE_GAME_FUNCTION(SurfaceLoadFile, bool, void*, std::string, bool);
DEFINE_GAME_FUNCTION(SurfaceBlitScaled, void, void* pSurf, float x, float y, CL_Vec2f& vScale, int alignment,
                     unsigned int rgba, float rotation, void* pRenderBatcher, bool flipX, bool flipY);
DEFINE_GAME_FUNCTION(SurfaceKill, void, void*);
DEFINE_GAME_FUNCTION(SurfaceAnimCtor, void*, void*);
DEFINE_GAME_FUNCTION(SurfaceAnimDtor, void, void*);
DEFINE_GAME_FUNCTION(SurfaceAnimBlitScaledAnim, void, void* pAnimSurf, float x, float y, int frameX, int frameY,
                     CL_Vec2f* vScale, int alignment, unsigned int rgba, float rotation, CL_Vec2f* vRotationPt,
                     bool flipX, bool flipY, void* pBatcher, int padding);
DEFINE_GAME_FUNCTION(DrawFilledRect, void, CL_Rectf const&, uint32_t, float, CL_Vec2f*);
DEFINE_GAME_FUNCTION(SendPacket, void, int, std::string, void*);
DEFINE_GAME_FUNCTION(AppGetCachedFileName, std::string*, App*, std::string*, std::string*);