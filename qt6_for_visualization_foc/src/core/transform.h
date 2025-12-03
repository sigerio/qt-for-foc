#ifndef CORE_TRANSFORM_H
#define CORE_TRANSFORM_H

#include "i_transform.h"

// 坐标变换实现
// Clark变换: 三相静止坐标系(abc) -> 两相静止坐标系(αβ)
// Park变换: 两相静止坐标系(αβ) -> 两相旋转坐标系(dq)
class transform : public i_transform {
public:
    // Clark变换: ia,ib,ic -> iα,iβ
    // 等幅值变换公式
    void clark(double ia, double ib, double ic,
               double& i_alpha, double& i_beta) override;

    // Park变换: iα,iβ,θ -> id,iq
    void park(double i_alpha, double i_beta, double theta,
              double& id, double& iq) override;

    // 逆Clark变换: uα,uβ -> ua,ub,uc
    void inv_clark(double u_alpha, double u_beta,
                   double& ua, double& ub, double& uc) override;

    // 逆Park变换: ud,uq,θ -> uα,uβ
    void inv_park(double ud, double uq, double theta,
                  double& u_alpha, double& u_beta) override;
};

#endif // CORE_TRANSFORM_H
