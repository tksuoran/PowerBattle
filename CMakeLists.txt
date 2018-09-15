cmake_minimum_required(VERSION 3.12)

project(PowerBattle)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." TRUE)
if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

add_executable(powerbattle
    src/area.cpp
    src/color.cpp
    src/combat_report.cpp
    src/coordinate.cpp
    src/file_util.cpp
    src/game_defs.cpp
    src/game_shapes.cpp
    src/game.cpp
    src/globals.cpp
    src/hotbar.cpp
    src/map_editor.cpp
    src/map_view.cpp
    src/map.cpp
    src/packer.cpp
    src/pb.cpp
    src/pixel_lookup.cpp
    src/player.cpp
    src/renderer.cpp
    src/terrain_palette.cpp
    src/terrain_type.cpp
    src/texture_util.cpp
    src/unit_type.cpp
    src/unit.cpp
    src/vec2.cpp)

# Default build is to enable all safe optimizations (-O3, LTO)
# If debugging needed, you can override this with
# cmake -D CMAKE_BUILD_TYPE=Debug ..

include(CheckIPOSupported)

# Optional LTO. Do not use LTO if it's not supported by compiler.
check_ipo_supported(RESULT result OUTPUT output)
if (result)
    set_property(TARGET powerbattle PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "LTO is not supported: ${output}")
endif()

# If no build type set, default to Release build
set(default_build_type "Release")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Default to full warnings
target_compile_options(powerbattle
    PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall;-Werror>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

# Default to C++20 -- no effect on C code
target_compile_features(powerbattle PUBLIC cxx_std_20)

# ASAN
# set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
# set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")

set_target_properties(powerbattle
    PROPERTIES
    CXX_STANDARD          20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS         "${WARNING_FLAGS}"
)

# Provide option to force O3 for RelWithDebInfo
if (RELWITHDEBINFO_FORCE_OPTIMIZATION_O3)
    string(REPLACE "-O2" "-O3" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    string(REPLACE "-O2" "-O3" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
endif()

# Directory all assets are contained in
set(ASSETS "${PROJECT_SOURCE_DIR}/data")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

add_subdirectory(external/fmt EXCLUDE_FROM_ALL)

# JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(external/nlohmann_json)

target_include_directories(powerbattle
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(powerbattle
    SYSTEM PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIR}
    ${SDL2_IMAGE_DIRS}
    external/GSL/include
    external/fmt/include
    external/nlohmann_json/include
    external/cista
)

# CMake-built SDL2 doesn't define SDL2_LIBRARIES while autotools-built SDL2 does
if (NOT DEFINED SDL2_LIBRARIES)
    set(SDL2_LIBRARIES "SDL2::SDL2")
endif()

target_link_libraries(powerbattle
    PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    fmt::fmt
    nlohmann_json::nlohmann_json
)

find_program(
    CLANG_TIDY_EXE
    NAMES "clang-tidy"
    DOC "Path to clang-tidy executable"
)

# if (NOT CLANG_TIDY_EXE)
#     message(STATUS "clang-tidy not found.")
# else()
#     message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
#     set(DO_CLANG_TIDY "${CLANG_TIDY_EXE}" "-checks=*,-clang-analyzer-alpha.*")
# endif()
# 
# if (CLANG_TIDY_EXE)
#     message(STATUS "Use of clang-tidy enabled.")
#     set_target_properties(powerbattle PROPERTIES CXX_CLANG_TIDY "${DO_CLANG_TIDY}")
# endif()
