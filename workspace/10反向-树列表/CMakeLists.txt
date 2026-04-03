cmake_minimum_required(VERSION 3.20)

file(GLOB MAIN_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB MAIN_HPP "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB MAIN_H "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB MAIN_UI "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")

set(MAIN_HEADERS ${MAIN_HPP} ${MAIN_H})

list(LENGTH MAIN_CPP CPP_COUNT)
if(CPP_COUNT EQUAL 0)
    message(FATAL_ERROR "No .cpp files were found in the current source directory")
endif()

list(GET MAIN_CPP 0 FIRST_CPP)
get_filename_component(PROJECT_NAME_VAR ${FIRST_CPP} NAME_WE)

message(STATUS "========================================")
message(STATUS "Auto-detected project files:")
message(STATUS "  CPP files (${CPP_COUNT}): ${MAIN_CPP}")
message(STATUS "  Header files: ${MAIN_HEADERS}")
message(STATUS "  UI files: ${MAIN_UI}")
message(STATUS "  Project name: ${PROJECT_NAME_VAR}")
message(STATUS "========================================")

project(${PROJECT_NAME_VAR} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-D_SECURE_SCL=0 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_NO_BYTE -DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_HAS_STD_BYTE -D_MBCS)

add_library(${PROJECT_NAME} SHARED ${MAIN_CPP} ${MAIN_HEADERS})

set(HEADER_DIR "D:/powercad/cad3.27/Release/PWOPEN/include")
set(LIB_DIR "D:/powercad/cad3.27/Release/PWOPEN/lib")

target_include_directories(${PROJECT_NAME} PRIVATE ${HEADER_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>$<$<CONFIG:Release>:>"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    /utf-8
    $<$<CONFIG:Debug>:/Zi>
    $<$<CONFIG:Release>:/O2>
)

target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})
target_link_libraries(${PROJECT_NAME} PWOpen.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")

if(MAIN_UI)
    foreach(UI_FILE ${MAIN_UI})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${UI_FILE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMENT "Copying UI file: ${UI_FILE}"
        )
    endforeach()
endif()
