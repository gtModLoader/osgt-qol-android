#include "game/game.hpp"
#include "game/struct/resourceutils.hpp"
#include "patch/patch.hpp"
#include <jni.h>

REGISTER_GAME_FUNCTION(GetAppCachePath, "_Z15GetAppCachePathv", std::string);

static std::string g_packageName = "";
static std::string g_cachePath = "";
class CacheLocationFixer : public patch::BasePatch
{
  public:
    void apply() const override
    {
        auto& game = game::GameHarness::get();
        // Give our own app cachepath when game asks for it, allows to clone the game to arbitrary length package names.
        game.hookFunction<GetAppCachePath_t>(game.resolveSymbol(pattern::GetAppCachePath), GetAppCachePath,
                                             &real::GetAppCachePath);
        // Retrieve current package name with JNI
        JNIEnv* env;
        game.getJavaVM()->GetEnv((void**)&env, JNI_VERSION_1_6);
        g_packageName = getPackageName(env, getGlobalContext(env));
    }

    static jobject getGlobalContext(JNIEnv* env)
    {
        jclass activityThread = env->FindClass("android/app/ActivityThread");
        jmethodID currentMid =
            env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
        jobject obj = env->CallStaticObjectMethod(activityThread, currentMid);
        jmethodID appMid = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
        return env->CallObjectMethod(obj, appMid);
    }

    static std::string getPackageName(JNIEnv* env, jobject context)
    {
        jclass ctxClass = env->GetObjectClass(context);
        jmethodID packageMid = env->GetMethodID(ctxClass, "getPackageName", "()Ljava/lang/String;");
        jstring nameJstr = (jstring)env->CallObjectMethod(context, packageMid);
        const char* namePtr = env->GetStringUTFChars(nameJstr, nullptr);
        std::string name(namePtr);
        env->ReleaseStringUTFChars(nameJstr, namePtr);
        return name;
    }

    static std::string GetAppCachePath()
    {
        if (g_cachePath.size() == 0)
        {
            g_cachePath = real::GetAppCachePath();
            StringReplace("com.rtsoft.growtopia", g_packageName, g_cachePath);
        }
        return g_cachePath;
    }
};
REGISTER_CORE_GAME_PATCH(CacheLocationFixer, cache_location_fixer);