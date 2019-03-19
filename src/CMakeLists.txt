file(GLOB WOS_SRC "*.cpp")
list(REMOVE_ITEM WOS_SRC "main.cpp")

add_library(SokobanSolverLib ${WOS_SRC})
target_include_directories(SokobanSolverLib PUBLIC common sparse_graph)
add_executable(SokobanSolver main.cpp)
target_link_libraries(SokobanSolver SokobanSolverLib)
