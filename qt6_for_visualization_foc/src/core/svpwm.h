#ifndef CORE_SVPWM_H
#define CORE_SVPWM_H

#include "types.h"

// i_svpwm接口
class i_svpwm {
public:
    virtual ~i_svpwm() = default;
    virtual void calc(double u_alpha, double u_beta, double udc,
                      double& Ta, double& Tb, double& Tc) = 0;
    virtual int get_sector() const = 0;
    virtual void get_vector(double& mag, double& angle) const = 0;
};

// SVPWM空间矢量脉宽调制算法
// 将αβ坐标系的电压矢量转换为三相占空比
class svpwm : public i_svpwm {
public:
    // 计算三相占空比
    void calc(double u_alpha, double u_beta, double udc,
              double& Ta, double& Tb, double& Tc) override;

    // 获取当前扇区(1-6)
    int get_sector() const override { return m_sector; }

    // 获取电压矢量的幅值和角度
    void get_vector(double& mag, double& angle) const override {
        mag = m_mag;
        angle = m_angle;
    }

private:
    // 判断扇区
    int calc_sector(double u_alpha, double u_beta);

    int m_sector = 1;
    double m_mag = 0.0;
    double m_angle = 0.0;
};

#endif // CORE_SVPWM_H
