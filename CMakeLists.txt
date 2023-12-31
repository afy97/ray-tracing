﻿cmake_minimum_required (VERSION 3.20)

project ("RayTracing")

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_SOURCE_DIR}/include")
set(PCH_DIR "${INC_DIR}/pch")

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.cc" "${SRC_DIR}/*.c")
file(GLOB_RECURSE INC_FILES "${INC_DIR}/*.hpp" "${INC_DIR}/*.hh" "${INC_DIR}/*.h")
file(GLOB_RECURSE PCH_FILES "${PCH_DIR}/*.hpp" "${PCH_DIR}/*.hh" "${PCH_DIR}/*.h")

include_directories(${INC_DIR})

add_executable(${PROJECT_NAME} ${SRC_FILES} ${INC_FILES})

target_include_directories(
${PROJECT_NAME}
    PRIVATE
        ${Stb_INCLUDE_DIR}
)

target_link_libraries(
${PROJECT_NAME}
	PRIVATE
		opengl32
		glfw
        GLEW::GLEW
        glm::glm
)

target_precompile_headers(
${PROJECT_NAME}
    PRIVATE
        ${PCH_FILES}
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SRC_DIR}/shaders/shader.vert" "${CMAKE_BINARY_DIR}/$<CONFIG>/shaders/shader.vert"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${SRC_DIR}/shaders/shader.frag" "${CMAKE_BINARY_DIR}/$<CONFIG>/shaders/shader.frag"
)

if (WIN32) 
    set_target_properties(
        ${PROJECT_NAME}
        PROPERTIES
            LINK_FLAGS
                "/ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS"
    )
endif()
