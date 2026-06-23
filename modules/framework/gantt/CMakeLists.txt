# ospf::framework::gantt — Gantt scheduling domain framework
add_library(ospf_framework_gantt INTERFACE)
target_include_directories(ospf_framework_gantt INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_framework_gantt INTERFACE cxx_std_23)
target_link_libraries(ospf_framework_gantt INTERFACE ospf_framework)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_framework_gantt_tests
    tests/test_gantt.cpp
)
target_link_libraries(ospf_framework_gantt_tests PRIVATE ospf_framework_gantt GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_framework_gantt_tests)
