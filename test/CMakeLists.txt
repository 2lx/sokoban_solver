find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${TEST_SOURCE_DIR}
                    ../src
                    ../src/sparse_graph
                    ../src/common
                    ${Boost_INCLUDE_DIRS}
                    )

add_definitions(-DBOOST_TEST_DYN_LINK)

add_library(SSTestLib test_solver_common.cpp)
add_executable(SS1Test test_solver_1.cpp)
target_link_libraries(SS1Test SSTestLib SokobanSolverLib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable(SSOriginalTest test_solver_original.cpp)
target_link_libraries(SSOriginalTest SSTestLib SokobanSolverLib ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable(SPQueueTest test_stable_priority_queue.cpp)
target_link_libraries(SPQueueTest ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable(ZobristHashTest test_zobrist_hash.cpp)
target_link_libraries(ZobristHashTest ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_executable(SparseGraphTest test_sparse_graph.cpp)
target_link_libraries(SparseGraphTest ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(NAME SS1Test COMMAND SS1Test WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_test(NAME SSOriginalTest COMMAND SSOriginalTest WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
add_test(NAME SPQueueTest COMMAND SPQueueTest)
add_test(NAME ZobristHashTest COMMAND ZobristHashTest)
add_test(NAME SparseGraphTest COMMAND SparseGraphTest)

set_target_properties(SS1Test SSOriginalTest SPQueueTest ZobristHashTest SparseGraphTest
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/test")

