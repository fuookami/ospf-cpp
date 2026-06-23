# ospf::core — symbol, variable, model, solver
add_library(ospf_core INTERFACE)
target_include_directories(ospf_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_core INTERFACE cxx_std_23)
target_link_libraries(ospf_core INTERFACE ospf_base ospf_math ospf_quantities)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_core_tests
    tests/test_core.cpp
    tests/test_solver.cpp
)
target_link_libraries(ospf_core_tests PRIVATE ospf_core GTest::gtest GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(ospf_core_tests)
