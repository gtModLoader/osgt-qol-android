<div align="center">

<img width="200" src="mod-assets/logo.png" alt="OSGT-QOL logo, created by Corrects">

# OSGT-QOL-ANDROID
</div>

OSGT-QOL-ANDROID is a quality of life mod for Growtopia 4.03 ARM64 Android clients aimed at Oldschool Growtopia players,, ported from the [original PC mod for V3.02 Windows clients](https://github.com/gtModLoader/osgt-qol). It features various quality of life and security changes along extending several limitations of the vanilla game client.

> [!NOTE]
> The game modification is provided as a working prototype. This is on a far more shaky foundation compared to the PC mod, volatility is expected.

## Table of Contents
- [Building](#building)
- [Installing](#installing)
- [What does this mod change?](#what-does-this-mod-change)
- [Credits & Contributors](#credits--contributors)

## Building
You must use **Android NDK r17c** with GNU STL to build the shared library and set an `ANDROID_NDK` environment value in your system. CMakePresets are created to ease compilation process and it integrates well with popular IDEs such as Visual Studio Code.
Once you have built the shared library, you will need to source a Growtopia V4.03 ARM64 Android client (build ID 294 on Google Play servers), unpack the app and its classes.dex and add your library to Main.smali (see [mod-assets/smali-patch.sh](mod-assets/smali-patch.sh)). Once you've done smali patching, you'll need to drop libosgtqol.so to your `lib/arm64-v8a/` folder, repack the app and sign it with your keys (can also be debug keys).

## Installing
Obtain a Growtopia V4.03 ARM64 apk file from a mirror site or the Internet Archive, go to the Releases tab and download libosgtqol.so. Once you have both, you can either use [GrowSlopPatcher](https://github.com/cernodile/GrowSlopPatcher/releases) or install it manually by following the [Building](#building) guide.

## What does this mod change?
Core Patches (always enabled):
- About Menu attribution - Adds credits for the modification & lists all the loaded in user patches
- Secure Item Manager - Scans to-be-downloaded files for path traversal attempts, stopping & warning the user on detection, while this protection is less meaningful on mobile, it is useful to know what some servers owners are up to.
- Secure Battle Event - Fixes a Remote Code Execution vulnerability in Battle Event processing logic and adds a few sanitizations for BattleStatus serialization. This can otherwise be used to mess with game's internal state.
- Cache Location Fixer - Allows you to clone the app to a different package (multi-install) after you've done smali patches.

User-facing, most notable/impactful patches:
- Framerate Unlock patch - Adjusts the game's framerate limit to match the users display refresh rate. Additionally fixes some visuals caused by higher FPS
- Server Switcher - You can now connect to any server that supports V4.03 and supplies a valid SSL certificate without modifying hosts file with this mod
- Status Effects Overlay - Showcases active status effects with timers on your character just below the chat
- Resizeable Hotbar - Allows you to resize the quick tools hotbar to be up to 10 slots instead of the vanilla 4
- Buildomatica - Allows you to overlay World Planner files in-game as schematics you can follow. To use, create a folder called "schematics" in your game folder and drop your .GMSF, .gtworld and/or .GMSF files there (may require ADB or root privileges on Android 11+ and newer phones).
- Double Jump State Fixer - Fixes annoying design decision where if any state change happened on your player (e.g. got Stinky mod) would nullify your currently available double jump.

User-facing, visible or impactful patches:
- Legacy Chat patch - Removes the chat tabs and `[S]` symbols
- No Guild Icon - Shifts Gem Store margin up to be next to Friends List
- Customized Title Screen - Allows you to select from multiple different weathers to show on the tilte screen & adds OSGT-QOL watermark
- Bubble Opacity Slider - Backport of the modern client bubble opacity feature
- Da Vinci Fake ID - Masquerades any instance of old Da Vinci Wings to the newer one so it would render properly
- Extended Chat Limit - Increases the chat history limit from the original 125 up to 500
- Locale Switcher - Allows you to change your in-game country flag without having to restart your PC or game
- Instant world buttons - Makes the buttons in the world selection screen appear instantly instead of fading in after a while.
- Anchor camera to player - Always centers camera around the player including at the world borders. This is disabled by default and can be enabled in the settings in-game.

User-facing patches that don't really impact your gameplay or are made for experimenting:
- Light Optimization - The light calculation for Growtopia is rather inefficient, this makes the light calculation algorithm a lot more performant
- Handle old logon handshakes - Allows the V4.03 to accept all historical logon handshake packets
- Blood Moon Demo Weather - An experimental extension to game that adds a completely new weather to the game without replacing any existing weather

## What won't be coming from PC version
- CRC Integrity Check bypass - Not needed for V4.03 Android
- No DACL Modifications - Not applicalbe for Android
- Save & Log Location Fixer - Not applicable for Android due to security frameworks present
- Live GUI Rebuilder - There is no GUI rebuilding in Android
- Music mute fix - It seems to be fixed in vanilla game(?)
- Dialog Walking - Not really practical on touchscreen unless you have a tablet & keyboard
- Hide my GUI - Can't run Ctrl+H hotkey to show/hide, unless something similar to modern versions is done
- Any hotkey mods - Touchscreen can't input hotkeys
- Better Log Handle DPI - Android benefits from DPI being higher instead
- Allow releasing mutex - Windows specific thing
- Items.dat crashmenot - There aren't many practical usecases, but porting it is welcome if you miss it

## Credits / Contributors
- Cernodile for porting the project to Android.
- osgt-qol project contributors for creating the original PC mod.