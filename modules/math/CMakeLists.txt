# ospf::math — algebra, geometry, symbolic, combinatorics, fractal, chaotic
add_library(ospf_math INTERFACE)
target_include_directories(ospf_math INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_math INTERFACE cxx_std_23)
target_link_libraries(ospf_math INTERFACE ospf_base ospf_multiarray)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_math_tests
    tests/test_algebra.cpp
    tests/test_geometry.cpp
    tests/test_chaotic.cpp
    tests/test_symbol.cpp
)
target_link_libraries(ospf_math_tests PRIVATE ospf_math GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_math_tests)
