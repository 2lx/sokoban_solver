file(GLOB WOS_SRC "*.cpp")
list(REMOVE_ITEM WOS_SRC "main.cpp")

add_executable(DeadlockGenerator main.cpp)
target_include_directories(DeadlockGenerator PUBLIC ../src)
target_link_libraries(DeadlockGenerator SokobanSolverLib)
