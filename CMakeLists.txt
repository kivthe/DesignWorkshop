# Compile On UNIX
# cmake CMakeLists.txt -B build -S ${PWD}
# make -C build -j4

# Compile On Windows
# cmake CMakeLists.txt -G "MinGW Makefiles" -B build -S %cd%
# mingw32-make -C build -j4


cmake_minimum_required(VERSION 3.21)
project(DesignWorkshop LANGUAGES CXX VERSION 1.0.0.0)
set(output_name "${PROJECT_NAME}-${PROJECT_VERSION}")

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
  src/DevConsole.cpp
)

IF(${CMAKE_BUILD_TYPE} STREQUAL "")
set(CMAKE_BUILD_TYPE "Release")
ENDIF()
IF(${CMAKE_BUILD_TYPE} STREQUAL "Release")
set(options
  -Wall
  -Wextra
  -Wpedantic
  -std=c++17
  -O2
  -fno-exceptions
)
set(build_type_defs
  NDEBUG
)
ELSEIF(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
set(options
  -Wall
  -Wextra
  -Wpedantic
  -std=c++17
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
  -std=c++17
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
  -std=c++17
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