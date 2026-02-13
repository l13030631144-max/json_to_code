# CAD Dialog Code Generator (Claude Code Skill)

基于 Claude Code 的 CAD 对话框代码生成器。读取 UI 设计器导出的 JSON 文件，自动生成 PowerCAD 对话框三件套：`.ui`、`.cpp`、`.h`。

## 目录结构

```
generate-dialog-skill/
├── .claude/
│   ├── skills/
│   │   ├── generate-dialog/
│   │   │   └── SKILL.md                # [长期] PW_ 前缀规则，面向适配完成后的 PowerCAD
│   │   └── generate-dialog-dev/
│   │       └── SKILL.md                # [开发] 无前缀规则，面向当前 PowerCAD（过渡期）
│   └── reference/
│       ├── json_interface_spec.md       # UI 设计器导出的 JSON 格式规范
│       └── working-example/            # 已验证可运行的参考示例
│           └── BlockFeature/
│               ├── BlockFeatureDialog.ui
│               ├── BlockFeatureDialog.cpp
│               └── BlockFeatureDialog.h
├── power_ui_template/                   # 61 个控件 .ui 模板（属性参考）
├── workspace/                           # 工作区（输入 JSON + 生成输出）
│   └── 倒斜角/                          # 示例：每个对话框一个文件夹
│       ├── 倒斜角.json                   #   输入文件
│       ├── TestDialog.ui                 #   生成的 UI 定义
│       ├── TestDialog.cpp                #   生成的 C++ 实现
│       └── TestDialog.h                  #   生成的 C++ 头文件
└── README.md                            # 本文件
```

## 环境要求

- [Claude Code](https://docs.anthropic.com/en/docs/claude-code) CLI 已安装
- 终端在本项目根目录下打开（Claude Code 会自动识别 `.claude/skills/`）

## 快速开始

### 1. 准备输入 JSON

从 UI 设计器导出 JSON 文件，放入 `workspace/` 下的子文件夹中：

```
workspace/
└── 我的对话框/
    └── 我的对话框.json
```

JSON 格式详见 `.claude/reference/json_interface_spec.md`。

### 2. 执行生成

项目提供两个 skill，根据目标 PowerCAD 版本选择：

| Skill | 命令 | 适用场景 |
|---|---|---|
| `generate-dialog-dev` | `/generate-dialog-dev workspace/xxx/xxx.json` | **当前开发使用** - 生成无前缀属性，可直接运行 |
| `generate-dialog` | `/generate-dialog workspace/xxx/xxx.json` | 长期版本 - 生成 PW_ 前缀属性，待适配完成后使用 |

**当前阶段请使用 dev 版本**：

```
/generate-dialog-dev workspace/我的对话框/我的对话框.json
```

生成完成后，文件夹内会出现三个文件：

```
workspace/我的对话框/
├── 我的对话框.json          # 输入（不变）
├── MyDialog.ui              # 生成 ← 文件名取决于 JSON 中的 className
├── MyDialog.cpp             # 生成
└── MyDialog.h               # 生成
```

### 3. 集成到项目

将生成的 `.ui`、`.cpp`、`.h` 复制到你的 PowerCAD 项目中，配合 CMakeLists.txt 编译即可。

## 关于属性命名与双 Skill 机制

### 背景

PowerCAD 正在适配 `PW_` 开头的属性名。当前版本尚未完全支持，因此项目维护了两个 skill：

| | `generate-dialog` (长期版) | `generate-dialog-dev` (开发版) |
|---|---|---|
| **`.ui` 属性名** | `PW_Label`, `PW_Value`, `PW_Data`... | `LabelText`, `Value`, `Data`... |
| **`PW_BlockID`** | 写入 .ui | 不写入（widget `name` 即标识） |
| **当前能否直接运行** | 需手动替换属性名 | 可以直接运行 |
| **C++ 代码** | 相同 | 相同 |
| **Skill 文件** | `.claude/skills/generate-dialog/SKILL.md` | `.claude/skills/generate-dialog-dev/SKILL.md` |

### 属性名映射关系

| PW_ 前缀（JSON 输入 / 长期版输出） | 无前缀（开发版输出 / 当前能跑） |
|---|---|
| `PW_Label` / `PW_LabelString` | `LabelText` |
| `PW_Value` | `Value` |
| `PW_Data` | `Data` |
| `PW_Left` / `PW_Right` / `PW_Top` / `PW_Bottom` | `Left` / `Right` / `Top` / `Bottom` |
| `PW_Formula` | `Formula` |
| `PW_StepStatus` | `StepStatus` |
| `PW_LabelVisibility` | `LabelVisibility` |
| `PW_BlockID` | 不写入 .ui，widget `name` 就是标识 |
| 其他 `PW_Xxx` | `Xxx`（去掉 `PW_` 前缀） |

**参考示例 `.claude/reference/working-example/BlockFeature/` 使用的是无前缀写法**（当前能跑的版本）。

### 后续计划

等 PowerCAD 完成 `PW_` 属性适配后：
1. 切换回使用 `/generate-dialog`（长期版）
2. 将新的验证通过的示例（使用 `PW_` 前缀）加入 reference 替换旧的 BlockFeature 示例
3. 可以删除或归档 `generate-dialog-dev` skill

## 关键规则速览

### C++ 类型映射

| JSON 类型 | C++ OPEN 层类型 |
|---|---|
| `BlockLinearExpression` | `LinearDimension`（不是 LinearExpression） |
| `BlockAngularExpression` | `AngularDimension`（不是 AngularExpression） |
| `BlockDouble` | `DoubleBlock`（不是 Double） |
| `BlockInteger` | `IntegerBlock`（不是 Integer） |
| `BlockJiList` | `SetList`，.ui 中用 `BlockSetList` |

### 特殊注意

- **SetList 头文件**：`<PWOpen/IncludeHeader.h>` 不包含 SetList，使用时必须手动添加 `#include <PWOpen/BlockStyler_SetList.hxx>`
- **动态效果**：`dialogShown_cb()` 中必须调用 `update_cb()` 初始化枚举触发的可见性状态，否则初始界面所有控件全部可见

## 维护指南

### 将生成文件升级为参考示例

当一个生成的对话框经过验证、在 PowerCAD 中能正确运行后，应将它加入参考库：

```bash
# 1. 在 reference 下新建文件夹
mkdir .claude/reference/working-example/我的对话框

# 2. 复制验证通过的三件套
cp workspace/我的对话框/MyDialog.ui  .claude/reference/working-example/我的对话框/
cp workspace/我的对话框/MyDialog.cpp .claude/reference/working-example/我的对话框/
cp workspace/我的对话框/MyDialog.h   .claude/reference/working-example/我的对话框/
```

参考示例越多，AI 生成质量越高。建议覆盖不同控件组合的场景：
- 包含枚举动态切换的对话框
- 包含集列表（SetList）的对话框
- 包含嵌套 Group 的对话框
- 包含 Toggle 联动的对话框

### 更新 SKILL.md

两个 skill 的规则文件分别位于：
- 长期版：`.claude/skills/generate-dialog/SKILL.md`
- 开发版：`.claude/skills/generate-dialog-dev/SKILL.md`

需要更新的常见场景：

| 场景 | 操作 | 影响哪个 Skill |
|---|---|---|
| 新增控件类型支持 | 在命名表、类型映射表、customwidgets 声明中分别添加 | 两个都要改 |
| 属性映射有误 | 修正 .ui 属性命名规则部分 | 视情况 |
| C++ 模板变化 | 更新 .h / .cpp 模板代码段 | 两个都要改 |
| 新增参考示例 | 更新 `## Reference Files` 部分的路径 | 两个都要改 |
| 属性名转换关系变化 | 更新 dev 版的属性名转换表 | 仅 dev 版 |

修改后建议用一个简单 JSON 重新生成测试，确认改动生效。

### 更新 power_ui_template

如果 PowerCAD 控件库有新增或变更：

1. 将新的 `.ui` 模板文件放入 `power_ui_template/`
2. AI 生成时会参考这些模板的属性结构

### 清理 workspace

`workspace/` 是工作区，生成文件集成到项目后可以清理：

```bash
# 删除已集成的对话框
rm -rf workspace/已完成的对话框/

# 或保留 JSON 作为记录，只删除生成文件
rm workspace/已完成的对话框/*.ui workspace/已完成的对话框/*.cpp workspace/已完成的对话框/*.h
```

## 常见问题

**Q: 生成后编译报 SetList 找不到定义？**
A: 在 `.cpp` 文件顶部手动添加 `#include <PWOpen/BlockStyler_SetList.hxx>`。

**Q: 动态效果（枚举切换显隐）不生效？**
A: 检查两点：
1. `.ui` 中枚举的 `Value` 和 `Data` 属性是否正确（不能带 `PW_` 前缀）
2. `dialogShown_cb()` 中是否调用了 `update_cb()` 设置初始状态

**Q: 控件没有显示标签文字？**
A: 确认 `.ui` 中使用的是 `LabelText`（不是 `PW_Label` 或 `Label`）。

**Q: 如何支持新的控件类型？**
A: 需要同时更新 SKILL.md 中的三处：控件命名表、OPEN 层类型映射表、customwidgets XML 声明。注意两个 skill（`generate-dialog` 和 `generate-dialog-dev`）都需要同步更新。

**Q: `generate-dialog` 和 `generate-dialog-dev` 选哪个？**
A: 当前阶段一律使用 `/generate-dialog-dev`，生成的文件可直接运行。等 PowerCAD 完成 `PW_` 前缀适配后再切回 `/generate-dialog`。
