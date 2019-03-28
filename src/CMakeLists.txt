file(GLOB SRC "*.cpp")
list(REMOVE_ITEM SRC "main.cpp")

add_library(SokobanSolverLib ${SRC})
target_include_directories(SokobanSolverLib PUBLIC common sparse_graph deadlocks)
target_link_libraries(SokobanSolverLib SokobanDeadlockLib)
add_executable(SokobanSolver main.cpp)
target_link_libraries(SokobanSolver SokobanSolverLib)
