cmake_minimum_required(VERSION 3.16)

project(SunSkin)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME sunskin)

file(GLOB ALL_FILES
  "headers/*"
  "sources/*"
  "SDK/*"
  "imgui/*"
  "translate/*"
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/SDK
    ${CMAKE_CURRENT_SOURCE_DIR}/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/translate
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    INTERPROCEDURAL_OPTIMIZATION      "TRUE"
    MSVC_RUNTIME_LIBRARY              "MultiThreaded"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    /MP;
    /sdl-;
    /GF;
    /w;
    /fp:except-;
    /O2;
    /GR-;
    /EHsc;
    /GS-;
    /permissive-;
    /std:c++latest;
    /Oi;
    /arch:AVX2;
    /Gy;
    /Y-
)

target_link_options(${PROJECT_NAME} PRIVATE
    /MANIFEST:NO;
    /SUBSYSTEM:WINDOWS;
    /OPT:REF;
    /OPT:ICF;
    /INCREMENTAL:NO
)
