#ifndef CORE_I_TRANSFORM_H
#define CORE_I_TRANSFORM_H

// 坐标变换接口
class i_transform {
public:
    virtual ~i_transform() = default;

    // Clark变换: abc -> αβ
    virtual void clark(double ia, double ib, double ic,
                       double& i_alpha, double& i_beta) = 0;

    // Park变换: αβ -> dq
    virtual void park(double i_alpha, double i_beta, double theta,
                      double& id, double& iq) = 0;

    // 逆Clark变换: αβ -> abc
    virtual void inv_clark(double u_alpha, double u_beta,
                           double& ua, double& ub, double& uc) = 0;

    // 逆Park变换: dq -> αβ
    virtual void inv_park(double ud, double uq, double theta,
                          double& u_alpha, double& u_beta) = 0;
};

#endif // CORE_I_TRANSFORM_H
