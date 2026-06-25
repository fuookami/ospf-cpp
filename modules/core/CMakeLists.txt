# ospf::core — symbol, variable, model, solver
add_library(ospf_core INTERFACE)
target_include_directories(ospf_core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(ospf_core INTERFACE cxx_std_23)
target_link_libraries(ospf_core INTERFACE ospf_base ospf_math ospf_quantities)

# --- Gurobi adapter (optional) ---
option(OSPF_ENABLE_GUROBI "Enable Gurobi solver adapter" OFF)
if(OSPF_ENABLE_GUROBI)
    set(GUROBI_ROOT "" CACHE PATH "Gurobi installation root")
    if(GUROBI_ROOT)
        set(GUROBI_INCLUDE_DIR "${GUROBI_ROOT}/include")
        set(GUROBI_LIB_DIR "${GUROBI_ROOT}/lib")
        set(GUROBI_BIN_DIR "${GUROBI_ROOT}/bin")
        # Use gurobi_c++md2017.lib (MD runtime, VS 2017+ ABI compatible)
        set(GUROBI_LIBRARIES "${GUROBI_LIB_DIR}/gurobi_c++md2017.lib;${GUROBI_LIB_DIR}/gurobi100.lib")
        if(EXISTS "${GUROBI_INCLUDE_DIR}/gurobi_c++.h")
            message(STATUS "Gurobi SDK found at ${GUROBI_ROOT}")
            target_include_directories(ospf_core INTERFACE ${GUROBI_INCLUDE_DIR})
            target_link_libraries(ospf_core INTERFACE ${GUROBI_LIBRARIES})
            target_compile_definitions(ospf_core INTERFACE OSPF_ENABLE_GUROBI=1)
        else()
            message(WARNING "Gurobi SDK not found at ${GUROBI_ROOT}, disabling")
            set(OSPF_ENABLE_GUROBI OFF)
        endif()
    else()
        message(WARNING "GUROBI_ROOT not set, disabling Gurobi")
        set(OSPF_ENABLE_GUROBI OFF)
    endif()
endif()

# --- Tests ---
find_package(GTest CONFIG REQUIRED)
add_executable(ospf_core_tests
    tests/test_core.cpp
    tests/test_solver.cpp
    tests/test_symbol_functions.cpp
)
target_link_libraries(ospf_core_tests PRIVATE ospf_core GTest::gtest GTest::gtest_main)
if(OSPF_ENABLE_GUROBI AND GUROBI_BIN_DIR)
    # Deploy gurobi100.dll to test output directory
    file(GLOB GUROBI_DLLS "${GUROBI_BIN_DIR}/gurobi100.dll")
    foreach(DLL ${GUROBI_DLLS})
        add_custom_command(TARGET ospf_core_tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${DLL} $<TARGET_FILE_DIR:ospf_core_tests>
            COMMENT "Deploying ${DLL}")
    endforeach()
endif()
include(GoogleTest)
gtest_discover_tests(ospf_core_tests)
