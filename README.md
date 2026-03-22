<div align="center">

<img width="200" src="mod-assets/logo.png" alt="OSGT-QOL logo, created by Corrects">

# OSGT-QOL-ANDROID
</div>

OSGT-QOL-ANDROID is a quality of life mod for Growtopia 4.03 ARM64 Android clients aimed at Oldschool Growtopia players. It features various quality of life and security changes along extending several limitations of the vanilla game client.

> [!NOTE]
> The game modification is provided as a working prototype and requires heavy techincal knowledge to install at the moment.

## Table of Contents
- [Building](#building)
- [Installing](#installing)
- [What does this mod change?](#what-does-this-mod-change)
- [Credits & Contributors](#credits--contributors)

## Building
You must use **Android NDK r17c** with GNU STL to build the shared library and set an `ANDROID_NDK` environment value in your system. CMakePresets are created to ease compilation process and it integrates well with popular IDEs such as Visual Studio Code.
Once you have built the shared library, you will need to source a Growtopia V4.03 ARM64 Android client (build ID 294 on Google Play servers), unpack the app and its classes.dex and add your library to Main.smali (see [mod-assets/smali-patch.sh](mod-assets/smali-patch.sh)). Once you've done smali patching, you'll need to drop libosgtqol.so to your `lib/arm64-v8a/` folder, repack the app and sign it with your keys (can also be debug keys).

## Installing
There is no simple installation process right now, you will need technical skills to patch your app right now as the apk cannot be redistributed officially. See [Building](#Building) section for more information.

## What does this mod change?
Core Patches (always enabled):
- About Menu attribution - Adds credits for the modification & lists all the loaded in user patches

User-facing, most notable/impactful patches:
- Framerate Unlock patch - Adjusts the game's framerate limit to match the users display refresh rate. Additionally fixes some visuals caused by higher FPS
- Server Switcher - You can now connect to any server that supports V4.03 and supplies a valid SSL certificate without modifying hosts file with this mod

User-facing, visible or impactful patches:
- Legacy Chat patch - Removes the chat tabs and `[S]` symbols
- Customized Title Screen - Changes the eye-searing sunny weather on title screen to a cozy night weather & adds OSGT-QOL watermark
- Bubble Opacity Slider - Backport of the modern client bubble opacity feature
- Locale Switcher - Allows you to change your in-game country flag without having to restart your PC or game

User-facing patches that don't really impact your gameplay or are made for experimenting:
- Handle old logon handshakes - Allows the V3.02 to accept all historical logon handshake packets
- Blood Moon Demo Weather - An experimental extension to game that adds a completely new weather to the game without replacing any existing weather.

## Credits / Contributors
- Cernodile for porting the project to Android.
- osgt-qol project contributors for creating the original PC mod.