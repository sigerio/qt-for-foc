## ADDED Requirements

### Requirement: 框图组件中文标签支持
系统必须(SHALL)支持所有框图组件添加中文文本标签。

#### Scenario: 编辑箭头名称
- **WHEN** 用户双击箭头组件
- **THEN** 弹出 QInputDialog 输入框，支持输入中文名称

#### Scenario: 显示箭头标签
- **WHEN** 箭头设置了名称
- **THEN** 在箭头上方（水平）或左侧（垂直）显示文本标签

#### Scenario: 编辑求和点名称
- **WHEN** 用户双击求和点组件
- **THEN** 弹出 QInputDialog 输入框，支持输入中文名称

#### Scenario: 显示求和点标签
- **WHEN** 求和点设置了名称
- **THEN** 在求和点下方显示文本标签

#### Scenario: 序列化组件标签
- **WHEN** 保存框图配置
- **THEN** 所有组件的名称随配置一起存储到 JSON 文件

### Requirement: 箭头样式优化
系统必须(SHALL)优化箭头样式以避免交叉时的视觉重叠。

#### Scenario: 交叉箭头显示
- **WHEN** 多个箭头交叉
- **THEN** 通过半透明或虚线样式确保视觉清晰

### Requirement: 控制框图配置联动
系统必须(SHALL)支持根据控制环使能状态自动加载对应的控制框图配置。

#### Scenario: 仅电流环模式
- **WHEN** 窗口2仅使能电流环
- **THEN** 窗口3的框图编辑器自动加载 `cascade_current.json` 配置

#### Scenario: 电流+速度环模式
- **WHEN** 窗口2使能电流环和速度环
- **THEN** 窗口3的框图编辑器自动加载 `cascade_current_velocity.json` 配置

#### Scenario: 完整三环模式
- **WHEN** 窗口2使能电流环、速度环和位置环
- **THEN** 窗口3的框图编辑器自动加载 `cascade_full.json` 配置

### Requirement: 预设框图配置存储
系统必须(SHALL)将控制框图预设配置存储在项目根目录的 config/ 文件夹中。

#### Scenario: 配置文件位置
- **WHEN** 用户保存框图配置
- **THEN** 配置文件存储在 `config/` 目录下

#### Scenario: 配置文件命名
- **WHEN** 保存三种预设框图
- **THEN** 分别命名为 `cascade_current.json`、`cascade_current_velocity.json`、`cascade_full.json`

### Requirement: cascade_widget 外部加载接口
系统必须(SHALL)为 cascade_widget 提供外部触发加载配置的公共接口。

#### Scenario: 加载预设配置
- **WHEN** 外部调用 `load_preset("current")` 
- **THEN** cascade_widget 自动加载 `config/cascade_current.json`
