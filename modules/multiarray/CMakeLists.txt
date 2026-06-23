# ospf::multiarray — multidimensional array container
add_library(ospf_multiarray INTERFACE)
target_include_directories(ospf_multiarray INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_multiarray INTERFACE cxx_std_23)
target_link_libraries(ospf_multiarray INTERFACE ospf_base)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_multiarray_tests
    tests/test_multiarray.cpp
)
target_link_libraries(ospf_multiarray_tests PRIVATE ospf_multiarray GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_multiarray_tests)
