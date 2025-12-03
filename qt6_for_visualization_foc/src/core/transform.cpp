#include "transform.h"
#include "types.h"
#include <cmath>

// Clark变换（等幅值变换）
// iα = ia
// iβ = (ia + 2*ib) / √3
// 假设三相对称: ia + ib + ic = 0
void transform::clark(double ia, double ib, double ic,
                      double& i_alpha, double& i_beta) {
    (void)ic;  // 三相对称时ic不需要
    i_alpha = ia;
    i_beta = (ia + 2.0 * ib) * ONE_DIV_SQRT3;
}

// Park变换
// id = iα*cos(θ) + iβ*sin(θ)
// iq = -iα*sin(θ) + iβ*cos(θ)
void transform::park(double i_alpha, double i_beta, double theta,
                     double& id, double& iq) {
    double cos_t = std::cos(theta);
    double sin_t = std::sin(theta);
    id = i_alpha * cos_t + i_beta * sin_t;
    iq = -i_alpha * sin_t + i_beta * cos_t;
}

// 逆Clark变换
// ua = uα
// ub = -0.5*uα + (√3/2)*uβ
// uc = -0.5*uα - (√3/2)*uβ
void transform::inv_clark(double u_alpha, double u_beta,
                          double& ua, double& ub, double& uc) {
    ua = u_alpha;
    ub = -0.5 * u_alpha + SQRT3_DIV_2 * u_beta;
    uc = -0.5 * u_alpha - SQRT3_DIV_2 * u_beta;
}

// 逆Park变换
// uα = ud*cos(θ) - uq*sin(θ)
// uβ = ud*sin(θ) + uq*cos(θ)
void transform::inv_park(double ud, double uq, double theta,
                         double& u_alpha, double& u_beta) {
    double cos_t = std::cos(theta);
    double sin_t = std::sin(theta);
    u_alpha = ud * cos_t - uq * sin_t;
    u_beta = ud * sin_t + uq * cos_t;
}
