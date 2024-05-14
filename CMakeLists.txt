# Set the minimum version of CMake required
cmake_minimum_required(VERSION 3.20)

# Set your project name
project(ZzRenderer)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# --- Include Directories ---
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib)

# --- Excutable Source Files --- 
# Source + Imgui + glad.c
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/src/*.cpp")

file(GLOB IMGUI_SOURCES "${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SOURCES} "${CMAKE_SOURCE_DIR}/src/glad.c")

# --- Library Linking ---
# GLFW + OpenGL + Assimp
target_link_libraries(${PROJECT_NAME} "/opt/homebrew/Cellar/glfw/3.4/lib/libglfw.3.4.dylib" "-framework OpenGL")

find_package(Assimp REQUIRED)

target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})

# Set the output directory for executables
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

add_definitions(-Wno-deprecated-declarations)

# Check if the sanitizer option is enabled
option(ENABLE_SANITIZER "Enable AddressSanitizer" OFF)

if(ENABLE_SANITIZER)
    # Add flags for AddressSanitizer
    set(SANITIZE_FLAGS "-fsanitize=address -fno-omit-frame-pointer -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SANITIZE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SANITIZE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZE_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${SANITIZE_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SANITIZE_FLAGS}")
endif()