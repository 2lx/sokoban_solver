file(GLOB WOS_SRC "*.cpp")
list(REMOVE_ITEM WOS_SRC "main.cpp")

add_library(SokobanLib ${WOS_SRC})
target_include_directories(SokobanLib PRIVATE common)
add_executable(SokobanSolver main.cpp)
target_link_libraries(SokobanSolver SokobanLib)

# add_executable(SudokuLogging EXCLUDE_FROM_ALL ${WOS_SRC} main.cpp)
# target_compile_definitions(SudokuLogging PRIVATE LOGGING)
# target_include_directories(SudokuLogging PRIVATE common)
