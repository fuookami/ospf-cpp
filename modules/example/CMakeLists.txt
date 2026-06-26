# ospf::example — demo 模块
add_library(ospf_example INTERFACE)
target_include_directories(ospf_example INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_example INTERFACE cxx_std_23)
target_link_libraries(ospf_example INTERFACE ospf_core ospf_framework)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_example_tests
    tests/test_example.cpp
)
target_link_libraries(ospf_example_tests PRIVATE ospf_example GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_example_tests)
