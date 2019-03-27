file(GLOB SRC "*.cpp")

add_library(SokobanDeadlockLib ${SRC})
target_include_directories(SokobanDeadlockLib PUBLIC ../common)
