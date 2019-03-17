find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${TEST_SOURCE_DIR}
                    ../src
                    ${Boost_INCLUDE_DIRS}
                    )

add_definitions(-DBOOST_TEST_DYN_LINK)

add_library(SSTestLib test_common.cpp)
add_executable(SS1Test test_ss1.cpp)
target_link_libraries(SS1Test SSTestLib SokobanSolverLib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(NAME SS1Test COMMAND SS1Test)

set_target_properties(SS1Test
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")

