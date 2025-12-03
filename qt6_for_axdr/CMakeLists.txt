cmake_minimum_required(VERSION 3.16)
project(qt6_for_axdr VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置UTF-8编码
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-finput-charset=UTF-8>")
add_compile_options("$<$<CXX_COMPILER_ID:GNU>:-fexec-charset=UTF-8>")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/src)

# 查找Qt6包
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Gui 
    Widgets 
    SerialPort
)

# 源文件 - 主程序
set(SOURCES
    main.cpp
)

# 串口通信模块源文件
set(SERIAL_SOURCES
    src/serial/modbus_client.cpp
    src/serial/modbus_client.h
)

# 从机模拟器模块源文件
set(SLAVE_SOURCES
    src/slave/modbus_slave.cpp
    src/slave/modbus_slave.h
    src/slave/test_data_config.cpp
    src/slave/test_data_config.h
    src/slave/slave_window.cpp
    src/slave/slave_window.h
)

# 参数管理模块源文件
set(PARAMS_SOURCES
    src/params/motor_params.cpp
    src/params/motor_params.h
    src/params/param_manager.cpp
    src/params/param_manager.h
)

# 日志模块源文件
set(LOG_SOURCES
    src/log/comm_logger.cpp
    src/log/comm_logger.h
)

# UI模块源文件
set(UI_SOURCES
    src/ui/main_window.cpp
    src/ui/main_window.h
    src/ui/serial_config_widget.cpp
    src/ui/serial_config_widget.h
    src/ui/pid_config_widget.cpp
    src/ui/pid_config_widget.h
    src/ui/motor_config_widget.cpp
    src/ui/motor_config_widget.h
)

# 资源文件
set(RESOURCES
    resources.qrc
)

# 创建可执行文件
add_executable(${PROJECT_NAME} 
    ${SOURCES} 
    ${SERIAL_SOURCES}
    ${SLAVE_SOURCES}
    ${PARAMS_SOURCES}
    ${LOG_SOURCES}
    ${UI_SOURCES}
    ${RESOURCES}
)

# 链接Qt6库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::SerialPort
)
