cmake_minimum_required(VERSION 3.21)
project(ospf-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /utf-8 /permissive- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline)
    # /std:c++latest is set via CMAKE_CXX_STANDARD=23 above
endif()

enable_testing()

# --- Modules ---
add_subdirectory(modules/base)
add_subdirectory(modules/multiarray)
add_subdirectory(modules/math)
add_subdirectory(modules/quantities)
add_subdirectory(modules/core)
add_subdirectory(modules/framework)

# --- Smoke test ---
find_package(GTest CONFIG REQUIRED)
add_executable(smoke_test tests/smoke_test.cpp)
target_link_libraries(smoke_test PRIVATE GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(smoke_test)

# --- Examples ---
add_executable(core_modeling examples/core_modeling.cpp)
target_link_libraries(core_modeling PRIVATE ospf_core)

add_executable(bpp3d_demo examples/bpp3d_demo.cpp)
target_link_libraries(bpp3d_demo PRIVATE ospf_framework_bpp3d)

add_executable(csp1d_demo examples/csp1d_demo.cpp)
target_link_libraries(csp1d_demo PRIVATE ospf_framework_csp1d)

add_executable(gantt_demo examples/gantt_demo.cpp)
target_link_libraries(gantt_demo PRIVATE ospf_framework_gantt)
