set(LIB_NAME editor)


file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED
        Core
        Widgets
)

add_library(${LIB_NAME} SHARED
        ${HEADERS}
        ${SOURCES}
)

target_include_directories(${LIB_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/editor
        ${CMAKE_SOURCE_DIR}/submodule
)

target_link_libraries(${LIB_NAME}
        Qt5::Core
        Qt5::Widgets
        redcgl
)

# 安装目标文件
install(TARGETS ${LIB_NAME}
        RUNTIME DESTINATION ${INSTALL_PATH}/bin
        LIBRARY DESTINATION ${INSTALL_PATH}/lib
        ARCHIVE DESTINATION ${INSTALL_PATH}/lib
)