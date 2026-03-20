cmake_minimum_required(VERSION 3.20)

# ============================================================
# 【自动识别模式】- 自动查找目录下唯一的 cpp/hpp/ui 文件
# ============================================================

# 自动查找文件
file(GLOB MAIN_CPP "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB MAIN_HPP "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB MAIN_H   "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB MAIN_UI  "${CMAKE_CURRENT_SOURCE_DIR}/*.ui")

# 合并 .hpp 和 .h 文件
set(MAIN_HEADERS ${MAIN_HPP} ${MAIN_H})

# 从第一个 CPP 文件名提取项目名称 (去掉路径和扩展名)
list(LENGTH MAIN_CPP CPP_COUNT)
if(CPP_COUNT EQUAL 0)
    message(FATAL_ERROR "未找到任何 .cpp 文件")
endif()

list(GET MAIN_CPP 0 FIRST_CPP)
get_filename_component(PROJECT_NAME_VAR ${FIRST_CPP} NAME_WE)

# 打印识别结果
message(STATUS "========================================")
message(STATUS "自动识别结果:")
message(STATUS "  CPP 文件 (${CPP_COUNT}): ${MAIN_CPP}")
message(STATUS "  HPP/H 文件: ${MAIN_HEADERS}")
message(STATUS "  UI 文件:  ${MAIN_UI}")
message(STATUS "  项目名: ${PROJECT_NAME_VAR}")
message(STATUS "========================================")

# ============================================================

project(${PROJECT_NAME_VAR} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 宏定义
add_definitions(-D_SECURE_SCL=0 -D_CRT_SECURE_NO_WARNINGS -D_USRDLL -D_NO_BYTE -DNOMINMAX -DWIN32_LEAN_AND_MEAN -D_HAS_STD_BYTE -D_MBCS)

# 创建 DLL 目标
add_library(${PROJECT_NAME} SHARED ${MAIN_CPP} ${MAIN_HEADERS})

# 路径定义
set(HEADER_DIR   "D:/py/py/PowerCAD/PowerCAD/include")
set(LIB_DIR      "D:/py/py/PowerCAD/PowerCAD/lib/Release")

# 包含路径
target_include_directories(${PROJECT_NAME} PRIVATE ${HEADER_DIR})

# 运行时库配置
set_target_properties(${PROJECT_NAME} PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:DebugDLL>$<$<CONFIG:Release>:>"
)

# 编译选项
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:/Zi>
    $<$<CONFIG:Release>:/O2>
)

# 链接库
target_link_directories(${PROJECT_NAME} PRIVATE ${LIB_DIR})
target_link_libraries(${PROJECT_NAME} PWOpen.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")

# 复制 UI 文件到输出目录
if(MAIN_UI)
    foreach(UI_FILE ${MAIN_UI})
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${UI_FILE}"
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
            COMMENT "复制 UI 文件: ${UI_FILE}"
        )
    endforeach()
endif()