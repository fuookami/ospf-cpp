# ospf::framework — context, pipeline, domain frameworks (bpp3d, csp1d, gantt)
add_library(ospf_framework INTERFACE)
target_include_directories(ospf_framework INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_framework INTERFACE cxx_std_23)
target_link_libraries(ospf_framework INTERFACE ospf_base ospf_core)

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
add_executable(ospf_framework_tests
    tests/test_framework.cpp
)
target_link_libraries(ospf_framework_tests PRIVATE ospf_framework GTest::gtest GTest::gtest_main nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(ospf_framework_tests)

# --- Domain frameworks ---
add_subdirectory(bpp3d)
add_subdirectory(csp1d)
add_subdirectory(gantt)
