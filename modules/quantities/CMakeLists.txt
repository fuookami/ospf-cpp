# ospf::quantities — dimension, unit, quantity, RealNumber concept
add_library(ospf_quantities INTERFACE)
target_include_directories(ospf_quantities INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_quantities INTERFACE cxx_std_23)
target_link_libraries(ospf_quantities INTERFACE ospf_base ospf_math)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_quantities_tests
    tests/test_quantities.cpp
)
target_link_libraries(ospf_quantities_tests PRIVATE ospf_quantities GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_quantities_tests)
