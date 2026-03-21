#pragma once
#include <android/log.h>

#define LOG_TAG "GROWTOPIA-OSGT-QOL-ANDROID"
#define LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define RESOLVE_SYMBOL(name) real::name = (name##_t)game.resolveSymbol(pattern::name);