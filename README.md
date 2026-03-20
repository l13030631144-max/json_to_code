# CAD Dialog Code Generator (Claude Code Skill)

基于 Claude Code 的 CAD 对话框代码生成器。读取 UI 设计器导出的 JSON 文件，自动生成 PowerCAD 对话框三件套：`.ui`、`.cpp`、`.h`。

## 目录结构

```
CAD-FRONTEND-AGENT-ALGO-CLAUDE-CODE/
├── .claude/
│   ├── skills/
│   │   └── generate-dialog/
│   │       └── SKILL.md              # 生成规则（PW_ 前缀属性名）
│   └── reference/
│       ├── json_interface_spec__2_.md # UI 设计器导出的 JSON 格式规范
│       ├── example/                   # 已验证可运行的参考示例
│       │   ├── FeatureBlockDialog.ui
│       │   ├── FeatureBlockDialog.cpp
│       │   └── FeatureBlockDialog.h
│       ├── error_example/             # 已知错误案例（用于改进规则）
│       │   ├── error_example.md       #   错误说明文档
│       │   ├── 1/                     #   案例 1：含 JSON + 错误输出
│       │   └── 基准平面/              #   案例 2：PW_BlockID 缺失问题
│       └── logical_example/           # 动态逻辑参考
│           └── PWOpenCppTemplate_Example.py  # update_cb 动态交互模式示例
├── power_ui_template/                 # 60 个控件 .ui 模板（属性参考）
├── workspace/                         # 工作区（输入 JSON + 生成输出）
│   ├── 11/                            # 示例：每个对话框一个文件夹
│   ├── 1组/
│   ├── 2滚动窗口/
│   └── ...
├── copy_files.py                      # 辅助脚本
├── CMakeLists.txt
├── 映射表.txt                         # 控件类型映射参考
├── 缺少头文件控件清单.md              # 需要额外 include 的控件清单
└── README.md
```

## 环境要求

- [Claude Code](https://docs.anthropic.com/en/docs/claude-code) CLI 已安装
- 终端在本项目根目录下打开（Claude Code 会自动识别 `.claude/skills/`）

## 快速开始

### 1. 准备输入 JSON

从 UI 设计器导出 JSON 文件，放入 `workspace/` 下的子文件夹中。JSON 格式详见 `.claude/reference/json_interface_spec__2_.md`。

### 2. 执行生成

```
/generate-dialog workspace/我的对话框/xxx.json
```

生成完成后，文件夹内会出现三个文件（文件名取决于 JSON 中的 `className`）：

```
workspace/我的对话框/
├── xxx.json                 # 输入（不变）
├── MyDialog.ui              # 生成
├── MyDialog.cpp             # 生成
└── MyDialog.h               # 生成
```

### 3. 集成到项目

将生成的 `.ui`、`.cpp`、`.h` 复制到你的 PowerCAD 项目中，配合 CMakeLists.txt 编译即可。

## 生成规则要点

### C++ 类型映射（易错项）

| JSON 类型 | C++ OPEN 层类型 |
|---|---|
| `BlockLinearExpression` | `LinearDimension`（不是 LinearExpression） |
| `BlockAngularExpression` | `AngularDimension`（不是 AngularExpression） |
| `BlockDouble` | `DoubleBlock`（不是 Double） |
| `BlockInteger` | `IntegerBlock`（不是 Integer） |

### 动态逻辑（altProperties）

Skill 支持以下动态交互模式（参考 `.claude/reference/logical_example/`）：

| 模式 | altProperties 示例 | C++ 生成 |
|------|-------------------|----------|
| 枚举触发显隐 | `"PW_Show": "enum0(a, c)"` | `SetLogical("Show", enumValue == "a" \|\| ...)` |
| 开关触发显隐 | `"PW_Show": "toggle1 == true"` | `SetLogical("Show", toggleValue)` |
| 组合条件 | `"PW_Show": "toggle1 == true && enum0(a)"` | 组合 if 判断 |
| 启用/禁用 | `"PW_Enable": "enum0(val)"` | `SetLogical("Enable", ...)` |
| 只读控制 | `"PW_ReadOnlyValue": "toggle0 == true"` | `SetLogical("ReadOnlyValue", ...)` |
| More 区域 | `"PW_Show": "Dialog0 == true"` | `theDialog->IsMore()` |
| More + 枚举 | `"PW_Show": "Dialog0 == true && enum1(a)"` | `IsMore() && enumValue == "a"` |

### 容器控件 customwidgets 声明

所有容器类控件必须同时声明 `<container>1</container>` 和 `<addpagemethod>`：

| 容器类型 | addpagemethod |
|---------|---------------|
| BlockDialog | `InitContainerWidget` |
| BlockFeatureDialog | `InitContainerWidget` |
| BlockGroup | `AddContent` |
| BlockScrollWindow | `AddContent` |
| BlockTabWidget | `AddTab` |
| BlockWizard | `addPage` |
| BlockExplorer | `AddContent` |

### 特殊注意

- **PW_BlockID 必须写入**：即使出现在 `hiddenProps` 中也不能跳过，否则 `FindBlock()` 运行时失败
- **SetList 头文件**：`<PWOpen/IncludeHeader.h>` 不包含 SetList，使用时必须手动添加 `#include <PWOpen/BlockStyler_SetList.hxx>`
- **dialogShown_cb 初始化**：必须调用 `update_cb()` 初始化枚举/开关触发的可见性状态，否则初始界面所有控件全部可见

## reference 目录说明

| 目录 | 用途 |
|------|------|
| `example/` | 已验证可运行的参考示例，AI 生成时参考其输出格式 |
| `error_example/` | 已知错误案例及说明，用于持续改进 SKILL 规则 |
| `logical_example/` | C++ 动态逻辑模式参考（update_cb 的各种写法） |
| `json_interface_spec__2_.md` | JSON 输入格式完整规范 |

## 维护指南

### 将生成文件升级为参考示例

当一个生成的对话框经过验证、在 PowerCAD 中能正确运行后，应将它加入 `.claude/reference/example/`。参考示例越多，AI 生成质量越高。

### 记录错误案例

发现生成结果有误时，将 JSON + 错误输出保存到 `.claude/reference/error_example/`，并在 `error_example.md` 中记录问题描述，然后修正 SKILL.md 中的规则。

### 更新 SKILL.md

规则文件位于 `.claude/skills/generate-dialog/SKILL.md`。常见更新场景：

| 场景 | 操作 |
|------|------|
| 新增控件类型支持 | 在命名表、类型映射表、customwidgets 声明中添加 |
| 新增动态逻辑模式 | 在 update_cb 规则部分添加模式说明和代码示例 |
| 修正生成错误 | 在对应规则处添加强调说明，同时将错误案例存入 error_example |

### 更新 power_ui_template

如果 PowerCAD 控件库有新增或变更，将新的 `.ui` 模板文件放入 `power_ui_template/`。

## 常见问题

**Q: 生成后编译报 SetList 找不到定义？**
A: 在 `.cpp` 文件顶部手动添加 `#include <PWOpen/BlockStyler_SetList.hxx>`。

**Q: 动态效果（枚举切换显隐）不生效？**
A: 检查 `dialogShown_cb()` 中是否调用了 `update_cb()` 设置初始状态。

**Q: 控件在运行时找不到（FindBlock 返回 null）？**
A: 检查 `.ui` 中该控件是否缺少 `PW_BlockID` 属性。这通常发生在 `PW_BlockID` 被列入 `hiddenProps` 时被错误跳过。
