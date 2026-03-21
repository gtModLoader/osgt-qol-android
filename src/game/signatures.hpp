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

DEFINE_GAME_FUNCTION(GetApp, App*);
DEFINE_GAME_FUNCTION(VariantDBPrint, void, VariantDB*);
DEFINE_GAME_FUNCTION(VariantDBGetFunction, FunctionObject*, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBGetFunctionIfExists, FunctionObject*, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBCallFunctionIfExists, void, VariantDB*, std::string const& key, VariantList* pVList);
DEFINE_GAME_FUNCTION(VariantDBGetVarIfExists, Variant*, VariantDB*, std::string const&);
DEFINE_GAME_FUNCTION(VariantDBGetVarWithDefault, Variant*, VariantDB*, std::string const& key, const Variant& vDefault);
DEFINE_GAME_FUNCTION(VariantDBGetVar, Variant*, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBDeleteVar, void, VariantDB*, std::string const& key);
DEFINE_GAME_FUNCTION(VariantDBDeleteAll, void, VariantDB*);
DEFINE_GAME_FUNCTION(GetFontAndScaleToFitThisLinesPerScreenY, void, uint32_t&, float&, float);
DEFINE_GAME_FUNCTION(CreateTextLabelEntity, Entity*, Entity*, std::string const&, float, float, std::string const&);
DEFINE_GAME_FUNCTION(SetupTextEntity, Entity*, Entity*, uint32_t, float);
DEFINE_GAME_FUNCTION(SetTextEntity, void, Entity*, std::string const&);
DEFINE_GAME_FUNCTION(CreateInputTextEntity, Entity*, Entity*, std::string const&, float, float, std::string const&,
                     float, float, std::string const&, bool, std::string const&, Entity*);
DEFINE_GAME_FUNCTION(SlideScreen, Entity*, Entity*, bool, int, int);
DEFINE_GAME_FUNCTION(LogToConsole, void, const char*...);
DEFINE_GAME_FUNCTION(iPhoneMapX, float, float);
DEFINE_GAME_FUNCTION(FadeInEntity, void, Entity*, bool, int, int, float, int);
DEFINE_GAME_FUNCTION(ResizeScrollBounds, void, VariantList*);
