set(EXE_NAME Canvas)

add_executable(${EXE_NAME}
        main.cpp
)

include_directories(${CMAKE_SOURCE_DIR}/libs)

target_link_libraries(${EXE_NAME}
        editor
)

# 安装目标文件
install(TARGETS ${EXE_NAME}
        RUNTIME DESTINATION ${INSTALL_PATH}/bin
        LIBRARY DESTINATION ${INSTALL_PATH}/lib
        ARCHIVE DESTINATION ${INSTALL_PATH}/lib
)