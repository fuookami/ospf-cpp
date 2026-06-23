# ospf::base — core types, error handling, functional, concepts
add_library(ospf_base INTERFACE)
target_include_directories(ospf_base INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_base INTERFACE cxx_std_23)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_base_tests
    tests/test_error_result.cpp
    tests/test_utils.cpp
)
target_link_libraries(ospf_base_tests PRIVATE ospf_base GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_base_tests)
