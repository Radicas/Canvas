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

add_library(${LIB_NAME} STATIC
        ${HEADERS}
        ${SOURCES}
)

target_include_directories(${LIB_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/editor
)

target_link_libraries(${LIB_NAME}
        Qt5::Core
        Qt5::Widgets
)