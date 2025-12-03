#include "svpwm.h"
#include <cmath>
#include <algorithm>

// SVPWM算法实现
// 基于七段式调制，采用中心对齐PWM
void svpwm::calc(double u_alpha, double u_beta, double udc,
                 double& Ta, double& Tb, double& Tc) {
    // 保存矢量信息
    m_mag = std::sqrt(u_alpha * u_alpha + u_beta * u_beta);
    m_angle = std::atan2(u_beta, u_alpha);
    if (m_angle < 0) m_angle += TWO_PI;

    // 判断扇区
    m_sector = calc_sector(u_alpha, u_beta);

    // 计算U1, U2, U3用于确定基本矢量作用时间
    double U1 = u_beta;
    double U2 = (SQRT3_DIV_2 * u_alpha - 0.5 * u_beta);
    double U3 = (-SQRT3_DIV_2 * u_alpha - 0.5 * u_beta);

    // 根据扇区计算T1, T2（基本矢量作用时间）
    double T1 = 0, T2 = 0;
    double Ts = 1.0;  // 归一化周期
    double K = SQRT3 * Ts / udc;

    switch (m_sector) {
        case 1:  // 扇区I: V4(100), V6(110)
            T1 = K * U2;
            T2 = K * U1;
            break;
        case 2:  // 扇区II: V6(110), V2(010)
            T1 = -K * U2;
            T2 = -K * U3;
            break;
        case 3:  // 扇区III: V2(010), V3(011)
            T1 = K * U1;
            T2 = K * U3;
            break;
        case 4:  // 扇区IV: V3(011), V1(001)
            T1 = -K * U1;
            T2 = -K * U2;
            break;
        case 5:  // 扇区V: V1(001), V5(101)
            T1 = K * U3;
            T2 = K * U2;
            break;
        case 6:  // 扇区VI: V5(101), V4(100)
            T1 = -K * U3;
            T2 = -K * U1;
            break;
    }

    // 限制T1+T2不超过Ts
    double T_sum = T1 + T2;
    if (T_sum > Ts) {
        T1 = T1 * Ts / T_sum;
        T2 = T2 * Ts / T_sum;
    }

    // 计算零矢量作用时间
    double T0 = (Ts - T1 - T2) / 2.0;

    // 计算三相占空比（七段式中心对齐）
    double Tcm1 = 0, Tcm2 = 0, Tcm3 = 0;

    switch (m_sector) {
        case 1:
            Tcm1 = T1 + T2 + T0;
            Tcm2 = T2 + T0;
            Tcm3 = T0;
            break;
        case 2:
            Tcm1 = T1 + T0;
            Tcm2 = T1 + T2 + T0;
            Tcm3 = T0;
            break;
        case 3:
            Tcm1 = T0;
            Tcm2 = T1 + T2 + T0;
            Tcm3 = T2 + T0;
            break;
        case 4:
            Tcm1 = T0;
            Tcm2 = T1 + T0;
            Tcm3 = T1 + T2 + T0;
            break;
        case 5:
            Tcm1 = T2 + T0;
            Tcm2 = T0;
            Tcm3 = T1 + T2 + T0;
            break;
        case 6:
            Tcm1 = T1 + T2 + T0;
            Tcm2 = T0;
            Tcm3 = T1 + T0;
            break;
    }

    // 归一化到[0, 1]
    Ta = std::clamp(Tcm1 / Ts, 0.0, 1.0);
    Tb = std::clamp(Tcm2 / Ts, 0.0, 1.0);
    Tc = std::clamp(Tcm3 / Ts, 0.0, 1.0);
}

// 扇区判断
// 基于Uα、Uβ的符号和幅值关系判断扇区
int svpwm::calc_sector(double u_alpha, double u_beta) {
    // 计算三个判断量
    int A = (u_beta > 0) ? 1 : 0;
    int B = (SQRT3_DIV_2 * u_alpha - 0.5 * u_beta > 0) ? 1 : 0;
    int C = (-SQRT3_DIV_2 * u_alpha - 0.5 * u_beta > 0) ? 1 : 0;

    int N = A + 2 * B + 4 * C;

    // N -> 扇区映射
    switch (N) {
        case 3: return 1;
        case 1: return 2;
        case 5: return 3;
        case 4: return 4;
        case 6: return 5;
        case 2: return 6;
        default: return 1;
    }
}
