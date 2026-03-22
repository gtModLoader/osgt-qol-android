cmake_minimum_required(VERSION 3.25)

# Make sure to always include the version patch number
project(osgt-qol-android LANGUAGES CXX C VERSION 0.50.0)

# In case somebody isn't using our presets
option(DEVELOPMENT_BUILD "Enable development features" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ANDROID_STL gnustl_static)

add_subdirectory(vendor)
add_subdirectory(src)
