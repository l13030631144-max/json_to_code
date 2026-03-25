# 前端智能体 JSON 接口规范文档 (Source Format)

**版本**: v2.0
**来源**: `qianduan.html` (Preview/Save Project 格式)

本文档描述了从前端设计器导出的最新 JSON 数据结构（对应 "保存项目" 输出的源格式）。该格式包含了完整的 UI 定义、布局信息及属性配置，用于作为智能体生成 CAD 软件代码的输入。

## 1. 根结构 (Root Object)

JSON 文件的根是一个对象，包含以下字段：

| 字段名                    | 类型    | 说明                                                                                                                                                                             |
| :------------------------ | :------ | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`tree`**        | Array   | **UI 组件树**。这是一个只包含**一个元素**的数组，该元素即为根节点（通常为 `BlockDialog` 类型）。所有其他 UI 组件都作为该根节点的子节点嵌套在 `children` 字段中。 |
| **`previewMode`** | Boolean | **预览模式状态**。指示保存时编辑器是否处于预览模式。                                                                                                                       |
| **`timestamp`**   | Number  | **时间戳**。文件生成的 Unix 时间戳（毫秒）。                                                                                                                               |

**示例**:

```json
{
  "tree": [...],
  "previewMode": false,
  "timestamp": 1770603333059
}
```

---

## 2. 组件对象结构 (Component Object)

`tree` 数组中的**唯一根对象**通常是 `BlockDialog` 类型的组件。

**层级规则**：

* **Root**: 顶层 `tree[0]` 必须是 `BlockDialog`。
* **Children**: 所有其他组件（如 `BlockGroup`, `BlockString` 等）必须作为后代节点存在于 `BlockDialog` 的 `children` 属性（或更深层嵌套）中。

### 2.1 核心字段

| 字段名                      | 类型   | 说明                                                                                                                                                                                                       |
| :-------------------------- | :----- | :--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **`id`**            | String | **组件唯一标识符**。`<br>`格式示例：`BlockDialog_1770451903113_37111`。用于内部索引和查找。                                                                                                      |
| **`type`**          | String | **组件类型**。`<br>`对应 C++ 类名，如 `BlockDialog`, `BlockGroup`, `BlockLabel` 等。                                                                                                         |
| **`props`**         | Object | **属性集合**。`<br>`包含所有组件配置，键名通常带有 `PW_` 前缀（对应 NX Open 属性）。                                                                                                             |
| **`hiddenProps`**   | Array  | **属性过滤/默认值列表**。`<br>`存储因使用默认值而无需生成的属性名。**关键用途**：为了节省 LLM 的 Token，凡是出现在此列表中的属性，在生成给 AI 的 Prompt 时应被忽略，由后端代码使用默认值。   |
| **`altProperties`** | Object | **动态交互规则集合**。`<br>`存储属性值的动态计算规则或条件表达式。**关键用途**：用于定义当其他控件（触发器）的值发生变化时，该属性如何随之变化（例如 Visibility = `enum1 == 'OptionA'`）。 |
| **`children`**      | Array  | **子组件列表**。`<br>`嵌套的子组件对象数组。                                                                                                                                                       |

**示例**:

```json
{
  "id": "BlockDialog_177...",
  "type": "BlockDialog",
  "props": { ... },
  "hiddenProps": ["PW_Show"],
  "altProperties": { ... },
  "children": [ ... ]
}
```

---

### 2.2 `props` 属性详解

`props` 对象存储了组件的具体配置。键名通常与 NX Open API 的属性名直接对应。

#### 示例：BlockDialog 属性对照表

以下以根组件 `BlockDialog` 为例，列出其常见属性及导出规则：

| UI设计器显示名称           | 导出JSON字段名称        | 类型    | 默认值              | 说明                                                                              |
| :------------------------- | :---------------------- | :------ | :------------------ | :-------------------------------------------------------------------------------- |
| **ClassName**        | `className`           | String  | `TestDialog`      | **核心类名**。决定生成的 `.ui/.cpp/.h` 文件名及类名。仅 Dialog 有此属性。 |
| **BlockID**          | `PW_BlockID`          | String  | `Dialog`          | **变量名**。生成的 C++ 代码中变量名将基于此 ID。                            |
| **Cue**              | `PW_Cue`              | String  | `""`              | 提示文本。                                                                        |
| **Enable**           | `PW_Enable`           | Boolean | `True`            | 是否启用。                                                                        |
| **Group**            | `PW_Group`            | Boolean | `False`           | 是否作为组显示                                                                    |
| **Label**            | `PW_Label`            | String  | `无标题`          | 对话框标题。                                                                      |
| **Dialog Sizing**    | `PW_Dialog_Sizing`    | String  | `Follow Policy`   | 对话框尺寸调整策略。                                                              |
| **Navigation Style** | `PW_Navigation_Style` | String  | `OK Apply Cancel` | 底部导航按钮样式（确定/应用/取消）。                                              |
| **UseMore**          | `PW_UseMore`          | Boolean | `True`            | **Power 独有属性**。控制是否显示"更多"选项或其他扩展功能。                  |
| **IsMore**           | `PW_IsMore`           | Boolean | `False`           | **Power 独有属性**。标记当前控件是否属于"更多"区域。                        |
| **(Hidden)**         | `_w`                  | Number  | -                   | **宽度** (像素)。不显示在 UI 设计器中，仅在保存时写入。                     |
| **(Hidden)**         | `_h`                  | Number  | -                   | **高度** (像素)。不显示在 UI 设计器中，仅在保存时写入。                     |
| **(Hidden)**         | `_x`                  | Number  | -                   | **屏幕 X 坐标**。不显示在 UI 设计器中，仅在保存时写入。                     |
| **(Hidden)**         | `_y`                  | Number  | -                   | **屏幕 Y 坐标**。不显示在 UI 设计器中，仅在保存时写入。                     |

**示例**:

```json
"props": {
  "className": "TestDialog",
  "PW_BlockID": "Dialog",
  "PW_Cue": "",
  "PW_Enable": true,
  "PW_Group": false,
  "PW_Label": "无标题",
  "PW_Dialog_Sizing": "Follow Policy",
  "PW_Navigation_Style": "OK Apply Cancel",
  "PW_UseMore": true,
  "PW_IsMore": false,
  "_w": 500,
  "_h": 400,
  "_x": 640,
  "_y": 320
}
```

> **注意**: `default` 列的值即为默认值。如果属性值与默认值相同，且该属性被包含在 `hiddenProps` 列表中，则后端在生成 JSON 时可能会省略该属性（详见 `hiddenProps` 说明）。


### 2.2.1 特征对话框 (FeatureDialog - `BlockFeatureDialog`)

`BlockFeatureDialog` 是 `BlockDialog` 的变体，额外支持预览功能。当对话框需要显示操作预览/结果时使用此类型替代 `BlockDialog`。

> **POWER 模板参考**: `power_ui_template/BlockFeatureDialog.ui`

除了 `BlockDialog` 的所有属性外，还包含以下额外属性：

| UI设计器显示名称        | 导出JSON字段名称       | 类型    | 默认值   | 说明                                                                                    |
| :---------------------- | :--------------------- | :------ | :------- | :-------------------------------------------------------------------------------------- |
| **DialogPreviews** | `PW_DialogPreviews`  | String  | `None` | 对话框预览模式 (None, Only_Preview, Only_Result, Preview_Result)。 |

**示例**:

```json
{
  "id": "BlockFeatureDialog_177...",
  "type": "BlockFeatureDialog",
  "props": {
    "className": "TestDialog",
    "PW_Cue": "",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "无标题",
    "PW_Dialog_Sizing": "Follow Policy",
    "PW_Navigation_Style": "OK Apply Cancel",
    "PW_UseMore": true,
    "PW_IsMore": false,
    "PW_DialogPreviews": "None",
    "PW_BlockID": "Dialog"
    "_w": 500,
    "_h": 400,
    "_x": 640,
    "_y": 320
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": [...]
}
```

---

### 2.3 `altProperties` 动态交互详解

`altProperties` 字段用于定义组件属性与其它控件值之间的动态关联关系。

* **格式**: 键值对对象。
  * **Keys**: 需要动态变化的属性名（如 `PW_Show`, `PW_Enable`）。
  * **Values**: 表达式字符串。
* **支持范围**:
  * **触发条件 (Triggers)**: 目前仅支持 **枚举 (Enumeration)** 和 **开关 (Toggle)** 类型的控件作为触发源。
  * **受控属性 (Targets)**: 目前仅支持 **Bool 类型** 的属性被控制（最常用的是 `PW_Show` 和 `PW_Enable`）。
* **常见交互逻辑**:
  * **Visibility (可见性)**: 当触发控件的值满足条件时显示。
    * Example (Enum Trigger): `"PW_Show": "Enum1(1, 2)"` (Enum1 的值为 1 或 2 时显示，支持多选)
    * Example (Toggle Trigger): `"PW_Show": "toggle1 == true"`
  * **Enablement (可用性)**: 当触发控件的值满足条件时启用。
    * Example: `"PW_Enable": "toggle1 == true"`
  * **Logic Combination (逻辑组合)**: 使用 `&&` (与) 和 `||` (或) 组合多个条件。
    * Example (AND): `"PW_Show": "toggle1 == true && Enum1(1)"` (同时满足)
    * Example (OR): `"PW_Enable": "toggle1 == true || toggle2 == true"` (满足任一)
    * Example (Complex): `"PW_Show": "(toggle1 == true || toggle2 == true) && Enum1(2)"` (混合逻辑)
* **代码生成影响**:
  * 后端应解析这些表达式 (特别是 `Enum(Val)` 格式)，并在 `update_cb` 回调函数中生成相应的 `if/else` 逻辑，以在运行时动态设置属性。

**示例**:

```json
"altProperties": {
  "PW_Show": "toggle1 == true",
  "PW_Enable": "Enum1(1)"
}
```

---

## 3. 组件属性详述 (Component Properties)

本节详细列出各组件支持的属性、导出字段及默认值。

### 3.1 组 (Group - `BlockGroup`)

对应 NX Open `BlockGroup` 组件。

> **POWER 模板参考**: `power_ui_template/BlockGroup.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值         | 说明                      |
| :----------------------------- | :-------------------------- | :--------- | :------------- | :------------------------ |
| **BlockID**              | `PW_BlockID`              | String     | `group`      | 组件唯一标识符。          |
| **Enable**               | `PW_Enable`               | Boolean    | `True`       | 是否启用。                |
| **Expanded**             | `PW_Expanded`             | Boolean    | `True`       | 初始时是否展开。          |
| **Label**                | `PW_Label`                | String     | `No Title`   | 组标题。                  |
| **Show**                 | `PW_Show`                 | Boolean    | `True`       | 是否可见。                |
| **Localize**             | `PW_Localize`             | Boolean    | `True`       | 是否支持本地化。          |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`    | 底部附着目标。            |
| **Left**                 | `PW_Left`                 | Attachment | `Default`    | 左侧附着目标。            |
| **Right**                | `PW_Right`                | Attachment | `Default`    | 右侧附着目标。            |
| **Top**                  | `PW_Top`                  | Attachment | `Default`    | 顶部附着目标。            |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`         | 气泡提示图片路径。        |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal` | 气泡提示布局方向。        |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`         | 气泡提示文本。            |
| **Column**               | `PW_Column`               | Integer    | `1`          | 组内布局列数 (通常为 1)。 |
| **ShowLabel**            | `PW_ShowLabel`            | Boolean    | `True`       | 是否显示组标题。          |

**示例**:

```json
{
  "id": "BlockGroup_1770451903113_555",
  "type": "BlockGroup",
  "props": {
    "PW_BlockID": "group0",
    "PW_Enable": true,
    "PW_Expanded": true,
    "PW_Label": "设置",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Column": 1,
    "PW_ShowLabel": true
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": [...]
}
```

### 3.2 枚举 (Enumeration - `BlockEnumeration`)

对应 NX Open `BlockEnumeration` 组件。

> **POWER 模板参考**: `power_ui_template/BlockEnumeration.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值         | 说明                                                                |
| :----------------------------- | :-------------------------- | :--------- | :------------- | :------------------------------------------------------------------ |
| **BlockID**              | `PW_BlockID`              | String     | `enumeration0` | 组件唯一标识符。                                                    |
| **Enable**               | `PW_Enable`               | Boolean    | `True`       | 是否启用。                                                          |
| **Group**                | `PW_Group`                | Boolean    | `False`      | 是否作为组显示。                                                    |
| **Label**                | `PW_Label`                | String     | `No Title`   | 标签文本。                                                          |
| **LabelVisibility**      | `PW_LabelVisibility`      | Boolean    | `True`       | 标签是否可见。                                                      |
| **Show**                 | `PW_Show`                 | Boolean    | `True`       | 是否可见。                                                          |
| **Localize**             | `PW_Localize`             | Boolean    | `True`       | 是否支持本地化。                                                    |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`       | 是否在对话框关闭后保留值。                                          |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`    | 底部附着目标。                                                      |
| **Left**                 | `PW_Left`                 | Attachment | `Default`    | 左侧附着目标。                                                      |
| **Right**                | `PW_Right`                | Attachment | `Default`    | 右侧附着目标。                                                      |
| **Top**                  | `PW_Top`                  | Attachment | `Default`    | 顶部附着目标。                                                      |
| **AllowShortcuts**       | `PW_AllowShortcuts`       | Boolean    | `True`       | 是否允许快捷操作。                                                  |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`         | 气泡提示图片。                                                      |
| **BalloonTooltipImages** | `PW_BalloonTooltipImages` | String[]   | `[]`         | 气泡提示图片列表。                                                  |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。                                |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`         | 气泡提示文本。                                                      |
| **BalloonTooltipTexts**  | `PW_BalloonTooltipTexts`  | String[]   | `[]`         | 气泡提示文本列表。                                                  |
| **Bitmaps**              | `PW_Bitmaps`              | String[]   | `[]`         | 选项对应的图标路径数组。                                            |
| **BorderVisibility**     | `PW_BorderVisibility`     | Boolean    | `True`       | 边框是否可见。                                                      |
| **EnumSensitivity**      | `PW_EnumSensitivity`      | String     | `""`         | 选项敏感度掩码。                                                    |
| **EnumVisibility**       | `PW_EnumVisibility`       | String     | `""`         | 选项可见性掩码。                                                    |
| **HighQualityBitmap**    | `PW_HighQualityBitmap`    | Boolean    | `True`       | 是否使用高质量位图。                                                |
| **IconsOnly**            | `PW_IconsOnly`            | Boolean    | `False`      | 是否仅显示图标。                                                    |
| **InitialShortcuts**     | `PW_InitialShortcuts`     | String     | `""`         | 初始快捷键。                                                        |
| **Layout**               | `PW_Layout`               | String     | `Vertical`   | 布局方向 (Vertical/Horizontal)。                                    |
| **NumberOfColumns**      | `PW_NumberOfColumns`      | Integer    | `65535`      | 列数。                                                              |
| **PackedColumns**        | `PW_PackedColumns`        | Boolean    | `False`      | 是否紧凑排列列。                                                    |
| **PresentationStyle**    | `PW_PresentationStyle`    | String     | `OptionMenu` | 展现样式 (OptionMenu, Radio Box, Pulldown)。                        |
| **Value**                | `PW_Value`                | String   | ""         | 字符串格式以逗号分隔                |
| **Data**                 | `PW_Data`                 | String   | ""         | 字符串格式以逗号分隔  |
| **ToolTipID**            | `PW_ToolTipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)**                             |

**示例**:

```json
{
  "id": "BlockEnumeration_1770451903114_666",
  "type": "BlockEnumeration",
  "props": {
    "PW_BlockID": "enum0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "选择类型",
    "PW_LabelVisibility": true,
    "PW_Show": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowShortcuts": true,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipImages": [],
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_BalloonTooltipTexts": [],
    "PW_Bitmaps": ["icon_a.png", "icon_b.png"],
    "PW_BorderVisibility": true,
    "PW_EnumSensitivity": "",
    "PW_EnumVisibility": "",
    "PW_HighQualityBitmap": true,
    "PW_IconsOnly": false,
    "PW_InitialShortcuts": "",
    "PW_Layout": "Horizontal",
    "PW_NumberOfColumns": 65535,
    "PW_PackedColumns": false,
    "PW_PresentationStyle": "Radio Box",
    "PW_Value": "aa,bb",
    "PW_Data": "1,2",
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.3 开关 (Toggle - `BlockToggle`)

对应 NX Open `BlockToggle` 组件，用于布尔值切换控制。

> **POWER 模板参考**: `power_ui_template/BlockToggle.ui`

| UI设计器显示名称                 | 导出JSON字段名称              | 类型       | 默认值         | 说明                                 |
| :------------------------------- | :---------------------------- | :--------- | :------------- | :----------------------------------- |
| **BlockID**                | `PW_BlockID`                | String     | `toggle0`    | 组件唯一标识符。                     |
| **Enable**                 | `PW_Enable`                 | Boolean    | `True`       | 是否启用。                           |
| **Group**                  | `PW_Group`                  | Boolean    | `False`      | 是否作为组显示。                     |
| **Label**                  | `PW_Label`                  | String     | `No Title`   | 标签文本。                           |
| **Show**                   | `PW_Show`                   | Boolean    | `True`       | 是否可见。                           |
| **Localize**               | `PW_Localize`               | Boolean    | `True`       | 是否支持本地化。                     |
| **RetainValue**            | `PW_RetainValue`            | Boolean    | `True`       | 是否在对话框关闭后保留值。           |
| **Bottom**                 | `PW_Bottom`                 | Attachment | `Default`    | 底部附着目标。                       |
| **Left**                   | `PW_Left`                   | Attachment | `Default`    | 左侧附着目标。                       |
| **Right**                  | `PW_Right`                  | Attachment | `Default`    | 右侧附着目标。                       |
| **Top**                    | `PW_Top`                    | Attachment | `Default`    | 顶部附着目标。                       |
| **BalloonTooltipLayout**   | `PW_BalloonTooltipLayout`   | String     | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipOffImage** | `PW_BalloonTooltipOffImage` | String     | `""`         | 关闭状态时的气泡提示图片。           |
| **BalloonTooltipOffText**  | `PW_BalloonTooltipOffText`  | String     | `""`         | 关闭状态时的气泡提示文本。           |
| **BalloonTooltipOnImage**  | `PW_BalloonTooltipOnImage`  | String     | `""`         | 开启状态时的气泡提示图片。           |
| **BalloonTooltipOnText**   | `PW_BalloonTooltipOnText`   | String     | `""`         | 开启状态时的气泡提示文本。           |
| **Bitmap**                 | `PW_Bitmap`                 | String     | `""`         | 图标路径。                           |
| **BitmapOnly**             | `PW_BitmapOnly`             | Boolean    | `False`      | 是否仅显示图标。                     |
| **Value**                  | `PW_Value`                  | Boolean    | `False`      | **当前值**（开/关状态）。      |
| **ToolTipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockToggle_1770451903115_777",
  "type": "BlockToggle",
  "props": {
    "PW_BlockID": "toggle0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "启用高级选项",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipOffImage": "",
    "PW_BalloonTooltipOffText": "点击以启用高级选项",
    "PW_BalloonTooltipOnImage": "",
    "PW_BalloonTooltipOnText": "点击以禁用高级选项",
    "PW_Bitmap": "",
    "PW_BitmapOnly": false,
    "PW_Value": false,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.4 按钮 (Button - `BlockButton`)

对应 NX Open `BlockButton` 组件，用于触发操作的按钮控件。

> **POWER 模板参考**: `power_ui_template/BlockButton.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值         | 说明                                    |
| :----------------------------- | :-------------------------- | :--------- | :------------- | :-------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `button0`    | 组件唯一标识符。                        |
| **Enable**               | `PW_Enable`               | Boolean    | `True`       | 是否启用。                              |
| **Group**                | `PW_Group`                | Boolean    | `False`      | 是否作为组显示。                        |
| **Label**                | `PW_Label`                | String     | `No Title`   | 按钮文本。                              |
| **Show**                 | `PW_Show`                 | Boolean    | `True`       | 是否可见。                              |
| **Localize**             | `PW_Localize`             | Boolean    | `True`       | 是否支持本地化。                        |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`    | 底部附着目标。                          |
| **Left**                 | `PW_Left`                 | Attachment | `Default`    | 左侧附着目标。                          |
| **Right**                | `PW_Right`                | Attachment | `Default`    | 右侧附着目标。                          |
| **Top**                  | `PW_Top`                  | Attachment | `Default`    | 顶部附着目标。                          |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`         | 气泡提示图片路径。                      |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。    |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`         | 气泡提示文本。                          |
| **Bitmap**               | `PW_Bitmap`               | String     | `""`         | 按钮图标路径。                          |
| **HighQualityBitmap**    | `PW_HighQualityBitmap`    | Boolean    | `True`       | 是否使用高质量位图。                    |
| **Tooltip**              | `PW_Tooltip`              | String     | `""`         | 工具提示文本。                          |
| **ToolTipID**            | `PW_ToolTipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockButton_1770451903116_888",
  "type": "BlockButton",
  "props": {
    "PW_BlockID": "button0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "执行操作",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "execute_icon.png",
    "PW_HighQualityBitmap": true,
    "PW_Tooltip": "点击执行当前操作",
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.5 曲线收集器 (CurveCollector - `BlockCurveCollector`)

对应 NX Open `BlockCurveCollector` 组件，用于选择曲线、边等几何对象。

> **POWER 模板参考**: `power_ui_template/BlockCurveCollector.ui`

| UI设计器显示名称                      | 导出JSON字段名称                   | 类型       | 默认值              | 说明                                                                                                                                         |
| :------------------------------------ | :--------------------------------- | :--------- | :------------------ | :------------------------------------------------------------------------------------------------------------------------------------------- |
| **BlockID**                     | `PW_BlockID`                     | String     | `edge_select0`    | 组件唯一标识符。                                                                                                                             |
| **Enable**                      | `PW_Enable`                      | Boolean    | `True`            | 是否启用。                                                                                                                                   |
| **Group**                       | `PW_Group`                       | Boolean    | `False`           | 是否作为组显示。                                                                                                                             |
| **Show**                        | `PW_Show`                        | Boolean    | `True`            | 是否可见。                                                                                                                                   |
| **Bottom**                      | `PW_Bottom`                      | Attachment | `Default`         | 底部附着目标。                                                                                                                               |
| **Left**                        | `PW_Left`                        | Attachment | `Default`         | 左侧附着目标。                                                                                                                               |
| **Right**                       | `PW_Right`                       | Attachment | `Default`         | 右侧附着目标。                                                                                                                               |
| **Top**                         | `PW_Top`                         | Attachment | `Default`         | 顶部附着目标。                                                                                                                               |
| **AllowConvergentObject**       | `PW_AllowConvergentObject`       | Boolean    | `False`           | 是否允许选择收敛对象。                                                                                                                       |
| **AutomaticProgression**        | `PW_AutomaticProgression`        | Boolean    | `False`           | 是否自动推进到下一步。                                                                                                                       |
| **BalloonTooltipImage**         | `PW_BalloonTooltipImage`         | String     | `""`              | 气泡提示图片路径。                                                                                                                           |
| **BalloonTooltipLayout**        | `PW_BalloonTooltipLayout`        | String     | `Horizontal`      | 气泡提示布局 (Horizontal/Vertical)。                                                                                                         |
| **BalloonTooltipText**          | `PW_BalloonTooltipText`          | String     | `""`              | 气泡提示文本。                                                                                                                               |
| **Bitmap**                      | `PW_Bitmap`                      | String     | `""`              | 图标路径。                                                                                                                                   |
| **BlendVirtualCurveOverlay**    | `PW_BlendVirtualCurveOverlay`    | Boolean    | `False`           | 是否显示混合虚拟曲线叠加。                                                                                                                   |
| **CreateInterpartLink**         | `PW_CreateInterpartLink`         | Boolean    | `False`           | 是否创建零部件间链接。                                                                                                                       |
| **Cue**                         | `PW_Cue`                         | String     | `Select Curve`    | 提示文本。                                                                                                                                   |
| **InterpartSelection**          | `PW_InterpartSelection`          | String     | `Simple`          | 零部件间选择模式 (Simple, Non-associative Interpart Copy Only, Associative and Non-associative Interpart Copy, Associative Interpart Copy)。 |
| **LabelString**                 | `PW_LabelString`                 | String     | `Select Curve`    | 标签文本。                                                                                                                                   |
| **MaximumScope**                | `PW_MaximumScope`                | String     | `Entire Assembly` | 最大选择范围 (Entire Assembly, Within Work Part Only, Within Work Part and Components)。                                                     |
| **SelectMode**                  | `PW_SelectMode`                  | String     | `Single`          | 选择模式 (Single, Multiple)。                                                                                                                |
| **StepStatus**                  | `PW_StepStatus`                  | String     | `Required`        | 步骤状态 (Required, Optional, Satisfied)。                                                                                                   |
| **ToolTip**                     | `PW_ToolTip`                     | String     | `Curve`           | 工具提示文本。                                                                                                                               |
| **AllowInferredCurveSelection** | `PW_AllowInferredCurveSelection` | Boolean    | `True`            | 是否允许推断曲线选择。                                                                                                                       |
| **CurveRules**                  | `PW_CurveRules`                  | Integer    | `799`             | 曲线规则位掩码。                                                                                                                             |
| **DefaultCurveRules**           | `PW_DefaultCurveRules`           | String     | `Single Curve`    | 默认曲线规则 (Single Curve, Connected Curves, Tangent Curves, Face Edges, Body Edges, Vertex Edges, Vertex Tangent Edges)。                  |
| **EntityType**                  | `PW_EntityType`                  | Integer    | `5`               | 实体类型。                                                                                                                                   |
| **InferredCurveSelection**      | `PW_InferredCurveSelection`      | Boolean    | `False`           | 是否使用推断曲线选择。                                                                                                                       |
| **PopupMenuEnabled**            | `PW_PopupMenuEnabled`            | Boolean    | `True`            | 是否启用右键弹出菜单。                                                                                                                       |
| **ToolTipID**                   | `PW_ToolTipID`                   | String     | `""`              | 工具提示 ID。**(POWER 特有属性)**                                                                                                      |

**示例**:

```json
{
  "id": "BlockCurveCollector_1770451903117_999",
  "type": "BlockCurveCollector",
  "props": {
    "PW_BlockID": "edge_select0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowConvergentObject": false,
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "选择曲线",
    "PW_Bitmap": "",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "Select Curve",
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "选择曲线",
    "PW_MaximumScope": "Entire Assembly",
    "PW_SelectMode": "Multiple",
    "PW_StepStatus": "Required",
    "PW_ToolTip": "Curve",
    "PW_AllowInferredCurveSelection": true,
    "PW_CurveRules": 799,
    "PW_DefaultCurveRules": "Single Curve",
    "PW_EntityType": 5,
    "PW_InferredCurveSelection": false,
    "PW_PopupMenuEnabled": true,
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.6 指定点 (SpecifyPoint - `BlockSpecifyPoint`)

对应 NX Open `BlockSpecifyPoint` 组件，用于选择或指定点位置。

> **POWER 模板参考**: `power_ui_template/BlockSpecifyPoint.ui`

| UI设计器显示名称                    | 导出JSON字段名称                 | 类型       | 默认值            | 说明                                                                                                                                         |
| :---------------------------------- | :------------------------------- | :--------- | :---------------- | :------------------------------------------------------------------------------------------------------------------------------------------- |
| **BlockID**                   | `PW_BlockID`                   | String     | `point0`        | 组件唯一标识符。                                                                                                                             |
| **Enable**                    | `PW_Enable`                    | Boolean    | `True`          | 是否启用。                                                                                                                                   |
| **Group**                     | `PW_Group`                     | Boolean    | `False`         | 是否作为组显示。                                                                                                                             |
| **Label**                     | `PW_Label`                     | String     | `""`            | 标签文本。                                                                                                                                   |
| **Show**                      | `PW_Show`                      | Boolean    | `True`          | 是否可见。                                                                                                                                   |
| **Bottom**                    | `PW_Bottom`                    | Attachment | `Default`       | 底部附着目标。                                                                                                                               |
| **Left**                      | `PW_Left`                      | Attachment | `Default`       | 左侧附着目标。                                                                                                                               |
| **Right**                     | `PW_Right`                     | Attachment | `Default`       | 右侧附着目标。                                                                                                                               |
| **Top**                       | `PW_Top`                       | Attachment | `Default`       | 顶部附着目标。                                                                                                                               |
| **AutomaticProgression**      | `PW_AutomaticProgression`      | Boolean    | `True`          | 是否自动推进到下一步。                                                                                                                       |
| **BalloonTooltipImage**       | `PW_BalloonTooltipImage`       | String     | `""`            | 气泡提示图片路径。                                                                                                                           |
| **BalloonTooltipLayout**      | `PW_BalloonTooltipLayout`      | String     | `Horizontal`    | 气泡提示布局 (Horizontal/Vertical)。                                                                                                         |
| **BalloonTooltipText**        | `PW_BalloonTooltipText`        | String     | `""`            | 气泡提示文本。                                                                                                                               |
| **CreateInterpartLink**       | `PW_CreateInterpartLink`       | Boolean    | `False`         | 是否创建零部件间链接。                                                                                                                       |
| **EnableFaceSelection**       | `PW_EnableFaceSelection`       | Boolean    | `False`         | 是否启用面选择。                                                                                                                             |
| **InterpartSelection**        | `PW_InterpartSelection`        | String     | `Simple`        | 零部件间选择模式 (Simple, Non-associative Interpart Copy Only, Associative and Non-associative Interpart Copy, Associative Interpart Copy)。 |
| **LabelString**               | `PW_LabelString`               | String     | `Specify Point` | 标签字符串。                                                                                                                                 |
| **SnapPointTypesEnabled**     | `PW_SnapPointTypesEnabled`     | String[]   | `[...]`         | 启用的捕捉点类型列表。                                                                                                                       |
| **SnapPointTypesOnByDefault** | `PW_SnapPointTypesOnByDefault` | String[]   | `[...]`         | 默认开启的捕捉点类型列表。                                                                                                                   |
| **StepStatus**                | `PW_StepStatus`                | String     | `Required`      | 步骤状态 (Required, Optional, Satisfied)。                                                                                                   |
| **ToolTipID**                 | `PW_ToolTipID`                 | String     | `""`            | 工具提示 ID。**(POWER 特有属性)**                                                                                                      |

**捕捉点类型说明** (SnapPointTypes):

| 掩码        | 类型名称                           | 说明              |
| :---------- | :--------------------------------- | :---------------- |
| `0x1`     | User Defined                       | 用户定义点        |
| `0x2`     | Inferred                           | 推断点            |
| `0x4`     | Screen Position                    | 屏幕位置          |
| `0x8`     | End Point                          | 端点              |
| `0x10`    | Mid Point                          | 中点              |
| `0x20`    | Control Point                      | 控制点            |
| `0x40`    | Intersection                       | 交点              |
| `0x80`    | Arc Center                         | 圆弧中心          |
| `0x100`   | Quadrant Point / Point Constructor | 象限点 / 点构造器 |
| `0x200`   | Existing Point                     | 现有点            |
| `0x400`   | Point on Curve                     | 曲线上的点        |
| `0x800`   | Point on Surface                   | 曲面上的点        |
| `0x2000`  | Two-curve Intersection             | 双曲线交点        |
| `0x4000`  | Tangent Point                      | 切点              |
| `0x8000`  | Poles                              | 极点              |
| `0x10000` | Bounded Grid Point                 | 边界网格点        |
| `0x20000` | Facet Vertex Point                 | 刻面顶点          |
| `0x40000` | Defining Point                     | 定义点            |

**示例**:

```json
{
  "id": "BlockSpecifyPoint_1770451903118_111",
  "type": "BlockSpecifyPoint",
  "props": {
    "PW_BlockID": "point0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": true,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_CreateInterpartLink": false,
    "PW_EnableFaceSelection": false,
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "指定点",
    "PW_SnapPointTypesEnabled": [],
    "PW_SnapPointTypesOnByDefault": [],
    "PW_StepStatus": "Required",
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.7 线性尺寸 (LinearDimension - `BlockLinearExpression`)

对应 NX Open `BlockLinearExpression` 组件，用于输入线性尺寸值，支持表达式和拖拽操作。

> **POWER 模板参考**: `power_ui_template/BlockLinearExpression.ui`

| UI设计器显示名称                | 导出JSON字段名称             | 类型       | 默认值            | 说明                                    |
| :------------------------------ | :--------------------------- | :--------- | :---------------- | :-------------------------------------- |
| **BlockID**               | `PW_BlockID`               | String     | `linear_dim0`   | 组件唯一标识符。                        |
| **Enable**                | `PW_Enable`                | Boolean    | `True`          | 是否启用。                              |
| **Group**                 | `PW_Group`                 | Boolean    | `False`         | 是否作为组显示。                        |
| **Label**                 | `PW_Label`                 | String     | `""`            | 标签文本。                              |
| **Show**                  | `PW_Show`                  | Boolean    | `True`          | 是否可见。                              |
| **RetainUnits**           | `PW_RetainUnits`           | Boolean    | `True`          | 是否保留单位设置。                      |
| **RetainValue**           | `PW_RetainValue`           | Boolean    | `True`          | 是否在对话框关闭后保留值。              |
| **Bottom**                | `PW_Bottom`                | Attachment | `Default`       | 底部附着目标。                          |
| **Left**                  | `PW_Left`                  | Attachment | `Default`       | 左侧附着目标。                          |
| **Right**                 | `PW_Right`                 | Attachment | `Default`       | 右侧附着目标。                          |
| **Top**                   | `PW_Top`                   | Attachment | `Default`       | 顶部附着目标。                          |
| **AdaptiveScaleLimits** | `PW_AdaptiveScaleLimits` | Boolean    | `False`         | 是否自适应缩放限制。                    |
| **AutoReverseDuringDrag** | `PW_AutoReverseDuringDrag` | Boolean    | `False`         | 拖拽时是否自动反转方向。                |
| **BalloonTooltipImage**   | `PW_BalloonTooltipImage`   | String     | `""`            | 气泡提示图片路径。                      |
| **BalloonTooltipLayout**  | `PW_BalloonTooltipLayout`  | String     | `Horizontal`    | 气泡提示布局 (Horizontal/Vertical)。    |
| **BalloonTooltipText**    | `PW_BalloonTooltipText`    | String     | `""`            | 气泡提示文本。                          |
| **Formula**               | `PW_Formula`               | String     | `"0"`           | 表达式公式。                            |
| **HandleOrientation**     | `PW_HandleOrientation`     | String     | `"0 0 0"`       | 拖拽手柄方向 (X Y Z)。                  |
| **HandleOrigin**          | `PW_HandleOrigin`          | String     | `"0 0 0"`       | 拖拽手柄原点 (X Y Z)。                  |
| **LimitCheckTolerance**   | `PW_LimitCheckTolerance`   | Double     | `-1`            | 限制检查容差 (-1 表示禁用)。            |
| **MaximumValue**          | `PW_MaximumValue`          | Double     | `1.79769e+308`  | 最大值。                                |
| **MaxInclusive**          | `PW_MaxInclusive`          | Boolean    | `True`          | 最大值是否包含边界。                    |
| **MinimumValue**          | `PW_MinimumValue`          | Double     | `-1.79769e+308` | 最小值。                                |
| **MinInclusive**          | `PW_MinInclusive`          | Boolean    | `True`          | 最小值是否包含边界。                    |
| **ShowFocusHandle**       | `PW_ShowFocusHandle`       | Boolean    | `False`         | 是否显示焦点手柄。                      |
| **ShowHandle**            | `PW_ShowHandle`            | Boolean    | `False`         | 是否显示拖拽手柄。                      |
| **ShowSpin**              | `PW_ShowSpin`              | Boolean    | `False`         | 是否显示旋转控制。                      |
| **Value**                 | `PW_Value`                 | Double     | `0`             | **当前值**。                      |
| **WithScale**             | `PW_WithScale`             | Boolean    | `False`         | 是否带缩放滑块。                        |
| **EnableReverseDirection** | `PW_EnableReverseDirection` | Boolean    | `False`         | 是否启用反向方向。                      |
| **ToolTipID**             | `PW_ToolTipID`             | String     | `""`            | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockLinearExpression_1770451903119_222",
  "type": "BlockLinearExpression",
  "props": {
    "PW_BlockID": "linear_dim0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "长度",
    "PW_Show": true,
    "PW_RetainUnits": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AdaptiveScaleLimits": false
    "PW_AutoReverseDuringDrag": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Formula": "10",
    "PW_HandleOrientation": "0 0 1",
    "PW_HandleOrigin": "0 0 0",
    "PW_LimitCheckTolerance": -1,
    "PW_MaximumValue": 1000.0,
    "PW_MaxInclusive": true,
    "PW_MinimumValue": 0.0,
    "PW_MinInclusive": true,
    "PW_ShowFocusHandle": false,
    "PW_ShowHandle": true,
    "PW_ShowSpin": false
    "PW_Value": 10.0,
    "PW_WithScale": false,
    "PW_EnableReverseDirection": false
    "PW_ToolTipID": "",
      },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.8 角度尺寸 (AngularDimension - `BlockAngularExpression`)

对应 NX Open `BlockAngularExpression` 组件，用于输入角度值，支持表达式和拖拽操作。

> **POWER 模板参考**: `power_ui_template/BlockAngularExpression.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值            | 说明                                    |
| :----------------------------- | :-------------------------- | :--------- | :---------------- | :-------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `angular_dim0`  | 组件唯一标识符。                        |
| **Enable**               | `PW_Enable`               | Boolean    | `True`          | 是否启用。                              |
| **Group**                | `PW_Group`                | Boolean    | `False`         | 是否作为组显示。                        |
| **Label**                | `PW_Label`                | String     | `""`            | 标签文本。                              |
| **Show**                 | `PW_Show`                 | Boolean    | `True`          | 是否可见。                              |
| **RetainUnits**          | `PW_RetainUnits`          | Boolean    | `True`          | 是否保留单位设置。                      |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`          | 是否在对话框关闭后保留值。              |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`       | 底部附着目标。                          |
| **Left**                 | `PW_Left`                 | Attachment | `Default`       | 左侧附着目标。                          |
| **Right**                | `PW_Right`                | Attachment | `Default`       | 右侧附着目标。                          |
| **Top**                  | `PW_Top`                  | Attachment | `Default`       | 顶部附着目标。                          |
| **AdaptiveScaleLimits** | `PW_AdaptiveScaleLimits` | Boolean    | `False`         | 是否自适应缩放限制。                    |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`            | 气泡提示图片路径。                      |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal`    | 气泡提示布局 (Horizontal/Vertical)。    |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`            | 气泡提示文本。                          |
| **Formula**              | `PW_Formula`              | String     | `"0"`           | 表达式公式。                            |
| **HandleFixedSizeFlag**  | `PW_HandleFixedSizeFlag`  | Boolean    | `False`         | 手柄是否固定大小。                      |
| **HandleOrigin**         | `PW_HandleOrigin`         | String     | `"0 0 0"`       | 拖拽手柄原点 (X Y Z)。                  |
| **HandleRadius**         | `PW_HandleRadius`         | Double     | `0`             | 手柄半径。                              |
| **HandleRadiusOffset**   | `PW_HandleRadiusOffset`   | Double     | `0`             | 手柄半径偏移。                          |
| **HandleXAxis**          | `PW_HandleXAxis`          | String     | `"0 0 0"`       | 手柄 X 轴方向。                         |
| **HandleZAxis**          | `PW_HandleZAxis`          | String     | `"0 0 0"`       | 手柄 Z 轴方向。                         |
| **LimitCheckTolerance**  | `PW_LimitCheckTolerance`  | Double     | `-1`            | 限制检查容差 (-1 表示禁用)。            |
| **MaximumValue**         | `PW_MaximumValue`         | Double     | `1.79769e+308`  | 最大值 (度)。                           |
| **MaxInclusive**         | `PW_MaxInclusive`         | Boolean    | `True`          | 最大值是否包含边界。                    |
| **MinimumValue**         | `PW_MinimumValue`         | Double     | `-1.79769e+308` | 最小值 (度)。                           |
| **MinInclusive**         | `PW_MinInclusive`         | Boolean    | `True`          | 最小值是否包含边界。                    |
| **MinRadius**            | `PW_MinRadius`            | Double     | `0`             | 最小半径。                              |
| **ShowFocusHandle**      | `PW_ShowFocusHandle`      | Boolean    | `False`         | 是否显示焦点手柄。                      |
| **ShowHandle**           | `PW_ShowHandle`           | Boolean    | `False`         | 是否显示拖拽手柄。                      |
| **ShowSpin**             | `PW_ShowSpin`             | Boolean    | `False`         | 是否显示旋转控制。                      |
| **Value**                | `PW_Value`                | Double     | `0`             | **当前值** (度)。                 |
| **WithScale**            | `PW_WithScale`            | Boolean    | `False`         | 是否带缩放滑块。                        |
| **ToolTipID**            | `PW_ToolTipID`            | String     | `""`            | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockAngularExpression_1770451903120_333",
  "type": "BlockAngularExpression",
  "props": {
    "PW_BlockID": "angular_dim0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "旋转角度",
    "PW_Show": true,
    "PW_RetainUnits": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AdaptiveScaleLimits": false
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Formula": "45",
    "PW_HandleFixedSizeFlag": false,
    "PW_HandleOrigin": "0 0 0",
    "PW_HandleRadius": 50.0,
    "PW_HandleRadiusOffset": 0,
    "PW_HandleXAxis": "1 0 0",
    "PW_HandleZAxis": "0 0 1",
    "PW_LimitCheckTolerance": -1,
    "PW_MaximumValue": 360.0,
    "PW_MaxInclusive": true,
    "PW_MinimumValue": 0.0,
    "PW_MinInclusive": true,
    "PW_MinRadius": 0,
    "PW_ShowFocusHandle": false,
    "PW_ShowHandle": true,
    "PW_ShowSpin": false,
    "PW_Value": 45.0,
    "PW_WithScale": false,
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.9 集列表 (SetList - `BlockSetList`)

对应 NX Open `BlockSetList` 组件，用于管理对象集合列表，支持添加、删除和重新排序。

> **POWER 模板参考**: `power_ui_template/BlockSetList.ui`

| UI设计器显示名称                      | 导出JSON字段名称                   | 类型       | 默认值          | 说明                           |
| :------------------------------------ | :--------------------------------- | :--------- | :-------------- | :----------------------------- |
| **BlockID**                     | `PW_BlockID`                     | String     | `list0`       | 组件唯一标识符。               |
| **Enable**                      | `PW_Enable`                      | Boolean    | `True`        | 是否启用。                     |
| **Group**                       | `PW_Group`                       | Boolean    | `False`       | 是否作为组显示。               |
| **Label**                       | `PW_Label`                       | String     | `""`          | 标签文本。                     |
| **Show**                        | `PW_Show`                        | Boolean    | `True`        | 是否可见。                     |
| **Bottom**                      | `PW_Bottom`                      | Attachment | `Default`     | 底部附着目标。                 |
| **Left**                        | `PW_Left`                        | Attachment | `Default`     | 左侧附着目标。                 |
| **Right**                       | `PW_Right`                       | Attachment | `Default`     | 右侧附着目标。                 |
| **Top**                         | `PW_Top`                         | Attachment | `Default`     | 顶部附着目标。                 |
| **AddNewSetLabel**              | `PW_AddNewSetLabel`              | String     | `Add New Set` | "添加新集"按钮标签。           |
| **ColumnLabels**                | `PW_ColumnLabels`                | String[]   | `[]`          | 列标签列表。                   |
| **ColumnWidths**                | `PW_ColumnWidths`                | String     | `"-1"`        | 列宽度 (-1 表示自动)。         |
| **DefaultColumnWidth**          | `PW_DefaultColumnWidth`          | Integer    | `100`         | 默认列宽度。                   |
| **Layout**                      | `PW_Layout`                      | String     | `Selection`   | 布局模式 (Selection, Value)。  |
| **ListExpanded**                | `PW_ListExpanded`                | Boolean    | `False`       | 列表是否展开。                 |
| **MarkSeedExpressionsUnusable** | `PW_MarkSeedExpressionsUnusable` | Boolean    | `False`       | 是否将种子表达式标记为不可用。 |
| **MaximumHeight**               | `PW_MaximumHeight`               | Integer    | `0`           | 最大高度 (0 表示无限制)。      |
| **MinimumHeight**               | `PW_MinimumHeight`               | Integer    | `4`           | 最小高度 (行数)。              |
| **MultipleEdit**                | `PW_MultipleEdit`                | Boolean    | `False`       | 是否允许多选编辑。             |
| **NumberColumnString**          | `PW_NumberColumnString`          | String     | `""`          | 序号列字符串。                 |
| **NumberOfColumns**             | `PW_NumberOfColumns`             | Integer    | `1`           | 列数。                         |
| **ResizeHeightWithDialog**      | `PW_ResizeHeightWithDialog`      | Boolean    | `True`        | 高度是否随对话框调整。         |
| **SeedDlxFile**                 | `PW_SeedDlxFile`                 | String     | `""`          | 种子 DLX 文件路径。            |
| **ShowAddNewSet**               | `PW_ShowAddNewSet`               | Boolean    | `True`        | 是否显示"添加新集"按钮。       |
| **ShowColumnHeadings**          | `PW_ShowColumnHeadings`          | Boolean    | `False`       | 是否显示列标题。               |
| **ShowRemove**                  | `PW_ShowRemove`                  | Boolean    | `True`        | 是否显示删除按钮。             |
| **ShowReorderControls**         | `PW_ShowReorderControls`         | Boolean    | `False`       | 是否显示重排序控制按钮。       |
| **ShowTopBottomControls**       | `PW_ShowTopBottomControls`       | Boolean    | `False`       | 是否显示置顶/置底控制按钮。    |

**示例**:

```json
{
  "id": "BlockSetList_1770451903121_444",
  "type": "BlockSetList",
  "props": {
    "PW_BlockID": "list0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "对象列表",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AddNewSetLabel": "添加新集",
    "PW_ColumnLabels": ["名称", "类型", "数量"],
    "PW_ColumnWidths": "-1",
    "PW_DefaultColumnWidth": 100,
    "PW_Layout": "Selection",
    "PW_ListExpanded": false,
    "PW_MarkSeedExpressionsUnusable": false,
    "PW_MaximumHeight": 0,
    "PW_MinimumHeight": 4,
    "PW_MultipleEdit": false,
    "PW_NumberColumnString": "",
    "PW_NumberOfColumns": 3,
    "PW_ResizeHeightWithDialog": true,
    "PW_SeedDlxFile": "",
    "PW_ShowAddNewSet": true,
    "PW_ShowColumnHeadings": true,
    "PW_ShowRemove": true,
    "PW_ShowReorderControls": true,
    "PW_ShowTopBottomControls": false
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.10 反向 (ReverseDirection - `BlockReverseDirection`)

对应 NX Open `BlockReverseDirection` 组件，用于控制方向的翻转，通常与矢量或方向相关的操作配合使用。

> **POWER 模板参考**: `power_ui_template/BlockReverseDirection.ui`

| UI设计器显示名称    | 导出JSON字段名称 | 类型       | 默认值                | 说明               |
| :------------------ | :--------------- | :--------- | :-------------------- | :----------------- |
| **BlockID**   | `PW_BlockID`   | String     | `direction0`        | 组件唯一标识符。   |
| **Enable**    | `PW_Enable`    | Boolean    | `True`              | 是否启用。         |
| **Group**     | `PW_Group`     | Boolean    | `False`             | 是否作为组显示。   |
| **Label**     | `PW_Label`     | String     | `Reverse Direction` | 标签文本。         |
| **Show**      | `PW_Show`      | Boolean    | `True`              | 是否可见。         |
| **Bottom**    | `PW_Bottom`    | Attachment | `Default`           | 底部附着目标。     |
| **Left**      | `PW_Left`      | Attachment | `Default`           | 左侧附着目标。     |
| **Right**     | `PW_Right`     | Attachment | `Default`           | 右侧附着目标。     |
| **Top**       | `PW_Top`       | Attachment | `Default`           | 顶部附着目标。     |
| **Direction** | `PW_Direction` | String     | `"0 0 1"`           | 方向向量 (X Y Z)。 |
| **Flip**      | `PW_Flip`      | Boolean    | `False`             | 是否翻转方向。     |
| **Origin**    | `PW_Origin`    | String     | `"0 0 0"`           | 方向原点 (X Y Z)。 |
| **TooltipID**    | `PW_TooltipID`   | String     | `""`                  | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockReverseDirection_1770451903122_555",
  "type": "BlockReverseDirection",
  "props": {
    "PW_BlockID": "direction0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "反转方向",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_Direction": "0 0 1",
    "PW_Flip": false,
    "PW_Origin": "0 0 0",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.11 指定平面 (SpecifyPlane - `BlockSpecifyPlane`)

对应 NX Open `BlockSpecifyPlane` 组件，用于选择或指定一个平面。

> **POWER 模板参考**: `power_ui_template/BlockSpecifyPlane.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值         | 说明                                                                                                                                         |
| :----------------------------- | :-------------------------- | :--------- | :------------- | :------------------------------------------------------------------------------------------------------------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `plane0`     | 组件唯一标识符。                                                                                                                             |
| **Enable**               | `PW_Enable`               | Boolean    | `True`       | 是否启用。                                                                                                                                   |
| **Group**                | `PW_Group`                | Boolean    | `False`      | 是否作为组显示。                                                                                                                             |
| **Label**                | `PW_Label`                | String     | `""`         | 标签文本。                                                                                                                                   |
| **Show**                 | `PW_Show`                 | Boolean    | `True`       | 是否可见。                                                                                                                                   |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`    | 底部附着目标。                                                                                                                               |
| **Left**                 | `PW_Left`                 | Attachment | `Default`    | 左侧附着目标。                                                                                                                               |
| **Right**                | `PW_Right`                | Attachment | `Default`    | 右侧附着目标。                                                                                                                               |
| **Top**                  | `PW_Top`                  | Attachment | `Default`    | 顶部附着目标。                                                                                                                               |
| **AutomaticProgression** | `PW_AutomaticProgression` | Boolean    | `True`       | 是否自动推进到下一步。                                                                                                                       |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`         | 气泡提示图片路径。                                                                                                                           |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。                                                                                                         |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`         | 气泡提示文本。                                                                                                                               |
| **CreateInterpartLink**  | `PW_CreateInterpartLink`  | Boolean    | `False`      | 是否创建零部件间链接。                                                                                                                       |
| **InterpartSelection**   | `PW_InterpartSelection`   | String     | `Simple`     | 零部件间选择模式 (Simple, Non-associative Interpart Copy Only, Associative and Non-associative Interpart Copy, Associative Interpart Copy)。 |
| **LabelString**          | `PW_LabelString`          | String     | `指定平面`   | 标签字符串。                                                                                                                                 |
| **StepStatus**           | `PW_StepStatus`           | String     | `Required`   | 步骤状态 (Required, Optional, Satisfied)。                                                                                                   |
| **EnabheReverseDirection** | `PW_EnabheReverseDirection` | Boolean    | `False`      | 是否启用反向方向。                                                                                                                           |
| **TooltipID**          | `PW_TooltipID`          | String     | `""`         | 工具提示 ID。**(POWER 特有属性)**                                                                                                      |

**示例**:

```json
{
  "id": "BlockSpecifyPlane_1770451903123_666",
  "type": "BlockSpecifyPlane",
  "props": {
    "PW_BlockID": "plane0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": true,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_CreateInterpartLink": false,
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "指定平面",
    "PW_StepStatus": "Required",
    "PW_EnabheReverseDirection": false
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.12 双精度 (Double - `BlockDouble`)

对应 NX Open `BlockDouble` 组件，用于输入双精度浮点数值。

> **POWER 模板参考**: `power_ui_template/BlockDouble.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值            | 说明                                               |
| :----------------------------- | :-------------------------- | :--------- | :---------------- | :------------------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `double0`       | 组件唯一标识符。                                   |
| **Enable**               | `PW_Enable`               | Boolean    | `True`          | 是否启用。                                         |
| **Group**                | `PW_Group`                | Boolean    | `False`         | 是否作为组显示。                                   |
| **Label**                | `PW_Label`                | String     | `No Title`      | 标签文本。                                         |
| **Show**                 | `PW_Show`                 | Boolean    | `True`          | 是否可见。                                         |
| **Localize**             | `PW_Localize`             | Boolean    | `True`          | 是否支持本地化。                                   |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`          | 是否在对话框关闭后保留值。                         |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`       | 底部附着目标。                                     |
| **Left**                 | `PW_Left`                 | Attachment | `Default`       | 左侧附着目标。                                     |
| **Right**                | `PW_Right`                | Attachment | `Default`       | 右侧附着目标。                                     |
| **Top**                  | `PW_Top`                  | Attachment | `Default`       | 顶部附着目标。                                     |
| **AllowUnitEdit**        | `PW_AllowUnitEdit`        | Boolean    | `False`         | 是否允许编辑单位。                                 |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`            | 气泡提示图片路径。                                 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal`    | 气泡提示布局 (Horizontal/Vertical)。               |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`            | 气泡提示文本。                                     |
| **Bitmap**               | `PW_Bitmap`               | String     | `""`            | 图标路径。                                         |
| **Dimensionality**       | `PW_Dimensionality`       | String     | `无单位`        | 量纲/单位类型。                                    |
| **KeyinFieldWidth**      | `PW_KeyinFieldWidth`      | String     | `Narrow`        | 输入框宽度 (Narrow, Standard)。                    |
| **LimitCheckTolerance**  | `PW_LimitCheckTolerance`  | Double     | `-1`            | 限制检查容差 (-1 表示禁用)。                       |
| **MaximumValue**         | `PW_MaximumValue`         | Double     | `1.79769e+308`  | 最大值。                                           |
| **MaxInclusive**         | `PW_MaxInclusive`         | Boolean    | `True`          | 最大值是否包含边界。                               |
| **MinimumValue**         | `PW_MinimumValue`         | Double     | `-1.79769e+308` | 最小值。                                           |
| **MinInclusive**         | `PW_MinInclusive`         | Boolean    | `True`          | 最小值是否包含边界。                               |
| **PresentationStyle**    | `PW_PresentationStyle`    | String     | `Keyin`         | 展示样式 (Keyin, Spin, Scale, ScaleKeyin, Combo)。 |
| **ReadOnlyValue**        | `PW_ReadOnlyValue`        | Boolean    | `False`         | 值是否只读。                                       |
| **RetainUnits**          | `PW_RetainUnits`          | Boolean    | `True`          | 是否保留单位设置。                                 |
| **ShowUnitLabel**        | `PW_ShowUnitLabel`        | Boolean    | `False`         | 是否显示单位标签。                                 |
| **TitleVisibility**      | `PW_TitleVisibility`      | Boolean    | `True`          | 标题是否可见。                                     |
| **Value**                | `PW_Value`                | Double     | `0`             | **当前值**。                                 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`            | 工具提示 ID。**(POWER 特有属性)**                                    |

**示例**:

```json
{
  "id": "BlockDouble_1770451903124_777",
  "type": "BlockDouble",
  "props": {
    "PW_BlockID": "double0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "半径",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowUnitEdit": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "",
    "PW_Dimensionality": "无单位",
    "PW_KeyinFieldWidth": "Standard",
    "PW_LimitCheckTolerance": -1,
    "PW_MaximumValue": 1000.0,
    "PW_MaxInclusive": true,
    "PW_MinimumValue": 0.0,
    "PW_MinInclusive": true,
    "PW_PresentationStyle": "Keyin",
    "PW_ReadOnlyValue": false,
    "PW_RetainUnits": true,
    "PW_ShowUnitLabel": false,
    "PW_TitleVisibility": true,
    "PW_Value": 5.0,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.13 整数 (Integer - `BlockInteger`)

对应 NX Open `BlockInteger` 组件，用于输入整数值。

> **POWER 模板参考**: `power_ui_template/BlockInteger.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值          | 说明                                               |
| :----------------------------- | :-------------------------- | :--------- | :-------------- | :------------------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `integer0`    | 组件唯一标识符。                                   |
| **Enable**               | `PW_Enable`               | Boolean    | `True`        | 是否启用。                                         |
| **Group**                | `PW_Group`                | Boolean    | `False`       | 是否作为组显示。                                   |
| **Label**                | `PW_Label`                | String     | `No Title`    | 标签文本。                                         |
| **Show**                 | `PW_Show`                 | Boolean    | `True`        | 是否可见。                                         |
| **Localize**             | `PW_Localize`             | Boolean    | `True`        | 是否支持本地化。                                   |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`        | 是否在对话框关闭后保留值。                         |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`     | 底部附着目标。                                     |
| **Left**                 | `PW_Left`                 | Attachment | `Default`     | 左侧附着目标。                                     |
| **Right**                | `PW_Right`                | Attachment | `Default`     | 右侧附着目标。                                     |
| **Top**                  | `PW_Top`                  | Attachment | `Default`     | 顶部附着目标。                                     |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`          | 气泡提示图片路径。                                 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal`  | 气泡提示布局 (Horizontal/Vertical)。               |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`          | 气泡提示文本。                                     |
| **Bitmap**               | `PW_Bitmap`               | String     | `""`          | 图标路径。                                         |
| **KeyinFieldWidth**      | `PW_KeyinFieldWidth`      | String     | `Narrow`      | 输入框宽度 (Narrow, Standard)。                    |
| **MaximumValue**         | `PW_MaximumValue`         | Integer    | `2147483647`  | 最大值。                                           |
| **MinimumValue**         | `PW_MinimumValue`         | Integer    | `-2147483648` | 最小值。                                           |
| **PresentationStyle**    | `PW_PresentationStyle`    | String     | `Keyin`       | 展示样式 (Keyin, Spin, Scale, ScaleKeyin, Combo)。 |
| **ReadOnlyValue**        | `PW_ReadOnlyValue`        | Boolean    | `False`       | 值是否只读。                                       |
| **TitleVisibility**      | `PW_TitleVisibility`      | Boolean    | `True`        | 标题是否可见。                                     |
| **Value**                | `PW_Value`                | Integer    | `0`           | **当前值**。                                 |
| **ToolTipID**            | `PW_ToolTipID`            | String     | `""`          | 工具提示 ID。**(POWER 特有属性)**            |

**示例**:

```json
{
  "id": "BlockInteger_1770451903125_888",
  "type": "BlockInteger",
  "props": {
    "PW_BlockID": "integer0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "数量",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "",
    "PW_KeyinFieldWidth": "Standard",
    "PW_MaximumValue": 100,
    "PW_MinimumValue": 1,
    "PW_PresentationStyle": "Keyin",
    "PW_ReadOnlyValue": false,
    "PW_TitleVisibility": true,
    "PW_Value": 1,
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.14 面收集器 (FaceCollector - `BlockFaceCollector`)

对应 NX Open `BlockFaceCollector` 组件，用于选择面几何对象。

> **POWER 模板参考**: `power_ui_template/BlockFaceCollector.ui`

| UI设计器显示名称                   | 导出JSON字段名称                | 类型       | 默认值                    | 说明                                       |
| :--------------------------------- | :------------------------------ | :--------- | :------------------------ | :----------------------------------------- |
| **BlockID**                  | `PW_BlockID`                  | String     | `face_select0`          | 组件唯一标识符。                           |
| **Enable**                   | `PW_Enable`                   | Boolean    | `True`                  | 是否启用。                                 |
| **Group**                    | `PW_Group`                    | Boolean    | `False`                 | 是否作为组显示。                           |
| **Show**                     | `PW_Show`                     | Boolean    | `True`                  | 是否可见。                                 |
| **Bottom**                   | `PW_Bottom`                   | Attachment | `Default`               | 底部附着目标。                             |
| **Left**                     | `PW_Left`                     | Attachment | `Default`               | 左侧附着目标。                             |
| **Right**                    | `PW_Right`                    | Attachment | `Default`               | 右侧附着目标。                             |
| **Top**                      | `PW_Top`                      | Attachment | `Default`               | 顶部附着目标。                             |
| **AllowConvergentObject**    | `PW_AllowConvergentObject`    | Boolean    | `False`                 | 是否允许选择收敛对象。                     |
| **AutomaticProgression**     | `PW_AutomaticProgression`     | Boolean    | `False`                 | 是否自动推进到下一步。                     |
| **BalloonTooltipImage**      | `PW_BalloonTooltipImage`      | String     | `""`                    | 气泡提示图片路径。                         |
| **BalloonTooltipLayout**     | `PW_BalloonTooltipLayout`     | String     | `Horizontal`            | 气泡提示布局 (Horizontal/Vertical)。       |
| **BalloonTooltipText**       | `PW_BalloonTooltipText`       | String     | `select_face`           | 气泡提示文本。                             |
| **Bitmap**                   | `PW_Bitmap`                   | String     | `select_face`           | 图标路径。                                 |
| **BlendVirtualCurveOverlay** | `PW_BlendVirtualCurveOverlay` | Boolean    | `False`                 | 是否显示混合虚拟曲线叠加。                 |
| **CreateInterpartLink**      | `PW_CreateInterpartLink`      | Boolean    | `False`                 | 是否创建零部件间链接。                     |
| **Cue**                      | `PW_Cue`                      | String     | `Select Face`           | 提示文本。                                 |
| **InterpartSelection**       | `PW_InterpartSelection`       | String     | `Simple`                | 零部件间选择模式。                         |
| **LabelString**              | `PW_LabelString`              | String     | `Select Face`           | 标签文本。                                 |
| **MaximumScope**             | `PW_MaximumScope`             | String     | `Entire Assembly`       | 最大选择范围。                             |
| **SelectMode**               | `PW_SelectMode`               | String     | `Single`                | 选择模式 (Single, Multiple)。              |
| **StepStatus**               | `PW_StepStatus`               | String     | `Required`              | 步骤状态 (Required, Optional, Satisfied)。 |
| **ToolTip**                  | `PW_ToolTip`                  | String     | `Face`                  | 工具提示文本。                             |
| **DefaultFaceRules**         | `PW_DefaultFaceRules`         | String     | `Single Face`           | 默认面规则。                               |
| **EntityType**               | `PW_EntityType`               | Integer    | `(0x10)`                | 实体类型 (0x10)。                          |
| **FaceRules**                | `PW_FaceRules`                | Integer    | `(0xe7f)`               | 面规则位掩码 (0xe7f)。                     |
| **PopupMenuEnabled**         | `PW_PopupMenuEnabled`         | Boolean    | `True`                  | 是否启用右键弹出菜单。                     |
| **ToolTipID**                | `PW_ToolTipID`                | String     | `""`                    | 工具提示 ID。**(POWER 特有属性)**    |

**示例**:

```json
{
  "id": "BlockFaceCollector_1770451903113_601",
  "type": "BlockFaceCollector",
  "props": {
    "PW_BlockID": "face_select0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowConvergentObject": false,
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "select_face",
    "PW_Bitmap": "select_face",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "Select Face",
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "Select Face",
    "PW_MaximumScope": "Entire Assembly",
    "PW_SelectMode": "Single",
    "PW_StepStatus": "Required",
    "PW_ToolTip": "Face",
    "PW_DefaultFaceRules": "Single Face",
    "PW_EntityType": 16,
    "PW_FaceRules": 3711,
    "PW_PopupMenuEnabled": true,
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.15 体收集器 (BodyCollector - `BlockBodyCollector`)

对应 NX Open `BlockBodyCollector` 组件，用于选择实体/片体几何对象。

> **POWER 模板参考**: `power_ui_template/BlockBodyCollector.ui`

| UI设计器显示名称                   | 导出JSON字段名称                | 类型       | 默认值                    | 说明                                       |
| :--------------------------------- | :------------------------------ | :--------- | :------------------------ | :----------------------------------------- |
| **BlockID**                  | `PW_BlockID`                  | String     | `bodySelect0`           | 组件唯一标识符。                           |
| **Enable**                   | `PW_Enable`                   | Boolean    | `True`                  | 是否启用。                                 |
| **Group**                    | `PW_Group`                    | Boolean    | `False`                 | 是否作为组显示。                           |
| **Show**                     | `PW_Show`                     | Boolean    | `True`                  | 是否可见。                                 |
| **Bottom**                   | `PW_Bottom`                   | Attachment | `Default`               | 底部附着目标。                             |
| **Left**                     | `PW_Left`                     | Attachment | `Default`               | 左侧附着目标。                             |
| **Right**                    | `PW_Right`                    | Attachment | `Default`               | 右侧附着目标。                             |
| **Top**                      | `PW_Top`                      | Attachment | `Default`               | 顶部附着目标。                             |
| **AllowConvergentObject**    | `PW_AllowConvergentObject`    | Boolean    | `False`                 | 是否允许选择收敛对象。                     |
| **AutomaticProgression**     | `PW_AutomaticProgression`     | Boolean    | `False`                 | 是否自动推进到下一步。                     |
| **BalloonTooltipImage**      | `PW_BalloonTooltipImage`      | String     | `""`                    | 气泡提示图片路径。                         |
| **BalloonTooltipLayout**     | `PW_BalloonTooltipLayout`     | String     | `Horizontal`            | 气泡提示布局 (Horizontal/Vertical)。       |
| **BalloonTooltipText**       | `PW_BalloonTooltipText`       | String     | `select_body`           | 气泡提示文本。                             |
| **Bitmap**                   | `PW_Bitmap`                   | String     | `""`                    | 图标路径。                                 |
| **BlendVirtualCurveOverlay** | `PW_BlendVirtualCurveOverlay` | Boolean    | `False`                 | 是否显示混合虚拟曲线叠加。                 |
| **CreateInterpartLink**      | `PW_CreateInterpartLink`      | Boolean    | `False`                 | 是否创建零部件间链接。                     |
| **Cue**                      | `PW_Cue`                      | String     | `Select Body`           | 提示文本。                                 |
| **InterpartSelection**       | `PW_InterpartSelection`       | String     | `Simple`                | 零部件间选择模式。                         |
| **LabelString**              | `PW_LabelString`              | String     | `Select Body`           | 标签文本。                                 |
| **MaximumScope**             | `PW_MaximumScope`             | String     | `Entire Assembly`       | 最大选择范围。                             |
| **SelectMode**               | `PW_SelectMode`               | String     | `Single`                | 选择模式 (Single, Multiple)。              |
| **StepStatus**               | `PW_StepStatus`               | String     | `Required`              | 步骤状态 (Required, Optional, Satisfied)。 |
| **ToolTip**                  | `PW_ToolTip`                  | String     | `Body`                  | 工具提示文本。                             |
| **BodyRules**                | `PW_BodyRules`                | Integer    | `(0x66)`                | 体规则位掩码 (0x66)。                      |
| **DefaultBodyRules**         | `PW_DefaultBodyRules`         | String     | `Single Body`           | 默认体规则。                               |
| **EntityType**               | `PW_EntityType`               | Integer    | `(0x40)`                | 实体类型 (0x40)。                          |
| **IncludeSheetBodies**       | `PW_IncludeSheetBodies`       | Boolean    | `True`                  | 是否包含片体。                             |
| **PopupMenuEnabled**         | `PW_PopupMenuEnabled`         | Boolean    | `True`                  | 是否启用右键弹出菜单。                     |
| **ToolTipID**                | `PW_ToolTipID`                | String     | `""`                    | 工具提示 ID。**(POWER 特有属性)**    |

**示例**:

```json
{
  "id": "BlockBodyCollector_1770451903113_602",
  "type": "BlockBodyCollector",
  "props": {
    "PW_BlockID": "bodySelect0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowConvergentObject": false,
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "select_body",
    "PW_Bitmap": "",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "Select Body",
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "Select Body",
    "PW_MaximumScope": "Entire Assembly",
    "PW_SelectMode": "Single",
    "PW_StepStatus": "Required",
    "PW_ToolTip": "Body",
    "PW_BodyRules": 102,
    "PW_DefaultBodyRules": "Single Body",
    "PW_EntityType": 64,
    "PW_IncludeSheetBodies": true,
    "PW_PopupMenuEnabled": true,
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.16 选择对象 (SelectObject - `BlockSelectObject`)

对应 NX Open `BlockSelectObject` 组件，用于通用对象选择。

> **POWER 模板参考**: `power_ui_template/BlockSelectObject.ui`

| UI设计器显示名称                    | 导出JSON字段名称                 | 类型       | 默认值                    | 说明                         |
| :---------------------------------- | :------------------------------- | :--------- | :------------------------ | :--------------------------- |
| **PointOverlay**              | `PW_PointOverlay`              | Boolean    | `False`                 | 是否显示点叠加。             |
| **SmartUpdateOption**         | `PW_SmartUpdateOption`         | String     | `Within Modeling`       | 智能更新选项。               |
| **SnapPointTypesEnabled**     | `PW_SnapPointTypesEnabled`     | Integer    | `(0x31ff8)`             | 启用的捕捉点类型位掩码。     |
| **SnapPointTypesOnByDefault** | `PW_SnapPointTypesOnByDefault` | Integer    | `(0x289)`               | 默认启用的捕捉点类型位掩码。 |
| **BlockID**                   | `PW_BlockID`                   | String     | `selection0`            | 组件唯一标识符。             |
| **Enable**                    | `PW_Enable`                    | Boolean    | `True`                  | 是否启用。                   |
| **Group**                     | `PW_Group`                     | Boolean    | `False`                 | 是否作为组显示。             |
| **Show**                      | `PW_Show`                      | Boolean    | `True`                  | 是否可见。                   |
| **Bottom**                    | `PW_Bottom`                    | Attachment | `Default`               | 底部附着目标。               |
| **Left**                      | `PW_Left`                      | Attachment | `Default`               | 左侧附着目标。               |
| **Right**                     | `PW_Right`                     | Attachment | `Default`               | 右侧附着目标。               |
| **Top**                       | `PW_Top`                       | Attachment | `Default`               | 顶部附着目标。               |
| **AllowConvergentObject**     | `PW_AllowConvergentObject`     | Boolean    | `False`                 | 是否允许选择收敛对象。       |
| **AutomaticProgression**      | `PW_AutomaticProgression`      | Boolean    | `True`                  | 是否自动推进到下一步。       |
| **BalloonTooltipImage**       | `PW_BalloonTooltipImage`       | String     | `""`                    | 气泡提示图片路径。           |
| **BalloonTooltipLayout**      | `PW_BalloonTooltipLayout`      | String     | `Vertical`              | 气泡提示布局。               |
| **BalloonTooltipText**        | `PW_BalloonTooltipText`        | String     | `""`                    | 气泡提示文本。               |
| **Bitmaps**                   | `PW_Bitmaps`                   | String     | `selection_cursor`      | 图标路径。                   |
| **BlendVirtualCurveOverlay**  | `PW_BlendVirtualCurveOverlay`  | Boolean    | `False`                 | 是否显示混合虚拟曲线叠加。   |
| **CreateInterpartLink**       | `PW_CreateInterpartLink`       | Boolean    | `False`                 | 是否创建零部件间链接。       |
| **Cue**                       | `PW_Cue`                       | String     | `""`                    | 提示文本。                   |
| **LabelString**               | `PW_LabelString`               | String     | `Select Object`         | 标签文本。                   |
| **MaximumScope**              | `PW_MaximumScope`              | String     | `Entire Assembly`       | 最大选择范围。               |
| **SelectMode**                | `PW_SelectMode`                | String     | `Single`                | 选择模式。                   |
| **StepStatus**                | `PW_StepStatus`                | String     | `Required`              | 步骤状态。                   |
| **ToolTip**                   | `PW_ToolTip`                   | String     | `""`                    | 工具提示文本。               |
| **ToolTipID**                 | `PW_ToolTipID`                 | String     | `""`                    | 工具提示 ID。                |
| **InterpartSelection**        | `PW_InterpartSelection`        | String     | `Simple`                | 零部件间选择模式。           |

**示例**:

```json
{
  "id": "BlockSelectObject_1770451903113_603",
  "type": "BlockSelectObject",
  "props": {
    "PW_PointOverlay": false,
    "PW_SmartUpdateOption": "Within Modeling",
    "PW_SnapPointTypesEnabled": 204792,
    "PW_SnapPointTypesOnByDefault": 649,
    "PW_BlockID": "selection0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowConvergentObject": false,
    "PW_AutomaticProgression": true,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Vertical",
    "PW_BalloonTooltipText": "",
    "PW_Bitmaps": "selection_cursor",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "",
    "PW_LabelString": "Select Object",
    "PW_MaximumScope": "Entire Assembly",
    "PW_SelectMode": "Single",
    "PW_StepStatus": "Required",
    "PW_ToolTip": "",
    "PW_ToolTipID": "",
    "PW_InterpartSelection": "Simple"
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.17 半径尺寸 (RadiusExpression - `BlockRadiusExpression`)

对应 NX Open `BlockRadiusExpression` 组件，用于输入半径值，支持表达式和拖拽操作。

> **POWER 模板参考**: `power_ui_template/BlockRadiusExpression.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值                       | 说明                         |
| :----------------------------- | :-------------------------- | :--------- | :--------------------------- | :--------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `radius_dim0`              | 组件唯一标识符。             |
| **Enable**               | `PW_Enable`               | Boolean    | `True`                     | 是否启用。                   |
| **Group**                | `PW_Group`                | Boolean    | `False`                    | 是否作为组显示。             |
| **Label**                | `PW_Label`                | String     | `""`                       | 标签文本。                   |
| **Show**                 | `PW_Show`                 | Boolean    | `True`                     | 是否可见。                   |
| **RetainUnits**          | `PW_RetainUnits`          | Boolean    | `True`                     | 是否保留单位设置。           |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`                     | 是否在对话框关闭后保留值。   |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`                  | 底部附着目标。               |
| **Left**                 | `PW_Left`                 | Attachment | `Default`                  | 左侧附着目标。               |
| **Right**                | `PW_Right`                | Attachment | `Default`                  | 右侧附着目标。               |
| **Top**                  | `PW_Top`                  | Attachment | `Default`                  | 顶部附着目标。               |
| **AdaptiveScaleLimits** | `PW_AdaptiveScaleLimits` | Boolean    | `False`         | 是否自适应缩放限制。                    |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`                       | 气泡提示图片路径。           |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal`               | 气泡提示布局。               |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`                       | 气泡提示文本。               |
| **Formula**              | `PW_Formula`              | String     | `"0"`                      | 表达式公式。                 |
| **HandleOrientation**    | `PW_HandleOrientation`    | String     | `"0 0 0"`                  | 拖拽手柄方向 (X Y Z)。       |
| **HandleOrigin**         | `PW_HandleOrigin`         | String     | `"0 0 0"`                  | 拖拽手柄原点 (X Y Z)。       |
| **LimitCheckTolerance**  | `PW_LimitCheckTolerance`  | Double     | `-1`                       | 限制检查容差 (-1 表示禁用)。 |
| **MaximumValue**         | `PW_MaximumValue`         | Double     | `1.7976931348623157e+308`  | 最大值。                     |
| **MaxInclusive**         | `PW_MaxInclusive`         | Boolean    | `True`                     | 最大值包含。                 |
| **MinimumValue**         | `PW_MinimumValue`         | Double     | `-1.7976931348623157e+308` | 最小值。                     |
| **MinInclusive**         | `PW_MinInclusive`         | Boolean    | `True`                     | 最小值包含。                 |
| **PageIncrement**        | `PW_PageIncrement`        | Double     | `1e+19`                    | 页面增量。                   |
| **ShowFocusHandle**      | `PW_ShowFocusHandle`      | Boolean    | `False`                    | 是否显示焦点手柄。           |
| **ShowHandle**           | `PW_ShowHandle`           | Boolean    | `False`                    | 是否显示拖拽手柄。           |
| **ShowSpin**             | `PW_ShowSpin`             | Boolean    | `False`                    | 是否显示旋转控制。           |
| **Value**                | `PW_Value`                | Double     | `0`                        | **当前值**。           |
| **WithScale**            | `PW_WithScale`            | Boolean    | `False`                    | 是否带缩放滑块。             |
| **ToolTipID**            | `PW_ToolTipID`            | String     | `""`                       | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockRadiusExpression_1770451903113_604",
  "type": "BlockRadiusExpression",
  "props": {
    "PW_BlockID": "radius_dim0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "半径",
    "PW_Show": true,
    "PW_RetainUnits": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AdaptiveScaleLimits": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Formula": "10.0",
    "PW_HandleOrientation": "0 0 0",
    "PW_HandleOrigin": "0 0 0",
    "PW_LimitCheckTolerance": -1,
    "PW_MaximumValue": 1000.0,
    "PW_MaxInclusive": true,
    "PW_MinimumValue": 0.0,
    "PW_MinInclusive": true,
    "PW_PageIncrement": 1e+19,
    "PW_ShowFocusHandle": false,
    "PW_ShowHandle": true,
    "PW_ShowSpin": false,
    "PW_Value": 10.0,
    "PW_WithScale": false,
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.18 标签 (Label - `BlockLabel`)

对应 NX Open `BlockLabel` 组件，用于显示静态文本标签。

> **POWER 模板参考**: `power_ui_template/BlockLabel.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值         | 说明                 |
| :----------------------------- | :-------------------------- | :--------- | :------------- | :------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `label0`     | 组件唯一标识符。     |
| **Enable**               | `PW_Enable`               | Boolean    | `True`       | 是否启用。           |
| **Group**                | `PW_Group`                | Boolean    | `False`      | 是否作为组显示。     |
| **Label**                | `PW_Label`                | String     | `无标题`     | 标签文本。           |
| **Show**                 | `PW_Show`                 | Boolean    | `True`       | 是否可见。           |
| **Localize**             | `PW_Localize`             | Boolean    | `True`       | 是否支持本地化。     |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`    | 底部附着目标。       |
| **Left**                 | `PW_Left`                 | Attachment | `Default`    | 左侧附着目标。       |
| **Right**                | `PW_Right`                | Attachment | `Default`    | 右侧附着目标。       |
| **Top**                  | `PW_Top`                  | Attachment | `Default`    | 顶部附着目标。       |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`         | 气泡提示图片路径。   |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal` | 气泡提示布局。       |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`         | 气泡提示文本。       |
| **Bitmap**               | `PW_Bitmap`               | String     | `""`         | 图标路径。           |
| **DisplayBitmapLabel**   | `PW_DisplayBitmapLabel`   | Boolean    | `False`      | 是否显示位图标签。   |
| **HighQualityBitmap**    | `PW_HighQualityBitmap`    | Boolean    | `True`       | 是否使用高质量位图。 |
| **Tooltip**              | `PW_Tooltip`              | String     | `""`         | 工具提示文本。       |
| **WordWrap**             | `PW_WordWrap`             | Boolean    | `False`      | 是否自动换行。       |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`            | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockLabel_1770451903113_605",
  "type": "BlockLabel",
  "props": {
    "PW_BlockID": "label0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "提示信息",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "",
    "PW_DisplayBitmapLabel": false,
    "PW_HighQualityBitmap": true,
    "PW_Tooltip": "",
    "PW_WordWrap": false,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.19 分隔符 (Separator - `BlockSeparator`)

对应 NX Open `BlockSeparator` 组件，用于在控件之间添加水平分隔线。

> **POWER 模板参考**: `power_ui_template/BlockSeparator.ui`

| UI设计器显示名称  | 导出JSON字段名称 | 类型       | 默认值         | 说明             |
| :---------------- | :--------------- | :--------- | :------------- | :--------------- |
| **BlockID** | `PW_BlockID`   | String     | `separator0` | 组件唯一标识符。 |
| **Show**    | `PW_Show`      | Boolean    | `True`       | 是否可见。       |
| **Bottom**  | `PW_Bottom`    | Attachment | `Default`    | 底部附着目标。   |
| **Left**    | `PW_Left`      | Attachment | `Default`    | 左侧附着目标。   |
| **Right**   | `PW_Right`     | Attachment | `Default`    | 右侧附着目标。   |
| **Top**     | `PW_Top`       | Attachment | `Default`    | 顶部附着目标。   |

**示例**:

```json
{
  "id": "BlockSeparator_1770451903113_608",
  "type": "BlockSeparator",
  "props": {
    "PW_BlockID": "separator0",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default"
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.20 滚动窗口 (ScrolledWindow - `BlockScrollWindow`)

对应 NX Open `BlockScrollWindow` 组件，提供可滚动的**容器窗口**，可包含子控件。

> **POWER 模板参考**: `power_ui_template/BlockScrollWindow.ui`

| UI设计器显示名称                 | 导出JSON字段名称              | 类型    | 默认值             | 说明                   |
| :------------------------------- | :---------------------------- | :------ | :----------------- | :--------------------- |
| **BlockID**                | `PW_BlockID`                | String  | `scrolledWindow` | 组件唯一标识符。       |
| **Enable**                 | `PW_Enable`                 | Boolean | `True`           | 是否启用。             |
| **Expanded**               | `PW_Expanded`               | Boolean | `True`           | 初始时是否展开。       |
| **Group**                  | `PW_Group`                  | Boolean | `False`          | 是否作为组显示。       |
| **Label**                  | `PW_Label`                  | String  | `No Title`       | 标签文本。             |
| **Show**                   | `PW_Show`                   | Boolean | `True`           | 是否可见。             |
| **Bottom**                 | `PW_Bottom`                 | String  | `Default`        | 底部边距。             |
| **Left**                   | `PW_Left`                   | String  | `Default`        | 左侧边距。             |
| **Right**                  | `PW_Right`                  | String  | `Default`        | 右侧边距。             |
| **Top**                    | `PW_Top`                    | String  | `Default`        | 顶部边距。             |
| **Height**                 | `PW_Height`                 | Integer | `3`              | 初始高度 (行数)。      |
| **ResizeHeightWithDialog** | `PW_ResizeHeightWithDialog` | Boolean | `True`           | 高度是否随对话框调整。 |
| **Width**                  | `PW_Width`                  | Integer | `0`              | 宽度 (0 表示自动)。    |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockScrollWindow_1770451903113_609",
  "type": "BlockScrollWindow",
  "props": {
    "PW_BlockID": "scrolledWindow",
    "PW_Enable": true,
    "PW_Expanded": true,
    "PW_Group": false,
    "PW_Label": "详细设置",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_Height": 5,
    "PW_ResizeHeightWithDialog": true,
    "PW_Width": 0,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": [...]
}
```

### 3.21 选项卡 (TabWidget - `BlockTabWidget`)

对应 NX Open `BlockTabWidget` 组件，用于分页显示多组控件，是一个**容器类控件**。`children` 中每个子组件通常是 `BlockGroup` 类型，每个 Group 代表一个选项卡页面。

> **POWER 模板参考**: `power_ui_template/BlockTabWidget.ui`

| UI设计器显示名称            | 导出JSON字段名称         | 类型       | 默认值         | 说明                           |
| :-------------------------- | :----------------------- | :--------- | :------------- | :----------------------------- |
| **BlockID**           | `PW_BlockID`           | String     | `tabControl` | 组件唯一标识符。               |
| **Enable**            | `PW_Enable`            | Boolean    | `TRUE`       | 是否启用。                     |
| **Expanded**          | `PW_Expanded`          | Boolean    | `TRUE`       | 初始时是否展开。               |
| **Group**             | `PW_Group`             | Boolean    | `FALSE`      | 是否作为组显示。               |
| **Label**             | `PW_Label`             | String     | `No Title`   | 标签文本。                     |
| **Show**              | `PW_Show`              | Boolean    | `TRUE`       | 是否可见。                     |
| **HighQualityBitmap** | `PW_HighQualityBitmap` | Boolean    | `TRUE`       | 是否使用高质量位图。           |
| **Localize**          | `PW_Localize`          | Boolean    | `TRUE`       | 是否支持本地化。               |
| **RetainValue**       | `PW_RetainValue`       | Boolean    | `TRUE`       | 是否在对话框关闭后保留值。     |
| **Bottom**            | `PW_Bottom`            | Attachment | `Default`    | 底部附着目标。                 |
| **Left**              | `PW_Left`              | Attachment | `Default`    | 左侧附着目标。                 |
| **Right**             | `PW_Right`             | Attachment | `Default`    | 右侧附着目标。                 |
| **Top**               | `PW_Top`               | Attachment | `Default`    | 顶部附着目标。                 |
| **ActivePage**        | `PW_ActivePage`        | Integer    | `-1`         | 初始活动页索引 (-1 表示默认)。 |
| **HiddenTabPages**    | `PW_HiddenTabPages`    | String     | `-1`         | 要隐藏的页面索引。             |
| **TabsPerRow**        | `PW_TabsPerRow`        | Integer    | `0`          | 每行选项卡数量 (0 表示自动)。  |

**示例**:

```json
{
  "id": "BlockTabControl_1770451903113_610",
  "type": "BlockTabControl",
  "props": {
        "PW_BlockID": "group1",
    "PW_Enable": true,
    "PW_Expanded": true,
    "PW_Group": false,
        "PW_Label": "高级",
        "PW_Show": true,
    "PW_HighQualityBitmap": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_ActivePage": -1,
    "PW_HiddenTabPages": -1,
    "PW_TabsPerRow": 0
      },
      "hiddenProps": [],
      "altProperties": {},
      "children": []
    }
  ]
}
```

### 3.22 树列表 (Tree - `BlockTree`)

对应 NX Open `BlockTree` 组件，用于展示层级结构的树形列表。

> **POWER 模板参考**: `power_ui_template/BlockTree.ui`

| UI设计器显示名称                   | 导出JSON字段名称                | 类型       | 默认值                   | 说明                                |
| :--------------------------------- | :------------------------------ | :--------- | :----------------------- | :---------------------------------- |
| **BlockID**                  | `PW_BlockID`                  | String     | `tree_control0`        | 组件唯一标识符。                    |
| **Enable**                   | `PW_Enable`                   | Boolean    | `TRUE`                 | 是否启用。                          |
| **Group**                    | `PW_Group`                    | Boolean    | `FALSE`                | 是否作为组显示。                    |
| **Label**                    | `PW_Label`                    | String     | `""`                   | 标签文本。                          |
| **Show**                     | `PW_Show`                     | Boolean    | `TRUE`                 | 是否可见。                          |
| **Localize**                 | `PW_Localize`                 | Boolean    | `TRUE`                 | 是否支持本地化。                    |
| **Bottom**                   | `PW_Bottom`                   | Attachment | `Default`              | 底部附着目标。                      |
| **Left**                     | `PW_Left`                     | Attachment | `Default`              | 左侧附着目标。                      |
| **Right**                    | `PW_Right`                    | Attachment | `Default`              | 右侧附着目标。                      |
| **Top**                      | `PW_Top`                      | Attachment | `Default`              | 顶部附着目标。                      |
| **CanStretchHeight**         | `PW_CanStretchHeight`         | Boolean    | `TRUE`                 | 高度是否可拉伸。                    |
| **CanStretchWidth**          | `PW_CanStretchWidth`          | Boolean    | `TRUE`                 | 宽度是否可拉伸。                    |
| **Height**                   | `PW_Height`                   | Integer    | `0`                    | 初始高度 (0 表示自动)。             |
| **MaximumHeight**            | `PW_MaximumHeight`            | Integer    | `0`                    | 最大高度 (0 表示无限制)。           |
| **MaximumWidth**             | `PW_MaximumWidth`             | Integer    | `0`                    | 最大宽度 (0 表示无限制)。           |
| **MinimumHeight**            | `PW_MinimumHeight`            | Integer    | `0`                    | 最小高度 (0 表示无限制)。           |
| **MinimumWidth**             | `PW_MinimumWidth`             | Integer    | `0`                    | 最小宽度 (0 表示无限制)。           |
| **ScrollFrozenColumn**       | `PW_ScrollFrozenColumn`       | Integer    | `0`                    | 冻结列数。                          |
| **ScrollLineNumber**         | `PW_ScrollLineNumber`         | Integer    | `-1`                   | 滚动行号 (-1 表示默认)。            |
| **SelectionMode**            | `PW_SelectionMode`            | String     | `Multiple,Single,None` | 选择模式 (Multiple, Single, None)。 |
| **ShowExpandCollapseMarker** | `PW_ShowExpandCollapseMarker` | Boolean    | `TRUE`                 | 是否显示展开/折叠标记。             |
| **ShowHeader**               | `PW_ShowHeader`               | Boolean    | `TRUE`                 | 是否显示列表头。                    |
| **ShowMultipleColumns**      | `PW_ShowMultipleColumns`      | Boolean    | `TRUE`                 | 是否显示多列。                      |
| **ShowToolTips**             | `PW_ShowToolTips`             | Boolean    | `TRUE`                 | 是否显示工具提示。                  |
| **SortRootNodes**            | `PW_SortRootNodes`            | Boolean    | `TRUE`                 | 是否对根节点排序。                  |
| **Width**                    | `PW_Width`                    | Integer    | `0`                    | 宽度 (0 表示自动)。                 |
| **ColumnNames**            | `PW_ColumnNames`            | String[]   | `[]`         | 列名称列表。                           |
| **ColumnWidths**           | `PW_ColumnWidths`           | String     | `""`         | 列宽度字符串（空格分隔）。             |

**示例**:

```json
{
  "id": "BlockTree_1770451903113_611",
  "type": "BlockTree",
  "props": {
    "PW_BlockID": "tree_control0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "结构树",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_CanStretchHeight": true,
    "PW_CanStretchWidth": true,
    "PW_Height": 0,
    "PW_MaximumHeight": 0,
    "PW_MaximumWidth": 0,
    "PW_MinimumHeight": 0,
    "PW_MinimumWidth": 0,
    "PW_ScrollFrozenColumn": 0,
    "PW_ScrollLineNumber": -1,
    "PW_SelectionMode": "Multiple",
    "PW_ShowExpandCollapseMarker": true,
    "PW_ShowHeader": true,
    "PW_ShowMultipleColumns": true,
    "PW_ShowToolTips": true,
    "PW_SortRootNodes": true,
    "PW_Width": 0,
    "PW_ColumnNames": ["1", "2", "3"],
    "PW_ColumnWidths": "100 150 200"
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.23 指定矢量 (SpecifyVector - `BlockSpecifyVector`)

对应 NX Open `BlockSpecifyVector` 组件，用于选择或指定一个方向矢量。

> **POWER 模板参考**: `power_ui_template/BlockSpecifyVector.ui`

| UI设计器显示名称                    | 导出JSON字段名称                 | 类型       | 默认值                     | 说明                                       |
| :---------------------------------- | :------------------------------- | :--------- | :------------------------- | :----------------------------------------- |
| **BlockID**                   | `PW_BlockID`                   | String     | `axis0`                  | 组件唯一标识符。                           |
| **Enable**                    | `PW_Enable`                    | Boolean    | `True`                   | 是否启用。                                 |
| **Group**                     | `PW_Group`                     | Boolean    | `False`                  | 是否作为组显示。                           |
| **Label**                     | `PW_Label`                     | String     | `Axis`                   | 标签文本。                                 |
| **Show**                      | `PW_Show`                      | Boolean    | `True`                   | 是否可见。                                 |
| **Bottom**                    | `PW_Bottom`                    | Attachment | `Default`                | 底部附着目标。                             |
| **Left**                      | `PW_Left`                      | Attachment | `Default`                | 左侧附着目标。                             |
| **Right**                     | `PW_Right`                     | Attachment | `Default`                | 右侧附着目标。                             |
| **Top**                       | `PW_Top`                       | Attachment | `Default`                | 顶部附着目标。                             |
| **AutomaticProgression**      | `PW_AutomaticProgression`      | Boolean    | `True`                   | 是否自动推进到下一步。                     |
| **BalloonTooltipImage**       | `PW_BalloonTooltipImage`       | String     | `""`                     | 气泡提示图片路径。                         |
| **BalloonTooltipLayout**      | `PW_BalloonTooltipLayout`      | String     | `Horizontal`             | 气泡提示布局 (Horizontal/Vertical)。       |
| **BalloonTooltipText**        | `PW_BalloonTooltipText`        | String     | `""`                     | 气泡提示文本。                             |
| **CreateInterpartLink**       | `PW_CreateInterpartLink`       | Boolean    | `False`                  | 是否创建零部件间链接。                     |
| **DoubleSide**                | `PW_DoubleSide`                | Boolean    | `False`                  | 是否双侧。                                 |
| **EnableFacetSelection**      | `PW_EnableFacetSelection`      | Boolean    | `False`                  | 是否启用刻面选择。                         |
| **EnableReverseDirection**    | `PW_EnableReverseDirection`    | Boolean    | `True`                   | 是否启用反向按钮。                         |
| **InterpartSelection**        | `PW_InterpartSelection`        | String     | `Simple`                 | 零部件间选择模式。                         |
| **Is2DMode**                  | `PW_Is2DMode`                  | Boolean    | `False`                  | 是否为 2D 模式。                           |
| **LabelString**               | `PW_LabelString`               | String     | `""`                     | 标签字符串。                               |
| **SmartUpdateOption**         | `PW_SmartUpdateOption`         | String     | `Within Modeling`        | 智能更新选项。                             |
| **SnapPointTypesOnByDefault** | `PW_SnapPointTypesOnByDefault` | String     | `(0x298)Arc Center...`   | 默认启用的捕捉点类型位掩码。               |
| **StepStatus**                | `PW_StepStatus`                | String     | `Required`               | 步骤状态 (Required, Optional, Satisfied)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockSpecifyVector_1770451903113_613",
  "type": "BlockSpecifyVector",
  "props": {
    "PW_BlockID": "axis0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "Axis",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": true,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_CreateInterpartLink": false,
    "PW_DoubleSide": false,
    "PW_EnableFacetSelection": false,
    "PW_EnableReverseDirection": true,
    "PW_InterpartSelection": "Simple",
    "PW_Is2DMode": false,
    "PW_LabelString": "",
    "PW_SmartUpdateOption": "Within Modeling",
    "PW_SnapPointTypesOnByDefault": "(0x298)",
    "PW_StepStatus": "Required",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.24 指定轴 (SpecifyAxis - `BlockSpecifyAxis`)

对应 NX Open `BlockSpecifyAxis` 组件，用于选择或指定一个轴（点 + 方向）。

> **POWER 模板参考**: `power_ui_template/BlockSpecifyAxis.ui`

| UI设计器显示名称                    | 导出JSON字段名称                 | 类型       | 默认值         | 说明                                       |
| :---------------------------------- | :------------------------------- | :--------- | :------------- | :----------------------------------------- |
| **BlockID**                   | `PW_BlockID`                   | String     | `axis0`      | 组件唯一标识符。                           |
| **Enable**                    | `PW_Enable`                    | Boolean    | `True`       | 是否启用。                                 |
| **Group**                     | `PW_Group`                     | Boolean    | `False`      | 是否作为组显示。                           |
| **Label**                     | `PW_Label`                     | String     | `Axis`       | 标签文本。                                 |
| **Show**                      | `PW_Show`                      | Boolean    | `True`       | 是否可见。                                 |
| **Bottom**                    | `PW_Bottom`                    | Attachment | `Default`    | 底部附着目标。                             |
| **Left**                      | `PW_Left`                      | Attachment | `Default`    | 左侧附着目标。                             |
| **Right**                     | `PW_Right`                     | Attachment | `Default`    | 右侧附着目标。                             |
| **Top**                       | `PW_Top`                       | Attachment | `Default`    | 顶部附着目标。                             |
| **BalloonTooltipLayout**      | `PW_BalloonTooltipLayout`      | String     | `Horizontal` | 气泡提示布局。                             |
| **BalloonTooltipPointImage**  | `PW_BalloonTooltipPointImage`  | String     | `""`         | 点部分的气泡提示图片。                     |
| **BalloonTooltipPointText**   | `PW_BalloonTooltipPointText`   | String     | `""`         | 点部分的气泡提示文本。                     |
| **BalloonTooltipVectorImage** | `PW_BalloonTooltipVectorImage` | String     | `""`         | 矢量部分的气泡提示图片。                   |
| **BalloonTooltipVectorText**  | `PW_BalloonTooltipVectorText`  | String     | `""`         | 矢量部分的气泡提示文本。                   |
| **StepStatus**                | `PW_StepStatus`                | String     | `Required`   | 步骤状态 (Required, Optional, Satisfied)。 |
| **VectorLabel**          | `PW_VectorLabel`          | String     | `""`         | 矢量标签文本。                         |
| **PointLabel**           | `PW_PointLabel`           | String     | `""`         | 点���签文本。                           |
| **VectorTooltipID**      | `PW_VectorTooltipID`      | String     | `""`         | 矢量工具提示 ID。**(POWER 特有属性)** |
| **PointTooltipID**       | `PW_PointTooltipID`       | String     | `""`         | 点工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockSpecifyAxis_1770451903113_614",
  "type": "BlockSpecifyAxis",
  "props": {
    "PW_BlockID": "axis0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "Axis",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipPointImage": "",
    "PW_BalloonTooltipPointText": "",
    "PW_BalloonTooltipVectorImage": "",
    "PW_BalloonTooltipVectorText": "",
    "PW_StepStatus": "Required",
    "PW_VectorLabel": "Specify Vector",
    "PW_PointLabel": "Specify Point",
    "PW_VectorTooltipID": "",
    "PW_PointTooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.25 截面构造器 (SectionBuilder - `BlockSectionBuilder`)

对应 NX Open `BlockSectionBuilder` 组件，用于构建截面轮廓（由曲线构成的闭合/开放剖面）。

> **POWER 模板参考**: `power_ui_template/Section.ui`

| UI设计器显示名称                           | 导出JSON字段名称                        | 类型       | 默认值                                                   | 说明                                |
| :----------------------------------------- | :-------------------------------------- | :--------- | :------------------------------------------------------- | :---------------------------------- |
| **BlockID**                          | `PW_BlockID`                          | String     | `section0`                                             | 组件唯一标识符。                    |
| **Enable**                           | `PW_Enable`                           | Boolean    | `TRUE`                                                 | 是否启用。                          |
| **Group**                            | `PW_Group`                            | Boolean    | `FALSE`                                                | 是否作为组显示。                    |
| **Show**                             | `PW_Show`                             | Boolean    | `TRUE`                                                 | 是否可见。                          |
| **PointOverlay**                     | `PW_PointOverlay`                     | Boolean    | `FALSE`                                                | 是否显示点叠加。                    |
| **SmartUpdateOption**                | `PW_SmartUpdateOption`                | String     | `Within Modeling`                                      | 智能更新选项。                      |
| **SnapPointTypesEnabled**            | `PW_SnapPointTypesEnabled`            | String[]   | `["(0x80)Arc Center"...]`                              | 启用的捕捉点类型 (完整列表见JSON)。 |
| **SnapPointTypesOnByDefault**        | `PW_SnapPointTypesOnByDefault`        | String[]   | `["(0x80)Arc Center"...]`                              | 默认启用的捕捉点类型。              |
| **Bottom**                           | `PW_Bottom`                           | Attachment | `Default`                                              | 底部附着目标。                      |
| **Left**                             | `PW_Left`                             | Attachment | `Default`                                              | 左侧附着目标。                      |
| **Right**                            | `PW_Right`                            | Attachment | `Default`                                              | 右侧附着目标。                      |
| **Top**                              | `PW_Top`                              | Attachment | `Default`                                              | 顶部附着目标。                      |
| **AllowConvergentObject**            | `PW_AllowConvergentObject`            | Boolean    | `FALSE`                                                | 是否允许选择收敛对象。              |
| **AllowSelfIntersecting**            | `PW_AllowSelfIntersecting`            | Boolean    | `FALSE`                                                | 是否允许自交。                      |
| **AutomaticProgression**             | `PW_AutomaticProgression`             | Boolean    | `FALSE`                                                | 是否自动推进到下一步。              |
| **BalloonTooltipImage**              | `PW_BalloonTooltipImage`              | String     | `""`                                                   | 气泡提示图片路径。                  |
| **BalloonTooltipLayout**             | `PW_BalloonTooltipLayout`             | String     | `Horizontal,Vertical`                                  | 气泡提示布局。                      |
| **BalloonTooltipText**               | `PW_BalloonTooltipText`               | String     | `""`                                                   | 气泡提示文本。                      |
| **Bitmap**                           | `PW_Bitmap`                           | String     | `sectionbuilder`                                       | 图标路径。                          |
| **BlendVirtualCurveOverlay**         | `PW_BlendVirtualCurveOverlay`         | Boolean    | `FALSE`                                                | 是否显示混合虚拟曲线叠加。          |
| **CreateInterpartLink**              | `PW_CreateInterpartLink`              | Boolean    | `FALSE`                                                | 是否创建零部件间链接。              |
| **Cue**                              | `PW_Cue`                              | String     | `Select curve`                                         | 提示文本。                          |
| **InterpartSelection**               | `PW_InterpartSelection`               | String     | `Simple`                                               | 零部件间选择模式。                  |
| **LabelString**                      | `PW_LabelString`                      | String     | `Select Curve`                                         | 标签字符串。                        |
| **ShowFlowDirectionAndOrigin**       | `PW_ShowFlowDirectionAndOrigin`       | Boolean    | `FALSE`                                                | 是否显示流向和起点。                |
| **StepStatus**                       | `PW_StepStatus`                       | String     | `Required,Optional,Satisfied`                          | 步骤状态。                          |
| **ToolTip**                          | `PW_ToolTip`                          | String     | `Curve`                                                | 工具提示文本。                      |
| **AllowInferredCurveSelection**      | `PW_AllowInferredCurveSelection`      | Boolean    | `TRUE`                                                 | 是否允许推断曲线选择。              |
| **AllowStopAtIntersectionFollowing** | `PW_AllowStopAtIntersectionFollowing` | Boolean    | `TRUE`                                                 | 是否允许在交叉处停止跟随。          |
| **AngularTolerance**                 | `PW_AngularTolerance`                 | Double     | `0.05`                                                 | 角度容差。                          |
| **ChainWithinFeature**               | `PW_ChainWithinFeature`               | Boolean    | `FALSE`                                                | 是否在特征内链接。                  |
| **CurveRules**                       | `PW_CurveRules`                       | String[]   | `["(0x1)Single Curve", "(0x1)Connected Curves"...]`    | 曲线规则列表。                      |
| **DefaultCurveRules**                | `PW_DefaultCurveRules`                | String     | `Single Curve`                                         | 默认曲线规则。                      |
| **EntityType**                       | `PW_EntityType`                       | String[]   | `["(0x5)Allow Edges and Curves", "(0x8)Allow Points"]` | 实体类型列表。                      |
| **FollowFillet**                     | `PW_FollowFillet`                     | Boolean    | `FALSE`                                                | 是否跟随圆角。                      |
| **InferredCurveSelection**           | `PW_InferredCurveSelection`           | Boolean    | `FALSE`                                                | 是否使用推断曲线选择。              |
| **StopAtIntersection**               | `PW_StopAtIntersection`               | Boolean    | `FALSE`                                                | 是否在交叉处停止。                  |
| **TooltipID**                        | `PW_TooltipID`                        | String     | `""`                                                   | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockSection_1770451903113_615",
  "type": "BlockSection",
  "props": {
    "PW_BlockID": "section0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_PointOverlay": false,
    "PW_SmartUpdateOption": "Within Modeling",
    "PW_SnapPointTypesEnabled": ["(0x80)Arc Center"],
    "PW_SnapPointTypesOnByDefault": ["(0x80)Arc Center"],
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowConvergentObject": false,
    "PW_AllowSelfIntersecting": false,
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "sectionbuilder",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "选择截面曲线",
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "Select Curve",
    "PW_ShowFlowDirectionAndOrigin": false,
    "PW_StepStatus": "Required",
    "PW_ToolTip": "Curve",
    "PW_AllowInferredCurveSelection": true,
    "PW_AllowStopAtIntersectionFollowing": true,
    "PW_AngularTolerance": 0.05,
    "PW_ChainWithinFeature": false,
    "PW_CurveRules": ["Single Curve", "Connected Curves", "Tangent Curves", "Face Edges"],
    "PW_DefaultCurveRules": "Single Curve",
    "PW_EntityType": ["Allow Edges and Curves", "Allow Points"],
    "PW_FollowFillet": false,
    "PW_InferredCurveSelection": false,
    "PW_StopAtIntersection": false,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.26 指定方位 (Manipulator - `BlockManipulator`)

对应 NX Open `BlockManipulator` 组件，用于通过 3D 操纵器交互式地指定位置和方向。

> **POWER 模板参考**: `power_ui_template/BlockManipulator.ui`

| UI设计器显示名称                    | 导出JSON字段名称                 | 类型       | 默认值                           | 说明                         |
| :---------------------------------- | :------------------------------- | :--------- | :------------------------------- | :--------------------------- |
| **BlockID**                   | `PW_BlockID`                   | String     | `manip0`                       | 组件唯一标识符。             |
| **Enable**                    | `PW_Enable`                    | Boolean    | `False`                        | 是否启用 (默认禁用)。        |
| **Group**                     | `PW_Group`                     | Boolean    | `False`                        | 是否作为组显示。             |
| **Label**                     | `PW_Label`                     | String     | `Manipulator`                  | 标签文本。                   |
| **Show**                      | `PW_Show`                      | Boolean    | `True`                         | 是否可见。                   |
| **Bottom**                    | `PW_Bottom`                    | Attachment | `Default`                      | 底部附着目标。               |
| **Left**                      | `PW_Left`                      | Attachment | `Default`                      | 左侧附着目标。               |
| **Right**                     | `PW_Right`                     | Attachment | `Default`                      | 右侧附着目标。               |
| **Top**                       | `PW_Top`                       | Attachment | `Default`                      | 顶部附着目标。               |
| **BalloonTooltipImage**       | `PW_BalloonTooltipImage`       | String     | `" "`                          | 气泡提示图片路径。           |
| **BalloonTooltipLayout**      | `PW_BalloonTooltipLayout`      | String     | `Horizontal`                   | 气泡提示布局。               |
| **BalloonTooltipText**        | `PW_BalloonTooltipText`        | String     | `" "`                          | 气泡提示文本。               |
| **EnableDoubleClickFlip**     | `PW_EnableDoubleClickFlip`     | Boolean    | `True`                         | 是否启用双击翻转。           |
| **EnableFaceSelection**       | `PW_EnableFaceSelection`       | Boolean    | `False`                        | 是否启用刻面选择。           |
| **HasOriginGwif**             | `PW_HasOriginGwif`             | Boolean    | `True`                         | 是否有原点 GWIF。            |
| **IsOriginSpecified**         | `PW_IsOriginSpecified`         | Boolean    | `False`                        | 原点是否已指定。             |
| **IsWCSCoordinates**          | `PW_IsWCSCoordinates`          | Boolean    | `False`                        | 是否使用 WCS 坐标。          |
| **Origin**                    | `PW_Origin`                    | String     | `"0.000000 0.000000 0.000000"` | 操纵器原点 (X Y Z)。         |
| **SnapPointTypesOnByDefault** | `PW_SnapPointTypesOnByDefault` | String     | `(0x480298)Arc Center...`      | 默认启用的捕捉点类型位掩码。 |
| **VisibleManipulatorHandles** | `PW_VisibleManipulatorHandles` | String     | `(0x3ff)Origin...`             | 可见的操纵器手柄。           |
| **WCSOrientationSupport**     | `PW_WCSOrientationSupport`     | Boolean    | `True`                         | 是否支持 WCS 方向。          |
| **XAxis**                     | `PW_XAxis`                     | String     | `"1.000000 0.000000 0.000000"` | X 轴方向。                   |
| **YAxis**                     | `PW_YAxis`                     | String     | `"0.000000 1.000000 0.000000"` | Y 轴方向。                   |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockManip_1770451903113_620",
  "type": "BlockManip",
  "props": {
    "PW_BlockID": "manip0",
    "PW_Enable": false,
    "PW_Group": false,
    "PW_Label": "Manipulator",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": " ",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": " ",
    "PW_EnableDoubleClickFlip": true,
    "PW_EnableFaceSelection": false,
    "PW_HasOriginGwif": true,
    "PW_IsOriginSpecified": false,
    "PW_IsWCSCoordinates": false,
    "PW_Origin": "0.000000 0.000000 0.000000",
    "PW_SnapPointTypesOnByDefault": "(0x480298)",
    "PW_VisibleManipulatorHandles": "(0x3ff)",
    "PW_WCSOrientationSupport": true,
    "PW_XAxis": "1.000000 0.000000 0.000000",
    "PW_YAxis": "0.000000 1.000000 0.000000",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.27 指定坐标系 (SpecifyCsys - `BlockSpecifyCsys`)

对应 NX Open `BlockSpecifyCsys` 组件，用于选择或指定一个坐标系。

> **POWER 模板参考**: `power_ui_template/BlockSpecifyCsys.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值             | 说明                                           |
| :----------------------------- | :-------------------------- | :--------- | :----------------- | :--------------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `coord_system0`  | 组件唯一标识符。                               |
| **Enable**               | `PW_Enable`               | Boolean    | `TRUE`           | 是否启用。                                     |
| **Group**                | `PW_Group`                | Boolean    | `FALSE`          | 是否作为组显示。                               |
| **Label**                | `PW_Label`                | String     | `" "`            | 标签文本。                                     |
| **Show**                 | `PW_Show`                 | Boolean    | `TRUE`           | 是否可见。                                     |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`        | 底部附着目标。                                 |
| **Left**                 | `PW_Left`                 | Attachment | `Default`        | 左侧附着目标。                                 |
| **Right**                | `PW_Right`                | Attachment | `Default`        | 右侧附着目标。                                 |
| **Top**                  | `PW_Top`                  | Attachment | `Default`        | 顶部附着目标。                                 |
| **AutomaticProgression** | `PW_AutomaticProgression` | Boolean    | `TRUE`           | 是否自动推进到下一步。                         |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `" "`            | 气泡提示图片路径。                             |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal`     | 气泡提示布局。                                 |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `" "`            | 气泡提示文本。                                 |
| **CreateInterpartLink**  | `PW_CreateInterpartLink`  | Boolean    | `FALSE`          | 是否创建零部件间链接。                         |
| **InterpartSelection**   | `PW_InterpartSelection`   | String     | `Simple`         | 零部件间选择模式。                             |
| **LabelString**          | `PW_LabelString`          | String     | `Specify CSYS`   | 标签字符串。                                   |
| **OutputType**           | `PW_OutputType`           | String     | `Cartesian`      | 输出类型 (Cartesian, Cylindrical, Spherical)。 |
| **SmartUpdateOption**    | `PW_SmartUpdateOption`    | String     | `Within Modeling` | 智能更新选项。                                 |
| **StepStatus**           | `PW_StepStatus`           | String     | `Required`       | 步骤状态 (Required, Optional, Satisfied)。     |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockCoordSystem_1770451903113_621",
  "type": "BlockCoordSystem",
  "props": {
    "PW_BlockID": "coord_system0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": " ",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": true,
    "PW_BalloonTooltipImage": " ",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": " ",
    "PW_CreateInterpartLink": false,
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "Specify CSYS",
    "PW_OutputType": "Cartesian",
    "PW_SmartUpdateOption": "Within Modeling",
    "PW_StepStatus": "Required",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.28 微定位 (Microposition - `BlockMicroposition`)

对应 NX Open `BlockMicroposition` 组件，用于精细调整位置。

> **POWER 模板参考**: `power_ui_template/BlockMicroposition.ui`

| UI设计器显示名称           | 导出JSON字段名称        | 类型       | 默认值                | 说明             |
| :------------------------- | :---------------------- | :--------- | :-------------------- | :--------------- |
| **BlockID**          | `PW_BlockID`          | String     | `micropositioning0` | 组件唯一标识符。 |
| **Enable**           | `PW_Enable`           | Boolean    | `TRUE`              | 是否启用。       |
| **Group**            | `PW_Group`            | Boolean    | `FALSE`             | 是否作为组显示。 |
| **Label**            | `PW_Label`            | String     | `微定位`            | 标签文本。       |
| **Show**             | `PW_Show`             | Boolean    | `TRUE`              | 是否可见。       |
| **Bottom**           | `PW_Bottom`           | Attachment | `Default`           | 底部附着目标。   |
| **Left**             | `PW_Left`             | Attachment | `Default`           | 左侧附着目标。   |
| **Right**            | `PW_Right`            | Attachment | `Default`           | 右侧附着目标。   |
| **Top**              | `PW_Top`              | Attachment | `Default`           | 顶部附着目标。   |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockMicropositioning_1770451903113_622",
  "type": "BlockMicropositioning",
  "props": {
    "PW_BlockID": "micropositioning0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "微定位",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.29 超级点 (SuperPoint - `BlockSuperPoint`)

对应 NX Open `BlockSuperPoint` 组件，功能与截面构造器类似，但专用于点集合选择，支持多种曲线规则和点类型。

> **POWER 模板参考**: `power_ui_template/BlockSuperPoint.ui`

| UI设计器显示名称                     | 导出JSON字段名称                  | 类型       | 默认值                                                                | 说明                                |
| :----------------------------------- | :-------------------------------- | :--------- | :-------------------------------------------------------------------- | :---------------------------------- |
| **BlockID**                    | `PW_BlockID`                    | String     | `superPoint0`                                                       | 组件唯一标识符。                    |
| **Enable**                     | `PW_Enable`                     | Boolean    | `TRUE`                                                              | 是否启用。                          |
| **Group**                      | `PW_Group`                      | Boolean    | `FALSE`                                                             | 是否作为组显示。                    |
| **Show**                       | `PW_Show`                       | Boolean    | `TRUE`                                                              | 是否可见。                          |
| **PointOverlay**               | `PW_PointOverlay`               | Boolean    | `TRUE`                                                              | 是否显示点叠加。                    |
| **SmartUpdateOption**          | `PW_SmartUpdateOption`          | String     | `Within Modeling`                                                   | 智能更新选项。                      |
| **SnapPointTypesEnabled**      | `PW_SnapPointTypesEnabled`      | String[]   | `["(0x80)Arc Center", "(0x10000)Bounded Grid Point"...]`            | 启用的捕捉点类型 (完整列表见JSON)。 |
| **SnapPointTypesOnByDefault**  | `PW_SnapPointTypesOnByDefault`  | String[]   | `["(0x80)Arc Center", "(0x10000)Bounded Grid Point"...]`            | 默认启用的捕捉点类型。              |
| **Bottom**                     | `PW_Bottom`                     | Attachment | `Default`                                                           | 底部附着目标。                      |
| **Left**                       | `PW_Left`                       | Attachment | `Default`                                                           | 左侧附着目标。                      |
| **Right**                      | `PW_Right`                      | Attachment | `Default`                                                           | 右侧附着目标。                      |
| **Top**                        | `PW_Top`                        | Attachment | `Default`                                                           | 顶部附着目标。                      |
| **AllowConvergentObject**      | `PW_AllowConvergentObject`      | Boolean    | `FALSE`                                                             | 是否允许选择收敛对象。              |
| **AllowSelfIntersecting**      | `PW_AllowSelfIntersecting`      | Boolean    | `TRUE`                                                              | 是否允许自交。                      |
| **AutomaticProgression**       | `PW_AutomaticProgression`       | Boolean    | `FALSE`                                                             | 是否自动推进到下一步。              |
| **BalloonTooltipImage**        | `PW_BalloonTooltipImage`        | String     | `""`                                                                | 气泡提示图片路径。                  |
| **BalloonTooltipLayout**       | `PW_BalloonTooltipLayout`       | String     | `Horizontal`                                                        | 气泡提示布局。                      |
| **BalloonTooltipText**         | `PW_BalloonTooltipText`         | String     | `""`                                                                | 气泡提示文本。                      |
| **Bitmap**                     | `PW_Bitmap`                     | String     | `pointset`                                                          | 图标路径。                          |
| **BlendVirtualCurveOverlay**   | `PW_BlendVirtualCurveOverlay`   | Boolean    | `FALSE`                                                             | 是否显示混合虚拟曲线叠加。          |
| **CreateInterpartLink**        | `PW_CreateInterpartLink`        | Boolean    | `FALSE`                                                             | 是否创建零部件间链接。              |
| **Cue**                        | `PW_Cue`                        | String     | `选择要绘制的平面，或为截面选择曲线`                                | 提示文本。                          |
| **InterpartSelection**         | `PW_InterpartSelection`         | String     | `Simple`                                                            | 零部件间选择模式。                  |
| **LabelString**                | `PW_LabelString`                | String     | `Specify Point`                                                     | 标签字符串。                        |
| **ShowFlowDirectionAndOrigin** | `PW_ShowFlowDirectionAndOrigin` | Boolean    | `FALSE`                                                             | 是否显示流向和起点。                |
| **SketchOnPath**               | `PW_SketchOnPath`               | Boolean    | `FALSE`                                                             | 是否在路径上草图。                  |
| **StepStatus**                 | `PW_StepStatus`                 | String     | `Required,Optional,Satisfied`                                       | 步骤状态。                          |
| **ToolTip**                    | `PW_ToolTip`                    | String     | `##289Point`                                                        | 工具提示文本。                      |
| **AngularTolerance**           | `PW_AngularTolerance`           | Double     | `0.05`                                                              | 角度容差。                          |
| **CurveRules**                 | `PW_CurveRules`                 | String[]   | `["(0x1)Single Curve", "(0x1)Infer Curves", "(0x1)Feature Points"]` | 曲线规则列表。                      |
| **DefaultCurveRules**          | `PW_DefaultCurveRules`          | String     | `Single Curve`                                                      | 默认曲线规则。                      |
| **EntityType**                 | `PW_EntityType`                 | String[]   | `["(0x8)Allow Points"]`                                             | 实体类型列表。                      |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockSuperPoint_1770451903113_623",
  "type": "BlockSuperPoint",
  "props": {
    "PW_BlockID": "superPoint0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_PointOverlay": true,
    "PW_SmartUpdateOption": "Within Modeling",
    "PW_SnapPointTypesEnabled": ["(0x80)Arc Center", "(0x10000)Bounded Grid Point"],
    "PW_SnapPointTypesOnByDefault": ["(0x80)Arc Center", "(0x10000)Bounded Grid Point"],
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowConvergentObject": false,
    "PW_AllowSelfIntersecting": true,
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "pointset",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "选择要绘制的平面，或为截面选择曲线",
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "Specify Point",
    "PW_ShowFlowDirectionAndOrigin": false,
    "PW_SketchOnPath": false,
    "PW_StepStatus": "Required",
    "PW_ToolTip": "##289Point",
    "PW_AngularTolerance": 0.05,
    "PW_CurveRules": ["Single Curve", "Infer Curves", "Feature Points"],
    "PW_DefaultCurveRules": "Single Curve",
    "PW_EntityType": ["Allow Points"],
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.30 集列表 (SetList - `BlockSetList`)

对应 NX Open `BlockJiList`（SetList）组件，用于管理多组数据集合，支持添加、删除、排序等操作。

> **POWER 模板参考**: `power_ui_template/BlockSetList.ui`

| UI设计器显示名称                      | 导出JSON字段名称                   | 类型       | 默认值          | 说明                          |
| :------------------------------------ | :--------------------------------- | :--------- | :-------------- | :---------------------------- |
| **BlockID**                     | `PW_BlockID`                     | String     | `list0`       | 组件唯一标识符。              |
| **Enable**                      | `PW_Enable`                      | Boolean    | `True`        | 是否启用。                    |
| **Group**                       | `PW_Group`                       | Boolean    | `False`       | 是否作为组显示。              |
| **Label**                       | `PW_Label`                       | String     | `""`          | 标签文本。                    |
| **Show**                        | `PW_Show`                        | Boolean    | `True`        | 是否可见。                    |
| **Bottom**                      | `PW_Bottom`                      | Attachment | `Default`     | 底部附着目标。                |
| **Left**                        | `PW_Left`                        | Attachment | `Default`     | 左侧附着目标。                |
| **Right**                       | `PW_Right`                       | Attachment | `Default`     | 右侧附着目标。                |
| **Top**                         | `PW_Top`                         | Attachment | `Default`     | 顶部附着目标。                |
| **AddNewSetLabel**              | `PW_AddNewSetLabel`              | String     | `Add New Set` | 添加新集合按钮标签。          |
| **ColumnLabels**                | `PW_ColumnLabels`                | String[]   | `[]`          | 列标签数组。                  |
| **ColumnWidths**                | `PW_ColumnWidths`                | String     | `"-1"`        | 列宽度 (-1 表示自动)。        |
| **DefaultColumnWidth**          | `PW_DefaultColumnWidth`          | Integer    | `100`         | 默认列宽度。                  |
| **Layout**                      | `PW_Layout`                      | String     | `Selection`   | 布局方式 (Selection, Value)。 |
| **ListExpanded**                | `PW_ListExpanded`                | Boolean    | `False`       | 列表是否展开。                |
| **MarkSeedExpressionsUnusable** | `PW_MarkSeedExpressionsUnusable` | Boolean    | `False`       | 是否标记种子表达式不可用。    |
| **MaximumHeight**               | `PW_MaximumHeight`               | Integer    | `0`           | 最大高度 (0 表示无限制)。     |
| **MinimumHeight**               | `PW_MinimumHeight`               | Integer    | `4`           | 最小高度。                    |
| **MultipleEdit**                | `PW_MultipleEdit`                | Boolean    | `False`       | 是否支持多选编辑。            |
| **NumberColumnString**          | `PW_NumberColumnString`          | String     | `""`          | 编号列字符串。                |
| **NumberOfColumns**             | `PW_NumberOfColumns`             | Integer    | `1`           | 列数。                        |
| **ResizeHeightWithDialog**      | `PW_ResizeHeightWithDialog`      | Boolean    | `True`        | 高度是否随对话框调整。        |
| **SeedDlxFile**                 | `PW_SeedDlxFile`                 | String     | `""`          | 种子 DLX 文件路径。           |
| **ShowAddNewSet**               | `PW_ShowAddNewSet`               | Boolean    | `True`        | 是否显示添加新集合按钮。      |
| **ShowColumnHeadings**          | `PW_ShowColumnHeadings`          | Boolean    | `False`       | 是否显示列标题。              |
| **ShowRemove**                  | `PW_ShowRemove`                  | Boolean    | `True`        | 是否显示删除按钮。            |
| **ShowReorderControls**         | `PW_ShowReorderControls`         | Boolean    | `False`       | 是否显示排序控制。            |
| **ShowTopBottomControls**       | `PW_ShowTopBottomControls`       | Boolean    | `False`       | 是否显示置顶/置底控制。       |

**示例**:

```json
{
  "id": "BlockJiList_1770451903113_720",
  "type": "BlockJiList",
  "props": {
    "PW_BlockID": "list0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "数据集列表",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AddNewSetLabel": "Add New Set",
    "PW_ColumnLabels": ["名称", "值"],
    "PW_ColumnWidths": "-1",
    "PW_DefaultColumnWidth": 100,
    "PW_Layout": "Selection",
    "PW_ListExpanded": false,
    "PW_MarkSeedExpressionsUnusable": false,
    "PW_MaximumHeight": 0,
    "PW_MinimumHeight": 4,
    "PW_MultipleEdit": false,
    "PW_NumberColumnString": "",
    "PW_NumberOfColumns": 2,
    "PW_ResizeHeightWithDialog": true,
    "PW_SeedDlxFile": "",
    "PW_ShowAddNewSet": true,
    "PW_ShowColumnHeadings": true,
    "PW_ShowRemove": true,
    "PW_ShowReorderControls": false,
    "PW_ShowTopBottomControls": false
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.31 多行字符串 (MultilineString - `BlockMultilineString`)

对应 NX Open `BlockMultilineString` 组件，用于输入多行文本内容。

> **POWER 模板参考**: `power_ui_template/BlockMultilineString.ui`

| UI设计器显示名称                    | 导出JSON字段名称                 | 类型       | 默认值                | 说明                        |
| :---------------------------------- | :------------------------------- | :--------- | :-------------------- | :-------------------------- |
| **BlockID**                   | `PW_BlockID`                   | String     | `multiline_string0` | 组件唯一标识符。            |
| **Enable**                    | `PW_Enable`                    | Boolean    | `True`              | 是否启用。                  |
| **Group**                     | `PW_Group`                     | Boolean    | `False`             | 是否作为组显示。            |
| **Label**                     | `PW_Label`                     | String     | `No Title`          | 标签文本。                  |
| **Show**                      | `PW_Show`                      | Boolean    | `True`              | 是否可见。                  |
| **Localize**                  | `PW_Localize`                  | Boolean    | `True`              | 是否支持本地化。            |
| **RetainValue**               | `PW_RetainValue`               | Boolean    | `True`              | 是否在对话框关闭后保留值。  |
| **Bottom**                    | `PW_Bottom`                    | Attachment | `Default`           | 底部附着目标。              |
| **Left**                      | `PW_Left`                      | Attachment | `Default`           | 左侧附着目标。              |
| **Right**                     | `PW_Right`                     | Attachment | `Default`           | 右侧附着目标。              |
| **Top**                       | `PW_Top`                       | Attachment | `Default`           | 顶部附着目标。              |
| **BalloonTooltipImage**       | `PW_BalloonTooltipImage`       | String     | `""`                | 气泡提示图片路径。          |
| **BalloonTooltipLayout**      | `PW_BalloonTooltipLayout`      | String     | `Horizontal`        | 气泡提示布局方向。          |
| **BalloonTooltipText**        | `PW_BalloonTooltipText`        | String     | `""`                | 气泡提示文本。              |
| **Height**                    | `PW_Height`                    | Integer    | `3`                 | 文本框高度 (行数)。         |
| **MaximumCharactersAccepted** | `PW_MaximumCharactersAccepted` | Integer    | `0`                 | 最大字符数 (0 表示无限制)。 |
| **MaximumHeight**             | `PW_MaximumHeight`             | Integer    | `0`                 | 最大高度 (0 表示无限制)。   |
| **MinimumHeight**             | `PW_MinimumHeight`             | Integer    | `0`                 | 最小高度 (0 表示无限制)。   |
| **ResizeHeightWithDialog**    | `PW_ResizeHeightWithDialog`    | Boolean    | `True`              | 高度是否随对话框调整。      |
| **Value**                     | `PW_Value`                     | String     | `""`                | **当前值**。          |
| **Width**                     | `PW_Width`                     | Integer    | `0`                 | 宽度 (0 表示自动)。         |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockMultilineString_1770451903113_606",
  "type": "BlockMultilineString",
  "props": {
    "PW_BlockID": "multiline_string0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "备注信息",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Height": 5,
    "PW_MaximumCharactersAccepted": 0,
    "PW_MaximumHeight": 0,
    "PW_MinimumHeight": 0,
    "PW_ResizeHeightWithDialog": true,
    "PW_Value": "",
    "PW_Width": 0,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.32 列表框 (ListBox - `BlockListBox`)

对应 NX Open `BlockListBox` 组件，用于显示可选列表项。

> **POWER 模板参考**: `power_ui_template/BlockListBox.ui`

| UI设计器显示名称                       | 导出JSON字段名称                    | 类型       | 默认值        | 说明                            |
| :------------------------------------- | :---------------------------------- | :--------- | :------------ | :------------------------------ |
| **BlockID**                      | `PW_BlockID`                      | String     | `list_box0` | 组件唯一标识符。                |
| **Enable**                       | `PW_Enable`                       | Boolean    | `True`      | 是否启用。                      |
| **Group**                        | `PW_Group`                        | Boolean    | `False`     | 是否作为组显示。                |
| **Label**                        | `PW_Label`                        | String     | `""`        | 标签文本。                      |
| **Show**                         | `PW_Show`                         | Boolean    | `True`      | 是否可见。                      |
| **Localize**                     | `PW_Localize`                     | Boolean    | `True`      | 是否支持本地化。                |
| **Bottom**                       | `PW_Bottom`                       | Attachment | `Default`   | 底部附着目标。                  |
| **Left**                         | `PW_Left`                         | Attachment | `Default`   | 左侧附着目标。                  |
| **Right**                        | `PW_Right`                        | Attachment | `Default`   | 右侧附着目标。                  |
| **Top**                          | `PW_Top`                          | Attachment | `Default`   | 顶部附着目标。                  |
| **AllowDeselectForSingleSelect** | `PW_AllowDeselectForSingleSelect` | Boolean    | `False`     | 单选模式下是否允许取消选择。    |
| **Height**                       | `PW_Height`                       | Integer    | `3`         | 列表框高度 (行数)。             |
| **ListItems**                    | `PW_ListItems`                    | String[]   | `[]`        | **列表项数组**。          |
| **MaximumHeight**                | `PW_MaximumHeight`                | Integer    | `0`         | 最大高度 (0 表示无限制)。       |
| **MaximumStringLength**          | `PW_MaximumStringLength`          | Integer    | `0`         | 最大字符串长度 (0 表示无限制)。 |
| **MinimumHeight**                | `PW_MinimumHeight`                | Integer    | `0`         | 最小高度 (0 表示无限制)。       |
| **ResizeHeightWithDialog**       | `PW_ResizeHeightWithDialog`       | Boolean    | `True`      | 高度是否随对话框调整。          |
| **ShowAddButton**                | `PW_ShowAddButton`                | Boolean    | `False`     | 是否显示添加按钮。              |
| **ShowDeleteButton**             | `PW_ShowDeleteButton`             | Boolean    | `False`     | 是否显示删除按钮。              |
| **ShowMoveUpDownButtons**        | `PW_ShowMoveUpDownButtons`        | Boolean    | `False`     | 是否显示上下移动按钮。          |
| **SingleSelect**                 | `PW_SingleSelect`                 | Boolean    | `False`     | 是否为单选模式。                |

**示例**:

```json
{
  "id": "BlockListBox_1770451903113_607",
  "type": "BlockListBox",
  "props": {
    "PW_BlockID": "list_box0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "特征列表",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowDeselectForSingleSelect": false,
    "PW_Height": 5,
    "PW_ListItems": ["项目 A", "项目 B", "项目 C"],
    "PW_MaximumHeight": 0,
    "PW_MaximumStringLength": 0,
    "PW_MinimumHeight": 0,
    "PW_ResizeHeightWithDialog": true,
    "PW_ShowAddButton": true,
    "PW_ShowDeleteButton": true,
    "PW_ShowMoveUpDownButtons": true,
    "PW_SingleSelect": false
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.33 对象取色器 (ObjectColorPicker - `BlockObjectColorPicker`)

对应 NX Open `BlockObjectColorPicker` 组件，用于选择颜色。

> **POWER 模板参考**: `power_ui_template/BlockObjectColorPicker.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值           | 说明                              |
| :----------------------------- | :-------------------------- | :--------- | :--------------- | :-------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `colorPicker0` | 组件唯一标识符。                  |
| **Enable**               | `PW_Enable`               | Boolean    | `True`         | 是否启用。                        |
| **Group**                | `PW_Group`                | Boolean    | `False`        | 是否作为组显示。                  |
| **Label**                | `PW_Label`                | String     | `No Title`     | 标签文本。                        |
| **Show**                 | `PW_Show`                 | Boolean    | `True`         | 是否可见。                        |
| **Localize**             | `PW_Localize`             | Boolean    | `True`         | 是否支持本地化。                  |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`         | 是否在对话框关闭后保留值。        |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`      | 底部附着目标。                    |
| **Left**                 | `PW_Left`                 | Attachment | `Default`      | 左侧附着目标。                    |
| **Right**                | `PW_Right`                | Attachment | `Default`      | 右侧附着目标。                    |
| **Top**                  | `PW_Top`                  | Attachment | `Default`      | 顶部附着目标。                    |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`           | 气泡提示图片路径。                |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String     | `Horizontal`   | 气泡提示布局方向。                |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`           | 气泡提示文本。                    |
| **NumberSelectable**     | `PW_NumberSelectable`     | Integer    | `1`            | 可选颜色数量。                    |
| **Value**                | `PW_Value`                | Integer[]  | `[1]`          | **当前值** (颜色索引数组)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockColorPicker_1770451903113_612",
  "type": "BlockColorPicker",
  "props": {
    "PW_BlockID": "colorPicker0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "颜色",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_NumberSelectable": 1,
    "PW_Value": [36],
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.34 通过浏览选择文件 (FileSelection - `BlockFileSelection`)

对应 NX Open `BlockFileSelection` 组件，用于浏览和选择本地文件。

> **POWER 模板参考**: `power_ui_template/BlockFileSelection.ui`

| UI设计器显示名称            | 导出JSON字段名称         | 类型       | 默认值                 | 说明                                |
| :-------------------------- | :----------------------- | :--------- | :--------------------- | :---------------------------------- |
| **BlockID**           | `PW_BlockID`           | String     | `nativeFileBrowser0` | 组件唯一标识符。                    |
| **Enable**            | `PW_Enable`            | Boolean    | `True`               | 是否启用。                          |
| **Group**             | `PW_Group`             | Boolean    | `False`              | 是否作为组显示。                    |
| **Label**             | `PW_Label`             | String     | `File Browser`       | 标签文本。                          |
| **Show**              | `PW_Show`              | Boolean    | `True`               | 是否可见。                          |
| **Bottom**            | `PW_Bottom`            | Attachment | `Default`            | 底部附着目标。                      |
| **Left**              | `PW_Left`              | Attachment | `Default`            | 左侧附着目标。                      |
| **Right**             | `PW_Right`             | Attachment | `Default`            | 右侧附着目标。                      |
| **Top**               | `PW_Top`               | Attachment | `Default`            | 顶部附着目标。                      |
| **Filter**            | `PW_Filter`            | String     | `""`                 | 文件过滤器 (如 `"*.prt;*.asm"`)。 |
| **FilterIndex**       | `PW_FilterIndex`       | Integer    | `-1`                 | 默认过滤器索引。                    |
| **Path**              | `PW_Path`              | String     | `""`                 | 初始路径。                          |
| **RetainStringValue** | `PW_RetainStringValue` | Boolean    | `True`               | 是否在对话框关闭后保留值。          |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockNativeFileBrowser_1770451903113_616",
  "type": "BlockNativeFileBrowser",
  "props": {
    "PW_BlockID": "nativeFileBrowser0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "选择文件",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_Filter": "*.prt;*.asm",
    "PW_FilterIndex": 0,
    "PW_Path": "",
    "PW_RetainStringValue": true,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.35 通过浏览选择文件夹 (FolderSelection - `BlockFolderSelection`)

对应 NX Open `BlockFolderSelection` 组件，用于浏览和选择本地文件夹。

> **POWER 模板参考**: `power_ui_template/BlockFolderSelection.ui`

| UI设计器显示名称            | 导出JSON字段名称         | 类型       | 默认值                   | 说明                       |
| :-------------------------- | :----------------------- | :--------- | :----------------------- | :------------------------- |
| **BlockID**           | `PW_BlockID`           | String     | `nativeFolderBrowser0` | 组件唯一标识符。           |
| **Enable**            | `PW_Enable`            | Boolean    | `True`                 | 是否启用。                 |
| **Group**             | `PW_Group`             | Boolean    | `False`                | 是否作为组显示。           |
| **Label**             | `PW_Label`             | String     | `Folder Browser`       | 标签文本。                 |
| **Show**              | `PW_Show`              | Boolean    | `True`                 | 是否可见。                 |
| **Bottom**            | `PW_Bottom`            | Attachment | `Default`              | 底部附着目标。             |
| **Left**              | `PW_Left`              | Attachment | `Default`              | 左侧附着目标。             |
| **Right**             | `PW_Right`             | Attachment | `Default`              | 右侧附着目标。             |
| **Top**               | `PW_Top`               | Attachment | `Default`              | 顶部附着目标。             |
| **Path**              | `PW_Path`              | String     | `""`                   | 初始路径。                 |
| **RetainStringValue** | `PW_RetainStringValue` | Boolean    | `True`                 | 是否在对话框关闭后保留值。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockNativeFolderBrowser_1770451903113_617",
  "type": "BlockNativeFolderBrowser",
  "props": {
    "PW_BlockID": "nativeFolderBrowser0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "选择文件夹",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_Path": "C:/Users/",
    "PW_RetainStringValue": true,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.36 选择特征 (SelectFeature - `BlockSelectFeature`)

对应 NX Open `BlockSelectFeature` 组件，用于选择特征对象。

> **POWER 模板参考**: `power_ui_template/BlockSelectFeature.ui`

| UI设计器显示名称                   | 导出JSON字段名称                | 类型       | 默认值              | 说明                                       |
| :--------------------------------- | :------------------------------ | :--------- | :------------------ | :----------------------------------------- |
| **BlockID**                  | `PW_BlockID`                  | String     | `select_feature0` | 组件唯一标识符。                           |
| **Enable**                   | `PW_Enable`                   | Boolean    | `True`            | 是否启用。                                 |
| **Group**                    | `PW_Group`                    | Boolean    | `False`           | 是否作为组显示。                           |
| **Show**                     | `PW_Show`                     | Boolean    | `True`            | 是否可见。                                 |
| **Bottom**                   | `PW_Bottom`                   | Attachment | `Default`         | 底部附着目标。                             |
| **Left**                     | `PW_Left`                     | Attachment | `Default`         | 左侧附着目标。                             |
| **Right**                    | `PW_Right`                    | Attachment | `Default`         | 右侧附着目标。                             |
| **Top**                      | `PW_Top`                      | Attachment | `Default`         | 顶部附着目标。                             |
| **AutomaticProgression**     | `PW_AutomaticProgression`     | Boolean    | `False`           | 是否自动推进到下一步。                     |
| **BalloonTooltipImage**      | `PW_BalloonTooltipImage`      | String     | `""`              | 气泡提示图片路径。                         |
| **BalloonTooltipLayout**     | `PW_BalloonTooltipLayout`     | String     | `Horizontal`      | 气泡提示布局方向。                         |
| **BalloonTooltipText**       | `PW_BalloonTooltipText`       | String     | `""`              | 气泡提示文本。                             |
| **BlendVirtualCurveOverlay** | `PW_BlendVirtualCurveOverlay` | Boolean    | `False`           | 是否显示混合虚拟曲线覆盖。                 |
| **Cue**                      | `PW_Cue`                      | String     | `Select Feature`  | 提示文本。                                 |
| **LabelString**              | `PW_LabelString`              | String     | `Select Feature`  | 标签文本。                                 |
| **SelectMode**               | `PW_SelectMode`               | String     | `Single`          | 选择模式 (Single, Multiple)。              |
| **StepStatus**               | `PW_StepStatus`               | String     | `Required`        | 步骤状态 (Required, Optional, Satisfied)。 |
| **ToolTip**                  | `PW_ToolTip`                  | String     | `""`              | 工具提示文本。                             |
| **ToolTipID**            | `PW_ToolTipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockSelectFeature_1770451903113_618",
  "type": "BlockSelectFeature",
  "props": {
    "PW_BlockID": "select_feature0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_Cue": "选择特征",
    "PW_LabelString": "Select Feature",
    "PW_SelectMode": "Single",
    "PW_StepStatus": "Required",
    "PW_ToolTip": "",
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.37 超级截面 (SuperSection - `BlockSuperSection`)

对应 NX Open `BlockSuperSection` 组件，功能与截面构造器类似，但支持更复杂的截面构建场景，包含草图路径等扩展功能。

> **POWER 模板参考**: `power_ui_template/BlockSuperSection.ui`

| UI设计器显示名称                           | 导出JSON字段名称                        | 类型       | 默认值                                   | 说明                                                                                                                                                                       |
| :----------------------------------------- | :-------------------------------------- | :--------- | :--------------------------------------- | :------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **BlockID**                          | `PW_BlockID`                          | String     | `section0`                             | 组件唯一标识符。                                                                                                                                                           |
| **Enable**                           | `PW_Enable`                           | Boolean    | `True`                                 | 是否启用。                                                                                                                                                                 |
| **Group**                            | `PW_Group`                            | Boolean    | `False`                                | 是否作为组显示。                                                                                                                                                           |
| **Show**                             | `PW_Show`                             | Boolean    | `True`                                 | 是否可见。                                                                                                                                                                 |
| **PointOverlay**                     | `PW_PointOverlay`                     | Boolean    | `False`                                | 是否显示点叠加。                                                                                                                                                           |
| **SmartUpdateOption**                | `PW_SmartUpdateOption`                | String     | `Within Modeling`                      | 智能更新选项。                                                                                                                                                             |
| **SnapPointTypesEnabled**            | `PW_SnapPointTypesEnabled`            | Integer    | `499708`                               | 启用的捕捉点类型位掩码。                                                                                                                                                   |
| **SnapPointTypesOnByDefault**        | `PW_SnapPointTypesOnByDefault`        | Integer    | `664`                                  | 默认启用的捕捉点类型位掩码。                                                                                                                                               |
| **Bottom**                           | `PW_Bottom`                           | Attachment | `Default`                              | 底部附着目标。                                                                                                                                                             |
| **Left**                             | `PW_Left`                             | Attachment | `Default`                              | 左侧附着目标。                                                                                                                                                             |
| **Right**                            | `PW_Right`                            | Attachment | `Default`                              | 右侧附着目标。                                                                                                                                                             |
| **Top**                              | `PW_Top`                              | Attachment | `Default`                              | 顶部附着目标。                                                                                                                                                             |
| **AllowConvergentObject**            | `PW_AllowConvergentObject`            | Boolean    | `False`                                | 是否允许收敛对象。                                                                                                                                                         |
| **AllowSelfIntersecting**            | `PW_AllowSelfIntersecting`            | Boolean    | `False`                                | 是否允许自交。                                                                                                                                                             |
| **AutomaticProgression**             | `PW_AutomaticProgression`             | Boolean    | `False`                                | 是否自动推进到下一步。                                                                                                                                                     |
| **BalloonTooltipImage**              | `PW_BalloonTooltipImage`              | String     | `""`                                   | 气泡提示图片路径。                                                                                                                                                         |
| **BalloonTooltipLayout**             | `PW_BalloonTooltipLayout`             | String     | `Horizontal`                           | 气泡提示布局方向 (Horizontal, Vertical)。                                                                                                                                  |
| **BalloonTooltipText**               | `PW_BalloonTooltipText`               | String     | `""`                                   | 气泡提示文本。                                                                                                                                                             |
| **Bitmap**                           | `PW_Bitmap`                           | String     | `sectionbuilder`                       | 图标路径。                                                                                                                                                                 |
| **BlendVirtualCurveOverlay**         | `PW_BlendVirtualCurveOverlay`         | Boolean    | `False`                                | 是否显示混合虚拟曲线覆盖。                                                                                                                                                 |
| **CreateInterpartLink**              | `PW_CreateInterpartLink`              | Boolean    | `False`                                | 是否创建跨部件链接。                                                                                                                                                       |
| **Cue**                              | `PW_Cue`                              | String     | `选择要绘制的平的面，或为截面选择曲线` | 提示文本。                                                                                                                                                                 |
| **InterpartSelection**               | `PW_InterpartSelection`               | String     | `Simple`                               | 跨部件选择模式 (Simple, Non-associative Interpart Copy Only, Associative and Non-associative Interpart Copy, Associative Interpart Copy)。                                 |
| **LabelString**                      | `PW_LabelString`                      | String     | `Select Curve`                         | 标签字符串。                                                                                                                                                               |
| **ShowFlowDirectionAndOrigin**       | `PW_ShowFlowDirectionAndOrigin`       | Boolean    | `False`                                | 是否显示流向和原点。                                                                                                                                                       |
| **SketchOnPath**                     | `PW_SketchOnPath`                     | Boolean    | `False`                                | 是否在路径上草图。                                                                                                                                                         |
| **StepStatus**                       | `PW_StepStatus`                       | String     | `Required`                             | 步骤状态 (Required, Optional, Satisfied)。                                                                                                                                 |
| **ToolTip**                          | `PW_ToolTip`                          | String     | `Curve`                                | 工具提示文本。                                                                                                                                                             |
| **AllowInferredCurveSelection**      | `PW_AllowInferredCurveSelection`      | Boolean    | `True`                                 | 是否允许推断曲线选择。                                                                                                                                                     |
| **AllowStopAtIntersectionFollowing** | `PW_AllowStopAtIntersectionFollowing` | Boolean    | `True`                                 | 是否允许在交叉处停止跟随。                                                                                                                                                 |
| **AngularTolerance**                 | `PW_AngularTolerance`                 | Double     | `0.05`                                 | 角度容差。                                                                                                                                                                 |
| **ChainWithinFeature**               | `PW_ChainWithinFeature`               | Boolean    | `False`                                | 是否在特征内链接。                                                                                                                                                         |
| **CurveRules**                       | `PW_CurveRules`                       | Integer    | `6383`                                 | 曲线规则位掩码。                                                                                                                                                           |
| **DefaultCurveRules**                | `PW_DefaultCurveRules`                | String     | `Single Curve`                         | 默认曲线规则 (Single Curve, Connected Curves, Tangent Curves, Face Edges, Sheet Edges, Feature Curves, Infer Curves, Region Boundary Curves, Curves in Group, Feature Intersection Edges, Outer Edges of Faces, Rib Top Face Edges)。 |
| **EntityType**                       | `PW_EntityType`                       | Integer    | `5`                                    | 实体类型。                                                                                                                                                                 |
| **FollowFillet**                     | `PW_FollowFillet`                     | Boolean    | `False`                                | 是否跟随圆角。                                                                                                                                                             |
| **InferredCurveSelection**           | `PW_InferredCurveSelection`           | Boolean    | `False`                                | 是否使用推断曲线选择。                                                                                                                                                     |
| **StopAtIntersection**               | `PW_StopAtIntersection`               | Boolean    | `False`                                | 是否在交叉处停止。                                                                                                                                                         |
| **TooltipID**                        | `PW_TooltipID`                        | String     | `""`                                   | 工具提示 ID。**(POWER 特有属性)**                                                                                                                                    |

**示例**:

```json
{
  "id": "BlockSuperSection_1770451903113_624",
  "type": "BlockSuperSection",
  "props": {
    "PW_BlockID": "section0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_PointOverlay": false,
    "PW_SmartUpdateOption": "Within Modeling",
    "PW_SnapPointTypesEnabled": 499708,
    "PW_SnapPointTypesOnByDefault": 664,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowConvergentObject": false,
    "PW_AllowSelfIntersecting": false,
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "sectionbuilder",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "选择要绘制的平的面，或为截面选择曲线",
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "Select Curve",
    "PW_ShowFlowDirectionAndOrigin": false,
    "PW_SketchOnPath": false,
    "PW_StepStatus": "Required",
    "PW_ToolTip": "Curve",
    "PW_AllowInferredCurveSelection": true,
    "PW_AllowStopAtIntersectionFollowing": true,
    "PW_AngularTolerance": 0.05,
    "PW_ChainWithinFeature": false,
    "PW_CurveRules": 6383,
    "PW_DefaultCurveRules": "Single Curve",
    "PW_EntityType": 5,
    "PW_FollowFillet": false,
    "PW_InferredCurveSelection": false,
    "PW_StopAtIntersection": false,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.38 表 (Table - `BlockTable`)
对应 NX Open `BlockTable` 组件，用于以表格形式排列子控件，是一个**容器类控件**。

> **POWER 模板参考**: `power_ui_template/BlockTable.ui`（属性模板）、`power_ui_template/BlockTableLayoutTemplate.ui`（含子控件布局模板）

| UI设计器显示名称          | 导出JSON字段名称       | 类型       | 默认值          | 说明             |
| :------------------------ | :--------------------- | :--------- | :-------------- | :--------------- |
| **BlockID**         | `PW_BlockID`         | String     | `table0`      | 组件唯一标识符。 |
| **Enable**          | `PW_Enable`          | Boolean    | `True`        | 是否启用。       |
| **Expanded**        | `PW_Expanded`        | Boolean    | `True`        | 初始时是否展开。 |
| **Group**           | `PW_Group`           | Boolean    | `False`       | 是否作为组显示。 |
| **Label**           | `PW_Label`           | String     | `TableLayout` | 标签文本。       |
| **Show**            | `PW_Show`            | Boolean    | `True`        | 是否可见。       |
| **Bottom**          | `PW_Bottom`          | Attachment | `Default`     | 底部附着目标。   |
| **Left**            | `PW_Left`            | Attachment | `Default`     | 左侧附着目标。   |
| **Right**           | `PW_Right`           | Attachment | `Default`     | 右侧附着目标。   |
| **Top**             | `PW_Top`             | Attachment | `Default`     | 顶部附着目标。   |
| **HasColumnLabels** | `PW_HasColumnLabels` | Boolean    | `True`        | 是否显示列标签。 |
| **NumberOfColumns** | `PW_NumberOfColumns` | Integer    | `2`           | 列数。           |

**示例**:

```json
{
  "id": "BlockTable_1770451903113_705",
  "type": "BlockTable",
  "props": {
    "PW_BlockID": "table0",
    "PW_Enable": true,
    "PW_Expanded": true,
    "PW_Group": false,
    "PW_Label": "参数表格",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_HasColumnLabels": true,
    "PW_NumberOfColumns": 2
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": [
    {
      "id": "BlockLabel_1770451903113_706",
      "type": "BlockLabel",
      "props": { "PW_BlockID": "blockLabel" },
      "hiddenProps": [], "altProperties": {}, "children": []
    },
    {
      "id": "BlockLabel_1770451903113_707",
      "type": "BlockLabel",
      "props": { "PW_BlockID": "blockLabel_2" },
      "hiddenProps": [], "altProperties": {}, "children": []
    },
    {
      "id": "BlockString_1770451903113_708",
      "type": "BlockString",
      "props": { "PW_BlockID": "blockString" },
      "hiddenProps": [], "altProperties": {}, "children": []
    },
    {
      "id": "BlockLabel_1770451903113_709",
      "type": "BlockLabel",
      "props": { "PW_BlockID": "blockLabel_3" },
      "hiddenProps": [], "altProperties": {}, "children": []
    },
    {
      "id": "BlockString_1770451903113_710",
      "type": "BlockString",
      "props": { "PW_BlockID": "blockString_2" },
      "hiddenProps": [], "altProperties": {}, "children": []
    }
  ]
}
```

#### BlockTable 子控件 UI 生成规则

`BlockTable` 是容器类控件，其 `children` 中的子控件需要按照 **`QGridLayout` 网格布局** 排列到表格中。生成 `.ui` 文件时需遵循以下规则：

1. **布局方式**：`BlockTable` 内部使用 `QGridLayout`（而非 `QVBoxLayout`），子控件通过 `row` 和 `column` 属性定位到网格单元格中。

2. **列数由 `PW_NumberOfColumns` 决定**：该属性指定表格的列数。子控件按照在 `children` 数组中的顺序，**从左到右、从上到下** 依次填入网格。

3. **行列分配算法**：
   - 第 0 行（`row="0"`）通常为列标题行（当 `PW_HasColumnLabels` 为 `true` 时），从 `column=0` 开始填充。
   - 后续子控件按 `children` 数组顺序，逐行逐列填充：
     - `children[i]` 的行号 = `i / NumberOfColumns`（整除），列号 = `i % NumberOfColumns`
   - 如果有列标题行，则数据行从 `row=1` 开始。

4. **UI 模板结构示例**（对应上述 JSON，`PW_NumberOfColumns=2`）：

```xml
<widget class="BlockTable" name="table0" native="true">
  <property name="PW_NumberOfColumns" stdset="0">
    <number>2</number>
  </property>
  <property name="PW_BlockID" stdset="0">
    <string>table0</string>
  </property>
  <layout class="QGridLayout" name="gridLayout">
    <!-- row 0: 列标题 (BlockLabel) -->
    <item row="0" column="1">
      <widget class="BlockLabel" name="blockLabel">
        <property name="PW_BlockID" stdset="0">
          <string>blockLabel</string>
        </property>
      </widget>
    </item>
    <!-- row 1: 第一行数据 -->
    <item row="1" column="0">
      <widget class="BlockLabel" name="blockLabel_2">
        <property name="PW_BlockID" stdset="0">
          <string>blockLabel_2</string>
        </property>
      </widget>
    </item>
    <item row="1" column="1">
      <widget class="BlockString" name="blockString">
        <property name="PW_BlockID" stdset="0">
          <string>blockString</string>
        </property>
      </widget>
    </item>
    <!-- row 2: 第二行数据 -->
    <item row="2" column="0">
      <widget class="BlockLabel" name="blockLabel_3">
        <property name="PW_BlockID" stdset="0">
          <string>blockLabel_3</string>
        </property>
      </widget>
    </item>
    <item row="2" column="1">
      <widget class="BlockString" name="blockString_2">
        <property name="PW_BlockID" stdset="0">
          <string>blockString_2</string>
        </property>
      </widget>
    </item>
  </layout>
</widget>
```

5. **与其他容器类控件的区别**：
   - `BlockGroup` 使用 `QVBoxLayout`，子控件垂直排列。
   - `BlockTabWidget` 使用多个 `QWidget` page，每个 page 对应一个选项卡。
   - `BlockTable` 使用 `QGridLayout`，子控件按行列定位，形成表格效果。

6. **`customwidgets` 声明**：`BlockTable` 在 `customwidgets` 中需声明 `<container>1</container>`，表示它是容器控件。同时需要声明其内部所有子控件的类型。

```xml
<customwidget>
  <class>BlockTable</class>
  <extends>QWidget</extends>
  <header>Widgets/ContainerBlock/BlockTable.h</header>
  <container>1</container>
</customwidget>
```

### 3.39 线型 (LineFont - `BlockLineFont`)
对应 NX Open `BlockLineFont` 组件，用于选择线型样式。

> **POWER 模板参考**: `power_ui_template/BlockLineFont.ui`

| UI设计器显示名称           | 导出JSON字段名称        | 类型       | 默认值        | 说明                            |
| :------------------------- | :---------------------- | :--------- | :------------ | :------------------------------ |
| **BlockID**          | `PW_BlockID`          | String     | `lineFont0` | 组件唯一标识符。                |
| **Enable**           | `PW_Enable`           | Boolean    | `True`      | 是否启用。                      |
| **Group**            | `PW_Group`            | Boolean    | `False`     | 是否作为组显示。                |
| **Label**            | `PW_Label`            | String     | `线型`      | 标签文本。                      |
| **Show**             | `PW_Show`             | Boolean    | `True`      | 是否可见。                      |
| **Bottom**           | `PW_Bottom`           | Attachment | `Default`   | 底部附件。                      |
| **Left**             | `PW_Left`             | Attachment | `Default`   | 左侧附件。                      |
| **Right**            | `PW_Right`            | Attachment | `Default`   | 右侧附件。                      |
| **Top**              | `PW_Top`              | Attachment | `Default`   | 顶部附件。                      |
| **AvailableOptions** | `PW_AvailableOptions` | Bits       | `(0x3fff)`  | 可用线型选项列表。              |
| **ShowOptionLabels** | `PW_ShowOptionLabels` | Boolean    | `True`      | 是否显示选项标签。              |
| **Value**            | `PW_Value`            | Enum       |               | **当前值** (选中的线型)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockLineFont_1770451903113_710",
  "type": "BlockLineFont",
  "props": {
    "PW_BlockID": "lineFont0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "线型",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AvailableOptions": [],
    "PW_ShowOptionLabels": true,
    "PW_Value": "",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.40 线宽 (LineWidth - `BlockLineWidth`)
对应 NX Open `BlockLineWidth` 组件，用于选择线宽。

> **POWER 模板参考**: `power_ui_template/BlockLineWidth.ui`

| UI设计器显示名称                | 导出JSON字段名称             | 类型       | 默认值         | 说明                       |
| :------------------------------ | :--------------------------- | :--------- | :------------- | :------------------------- |
| **BlockID**               | `PW_BlockID`               | String     | `lineWidth0` | 组件唯一标识符。           |
| **Enable**                | `PW_Enable`                | Boolean    | `True`       | 是否启用。                 |
| **Group**                 | `PW_Group`                 | Boolean    | `False`      | 是否作为组显示。           |
| **Label**                 | `PW_Label`                 | String     | `Line Width` | 标签文本。                 |
| **LabelVisibility**       | `PW_LabelVisibility`       | Boolean    | `True`       | 标签是否可见。             |
| **Show**                  | `PW_Show`                  | Boolean    | `True`       | 是否可见。                 |
| **RetainValue**           | `PW_RetainValue`           | Boolean    | `True`       | 是否在对话框关闭后保留值。 |
| **Bottom**                | `PW_Bottom`                | Attachment | `Default`    | 底部附件。                 |
| **Left**                  | `PW_Left`                  | Attachment | `Default`    | 左侧附件。                 |
| **Right**                 | `PW_Right`                 | Attachment | `Default`    | 右侧附件。                 |
| **Top**                   | `PW_Top`                   | Attachment | `Default`    | 顶部附件。                 |
| **AllowDefaultWidth**     | `PW_AllowDefaultWidth`     | Boolean    | `True`       | 是否允许默认宽度。         |
| **AllowNoChangeWidth**    | `PW_AllowNoChangeWidth`    | Boolean    | `True`       | 是否允许无更改宽度。       |
| **ShowDefaultAsOriginal** | `PW_ShowDefaultAsOriginal` | Boolean    | `False`      | 是否将默认显示为原始。     |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockLineWidth_1770451903113_711",
  "type": "BlockLineWidth",
  "props": {
    "PW_BlockID": "lineWidth0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "线宽",
    "PW_LabelVisibility": true,
    "PW_Show": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AllowDefaultWidth": true,
    "PW_AllowNoChangeWidth": true,
    "PW_ShowDefaultAsOriginal": false,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.41 线条颜色/样式/线宽 (LineColorFontWidth - `BlockLineColorFontWidth`)
对应 NX Open `BlockLineColorFontWidth` 组件，组合了颜色、线型和线宽三个子控件。

> **POWER 模板参考**: `power_ui_template/BlockLineColorFontWidth.ui`

| UI设计器显示名称                | 导出JSON字段名称             | 类型       | 默认值                  | 说明                                    |
| :------------------------------ | :--------------------------- | :--------- | :---------------------- | :-------------------------------------- |
| **BlockID**               | `PW_BlockID`               | String     | `lineColorFontWidth0` | 组件唯一标识符。                        |
| **Enable**                | `PW_Enable`                | Boolean    | `True`                | 是否启用。                              |
| **Group**                 | `PW_Group`                 | Boolean    | `False`               | 是否作为组显示。                        |
| **Label**                 | `PW_Label`                 | String     | `""`                  | 标签文本。                              |
| **Show**                  | `PW_Show`                  | Boolean    | `True`                | 是否可见。                              |
| **Bottom**                | `PW_Bottom`                | Attachment | `Default`             | 底部附件。                              |
| **Left**                  | `PW_Left`                  | Attachment | `Default`             | 左侧附件。                              |
| **Right**                 | `PW_Right`                 | Attachment | `Default`             | 右侧附件。                              |
| **Top**                   | `PW_Top`                   | Attachment | `Default`             | 顶部附件。                              |
| **ColorValue**            | `PW_ColorValue`            | String     | `1`                   | 颜色值。                                |
| **HideSubBlocks**         | `PW_HideSubBlocks`         | Enum       | `None`                | 隐藏的子块 (None, Color, 样式, Width)。 |
| **Layout**                | `PW_Layout`                | Enum       | `Vertical`            | 布局方向 (Vertical, Horizontal)。       |
| **LabelString**          | `PW_LabelString`          | String     | `""`          | 标签字符串。                           |
| **WidthLabel**           | `PW_WidthLabel`           | String     | `Width`       | 宽度标签文本。                         |
| **ShowLabelString**      | `PW_ShowLabelString`      | Boolean    | `False`       | 是否显示标签字符串。                   |
| **ShowDefault**           | `PW_ShowDefault`           | Boolean    | `True`                | 是否显示默认选项。                      |
| **ShowDefaultAsOriginal** | `PW_ShowDefaultAsOriginal` | Boolean    | `False`               | 是否将默认显示为原始。                  |
| **ShowNoChange**          | `PW_ShowNoChange`          | Boolean    | `True`                | 是否显示无更改选项。                    |
| **UseWideLines**          | `PW_UseWideLines`          | Boolean    | `True`                | 是否使用宽线。                          |
| **AvailableOptions**      | `PW_AvailableOptions`      | Bits       | `(0x3fff)`            | 可用线型选项。                          |
| **Value**                 | `PW_Value`                 | Enum       | `No Change`           | **当前线型值**。                  |
| **FontLabel**            | `PW_FontLabel`            | String     | `样式`        | 字体标签文本。                         |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockLineColorFontWidth_1770451903113_712",
  "type": "BlockLineColorFontWidth",
  "props": {
    "PW_BlockID": "lineColorFontWidth0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_ColorValue": "1",
    "PW_HideSubBlocks": "None",
    "PW_Layout": "Vertical",
    "PW_LabelString": "",
    "PW_WidthLabel": "Width",
    "PW_ShowLabelString": false,
    "PW_ShowDefault": true,
    "PW_ShowDefaultAsOriginal": false,
    "PW_ShowNoChange": true,
    "PW_UseWideLines": true,
    "PW_AvailableOptions": [],
    "PW_Value": "No Change",
    "PW_FontLabel": "样式",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.42 文本颜色/字体/宽度 (TextColorFontWidth - `BlockTextColorFontWidth`)
对应 NX Open `BlockTextColorFontWidth` 组件，组合了文本颜色、字体和宽度三个子控件。

> **POWER 模板参考**: `power_ui_template/BlockTextColorFontWidth.ui`

| UI设计器显示名称             | 导出JSON字段名称          | 类型       | 默认值                  | 说明                                             |
| :--------------------------- | :------------------------ | :--------- | :---------------------- | :----------------------------------------------- |
| **BlockID**            | `PW_BlockID`            | String     | `textColorFontWidth0` | 组件唯一标识符。                                 |
| **Enable**             | `PW_Enable`             | Boolean    | `True`                | 是否启用。                                       |
| **Group**              | `PW_Group`              | Boolean    | `False`               | 是否作为组显示。                                 |
| **Label**              | `PW_Label`              | String     | `""`                  | 标签文本。                                       |
| **Show**               | `PW_Show`               | Boolean    | `True`                | 是否可见。                                       |
| **Bottom**             | `PW_Bottom`             | Attachment | `Default`             | 底部附件。                                       |
| **Left**               | `PW_Left`               | Attachment | `Default`             | 左侧附件。                                       |
| **Right**              | `PW_Right`              | Attachment | `Default`             | 右侧附件。                                       |
| **Top**                | `PW_Top`                | Attachment | `Default`             | 顶部附件。                                       |
| **AvailableFontTypes** | `PW_AvailableFontTypes` | Enum       | `PW Font`             | 可用字体类型 (PW Font, Standard Font, Both)。    |
| **ColorValue**         | `PW_ColorValue`         | String     | `1`                   | 颜色值。                                         |
| **FontValue**            | `PW_FontValue`            | String     | `alien`       | 默认字体值。                           |
| **IsNXFont**           | `PW_IsNXFont`           | Boolean    | `True`                | 是否为 NX 字体。                                 |
| **Layout**             | `PW_Layout`             | Enum       | `Horizontal`          | 布局方向 (Horizontal, Vertical)。                |
| **WidthValue**         | `PW_WidthValue`         | Enum       | `Thin Width`          | 宽度值 (Thin Width, Normal Width, Thick Width)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |
| **Font Value**         | `PW_Font Value`         | String     | `alien`               | 字体值。                                         |

**示例**:

```json
{
  "id": "BlockTextColorFontWidth_1770451903113_713",
  "type": "BlockTextColorFontWidth",
  "props": {
    "PW_BlockID": "textColorFontWidth0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AvailableFontTypes": "PW Font",
    "PW_ColorValue": "1",
    "PW_FontValue": "Solid",
    "PW_IsNXFont": true,
    "PW_Layout": "Horizontal",
    "PW_WidthValue": "Thin Width",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.43 RGB取色器 (RGBColorPicker - `BlockRGBColorPicker`)
对应 NX Open `BlockRGBColorPicker` 组件，用于通过 RGB 值选择颜色。

> **POWER 模板参考**: `power_ui_template/BlockRGBColorPicker.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值                | 说明                           |
| :----------------------------- | :-------------------------- | :--------- | :-------------------- | :----------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `rGBColorPicker0`   | 组件唯一标识符。               |
| **Enable**               | `PW_Enable`               | Boolean    | `True`              | 是否启用。                     |
| **Group**                | `PW_Group`                | Boolean    | `False`             | 是否作为组显示。               |
| **Label**                | `PW_Label`                | String     | `No Title`          | 标签文本。                     |
| **Show**                 | `PW_Show`                 | Boolean    | `True`              | 是否可见。                     |
| **Localize**             | `PW_Localize`             | Boolean    | `True`              | 是否支持本地化。               |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`              | 是否在对话框关闭后保留值。     |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`           | 底部附件。                     |
| **Left**                 | `PW_Left`                 | Attachment | `Default`           | 左侧附件。                     |
| **Right**                | `PW_Right`                | Attachment | `Default`           | 右侧附件。                     |
| **Top**                  | `PW_Top`                  | Attachment | `Default`           | 顶部附件。                     |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`                | 气泡提示图片路径。             |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | Enum       | `Horizontal`        | 气泡提示布局。                 |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`                | 气泡提示文本。                 |
| **Value**                | `PW_Value`                | Integer    | `R 255 G 255 B 255` | **当前值** (RGB颜色值)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockRGBColorPicker_1770451903113_714",
  "type": "BlockRGBColorPicker",
  "props": {
    "PW_BlockID": "rGBColorPicker0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "No Title",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Value": "R 255 G 255 B 255",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.44 指定位置 (CursorLocation - `BlockCursorLocation`)
对应 NX Open `BlockCursorLocation` 组件，用于通过光标在 3D 视图中指定一个位置坐标。

> **POWER 模板参考**: `power_ui_template/BlockCursorLocation.ui`

| UI设计器显示名称                | 导出JSON字段名称             | 类型       | 默认值                      | 说明                                       |
| :------------------------------ | :--------------------------- | :--------- | :-------------------------- | :----------------------------------------- |
| **BlockID**               | `PW_BlockID`               | String     | `cursorLocation0`         | 组件唯一标识符。                           |
| **Enable**                | `PW_Enable`                | Boolean    | `True`                    | 是否启用。                                 |
| **Group**                 | `PW_Group`                 | Boolean    | `False`                   | 是否作为组显示。                           |
| **Label**                 | `PW_Label`                 | String     | `Cursor Location`         | 标签文本。                                 |
| **Show**                  | `PW_Show`                  | Boolean    | `True`                    | 是否可见。                                 |
| **Bottom**                | `PW_Bottom`                | Attachment | `Default`                 | 底部附件。                                 |
| **Left**                  | `PW_Left`                  | Attachment | `Default`                 | 左侧附件。                                 |
| **Right**                 | `PW_Right`                 | Attachment | `Default`                 | 右侧附件。                                 |
| **Top**                   | `PW_Top`                   | Attachment | `Default`                 | 顶部附件。                                 |
| **AutomaticProgression**  | `PW_AutomaticProgression`  | Boolean    | `True`                    | 是否自动推进到下一步。                     |
| **DisplayTemporaryPoint** | `PW_DisplayTemporaryPoint` | Boolean    | `False`                   | 是否显示临时点。                           |
| **LabelString**           | `PW_LabelString`           | String     | `Specify Cursor Location` | 标签字符串。                               |
| **LocationSpecified**     | `PW_LocationSpecified`     | Boolean    | `False`                   | 位置是否已指定。                           |
| **StepStatus**            | `PW_StepStatus`            | Enum       | `Required`                | 步骤状态 (Required, Optional, Satisfied)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockCursorLocation_1770451903113_716",
  "type": "BlockCursorLocation",
  "props": {
    "PW_BlockID": "cursorLocation0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "Cursor Location",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": true,
    "PW_DisplayTemporaryPoint": false,
    "PW_LabelString": "Specify Cursor Location",
    "PW_LocationSpecified": false,
    "PW_StepStatus": "Required",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.45 从列表选择部件 (SelectPart - `BlockSelectPart`)
对应 NX Open `BlockSelectPart` 组件，用于从已加载/最近/未加载的部件列表中选择部件。

> **POWER 模板参考**: `power_ui_template/BlockSelectPart.ui`

| UI设计器显示名称                    | 导出JSON字段名称                 | 类型       | 默认值             | 说明                                        |
| :---------------------------------- | :------------------------------- | :--------- | :----------------- | :------------------------------------------ |
| **BlockID**                   | `PW_BlockID`                   | String     | `selectPart0`    | 组件唯一标识符。                            |
| **Enable**                    | `PW_Enable`                    | Boolean    | `True`           | 是否启用。                                  |
| **Group**                     | `PW_Group`                     | Boolean    | `False`          | 是否作为组显示。                            |
| **Label**                     | `PW_Label`                     | String     | `Select Part`    | 标签文本。                                  |
| **Show**                      | `PW_Show`                      | Boolean    | `True`           | 是否可见。                                  |
| **Bottom**                    | `PW_Bottom`                    | Attachment | `Default`        | 底部附件。                                  |
| **Left**                      | `PW_Left`                      | Attachment | `Default`        | 左侧附件。                                  |
| **Right**                     | `PW_Right`                     | Attachment | `Default`        | 右侧附件。                                  |
| **Top**                       | `PW_Top`                       | Attachment | `Default`        | 顶部附件。                                  |
| **AvailableViewStyles**       | `PW_AvailableViewStyles`       | Bits       | `(0x1f)`         | 可用视图样式 (中, 平铺, 小, 特别小, 列表)。 |
| **ColumnVisibility**          | `PW_ColumnVisibility`          | Bits       | `(0x3)`          | 列可见性 (描述, 文件名)。                   |
| **LoadedListExpandedState**   | `PW_LoadedListExpandedState`   | Boolean    | `True`           | 已加载列表是否展开。                        |
| **LoadedListHeight**          | `PW_LoadedListHeight`          | Integer    | `254`            | 已加载列表高度。                            |
| **LoadedListLabel**           | `PW_LoadedListLabel`           | String     | `Loaded Parts`   | 已加载列表标签。                            |
| **LoadedListVisibility**      | `PW_LoadedListVisibility`      | Boolean    | `True`           | 已加载列表是否可见。                        |
| **LoadedListWidth**           | `PW_LoadedListWidth`           | Integer    | `400`            | 已加载列表宽度。                            |
| **RecentListExpandedState**   | `PW_RecentListExpandedState`   | Boolean    | `False`          | 最近列表是否展开。                          |
| **RecentListLabel**           | `PW_RecentListLabel`           | String     | `Recent Parts`   | 最近列表标签。                              |
| **RecentListVisibility**      | `PW_RecentListVisibility`      | Boolean    | `True`           | 最近列表是否可见。                          |
| **SearchButtonVisibility**    | `PW_SearchButtonVisibility`    | Boolean    | `True`           | 搜索按钮是否可见。                          |
| **UnloadedListExpandedState** | `PW_UnloadedListExpandedState` | Boolean    | `True`           | 未加载列表是否展开。                        |
| **UnloadedListLabel**         | `PW_UnloadedListLabel`         | String     | `Unloaded Parts` | 未加载列表标签。                            |
| **UnloadedListVisibility**    | `PW_UnloadedListVisibility`    | Boolean    | `False`          | 未加载列表是否可见。                        |
| **ViewStyle**                 | `PW_ViewStyle`                 | Enum       | `列表`           | 视图样式。                                  |

**示例**:

```json
{
  "id": "BlockSelectPart_1770451903113_717",
  "type": "BlockSelectPart",
  "props": {
    "PW_BlockID": "selectPart0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "Select Part",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AvailableViewStyles": [],
    "PW_ColumnVisibility": [],
    "PW_LoadedListExpandedState": true,
    "PW_LoadedListHeight": 254,
    "PW_LoadedListLabel": "Loaded Parts",
    "PW_LoadedListVisibility": true,
    "PW_LoadedListWidth": 400,
    "PW_RecentListExpandedState": false,
    "PW_RecentListLabel": "Recent Parts",
    "PW_RecentListVisibility": true,
    "PW_SearchButtonVisibility": true,
    "PW_UnloadedListExpandedState": true,
    "PW_UnloadedListLabel": "Unloaded Parts",
    "PW_UnloadedListVisibility": false,
    "PW_ViewStyle": "列表"
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.46 快速定位 (OrientXpress - `BlockOrientXpress`)
对应 NX Open `BlockOrientXpress` 组件，用于快速定义方位（参考、方向、平面），提供场景控制和轴/平面选择。

> **POWER 模板参考**: `power_ui_template/BlockOrientXpress.ui`

| UI设计器显示名称                | 导出JSON字段名称             | 类型       | 默认值                   | 说明                                                          |
| :------------------------------ | :--------------------------- | :--------- | :----------------------- | :------------------------------------------------------------ |
| **BlockID**               | `PW_BlockID`               | String     | `orientXpress0`        | 组件唯一标识符。                                              |
| **Enable**                | `PW_Enable`                | Boolean    | `True`                 | 是否启用。                                                    |
| **Group**                 | `PW_Group`                 | Boolean    | `False`                | 是否作为组显示。                                              |
| **Label**                 | `PW_Label`                 | String     | `""`                   | 标签文本。                                                    |
| **Show**                  | `PW_Show`                  | Boolean    | `True`                 | 是否可见。                                                    |
| **Bottom**                | `PW_Bottom`                | Attachment | `Default`              | 底部附件。                                                    |
| **Left**                  | `PW_Left`                  | Attachment | `Default`              | 左侧附件。                                                    |
| **Right**                 | `PW_Right`                 | Attachment | `Default`              | 右侧附件。                                                    |
| **Top**                   | `PW_Top`                   | Attachment | `Default`              | 顶部附件。                                                    |
| **Direction**             | `PW_Direction`             | Enum       | `X-Axis`               | 方向 (X-Axis, Y-Axis, Z-Axis)。                               |
| **Plane**                 | `PW_Plane`                 | Enum       | `YZ-Plane`             | 平面 (YZ-Plane, XZ-Plane, XY-Plane)。                         |
| **Reference**             | `PW_Reference`             | Enum       | `Absolute - Work Part` | 参考系 (Absolute - Work Part, WCS - Work Part, New CSYS 等)。 |
| **ShowAxisSubBlock**      | `PW_ShowAxisSubBlock`      | Boolean    | `True`                 | 是否显示轴子块。                                              |
| **ShowPlaneSubBlock**     | `PW_ShowPlaneSubBlock`     | Boolean    | `True`                 | 是否显示平面子块。                                            |
| **ShowReferenceSubBlock** | `PW_ShowReferenceSubBlock` | Boolean    | `True`                 | 是否显示参考子块。                                            |
| **ShowSceneControl**      | `PW_ShowSceneControl`      | Boolean    | `True`                 | 是否显示场景控制。                                            |
| **ShowXAxis**             | `PW_ShowXAxis`             | Boolean    | `True`                 | 是否显示 X 轴。                                               |
| **ShowXYPlane**           | `PW_ShowXYPlane`           | Boolean    | `True`                 | 是否显示 XY 平面。                                            |
| **ShowXZPlane**           | `PW_ShowXZPlane`           | Boolean    | `True`                 | 是否显示 XZ 平面。                                            |
| **ShowYAxis**             | `PW_ShowYAxis`             | Boolean    | `True`                 | 是否显示 Y 轴。                                               |
| **ShowYZPlane**           | `PW_ShowYZPlane`           | Boolean    | `True`                 | 是否显示 YZ 平面。                                            |
| **ShowZAxis**             | `PW_ShowZAxis`             | Boolean    | `True`                 | 是否显示 Z 轴。                                               |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockOrientXpress_1770451903113_719",
  "type": "BlockOrientXpress",
  "props": {
    "PW_BlockID": "orientXpress0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_Direction": "X-Axis",
    "PW_Plane": "YZ-Plane",
    "PW_Reference": "Absolute - Work Part",
    "PW_ShowAxisSubBlock": true,
    "PW_ShowPlaneSubBlock": true,
    "PW_ShowReferenceSubBlock": true,
    "PW_ShowSceneControl": true,
    "PW_ShowXAxis": true,
    "PW_ShowXYPlane": true,
    "PW_ShowXZPlane": true,
    "PW_ShowYAxis": true,
    "PW_ShowYZPlane": true,
    "PW_ShowZAxis": true,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.47 选择表达式 (SelectExpression - `BlockSelectExpression`)
对应 NX Open `BlockSelectExpression` 组件，用于从已有表达式列表中选择一个表达式。

> **POWER 模板参考**: `power_ui_template/BlockSelectExpression.ui`

| UI设计器显示名称              | 导出JSON字段名称           | 类型       | 默认值                | 说明                                                           |
| :---------------------------- | :------------------------- | :--------- | :-------------------- | :------------------------------------------------------------- |
| **BlockID**             | `PW_BlockID`             | String     | `selectExpression0` | 组件唯一标识符。                                               |
| **Enable**              | `PW_Enable`              | Boolean    | `True`              | 是否启用。                                                     |
| **Group**               | `PW_Group`               | Boolean    | `False`             | 是否作为组显示。                                               |
| **Label**               | `PW_Label`               | String     | `Choose Expression` | 标签文本。                                                     |
| **Show**                | `PW_Show`                | Boolean    | `True`              | 是否可见。                                                     |
| **Bottom**              | `PW_Bottom`              | Attachment | `Default`           | 底部附件。                                                     |
| **Left**                | `PW_Left`                | Attachment | `Default`           | 左侧附件。                                                     |
| **Right**               | `PW_Right`               | Attachment | `Default`           | 右侧附件。                                                     |
| **Top**                 | `PW_Top`                 | Attachment | `Default`           | 顶部附件。                                                     |
| **ExpressionSortType**  | `PW_ExpressionSortType`  | Enum       | `AlphaNumeric`      | 排序方式 (AlphaNumeric, TimeStamp, ReverseTimeStamp)。         |
| **ExpressionTypeIndex** | `PW_ExpressionTypeIndex` | Enum       | `Number`            | 表达式类型 (Number, String, Boolean, Integer, Point, Vector)。 |

**示例**:

```json
{
  "id": "BlockSelectExpression_1770451903113_721",
  "type": "BlockSelectExpression",
  "props": {
    "PW_BlockID": "selectExpression0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "Choose Expression",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_ExpressionSortType": "AlphaNumeric",
    "PW_ExpressionTypeIndex": "Number"
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.48 表达式 (Expression - `BlockExpression`)
对应 NX Open `BlockExpression` 组件，用于输入和编辑表达式。

> **POWER 模板参考**: `power_ui_template/BlockExpression.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值            | 说明                       |
| :----------------------------- | :-------------------------- | :--------- | :---------------- | :------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `expression0`   | 组件唯一标识符。           |
| **Enable**               | `PW_Enable`               | Boolean    | `True`          | 是否启用。                 |
| **Group**                | `PW_Group`                | Boolean    | `False`         | 是否作为组显示。           |
| **Label**                | `PW_Label`                | String     | `No Title`      | 标签文本。                 |
| **Show**                 | `PW_Show`                 | Boolean    | `True`          | 是否可见。                 |
| **RetainUnits**          | `PW_RetainUnits`          | Boolean    | `True`          | 是否保留单位设置。         |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`          | 是否在对话框关闭后保留值。 |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`       | 底部附件。                 |
| **Left**                 | `PW_Left`                 | Attachment | `Default`       | 左侧附件。                 |
| **Right**                | `PW_Right`                | Attachment | `Default`       | 右侧附件。                 |
| **Top**                  | `PW_Top`                  | Attachment | `Default`       | 顶部附件。                 |
| **AdaptiveScaleLimits** | `PW_AdaptiveScaleLimits` | Boolean    | `False`         | 是否自适应缩放限制。                    |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | Enum       | `Horizontal`    | 气泡提示布局。             |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`            | 气泡提示图片路径。         |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`            | 气泡提示文本。             |
| **Dimensionality**       | `PW_Dimensionality`       | String     | `无单位`        | 量纲类型。                 |
| **Formula**              | `PW_Formula`              | String     | `""`            | 表达式公式。               |
| **HasUnitsMenu**         | `PW_HasUnitsMenu`         | Boolean    | `False`         | 是否显示单位菜单。         |
| **LimitCheckTolerance**  | `PW_LimitCheckTolerance`  | Double     | `-1`            | 限制检查容差。             |
| **MaximumValue**         | `PW_MaximumValue`         | Double     | `1.79769e+308`  | 最大值。                   |
| **MaxInclusive**         | `PW_MaxInclusive`         | Boolean    | `True`          | 最大值是否包含。           |
| **MinimumValue**         | `PW_MinimumValue`         | Double     | `-1.79769e+308` | 最小值。                   |
| **MinInclusive**         | `PW_MinInclusive`         | Boolean    | `True`          | 最小值是否包含。           |
| **ShowSpin**             | `PW_ShowSpin`             | Boolean    | `False`         | 是否显示旋转控制。         |
| **Value**                | `PW_Value`                | Double     | `0`             | **当前值**。               |
| **WithScale**            | `PW_WithScale`            | Boolean    | `False`         | 是否带缩放滑块。           |
| **TooltipId**            | `PW_TooltipId`            | String     | `""`            | 工具提示ID。(POWER 特有属性) |

**示例**:

```json
{
  "id": "BlockExpression_1770451903113_722",
  "type": "BlockExpression",
  "props": {
    "PW_BlockID": "expression0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "No Title",
    "PW_Show": true,
    "PW_RetainUnits": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AdaptiveScaleLimits": false
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipText": "",
    "PW_Dimensionality": "无单位",
    "PW_Formula": "",
    "PW_HasUnitsMenu": false,
    "PW_LimitCheckTolerance": -1,
    "PW_MaximumValue": 1.79769e+308,
    "PW_MaxInclusive": true,
    "PW_MinimumValue": -1.79769e+308,
    "PW_MinInclusive": true,
    "PW_ShowSpin": false,
    "PW_Value": 0,
    "PW_WithScale": false,
    "PW_TooltipId": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.49 字符串 (String - `BlockString`)
对应 NX Open `BlockString` 组件，用于输入单行文本字符串。

> **POWER 模板参考**: `power_ui_template/BlockString.ui`

| UI设计器显示名称                      | 导出JSON字段名称                   | 类型       | 默认值        | 说明                                                                |
| :------------------------------------ | :--------------------------------- | :--------- | :------------ | :------------------------------------------------------------------ |
| **BlockID**                     | `PW_BlockID`                     | String     | `string0`   | 组件唯一标识符。                                                    |
| **Enable**                      | `PW_Enable`                      | Boolean    | `True`      | 是否启用。                                                          |
| **Group**                       | `PW_Group`                       | Boolean    | `False`     | 是否作为组显示。                                                    |
| **Label**                       | `PW_Label`                       | String     | `No Title`  | 标签文本。                                                          |
| **Show**                        | `PW_Show`                        | Boolean    | `True`      | 是否可见。                                                          |
| **AllowInternationalTextInput** | `PW_AllowInternationalTextInput` | Boolean    | `False`     | 是否允许国际文本输入。                                              |
| **IsPassword**                  | `PW_IsPassword`                  | Boolean    | `False`     | 是否为密码输入模式。                                                |
| **Localize**                    | `PW_Localize`                    | Boolean    | `True`      | 是否支持本地化。                                                    |
| **RetainValue**                 | `PW_RetainValue`                 | Boolean    | `True`      | 是否在对话框关闭后保留值。                                          |
| **Bottom**                      | `PW_Bottom`                      | Attachment | `Default`   | 底部附件。                                                          |
| **Left**                        | `PW_Left`                        | Attachment | `Default`   | 左侧附件。                                                          |
| **Right**                       | `PW_Right`                       | Attachment | `Default`   | 右侧附件。                                                          |
| **Top**                         | `PW_Top`                         | Attachment | `Default`   | 顶部附件。                                                          |
| **BalloonTooltipLayout**        | `PW_BalloonTooltipLayout`        | Enum       | `Horizontal`| 气泡提示布局 (Horizontal, Vertical)。                               |
| **BalloonTooltipImage**         | `PW_BalloonTooltipImage`         | String     | `""`        | 气泡提示图片路径。                                                  |
| **BalloonTooltipImages**        | `PW_BalloonTooltipImages`        | Strings    | `[]`        | 气泡提示图片路径数组。                                              |
| **BalloonTooltipText**          | `PW_BalloonTooltipText`          | String     | `""`        | 气泡提示文本。                                                      |
| **BalloonTooltipTexts**         | `PW_BalloonTooltipTexts`         | Strings    | `[]`        | 气泡提示文本数组。                                                  |
| **Bitmap**                      | `PW_Bitmap`                      | String     | `""`        | 图标路径。                                                          |
| **Hint**                        | `PW_Hint`                        | String     | `""`        | 提示文本。                                                          |
| **ListItems**            | `PW_ListItems`            | String[]   | `[]`            | 列表项（用于自动补全等）。             |
| **MaxTextLength**               | `PW_MaxTextLength`               | Integer    | `0`         | 最大文本长度 (0 表示不限制)。                                       |
| **PresentationStyle**           | `PW_PresentationStyle`           | Enum       | `KeyIn`     | 显示样式 (KeyIn, Wide, List, Autocomplete, 带组合自动补全)。        |
| **ReadOnlyString**              | `PW_ReadOnlyString`              | Boolean    | `False`     | 是否为只读。                                                        |
| **RequiredInput**               | `PW_RequiredInput`               | Boolean    | `False`     | 是否为必填项。                                                      |
| **Tooltip**                     | `PW_Tooltip`                     | String     | `""`        | 工具提示文本。                                                      |
| **UncommittedValue**            | `PW_UncommittedValue`            | String     | `""`        | 未提交的值。                                                        |
| **Value**                       | `PW_Value`                       | String     | `""`        | **当前值**。                                                        |
| **Width**                       | `PW_Width`                       | Enum       | `Narrow`    | 宽度样式 (Narrow, Standard)。                                       |
| **TooltipId**                   | `PW_TooltipId`                   | String     | `""`        | 工具提示ID。(POWER 特有属性)                                        |

**示例**:

```json
{
  "id": "BlockString_1770451903113_720",
  "type": "BlockString",
  "props": {
    "PW_BlockID": "string0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "No Title",
    "PW_Show": true,
    "PW_AllowInternationalTextInput": false,
    "PW_IsPassword": false,
    "PW_Localize": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipImages": [],
    "PW_BalloonTooltipText": "",
    "PW_BalloonTooltipTexts": [],
    "PW_Bitmap": "",
    "PW_Hint": "",
    "PW_ListItems": [],
    "PW_MaxTextLength": 0,
    "PW_PresentationStyle": "KeyIn",
    "PW_ReadOnlyString": false,
    "PW_RequiredInput": false,
    "PW_Tooltip": "",
    "PW_UncommittedValue": "",
    "PW_Value": "",
    "PW_Width": "Narrow",
    "PW_TooltipId": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

---

### 3.50 绘图区域 (DrawingArea - `BlockDrawingArea`)
对应 NX Open `BlockDrawingArea` 组件，用于在对话框中嵌入自定义绘图区域。

> **POWER 模板参考**: `power_ui_template/BlockDrawingArea.ui`

| UI设计器显示名称  | 导出JSON字段名称 | 类型       | 默认值           | 说明                |
| :---------------- | :--------------- | :--------- | :--------------- | :------------------ |
| **BlockID** | `PW_BlockID`   | String     | `drawingArea0` | 组件唯一标识符。    |
| **Enable**  | `PW_Enable`    | Boolean    | `True`         | 是否启用。          |
| **Group**   | `PW_Group`     | Boolean    | `False`        | 是否作为组显示。    |
| **Show**    | `PW_Show`      | Boolean    | `True`         | 是否可见。          |
| **Bottom**  | `PW_Bottom`    | Attachment | `Default`      | 底部附件。          |
| **Left**    | `PW_Left`      | Attachment | `Default`      | 左侧附件。          |
| **Right**   | `PW_Right`     | Attachment | `Default`      | 右侧附件。          |
| **Top**     | `PW_Top`       | Attachment | `Default`      | 顶部附件。          |
| **Height**  | `PW_Height`    | Integer    | `0`            | 高度 (0 表示自动)。 |
| **Image**   | `PW_Image`     | String     | `""`           | 图片路径。          |
| **Width**   | `PW_Width`     | Integer    | `0`            | 宽度 (0 表示自动)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockDrawingArea_1770451903113_708",
  "type": "BlockDrawingArea",
  "props": {
    "PW_BlockID": "drawingArea0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_Height": 0,
    "PW_Image": "",
    "PW_Width": 0,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.51 图层 (Layer - `BlockLayer`)
对应 NX Open `BlockLayer` 组件，用于选择或指定图层。

> **POWER 模板参考**: `power_ui_template/BlockLayer.ui`

| UI设计器显示名称                     | 导出JSON字段名称                  | 类型       | 默认值           | 说明                         |
| :----------------------------------- | :-------------------------------- | :--------- | :--------------- | :--------------------------- |
| **BlockID**                    | `PW_BlockID`                    | String     | `layer0`       | 组件唯一标识符。             |
| **Enable**                     | `PW_Enable`                     | Boolean    | `True`         | 是否启用。                   |
| **Group**                      | `PW_Group`                      | Boolean    | `False`        | 是否作为组显示。             |
| **Label**                      | `PW_Label`                      | String     | `""`           | 标签文本。                   |
| **Show**                       | `PW_Show`                       | Boolean    | `True`         | 是否可见。                   |
| **Bottom**                     | `PW_Bottom`                     | Attachment | `Default`      | 底部附件。                   |
| **Left**                       | `PW_Left`                       | Attachment | `Default`      | 左侧附件。                   |
| **Right**                      | `PW_Right`                      | Attachment | `Default`      | 右侧附件。                   |
| **Top**                        | `PW_Top`                        | Attachment | `Default`      | 顶部附件。                   |
| **LayerOption**                | `PW_LayerOption`                | Enum       | `User Defined` | 图层选项。                   |
| **LayerValue**                 | `PW_LayerValue`                 | Integer    | `1`            | 图层值。                     |
| **ShowMaintainLayerOption**    | `PW_ShowMaintainLayerOption`    | Boolean    | `False`        | 是否显示保持图层选项。       |
| **ShowOriginalLayerOption**    | `PW_ShowOriginalLayerOption`    | Boolean    | `False`        | 是否显示原始图层选项。       |
| **ShowUserDefinedLayerOption** | `PW_ShowUserDefinedLayerOption` | Boolean    | `True`         | 是否显示用户自定义图层选项。 |
| **ShowWorkLayerOption**        | `PW_ShowWorkLayerOption`        | Boolean    | `True`         | 是否显示工作图层选项。       |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockLayer_1770451903113_709",
  "type": "BlockLayer",
  "props": {
    "PW_BlockID": "layer0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_LayerOption": "User Defined",
    "PW_LayerValue": 1,
    "PW_ShowMaintainLayerOption": false,
    "PW_ShowOriginalLayerOption": false,
    "PW_ShowUserDefinedLayerOption": true,
    "PW_ShowWorkLayerOption": true,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.52 轨迹上尺寸（沿着曲线的位置） (OnPathDim - `BlockOnPathDim`)
对应 NX Open `BlockOnPathDim` 组件，用于在路径/曲线上指定位置，支持长度、百分比、参数等定位方式。

> **POWER 模板参考**: `power_ui_template/BlockOnPathDim.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值            | 说明                                                       |
| :----------------------------- | :-------------------------- | :--------- | :---------------- | :--------------------------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `on_path_dim0`  | 组件唯一标识符。                                           |
| **Enable**               | `PW_Enable`               | Boolean    | `True`          | 是否启用。                                                 |
| **Group**                | `PW_Group`                | Boolean    | `False`         | 是否作为组显示。                                           |
| **Label**                | `PW_Label`                | String     | `""`            | 标签文本。                                                 |
| **Show**                 | `PW_Show`                 | Boolean    | `True`          | 是否可见。                                                 |
| **RetainUnits**          | `PW_RetainUnits`          | Boolean    | `True`          | 是否保留单位设置。                                         |
| **RetainValue**          | `PW_RetainValue`          | Boolean    | `True`          | 是否在对话框关闭后保留值。                                 |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`       | 底部附件。                                                 |
| **Left**                 | `PW_Left`                 | Attachment | `Default`       | 左侧附件。                                                 |
| **Right**                | `PW_Right`                | Attachment | `Default`       | 右侧附件。                                                 |
| **Top**                  | `PW_Top`                  | Attachment | `Default`       | 顶部附件。                                                 |
| **AdaptiveScaleLimits** | `PW_AdaptiveScaleLimits` | Boolean    | `False`         | 是否自适应缩放限制。                    |
| **BalloonTooltipImage**  | `PW_BalloonTooltipImage`  | String     | `""`            | 气泡提示图片路径。                                         |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | Enum       | `Horizontal`    | 气泡提示布局。                                             |
| **BalloonTooltipText**   | `PW_BalloonTooltipText`   | String     | `""`            | 气泡提示文本。                                             |
| **Formula**              | `PW_Formula`              | String     | `""`            | 表达式公式。                                               |
| **LimitCheckTolerance**  | `PW_LimitCheckTolerance`  | Double     | `-1`            | 限制检查容差。                                             |
| **LocationOption**       | `PW_LocationOption`       | Enum       | `Length`        | 定位方式 (Length, Percent)。                                |
| **MaximumValue**         | `PW_MaximumValue`         | Double     | `1.79769e+308`  | 最大值。                                                   |
| **MaxInclusive**         | `PW_MaxInclusive`         | Boolean    | `True`          | 最大值是否包含。                                           |
| **MinimumValue**         | `PW_MinimumValue`         | Double     | `-1.79769e+308` | 最小值。                                                   |
| **MinInclusive**         | `PW_MinInclusive`         | Boolean    | `True`          | 最小值是否包含。                                           |
| **OptionMask**           | `PW_OptionMask`           | Bits       | `(0xf)`         | 可用选项掩码 (Length, Percent, Parameter, Through Point)。 |
| **OptionMenuTitle**      | `PW_OptionMenuTitle`      | String     | `Location`      | 选项菜单标题。                                             |
| **ShowFocusHandle**      | `PW_ShowFocusHandle`      | Boolean    | `False`         | 是否显示焦点手柄。                                         |
| **ShowSpin**             | `PW_ShowSpin`             | Boolean    | `False`         | 是否显示旋转控制。                                         |
| **Value**                | `PW_Value`                | Double     | `0`             | **当前值**。                                               |
| **WithScale**            | `PW_WithScale`            | Boolean    | `False`         | 是否带缩放滑块。                                           |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockOnPathDim_1770451903113_702",
  "type": "BlockOnPathDim",
  "props": {
    "PW_BlockID": "on_path_dim0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_RetainUnits": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AdaptiveScaleLimits": false
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Formula": "",
    "PW_LimitCheckTolerance": -1,
    "PW_LocationOption": "Length",
    "PW_MaximumValue": 1.79769e+308,
    "PW_MaxInclusive": true,
    "PW_MinimumValue": -1.79769e+308,
    "PW_MinInclusive": true,
    "PW_OptionMask": [],
    "PW_OptionMenuTitle": "Location",
    "PW_ShowFocusHandle": false,
    "PW_ShowSpin": false,
    "PW_Value": 0,
    "PW_WithScale": false,
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.53 整数表 (IntegerTable - `BlockIntegerTable`)
对应 NX Open `BlockIntegerTable` 组件，用于以表格形式输入整数数据。

> **POWER 模板参考**: `power_ui_template/BlockIntegerTable.ui`

| UI设计器显示名称       | 导出JSON字段名称    | 类型       | 默认值            | 说明                       |
| :--------------------- | :------------------ | :--------- | :---------------- | :------------------------- |
| **BlockID**      | `PW_BlockID`      | String     | `integerTable0` | 组件唯一标识符。           |
| **Enable**       | `PW_Enable`       | Boolean    | `True`          | 是否启用。                 |
| **Group**        | `PW_Group`        | Boolean    | `False`         | 是否作为组显示。           |
| **Label**        | `PW_Label`        | String     | `##16Table`     | 标签文本。                 |
| **Show**         | `PW_Show`         | Boolean    | `True`          | 是否可见。                 |
| **RetainValue**  | `PW_RetainValue`  | Boolean    | `True`          | 是否在对话框关闭后保留值。 |
| **Bottom**       | `PW_Bottom`       | Attachment | `Default`       | 底部附件。                 |
| **Left**         | `PW_Left`         | Attachment | `Default`       | 左侧附件。                 |
| **Right**        | `PW_Right`        | Attachment | `Default`       | 右侧附件。                 |
| **Top**          | `PW_Top`          | Attachment | `Default`       | 顶部附件。                 |
| **ColumnTitles** | `PW_ColumnTitles` | Strings    | `[]`            | 列标题数组。               |
| **Increment**            | `PW_Increment`            | Double     | `1`           | 增量值。                               |
| **RowTitles**    | `PW_RowTitles`    | Strings    | `[]`            | 行标题数组。               |
| **Spin**         | `PW_Spin`         | Boolean    | `False`         | 是否显示旋转控制。         |
| **WrapSpin**             | `PW_WrapSpin`             | Boolean    | `False`       | 是否循环旋转。                         |

**示例**:

```json
{
  "id": "BlockIntegerTable_1770451903113_703",
  "type": "BlockIntegerTable",
  "props": {
    "PW_BlockID": "integerTable0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "##16Table",
    "PW_Show": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_ColumnTitles": [],
    "PW_Increment": 1,
    "PW_RowTitles": [],
    "PW_Spin": false,
    "PW_WrapSpin": false
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.54 双精度表 (DoubleTable - `BlockDoubleTable`)
对应 NX Open `BlockDoubleTable` 组件，用于以表格形式输入双精度浮点数据。

> **POWER 模板参考**: `power_ui_template/BlockDoubleTable.ui`

| UI设计器显示名称       | 导出JSON字段名称    | 类型       | 默认值           | 说明                       |
| :--------------------- | :------------------ | :--------- | :--------------- | :------------------------- |
| **BlockID**      | `PW_BlockID`      | String     | `doubleTable0` | 组件唯一标识符。           |
| **Enable**       | `PW_Enable`       | Boolean    | `True`         | 是否启用。                 |
| **Group**        | `PW_Group`        | Boolean    | `False`        | 是否作为组显示。           |
| **Label**        | `PW_Label`        | String     | `##16Table`    | 标签文本。                 |
| **Show**         | `PW_Show`         | Boolean    | `True`         | 是否可见。                 |
| **RetainValue**  | `PW_RetainValue`  | Boolean    | `True`         | 是否在对话框关闭后保留值。 |
| **Bottom**       | `PW_Bottom`       | Attachment | `Default`      | 底部附件。                 |
| **Left**         | `PW_Left`         | Attachment | `Default`      | 左侧附件。                 |
| **Right**        | `PW_Right`        | Attachment | `Default`      | 右侧附件。                 |
| **Top**          | `PW_Top`          | Attachment | `Default`      | 顶部附件。                 |
| **CellWidth**    | `PW_CellWidth`    | Integer    | `0`            | 单元格宽度 (0 表示自动)。  |
| **ColumnTitles** | `PW_ColumnTitles` | Strings    | `[]`           | 列标题数组。               |
| **Increment**            | `PW_Increment`            | Double     | `0`           | 增量值。                               |
| **RowTitles**    | `PW_RowTitles`    | Strings    | `[]`           | 行标题数组。               |
| **Spin**         | `PW_Spin`         | Boolean    | `False`        | 是否显示旋转控制。         |
| **WrapSpin**             | `PW_WrapSpin`             | Boolean    | `False`       | 是否循环旋转。                         |

**示例**:

```json
{
  "id": "BlockDoubleTable_1770451903113_704",
  "type": "BlockDoubleTable",
  "props": {
    "PW_BlockID": "doubleTable0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "##16Table",
    "PW_Show": true,
    "PW_RetainValue": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_CellWidth": 0,
    "PW_ColumnTitles": [],
    "PW_Increment": 1,
    "PW_RowTitles": [],
    "PW_Spin": false,
    "PW_WrapSpin": false
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.55 向导 (Wizard - `BlockWizard`)
对应 NX Open `BlockWizard` 组件，用于分步引导用户完成操作，是一个**容器类控件**。每个步骤对应一个子组件。

> **POWER 模板参考**: `power_ui_template/BlockWizard.ui`

| UI设计器显示名称            | 导出JSON字段名称         | 类型       | 默认值      | 说明                 |
| :-------------------------- | :----------------------- | :--------- | :---------- | :------------------- |
| **BlockID**           | `PW_BlockID`           | String     | `wizard0`  | 组件唯一标识符。     |
| **Enable**            | `PW_Enable`            | Boolean    | `True`    | 是否启用。           |
| **Expanded**          | `PW_Expanded`          | Boolean    | `True`    | 初始时是否展开。     |
| **Group**             | `PW_Group`             | Boolean    | `False`   | 是否作为组显示。     |
| **Label**             | `PW_Label`             | String     | `Wizard`  | 标签文本。           |
| **Show**              | `PW_Show`              | Boolean    | `True`    | 是否可见。           |
| **Localize**          | `PW_Localize`          | Boolean    | `True`    | 是否支持本地化。     |
| **Bottom**            | `PW_Bottom`            | Attachment | `Default` | 底部附件。           |
| **Left**              | `PW_Left`              | Attachment | `Default` | 左侧附件。           |
| **Right**             | `PW_Right`             | Attachment | `Default` | 右侧附件。           |
| **Top**               | `PW_Top`               | Attachment | `Default` | 顶部附件。           |
| **CurrentStep**       | `PW_CurrentStep`       | Integer    | `0`       | 当前步骤索引。       |
| **HighQualityBitmap** | `PW_HighQualityBitmap` | Boolean    | `True`    | 是否使用高质量位图。 |
| **ShowBannerArea**    | `PW_ShowBannerArea`    | Boolean    | `True`    | 是否显示横幅区域。   |
| **ShowTaskNavigator** | `PW_ShowTaskNavigator` | Boolean    | `True`    | 是否显示任务导航器。 |
| **StepBannerBitmaps** | `PW_StepBannerBitmaps` | Strings    | `[]`      | 各步骤横幅图片数组。 |
| **StepBitmaps**       | `PW_StepBitmaps`       | Strings    | `[]`      | 各步骤图标数组。     |
| **StepCues**          | `PW_StepCues`          | Strings    | `[]`      | 各步骤提示文本。     |
| **StepText**          | `PW_StepText`          | Strings    | `[]`      | 各步骤标题文本。     |

**示例**:

```json
{
  "id": "BlockWizard_1770451903113_706",
  "type": "BlockWizard",
  "props": {
    "PW_BlockID": "wizard0",
    "PW_Enable": true,
    "PW_Expanded": true,
    "PW_Group": false,
    "PW_Label": "Wizard",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_CurrentStep": 0,
    "PW_HighQualityBitmap": true,
    "PW_ShowBannerArea": true,
    "PW_ShowTaskNavigator": true,
    "PW_StepBannerBitmaps": ["步骤1横幅", "步骤2横幅"],
    "PW_StepBitmaps": ["step1_icon", "step2_icon"],
    "PW_StepCues": ["请完成步骤1", "请完成步骤2"],
    "PW_StepText": ["步骤 1", "步骤 2"]
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": [...]
}
```

### 3.56 浏览器 (Explorer - `BlockExplorer`)
对应 NX Open `BlockExplorer` 组件，提供树形导航 + 内容区域的**容器类控件**，类似资源管理器布局。

> **POWER 模板参考**: `power_ui_template/BlockExplorer.ui`

| UI设计器显示名称      | 导出JSON字段名称   | 类型       | 默认值        | 说明                      |
| :-------------------- | :----------------- | :--------- | :------------ | :------------------------ |
| **BlockID**     | `PW_BlockID`     | String     | `explorer0`  | 组件唯一标识符。          |
| **Enable**      | `PW_Enable`      | Boolean    | `True`      | 是否启用。                |
| **Expanded**    | `PW_Expanded`    | Boolean    | `True`      | 初始时是否展开。          |
| **Group**       | `PW_Group`       | Boolean    | `False`     | 是否作为组显示。          |
| **Label**       | `PW_Label`       | String     | `Explorer`  | 标签文本。                |
| **Show**        | `PW_Show`        | Boolean    | `True`      | 是否可见。                |
| **Localize**    | `PW_Localize`    | Boolean    | `True`      | 是否支持本地化。          |
| **Bottom**      | `PW_Bottom`      | Attachment | `Default`   | 底部附件。                |
| **Left**        | `PW_Left`        | Attachment | `Default`   | 左侧附件。                |
| **Right**       | `PW_Right`       | Attachment | `Default`   | 右侧附件。                |
| **Top**         | `PW_Top`         | Attachment | `Default`   | 顶部附件。                |
| **CurrentNode** | `PW_CurrentNode` | Integer    | `0`         | 当前选中节点索引。        |
| **TreeWidth**   | `PW_TreeWidth`   | Integer    | `175`       | 树形导航区域宽度 (像素)。 |

**示例**:

```json
{
  "id": "BlockExplorer_1770451903113_707",
  "type": "BlockExplorer",
  "props": {
    "PW_BlockID": "explorer0",
    "PW_Enable": true,
    "PW_Expanded": true,
    "PW_Group": false,
    "PW_Label": "Explorer",
    "PW_Show": true,
    "PW_Localize": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_CurrentNode": 0,
    "PW_TreeWidth": 175
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": [...]
}
```

### 3.57 选择小平面区域 (FacetSelect - `BlockFacetSelect`)
对应 NX Open `BlockFacetSelect` 组件，用于选择小平面体（Facet Body）上的区域。

> **POWER 模板参考**: `power_ui_template/BlockFacetSelect.ui`

| UI设计器显示名称                      | 导出JSON字段名称                   | 类型       | 默认值                   | 说明                                                         |
| :------------------------------------ | :--------------------------------- | :--------- | :----------------------- | :----------------------------------------------------------- |
| **BlockID**                     | `PW_BlockID`                     | String     | `facetSelect0`         | 组件唯一标识符。                                             |
| **Enable**                      | `PW_Enable`                      | Boolean    | `True`                 | 是否启用。                                                   |
| **Group**                       | `PW_Group`                       | Boolean    | `False`                | 是否作为组显示。                                             |
| **Show**                        | `PW_Show`                        | Boolean    | `True`                 | 是否可见。                                                   |
| **Bottom**                      | `PW_Bottom`                      | Attachment | `Default`              | 底部附件。                                                   |
| **Left**                        | `PW_Left`                        | Attachment | `Default`              | 左侧附件。                                                   |
| **Right**                       | `PW_Right`                       | Attachment | `Default`              | 右侧附件。                                                   |
| **Top**                         | `PW_Top`                         | Attachment | `Default`              | 顶部附件。                                                   |
| **PointOverlay**                | `PW_PointOverlay`                | Boolean    | `False`                | 是否显示点叠加。                                             |
| **SmartUpdateOption**           | `PW_SmartUpdateOption`           | String     | `Within Modeling`      | 智能更新选项。                                               |
| **SnapPointTypesEnabled**       | `PW_SnapPointTypesEnabled`       | Bits       | `(0x7ffff)`            | 启用的捕捉点类型。                                           |
| **SnapPointTypesOnByDefault**   | `PW_SnapPointTypesOnByDefault`   | Bits       | `(0x11df8)`            | 默认开启的捕捉点类型。                                       |
| **AutomaticProgression**        | `PW_AutomaticProgression`        | Boolean    | `False`                | 是否自动推进到下一步。                                       |
| **BalloonTooltipImage**         | `PW_BalloonTooltipImage`         | String     | `""`                   | 气泡提示图片路径。                                           |
| **BalloonTooltipLayout**        | `PW_BalloonTooltipLayout`        | Enum       | `Horizontal`           | 气泡提示布局。                                               |
| **BalloonTooltipText**          | `PW_BalloonTooltipText`          | String     | `""`                   | 气泡提示文本。                                               |
| **Bitmap**                      | `PW_Bitmap`                      | String     | `select_facet_regions` | 图标路径。                                                   |
| **BlendVirtualCurveOverlay**    | `PW_BlendVirtualCurveOverlay`    | Boolean    | `False`                | 是否显示混合虚拟曲线叠加。                                   |
| **BrushToolSize**               | `PW_BrushToolSize`               | Double     | `10`                   | 画笔工具大小。                                               |
| **Cue**                         | `PW_Cue`                         | String     | `选择小平面体区域`     | 提示文本。                                                   |
| **FacetCollectionRules**        | `PW_FacetCollectionRules`        | Bits       | `(0x1f)`               | 小平面收集规则 (Body Facets, Color Region, Face Facets 等)。 |
| **LabelString**                 | `PW_LabelString`                 | String     | `选择小平面体区域`     | 标签字符串。                                                 |
| **OwningFacetCollector**        | `PW_OwningFacetCollector`        | Integer    | `0`                    | 所属小平面收集器。                                           |
| **SelectedFacetCollectionRule** | `PW_SelectedFacetCollectionRule` | Integer    | `0`                    | 选中的收集规则索引。                                         |
| **StepStatus**                  | `PW_StepStatus`                  | Enum       | `Optional`             | 步骤状态。                                                   |
| **SupportedFacetTypes**         | `PW_SupportedFacetTypes`         | Bits       | `(0x0)`                | 支持的小平面类型。                                           |
| **ToolTip**                     | `PW_ToolTip`                     | String     | `小平面区域`           | 工具提示文本。(POWER 特有属性)                               |
| **ToolTipID**            | `PW_ToolTipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockFacetSelect_1770451903113_715",
  "type": "BlockFacetSelect",
  "props": {
    "PW_BlockID": "facetSelect0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_PointOverlay": false,
    "PW_SmartUpdateOption": "Within Modeling",
    "PW_SnapPointTypesEnabled": [],
    "PW_SnapPointTypesOnByDefault": [],
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "",
    "PW_Bitmap": "select_facet_regions",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_BrushToolSize": 10,
    "PW_Cue": "选择小平面体区域",
    "PW_FacetCollectionRules": [],
    "PW_LabelString": "选择小平面体区域",
    "PW_OwningFacetCollector": 0,
    "PW_SelectedFacetCollectionRule": 0,
    "PW_StepStatus": "Optional",
    "PW_SupportedFacetTypes": [],
    "PW_ToolTip": "小平面区域",
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.58 选择节点 (SelectNodes - `BlockSelectNodes`)
对应 NX Open `BlockSelectNodes` 组件，用于选择有限元网格节点。

> **POWER 模板参考**: `power_ui_template/BlockSelectNodes.ui`

| UI设计器显示名称               | 导出JSON字段名称            | 类型       | 默认值           | 说明                                       |
| :----------------------------- | :-------------------------- | :--------- | :--------------- | :----------------------------------------- |
| **BlockID**              | `PW_BlockID`              | String     | `selectNodes0` | 组件唯一标识符。                           |
| **Enable**               | `PW_Enable`               | Boolean    | `True`         | 是否启用。                                 |
| **Group**                | `PW_Group`                | Boolean    | `False`        | 是否作为组显示。                           |
| **Label**                | `PW_Label`                | String     | `""`           | 标签文本。(POWER 特有属性)                 |
| **Show**                 | `PW_Show`                 | Boolean    | `True`         | 是否可见。                                 |
| **Bottom**               | `PW_Bottom`               | Attachment | `Default`      | 底部附件。                                 |
| **Left**                 | `PW_Left`                 | Attachment | `Default`      | 左侧附件。                                 |
| **Right**                | `PW_Right`                | Attachment | `Default`      | 右侧附件。                                 |
| **Top**                  | `PW_Top`                  | Attachment | `Default`      | 顶部附件。                                 |
| **AutomaticProgression** | `PW_AutomaticProgression` | Boolean    | `False`        | 是否自动推进到下一步。                     |
| **Bitmap**               | `PW_Bitmap`               | String     | `node`         | 图标路径。                                 |
| **Cue**                  | `PW_Cue`                  | String     | `选择节点`     | 提示文本。                                 |
| **LabelString**          | `PW_LabelString`          | String     | `选择节点`     | 标签文本。                                 |
| **SelectMode**           | `PW_SelectMode`           | Enum       | `Multiple`     | 选择模式 (Single, Multiple)。              |
| **ShowSelection**        | `PW_ShowSelection`        | Boolean    | `True`         | 是否显示选择结果。                         |
| **StepStatus**           | `PW_StepStatus`           | Enum       | `Required`     | 步骤状态 (Required, Optional, Satisfied)。 |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockSelectNodes_1770451903113_718",
  "type": "BlockSelectNodes",
  "props": {
    "PW_BlockID": "selectNodes0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": false,
    "PW_Bitmap": "node",
    "PW_Cue": "选择节点",
    "PW_LabelString": "选择节点",
    "PW_SelectMode": "Multiple",
    "PW_ShowSelection": true,
    "PW_StepStatus": "Required",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.59 选择单元 (SelectElements - `BlockSelectElements`)
对应 NX Open `BlockSelectElements` 组件，用于选择有限元网格单元。

> **POWER 模板参考**: `power_ui_template/BlockSelectElements.ui`

| UI设计器显示名称        | 导出JSON字段名称     | 类型       | 默认值               | 说明                                               |
| :---------------------- | :------------------- | :--------- | :------------------- | :------------------------------------------------- |
| **BlockID**       | `PW_BlockID`       | String     | `selectElements0`  | 组件唯一标识符。                                   |
| **Enable**        | `PW_Enable`        | Boolean    | `True`             | 是否启用。                                         |
| **Group**         | `PW_Group`         | Boolean    | `False`            | 是否作为组显示。                                   |
| **Label**         | `PW_Label`         | String     | `""`               | 标签文本。(POWER 特有属性)                         |
| **Show**          | `PW_Show`          | Boolean    | `True`             | 是否可见。                                         |
| **Bottom**        | `PW_Bottom`        | Attachment | `Default`          | 底部附件。                                         |
| **Left**          | `PW_Left`          | Attachment | `Default`          | 左侧附件。                                         |
| **Right**         | `PW_Right`         | Attachment | `Default`          | 右侧附件。                                         |
| **Top**           | `PW_Top`           | Attachment | `Default`          | 顶部附件。                                         |
| **AutomaticProgression** | `PW_AutomaticProgression` | Boolean | `False`     | 是否自动推进到下一步。(POWER 特有属性)             |
| **Bitmap**        | `PW_Bitmap`        | String     | `selection_cursor` | 图标路径。                                         |
| **Cue**           | `PW_Cue`           | String     | `选择单元`         | 提示文本。                                         |
| **LabelString**   | `PW_LabelString`   | String     | `选择单元`         | 标签文本。                                         |
| **SelectMode**    | `PW_SelectMode`    | Enum       | `Single`           | 选择模式 (Single, Multiple)。                      |
| **SelectSubType** | `PW_SelectSubType` | Enum       | `Element`          | 选择子类型 (Element, Element Face, Element Edge)。 |
| **ShowSelection** | `PW_ShowSelection` | Boolean    | `True`             | 是否显示选择结果。                                 |
| **StepStatus**    | `PW_StepStatus`    | Enum       | `Required`         | 步骤状态。                                         |
| **TooltipID**            | `PW_TooltipID`            | String     | `""`         | 工具提示 ID。**(POWER 特有属性)** |

**示例**:

```json
{
  "id": "BlockSelectElements_1770451903113_619",
  "type": "BlockSelectElements",
  "props": {
    "PW_BlockID": "selectElements0",
    "PW_Enable": true,
    "PW_Group": false,
    "PW_Label": "",
    "PW_Show": true,
    "PW_Bottom": "Default",
    "PW_Left": "Default",
    "PW_Right": "Default",
    "PW_Top": "Default",
    "PW_AutomaticProgression": false,
    "PW_Bitmap": "selection_cursor",
    "PW_Cue": "选择单元",
    "PW_LabelString": "选择单元",
    "PW_SelectMode": "Multiple",
    "PW_SelectSubType": "Element",
    "PW_ShowSelection": true,
    "PW_StepStatus": "Required",
    "PW_TooltipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```
