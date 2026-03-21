#pragma once
#include "game/struct/app.hpp"
#include "game/struct/ubistring.hpp"

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

DEFINE_GAME_FUNCTION(GetApp, App*);
DEFINE_GAME_FUNCTION(VariantDBPrint, void, VariantDB*);
DEFINE_GAME_FUNCTION(VariantDBGetFunction, FunctionObject*, VariantDB*, void* key);
DEFINE_GAME_FUNCTION(VariantDBGetFunctionIfExists, FunctionObject*, VariantDB*, void* key);
DEFINE_GAME_FUNCTION(VariantDBCallFunctionIfExists, void, VariantDB*, void* key, VariantList* pVList);
DEFINE_GAME_FUNCTION(VariantDBGetVarIfExists, Variant*, VariantDB*, void*);
DEFINE_GAME_FUNCTION(VariantDBGetVarWithDefault, Variant*, VariantDB*, void* key, const Variant& vDefault);
DEFINE_GAME_FUNCTION(VariantDBGetVar, Variant*, VariantDB*, void* key);
DEFINE_GAME_FUNCTION(VariantDBDeleteVar, void, VariantDB*, void* key);
DEFINE_GAME_FUNCTION(VariantDBDeleteAll, void, VariantDB*);
DEFINE_GAME_FUNCTION(GetFontAndScaleToFitThisLinesPerScreenY, void, uint32_t&, float&, float);
DEFINE_GAME_FUNCTION(CreateTextLabelEntity, Entity*, Entity*, void*, float, float, void*);
DEFINE_GAME_FUNCTION(SetupTextEntity, Entity*, Entity*, uint32_t, float);
DEFINE_GAME_FUNCTION(SetTextEntity, void, Entity*, void*);
DEFINE_GAME_FUNCTION(CreateInputTextEntity, Entity*, Entity*, const UbiString&, float, float, const UbiString&, float,
                     float, const UbiString&, bool, const UbiString&, Entity*);
DEFINE_GAME_FUNCTION(SlideScreen, Entity*, Entity*, bool, int, int);
DEFINE_GAME_FUNCTION(LogToConsole, void, const char*...);
DEFINE_GAME_FUNCTION(iPhoneMapX, float, float);
DEFINE_GAME_FUNCTION(FadeInEntity, void, Entity*, bool, int, int, float, int);
DEFINE_GAME_FUNCTION(ResizeScrollBounds, void, VariantList*);
