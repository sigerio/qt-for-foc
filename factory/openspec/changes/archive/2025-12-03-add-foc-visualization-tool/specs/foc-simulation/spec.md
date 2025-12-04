# FOC仿真引擎规格

## ADDED Requirements

### Requirement: PMSM电机模型
系统必须(SHALL)实现PMSM永磁同步电机的dq轴数学模型。

#### Scenario: 电压方程计算
- **WHEN** 给定dq轴电流和转速
- **THEN** 计算dq轴电压（Ud, Uq）

#### Scenario: 转矩计算
- **WHEN** 给定dq轴电流
- **THEN** 计算电磁转矩Te

### Requirement: 仿真步长配置
系统必须(SHALL)支持10μs和100μs两种仿真步长。

#### Scenario: 切换仿真步长
- **WHEN** 用户选择不同步长
- **THEN** 仿真引擎按新步长运行

### Requirement: 坐标变换
系统必须(SHALL)实现Clark变换和Park变换及其逆变换。

#### Scenario: Clark变换
- **WHEN** 输入三相电流(Ia, Ib, Ic)
- **THEN** 输出两相静止坐标(Iα, Iβ)

#### Scenario: Park变换
- **WHEN** 输入静止坐标(Iα, Iβ)和转子角度θ
- **THEN** 输出旋转坐标(Id, Iq)

### Requirement: SVPWM算法
系统必须(SHALL)实现空间矢量脉宽调制算法。

#### Scenario: 扇区判断
- **WHEN** 输入αβ轴电压
- **THEN** 判断所在扇区(1-6)

#### Scenario: 占空比计算
- **WHEN** 输入电压矢量和扇区
- **THEN** 计算三相占空比(Ta, Tb, Tc)
