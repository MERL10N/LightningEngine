cmake_minimum_required(VERSION 3.29)
project(LightningEngine)

set(CMAKE_CXX_STANDARD 23)

# Set output directories for binaries and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Automatically gather all .cpp and .h files for the main engine
file(GLOB_RECURSE ENGINE_CPP_FILES ${CMAKE_CURRENT_SOURCE_DIR}/LightningCore/**/*.cpp)
file(GLOB_RECURSE ENGINE_H_FILES ${CMAKE_CURRENT_SOURCE_DIR}/LightningCore/**/*.h)

set(ENGINE_FILES ${ENGINE_CPP_FILES} ${ENGINE_H_FILES})

# Organize files into folders for Xcode
foreach(file IN LISTS ENGINE_FILES)
    # Get the relative path of the file
    file(RELATIVE_PATH relative_path ${CMAKE_CURRENT_SOURCE_DIR}/LightningCore ${file})

    # Extract the directory
    get_filename_component(directory ${relative_path} PATH)

    # Add the source file to a group in Xcode
    if (directory)
        string(REPLACE "/" "\\" group ${directory}) # Convert path separators for Xcode
        source_group("${group}" FILES ${file})
    else()
        source_group("" FILES ${file}) # Files in the root folder
    endif()
endforeach()

file(GLOB SOURCE_FILES
      LightningEngine/main.cpp)

# Create a library for the core engine (optional for workspace modularity)
add_library(LightningEngineCore STATIC ${ENGINE_FILES})

# Define the executable for the game engine
add_executable(LightningEngine ${SOURCE_FILES})

# Add dependencies to the executable
target_link_libraries(LightningEngine
        LightningEngineCore
        "-framework Metal"
        "-framework MetalKit"
        "-framework AppKit"
        "-framework Foundation"
        "-framework QuartzCore" objc
)

# Set properties to support Mac bundle generation
set_target_properties(LightningEngine PROPERTIES
        MACOSX_BUNDLE TRUE
        XCODE_GENERATE_SCHEME TRUE
)

# Enable workspace generation
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add Metal-Cpp
target_include_directories(LightningEngineCore
        PRIVATE
        ThirdParty/metal-cpp
        ThirdParty/metal-cpp-extensions)

target_include_directories(LightningEngine
        PRIVATE
        ThirdParty/metal-cpp
        ThirdParty/metal-cpp-extensions)

# Enable Xcode Workspace Generation
set(CMAKE_XCODE_GENERATE_TOP_LEVEL_PROJECT_ONLY OFF)