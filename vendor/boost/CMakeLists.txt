cmake_minimum_required(VERSION 3.10)

add_library(boost_signals2 INTERFACE)
target_include_directories(boost_signals2 INTERFACE include)

add_compile_options(-Wno-deprecated-builtins)
# Set BOOST_ALL_NO_LIB to disable the boost auto-linking stuff
target_compile_definitions(boost_signals2 INTERFACE
    BOOST_ALL_NO_LIB
    BOOST_SIGNALS_NO_DEPRECATION_WARNING
    BOOST_SKIP_MSVC_VERSION_CHECK
    LIBCPP_ENABLE_CXX17_REMOVED_UNARY_BINARY_FUNCTION
)
