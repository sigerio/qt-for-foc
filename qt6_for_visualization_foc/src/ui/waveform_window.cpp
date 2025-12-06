#include "waveform_window.h"
#include "widgets/waveform_view.h"
#include "widgets/resizable_group.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

waveform_window::waveform_window(QWidget* parent) : QMainWindow(parent) {
    setup_ui();
}

void waveform_window::setup_ui() {
    setWindowTitle("FOC波形显示");
    setMinimumSize(800, 900);
    
    // 创建滚动区域
    m_scroll_area = new QScrollArea(this);
    m_scroll_area->setWidgetResizable(true);
    m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(m_scroll_area);
    
    // 内容区域
    m_content = new QWidget(this);
    auto* layout = new QVBoxLayout(m_content);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(5);
    
    // ========== 第1行：dq电压电流（两列）==========
    auto* row1 = new QWidget(m_content);
    auto* row1_layout = new QHBoxLayout(row1);
    row1_layout->setContentsMargins(0, 0, 0, 0);
    row1_layout->setSpacing(5);
    
    auto* group_ud_uq = new resizable_group("dq电压 (Ud/Uq)", row1);
    auto* layout_ud_uq = new QVBoxLayout(group_ud_uq);
    m_wave_ud_uq = new waveform_view(group_ud_uq);
    m_wave_ud_uq->set_channels(2, {QColor(255, 165, 0), QColor(128, 0, 128)});  // 橙、紫
    m_wave_ud_uq->set_channel_names({"Ud", "Uq"});
    layout_ud_uq->addWidget(m_wave_ud_uq);
    row1_layout->addWidget(group_ud_uq);
    
    auto* group_id_iq = new resizable_group("dq电流 (Id/Iq)", row1);
    auto* layout_id_iq = new QVBoxLayout(group_id_iq);
    m_wave_id_iq = new waveform_view(group_id_iq);
    m_wave_id_iq->set_channels(2, {QColor(255, 165, 0), QColor(128, 0, 128)});  // 橙、紫
    m_wave_id_iq->set_channel_names({"Id", "Iq"});
    layout_id_iq->addWidget(m_wave_id_iq);
    row1_layout->addWidget(group_id_iq);
    
    layout->addWidget(row1);
    
    // ========== 第2行：电流参考 | 速度 | 位置（三列）==========
    auto* row2 = new QWidget(m_content);
    auto* row2_layout = new QHBoxLayout(row2);
    row2_layout->setContentsMargins(0, 0, 0, 0);
    row2_layout->setSpacing(5);
    
    auto* group_i_ref = new resizable_group("电流参考 (Id_ref/Iq_ref)", row2);
    auto* layout_i_ref = new QVBoxLayout(group_i_ref);
    m_wave_i_ref = new waveform_view(group_i_ref);
    m_wave_i_ref->set_channels(2, {QColor(255, 165, 0), QColor(128, 0, 128)});  // 橙、紫
    m_wave_i_ref->set_channel_names({"Id_ref", "Iq_ref"});
    layout_i_ref->addWidget(m_wave_i_ref);
    row2_layout->addWidget(group_i_ref);
    
    auto* group_vel = new resizable_group("速度 (vel_ref/velocity)", row2);
    auto* layout_vel = new QVBoxLayout(group_vel);
    m_wave_vel = new waveform_view(group_vel);
    m_wave_vel->set_channels(2, {QColor(128, 128, 128), QColor(0, 100, 255)});  // 灰、蓝
    m_wave_vel->set_channel_names({"vel_ref", "velocity"});
    layout_vel->addWidget(m_wave_vel);
    row2_layout->addWidget(group_vel);
    
    auto* group_pos = new resizable_group("位置 (pos_ref/position)", row2);
    auto* layout_pos = new QVBoxLayout(group_pos);
    m_wave_pos = new waveform_view(group_pos);
    m_wave_pos->set_channels(2, {QColor(128, 128, 128), QColor(0, 200, 0)});  // 灰、绿
    m_wave_pos->set_channel_names({"pos_ref", "position"});
    layout_pos->addWidget(m_wave_pos);
    row2_layout->addWidget(group_pos);
    
    layout->addWidget(row2);
    
    // ========== 第3行：αβ电压 ==========
    auto* group_u_ab = new resizable_group("αβ电压 (Uα/Uβ)", m_content);
    auto* layout_u_ab = new QVBoxLayout(group_u_ab);
    m_wave_u_ab = new waveform_view(group_u_ab);
    m_wave_u_ab->set_channels(2, {QColor(0, 255, 255), QColor(255, 0, 255)});  // 青、品红
    m_wave_u_ab->set_channel_names({"Uα", "Uβ"});
    layout_u_ab->addWidget(m_wave_u_ab);
    layout->addWidget(group_u_ab);
    
    // ========== 第4行：αβ电流 ==========
    auto* group_i_ab = new resizable_group("αβ电流 (Iα/Iβ)", m_content);
    auto* layout_i_ab = new QVBoxLayout(group_i_ab);
    m_wave_i_ab = new waveform_view(group_i_ab);
    m_wave_i_ab->set_channels(2, {QColor(0, 255, 255), QColor(255, 0, 255)});  // 青、品红
    m_wave_i_ab->set_channel_names({"Iα", "Iβ"});
    layout_i_ab->addWidget(m_wave_i_ab);
    layout->addWidget(group_i_ab);
    
    // ========== 第5行：三相电压 ==========
    auto* group_u_abc = new resizable_group("三相电压 (Ua/Ub/Uc)", m_content);
    auto* layout_u_abc = new QVBoxLayout(group_u_abc);
    m_wave_u_abc = new waveform_view(group_u_abc);
    m_wave_u_abc->set_channels(3, {QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255)});  // 红绿蓝
    m_wave_u_abc->set_channel_names({"Ua", "Ub", "Uc"});
    layout_u_abc->addWidget(m_wave_u_abc);
    layout->addWidget(group_u_abc);
    
    // ========== 第6行：三相电流 ==========
    auto* group_i_abc = new resizable_group("三相电流 (Ia/Ib/Ic)", m_content);
    auto* layout_i_abc = new QVBoxLayout(group_i_abc);
    m_wave_i_abc = new waveform_view(group_i_abc);
    m_wave_i_abc->set_channels(3, {QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255)});  // 红绿蓝
    m_wave_i_abc->set_channel_names({"Ia", "Ib", "Ic"});
    layout_i_abc->addWidget(m_wave_i_abc);
    layout->addWidget(group_i_abc);
    
    // ========== 第7行：PWM占空比 ==========
    auto* group_pwm = new resizable_group("PWM占空比 (Ta/Tb/Tc)", m_content);
    auto* layout_pwm = new QVBoxLayout(group_pwm);
    m_wave_pwm = new waveform_view(group_pwm);
    m_wave_pwm->set_channels(3, {QColor(255, 0, 0), QColor(0, 255, 0), QColor(0, 0, 255)});  // 红绿蓝
    m_wave_pwm->set_channel_names({"Ta", "Tb", "Tc"});
    layout_pwm->addWidget(m_wave_pwm);
    layout->addWidget(group_pwm);
    
    layout->addStretch();
    m_scroll_area->setWidget(m_content);
}

void waveform_window::update_currents(double ia, double ib, double ic,
                                      double i_alpha, double i_beta,
                                      double id, double iq,
                                      double id_ref, double iq_ref) {
    if (m_wave_i_abc) m_wave_i_abc->add_points({ia, ib, ic});
    if (m_wave_i_ab) m_wave_i_ab->add_points({i_alpha, i_beta});
    if (m_wave_id_iq) m_wave_id_iq->add_points({id, iq});
    if (m_wave_i_ref) m_wave_i_ref->add_points({id_ref, iq_ref});
}

void waveform_window::update_voltages(double ud, double uq,
                                      double u_alpha, double u_beta,
                                      double ua, double ub, double uc) {
    if (m_wave_ud_uq) m_wave_ud_uq->add_points({ud, uq});
    if (m_wave_u_ab) m_wave_u_ab->add_points({u_alpha, u_beta});
    if (m_wave_u_abc) m_wave_u_abc->add_points({ua, ub, uc});
}

void waveform_window::update_pwm(double ta, double tb, double tc) {
    if (m_wave_pwm) m_wave_pwm->add_points({ta, tb, tc});
}

void waveform_window::update_velocity(double vel_ref, double velocity) {
    if (m_wave_vel) m_wave_vel->add_points({vel_ref, velocity});
}

void waveform_window::update_position(double pos_ref, double position) {
    if (m_wave_pos) m_wave_pos->add_points({pos_ref, position});
}
