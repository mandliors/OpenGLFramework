cmake_minimum_required(VERSION 3.15)
project(OpenGLFramework)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(OpenGLFramework STATIC)

include(FetchContent)

# ---------------- GLFW ----------------
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

FetchContent_MakeAvailable(glfw)

# ---------------- OpenMesh ----------------
FetchContent_Declare(
    OpenMesh
    GIT_REPOSITORY https://github.com/Lawrencemm/openmesh
    GIT_TAG master
    PATCH_COMMAND git apply "${CMAKE_SOURCE_DIR}/openmesh_subdiv_mod.patch"
)

FetchContent_MakeAvailable(OpenMesh)

target_include_directories(OpenMeshCore INTERFACE "${openmesh_SOURCE_DIR}/src")
target_include_directories(OpenMeshTools INTERFACE "${openmesh_SOURCE_DIR}/src")

target_compile_definitions(OpenMeshCore INTERFACE _USE_MATH_DEFINES NOMINMAX)
target_compile_definitions(OpenMeshTools INTERFACE _USE_MATH_DEFINES NOMINMAX)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui")
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
)
set(IMGUI_BACKEND_DIR "${IMGUI_DIR}/backends")
list(APPEND IMGUI_SOURCES
    ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

# ---------------- sources ----------------
file(GLOB_RECURSE FRAMEWORK_SOURCES
    "src/*.cpp"
    "src/*.hpp"
)

target_sources(OpenGLFramework PRIVATE
    ${FRAMEWORK_SOURCES}
    ${IMGUI_SOURCES}
    vendor/glad/glad.c
)

target_include_directories(OpenGLFramework PUBLIC
    src
    vendor
    vendor/imgui
    vendor/imgui/backends
)

target_link_libraries(OpenGLFramework PUBLIC
    glfw
    OpenMeshCore
    OpenMeshTools
)
if (WIN32)
    target_link_libraries(OpenGLFramework PUBLIC opengl32)
elseif(UNIX)
    target_link_libraries(OpenGLFramework PUBLIC GL)
endif()
