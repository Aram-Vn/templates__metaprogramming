# CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(test)

#add compile_commands.json for vscode
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# Add the source files
set(SOURCES
    src/test.cpp
)

# Create an executable
add_executable(test.out ${SOURCES})

# Set the C++ version to 20
target_compile_features(test.out PRIVATE cxx_std_20)