cmake_minimum_required(VERSION 3.10)

add_library(and64inline STATIC 
    And64InlineHook.cpp
    And64InlineHook.hpp
)

target_include_directories(and64inline PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(and64inline)