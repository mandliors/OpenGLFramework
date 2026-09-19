cmake_minimum_required(VERSION 3.21)
project(OpenGLFramework)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(OpenGLFramework STATIC)

include(FetchContent)

# ------------------ GLFW ------------------
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
)

FetchContent_MakeAvailable(glfw)

# ---------------- OpenMesh ----------------
FetchContent_Declare(
    OpenMesh
    GIT_REPOSITORY https://github.com/mandliors/openmesh
    GIT_TAG master
)

FetchContent_MakeAvailable(OpenMesh)

target_include_directories(OpenMeshCore INTERFACE "${openmesh_SOURCE_DIR}/src")
target_include_directories(OpenMeshTools INTERFACE "${openmesh_SOURCE_DIR}/src")

target_compile_definitions(OpenMeshCore INTERFACE _USE_MATH_DEFINES NOMINMAX)
target_compile_definitions(OpenMeshTools INTERFACE _USE_MATH_DEFINES NOMINMAX)

# ---------------- ImGui -------------------
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

# ---------------- Sources -----------------
file(GLOB_RECURSE FRAMEWORK_SOURCES CONFIGURE_DEPENDS
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

# ---------------- Sandbox ----------------
if(PROJECT_IS_TOP_LEVEL)
    option(BUILD_SANDBOX "Build the sandbox test application" ON)

    if(BUILD_SANDBOX)
        file(GLOB_RECURSE SANDBOX_SOURCES CONFIGURE_DEPENDS
            "sandbox/*.cpp"
            "sandbox/*.hpp"
        )

        add_executable(Sandbox)
        target_sources(Sandbox PRIVATE ${SANDBOX_SOURCES})
        target_link_libraries(Sandbox PRIVATE OpenGLFramework)

        add_custom_command(TARGET Sandbox POST_BUILD
            COMMAND
                ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/assets
                $<TARGET_FILE_DIR:Sandbox>/assets
        )
    endif()
endif()