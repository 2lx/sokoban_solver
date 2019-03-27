file(GLOB SRC "*.cpp")
list(REMOVE_ITEM SRC "main.cpp")

add_executable(DeadlockGenerator ${SRC} main.cpp)
target_include_directories(DeadlockGenerator PUBLIC ../src ../src/deadlocks)
target_link_libraries(DeadlockGenerator SokobanSolverLib SokobanDeadlockLib)
