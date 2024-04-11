# On UNIX
# cmake CMakeLists.txt -B build -S ${PWD}

# On Windows
# cmake CMakeLists.txt -B build -S %cd%


cmake_minimum_required(VERSION 3.21)
project(server LANGUAGES CXX VERSION 1.0.0.0)
set(output_name "${PROJECT_NAME}")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED 17)

set(libs
)

set(libdirs
)

set(defs
)

set(include
    include
    vendor
)

set(source
  src/Main.cpp
  src/Clock.cpp
  src/Logger.cpp
  src/Server.cpp
)

IF(${CMAKE_BUILD_TYPE} STREQUAL "Release")
set(options
  -O2
)
set(build_type_defs
  NDEBUG
)
ELSEIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
set(options
  -O0
  -g3
  -ggdb3
)
set(build_type_defs
)
ELSEIF(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
set(options
  -Wall
  -Wextra
  -Wpedantic
  -O2
  -g3
  -ggdb3
)
set(build_type_defs
  NDEBUG
)
ELSEIF(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
set(options
  -Wall
  -Wextra
  -Wpedantic
  -Os
)
set(build_type_defs
  NDEBUG
)
ENDIF()

add_executable(${output_name})
target_link_directories(${output_name} PRIVATE ${libdirs})
target_link_libraries(${output_name} PRIVATE ${libs})
target_compile_definitions(${output_name} PRIVATE ${defs})
target_compile_definitions(${output_name} PRIVATE ${build_type_defs})
target_include_directories(${output_name} PRIVATE ${include})
target_sources(${output_name} PRIVATE ${source})
target_compile_options(${output_name} PRIVATE ${options})
set_target_properties(${output_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "bin")