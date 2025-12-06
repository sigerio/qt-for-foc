cmake_minimum_required(VERSION 3.16)
project(foc_visualization LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 查找Qt6组件
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
)

# 源文件
set(CORE_SOURCES
    src/core/types.h
    src/core/i_motor_model.h
    src/core/pmsm_model.h
    src/core/pmsm_model.cpp
    src/core/bldc_model.h
    src/core/bldc_model.cpp
    src/core/i_transform.h
    src/core/transform.h
    src/core/transform.cpp
    src/core/svpwm.h
    src/core/svpwm.cpp
    src/core/i_pid_controller.h
    src/core/pid_controller.h
    src/core/pid_controller.cpp
    src/core/sim_engine.h
    src/core/sim_engine.cpp
    src/core/data_buffer.h
    src/core/data_buffer.cpp
    src/core/motor_model_factory.h
    src/core/motor_model_factory.cpp
    src/core/config_loader.h
    src/core/config_loader.cpp
)

set(CONTROL_SOURCES
    src/control/sim_controller.h
    src/control/sim_controller.cpp
    src/control/i_loop_controller.h
    src/control/loop_controller.h
    src/control/loop_controller.cpp
    src/control/six_step_controller.h
    src/control/six_step_controller.cpp
)

set(UI_SOURCES
    src/ui/foc_visualization_window.h
    src/ui/foc_visualization_window.cpp
    src/ui/control_loop_window.h
    src/ui/control_loop_window.cpp
    src/ui/waveform_window.h
    src/ui/waveform_window.cpp
    src/ui/algorithm_explanation_window.h
    src/ui/algorithm_explanation_window.cpp
    src/ui/panels/coord_transform_panel.h
    src/ui/panels/coord_transform_panel.cpp
    src/ui/panels/svpwm_panel.h
    src/ui/panels/svpwm_panel.cpp
    src/ui/panels/motor_state_panel.h
    src/ui/panels/motor_state_panel.cpp
    src/ui/panels/current_loop_panel.h
    src/ui/panels/current_loop_panel.cpp
    src/ui/panels/velocity_loop_panel.h
    src/ui/panels/velocity_loop_panel.cpp
    src/ui/panels/position_loop_panel.h
    src/ui/panels/position_loop_panel.cpp
    src/ui/panels/motor_params_panel.h
    src/ui/panels/motor_params_panel.cpp
    src/ui/panels/pid_config_panel.h
    src/ui/panels/pid_config_panel.cpp
    src/ui/panels/control_toolbar_panel.h
    src/ui/panels/control_toolbar_panel.cpp
    src/ui/panels/motor_model_explanation_panel.h
    src/ui/panels/motor_model_explanation_panel.cpp
    src/ui/panels/transform_explanation_panel.h
    src/ui/panels/transform_explanation_panel.cpp
    src/ui/panels/svpwm_explanation_panel.h
    src/ui/panels/svpwm_explanation_panel.cpp
    src/ui/panels/control_loop_explanation_panel.h
    src/ui/panels/control_loop_explanation_panel.cpp
    src/ui/widgets/vector_scope.h
    src/ui/widgets/vector_scope.cpp
    src/ui/widgets/waveform_view.h
    src/ui/widgets/waveform_view.cpp
    src/ui/widgets/rotor_animator.h
    src/ui/widgets/rotor_animator.cpp
    src/ui/widgets/step_indicator.h
    src/ui/widgets/step_indicator.cpp
    src/ui/widgets/hall_indicator.h
    src/ui/widgets/hall_indicator.cpp
    src/ui/widgets/hexagon_widget.h
    src/ui/widgets/hexagon_widget.cpp
    src/ui/widgets/coordinate_widget.h
    src/ui/widgets/coordinate_widget.cpp
    src/ui/widgets/circuit_widget.h
    src/ui/widgets/circuit_widget.cpp
    src/ui/widgets/cascade_widget.h
    src/ui/widgets/cascade_widget.cpp
    src/ui/widgets/block_item.h
    src/ui/widgets/block_item.cpp
    src/ui/widgets/sumpoint_item.h
    src/ui/widgets/sumpoint_item.cpp
    src/ui/widgets/label_item.h
    src/ui/widgets/label_item.cpp
    src/ui/widgets/arrow_item.h
    src/ui/widgets/arrow_item.cpp
    src/ui/widgets/resizable_group.h
    src/ui/widgets/resizable_group.cpp
)

# 资源文件
set(RESOURCES
    resources/resources.qrc
)

# 可执行文件
add_executable(${PROJECT_NAME}
    main.cpp
    ${CORE_SOURCES}
    ${CONTROL_SOURCES}
    ${UI_SOURCES}
    ${RESOURCES}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 链接Qt库
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
