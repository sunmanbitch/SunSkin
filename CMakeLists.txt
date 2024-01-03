cmake_minimum_required(VERSION 3.16)

project(SunSkin)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PROJECT_NAME sunskin)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90
)
FetchContent_MakeAvailable(imgui)

file(GLOB ALL_FILES
  "sources/*"
  "SDK/*.cpp"
  ${imgui_SOURCE_DIR}/*.cpp
  ${imgui_SOURCE_DIR}/backends/*win32.cpp
  ${imgui_SOURCE_DIR}/backends/*dx9.cpp
  ${imgui_SOURCE_DIR}/backends/*dx11.cpp
)

add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/SDK
    ${CMAKE_CURRENT_SOURCE_DIR}/translate
    ${imgui_SOURCE_DIR}/backends
    ${imgui_SOURCE_DIR}
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
