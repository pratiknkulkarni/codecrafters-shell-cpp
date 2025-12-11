cmake_minimum_required(VERSION 3.13)

project(shell-starter-cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.hpp)

set(CMAKE_CXX_STANDARD 23) # Enable the C++23 standard

add_executable(shell ${SOURCE_FILES}
        src/utils/string_utils.cpp
        src/utils/string_utils.h
        src/utils/path_utils.cpp
        src/utils/path_utils.h
        src/commands/Command.h
        src/commands/builtin_commands.cpp
        src/commands/builtin_commands.h
        src/shell/shell.cpp
        src/shell/shell.h
)

target_link_libraries(shell PRIVATE readline)
