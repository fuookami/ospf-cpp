# ospf::framework::csp1d — 1D Cutting Stock Problem domain framework
add_library(ospf_framework_csp1d INTERFACE)
target_include_directories(ospf_framework_csp1d INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_framework_csp1d INTERFACE cxx_std_23)
target_link_libraries(ospf_framework_csp1d INTERFACE ospf_framework)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_framework_csp1d_tests
    tests/test_csp1d.cpp
)
target_link_libraries(ospf_framework_csp1d_tests PRIVATE ospf_framework_csp1d GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_framework_csp1d_tests)
