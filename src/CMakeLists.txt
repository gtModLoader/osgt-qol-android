set(SOURCES
    main.cpp
    game/game.cpp
    game/signatures.cpp
    game/struct/variantdb.cpp
    patch/patch.cpp
)

set(SOURCES_USER_PATCHES
    patch/user/framerate.cpp
    patch/user/net.cpp
    patch/user/render.cpp
)

set (SOURCES_CORE_PATCHES
    patch/core/about.cpp
)

add_library(${PROJECT_NAME} SHARED
    ${SOURCES}
    ${SOURCES_CORE_PATCHES}
    ${SOURCES_USER_PATCHES}
)
# Find Android log library
find_library(
    log-lib
    log
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "osgtqol"
    SUFFIX ".so"
)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "")

# Display version format for visual elements
if (PROJECT_VERSION_PATCH EQUAL 0)
    set(PROJECT_DISPLAY_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
else()
    set(PROJECT_DISPLAY_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
endif()

configure_file(version.h.in "${CMAKE_CURRENT_BINARY_DIR}/version.h" @ONLY)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

# /W4 is too noisy
#target_compile_options(${PROJECT_NAME} PRIVATE /W3)

target_link_libraries(${PROJECT_NAME} PRIVATE
${log-lib}
and64inline
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<BOOL:${DEVELOPMENT_BUILD}>:OSGT_QOL_DEVELOPMENT>
    _GLIBCXX_USE_CXX11_ABI=1
)
