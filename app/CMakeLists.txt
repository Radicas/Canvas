set(EXE_NAME Canvas)

add_executable(${EXE_NAME}
        main.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(${EXE_NAME}
        editor
)