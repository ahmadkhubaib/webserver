cmake_minimum_required(VERSION 3.16)
project(WebServer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
# Include directories (from "includePath")
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/boost/boost)

# Add Boost library path
link_directories(${CMAKE_SOURCE_DIR}/thirdparty/boost/lib)

# Add source files
set(SOURCES
        src/http_client/main.cpp
        # Add other source files here
)

add_executable(tester ${SOURCES})

# Link Boost libraries
target_link_libraries(tester PRIVATE boost_system boost_thread)
target_include_directories(tester PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Set RPATH for runtime
set_target_properties(tester PROPERTIES
        BUILD_RPATH ${CMAKE_SOURCE_DIR}/thirdparty/boost/lib
)
set_target_properties(tester PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/output
)
# Optional: Set architecture and libc++ if needed (from "compilerPath" args)
if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set_target_properties(tester PROPERTIES
            COMPILE_OPTIONS "-arch;arm64;-stdlib=libc++"
            LINK_OPTIONS "-arch;arm64;-stdlib=libc++"
    )
endif()

# Add defines if needed (from "defines")
# target_compile_definitions(tester PRIVATE YOUR_DEFINE)
