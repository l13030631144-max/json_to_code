# 前端智能体 JSON 接口规范文档 (Source Format)

**版本**: v2.0
**来源**: `qianduan.html` (Preview/Save Project 格式)

本文档描述了从前端设计器导出的最新 JSON 数据结构（对应 "保存项目" 输出的源格式）。该格式包含了完整的 UI 定义、布局信息及属性配置，用于作为智能体生成 CAD 软件代码的输入。

## 1. 根结构 (Root Object)

JSON 文件的根是一个对象，包含以下字段：

| 字段名 | 类型 | 说明 |
| :--- | :--- | :--- |
| **`tree`** | Array | **UI 组件树**。这是一个只包含**一个元素**的数组，该元素即为根节点（通常为 `BlockDialog` 类型）。所有其他 UI 组件都作为该根节点的子节点嵌套在 `children` 字段中。 |
| **`previewMode`** | Boolean | **预览模式状态**。指示保存时编辑器是否处于预览模式。 |
| **`timestamp`** | Number | **时间戳**。文件生成的 Unix 时间戳（毫秒）。 |

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
*   **Root**: 顶层 `tree[0]` 必须是 `BlockDialog`。
*   **Children**: 所有其他组件（如 `BlockGroup`, `BlockString` 等）必须作为后代节点存在于 `BlockDialog` 的 `children` 属性（或更深层嵌套）中。

### 2.1 核心字段

| 字段名 | 类型 | 说明 |
| :--- | :--- | :--- |
| **`id`** | String | **组件唯一标识符**。<br>格式示例：`BlockDialog_1770451903113_37111`。用于内部索引和查找。 |
| **`type`** | String | **组件类型**。<br>对应 C++ 类名，如 `BlockDialog`, `BlockGroup`, `BlockLabel` 等。 |
| **`props`** | Object | **属性集合**。<br>包含所有组件配置，键名通常带有 `PW_` 前缀（对应 NX Open 属性）。 |
| **`hiddenProps`** | Array | **属性过滤/默认值列表**。<br>存储因使用默认值而无需生成的属性名。**关键用途**：为了节省 LLM 的 Token，凡是出现在此列表中的属性，在生成给 AI 的 Prompt 时应被忽略，由后端代码使用默认值。 |
| **`altProperties`** | Object | **动态交互规则集合**。<br>存储属性值的动态计算规则或条件表达式。**关键用途**：用于定义当其他控件（触发器）的值发生变化时，该属性如何随之变化（例如 Visibility = `enum1 == 'OptionA'`）。 |
| **`children`** | Array | **子组件列表**。<br>嵌套的子组件对象数组。 |

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

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **ClassName** | `className` | String | `TestDialog` | **核心类名**。决定生成的 `.ui/.cpp/.h` 文件名及类名。仅 Dialog 有此属性。 |
| **BlockID** | `PW_BlockID` | String | `Dialog` | **变量名**。生成的 C++ 代码中变量名将基于此 ID。 |
| **Cue** | `PW_Cue` | String | `""` | 提示文本。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示 |
| **Label** | `PW_Label` | String | `无标题` | 对话框标题。 |
| **Dialog Sizing** | `PW_Dialog_Sizing` | String | `Follow Policy` | 对话框尺寸调整策略。 |
| **Navigation Style** | `PW_Navigation_Style` | String | `OK Apply Cancel` | 底部导航按钮样式（确定/应用/取消）。 |
| **UseMore** | `PW_UseMore` | Boolean | `True` | **Power 独有属性**。控制是否显示"更多"选项或其他扩展功能。 |
| **(Hidden)** | `_w` | Number | - | **宽度** (像素)。不显示在 UI 设计器中，仅在保存时写入。 |
| **(Hidden)** | `_h` | Number | - | **高度** (像素)。不显示在 UI 设计器中，仅在保存时写入。 |
| **(Hidden)** | `_x` | Number | - | **屏幕 X 坐标**。不显示在 UI 设计器中，仅在保存时写入。 |
| **(Hidden)** | `_y` | Number | - | **屏幕 Y 坐标**。不显示在 UI 设计器中，仅在保存时写入。 |

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
  "_w": 500,
  "_h": 400,
  "_x": 640,
  "_y": 320
}
```

> **注意**: `default` 列的值即为默认值。如果属性值与默认值相同，且该属性被包含在 `hiddenProps` 列表中，则后端在生成 JSON 时可能会省略该属性（详见 `hiddenProps` 说明）。



### 2.3 `altProperties` 动态交互详解

`altProperties` 字段用于定义组件属性与其它控件值之间的动态关联关系。

*   **格式**: 键值对对象。
    *   **Keys**: 需要动态变化的属性名（如 `PW_Show`, `PW_Enable`）。
    *   **Values**: 表达式字符串。
*   **支持范围**:
    *   **触发条件 (Triggers)**: 目前仅支持 **枚举 (Enumeration)** 和 **开关 (Toggle)** 类型的控件作为触发源。
    *   **受控属性 (Targets)**: 目前仅支持 **Bool 类型** 的属性被控制（最常用的是 `PW_Show` 和 `PW_Enable`）。
*   **常见交互逻辑**:
    *   **Visibility (可见性)**: 当触发控件的值满足条件时显示。
        *   Example (Enum Trigger): `"PW_Show": "Enum1(1, 2)"` (Enum1 的值为 1 或 2 时显示，支持多选)
        *   Example (Toggle Trigger): `"PW_Show": "toggle1 == true"`
    *   **Enablement (可用性)**: 当触发控件的值满足条件时启用。
        *   Example: `"PW_Enable": "toggle1 == true"`
    *   **Logic Combination (逻辑组合)**: 使用 `&&` (与) 和 `||` (或) 组合多个条件。
        *   Example (AND): `"PW_Show": "toggle1 == true && Enum1(1)"` (同时满足)
        *   Example (OR): `"PW_Enable": "toggle1 == true || toggle2 == true"` (满足任一)
        *   Example (Complex): `"PW_Show": "(toggle1 == true || toggle2 == true) && Enum1(2)"` (混合逻辑)
*   **代码生成影响**:
    *   后端应解析这些表达式 (特别是 `Enum(Val)` 格式)，并在 `update_cb` 回调函数中生成相应的 `if/else` 逻辑，以在运行时动态设置属性。

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

> **POWER 模板参考**: `power_ui_template/group.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `group` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Expanded** | `PW_Expanded` | Boolean | `True` | 初始时是否展开。 |
| **Label** | `PW_Label` | String | `No Title` | 组标题。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Localize** | `PW_Localize` | Boolean | `True` | 是否支持本地化。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局方向。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **Column** | `PW_Column` | Integer | `1` | 组内布局列数 (通常为 1)。 |
| **ShowLabel** | `PW_ShowLabel` | Boolean | `True` | 是否显示组标题。 |

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

> **POWER 模板参考**: `power_ui_template/Enumeration.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `enum0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `No Title` | 标签文本。 |
| **LabelVisibility** | `PW_LabelVisibility` | Boolean | `True` | 标签是否可见。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Localize** | `PW_Localize` | Boolean | `True` | 是否支持本地化。 |
| **RetainValue** | `PW_RetainValue` | Boolean | `True` | 是否在对话框关闭后保留值。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **AllowShortcuts** | `PW_AllowShortcuts` | Boolean | `True` | 是否允许快捷操作。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片。 |
| **BalloonTooltipImages** | `PW_BalloonTooltipImages` | String[] | `[]` | 气泡提示图片列表。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **BalloonTooltipTexts** | `PW_BalloonTooltipTexts` | String[] | `[]` | 气泡提示文本列表。 |
| **Bitmaps** | `PW_Bitmaps` | String[] | `[]` | 选项对应的图标路径数组。 |
| **BorderVisibility** | `PW_BorderVisibility` | Boolean | `True` | 边框是否可见。 |
| **EnumSensitivity** | `PW_EnumSensitivity` | String | `""` | 选项敏感度掩码。 |
| **EnumVisibility** | `PW_EnumVisibility` | String | `""` | 选项可见性掩码。 |
| **HighQualityBitmap** | `PW_HighQualityBitmap` | Boolean | `True` | 是否使用高质量位图。 |
| **IconsOnly** | `PW_IconsOnly` | Boolean | `False` | 是否仅显示图标。 |
| **InitialShortcuts** | `PW_InitialShortcuts` | String | `""` | 初始快捷键。 |
| **Layout** | `PW_Layout` | String | `Vertical` | 布局方向 (Vertical/Horizontal)。 |
| **NumberOfColumns** | `PW_NumberOfColumns` | Integer | `65535` | 列数。 |
| **PackedColumns** | `PW_PackedColumns` | Boolean | `False` | 是否紧凑排列列。 |
| **PresentationStyle** | `PW_PresentationStyle` | String | `OptionMenu` | 展现样式 (OptionMenu, Radio Box, Pulldown)。 |
| **Value** | `PW_Value` | String[] | `[]` | **选项列表** (如 `["Option1", "Option2"]`)。 |
| **Data** | `PW_Data` | String[] | `[]` | **数据列表** (如 `["1", "2"]`)。 **(POWER 特有属性)** |
| **ToolTipID** | `PW_ToolTipID` | String | `""` | 工具提示 ID。 **(POWER 特有属性)** |

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
    "PW_Value": ["类型 A", "类型 B"],
    "PW_Data": ["1", "2"],
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

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `toggle0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `No Title` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Localize** | `PW_Localize` | Boolean | `True` | 是否支持本地化。 |
| **RetainValue** | `PW_RetainValue` | Boolean | `True` | 是否在对话框关闭后保留值。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipOffImage** | `PW_BalloonTooltipOffImage` | String | `""` | 关闭状态时的气泡提示图片。 |
| **BalloonTooltipOffText** | `PW_BalloonTooltipOffText` | String | `""` | 关闭状态时的气泡提示文本。 |
| **BalloonTooltipOnImage** | `PW_BalloonTooltipOnImage` | String | `""` | 开启状态时的气泡提示图片。 |
| **BalloonTooltipOnText** | `PW_BalloonTooltipOnText` | String | `""` | 开启状态时的气泡提示文本。 |
| **Bitmap** | `PW_Bitmap` | String | `""` | 图标路径。 |
| **BitmapOnly** | `PW_BitmapOnly` | Boolean | `False` | 是否仅显示图标。 |
| **Value** | `PW_Value` | Boolean | `False` | **当前值**（开/关状态）。 |

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
    "PW_Value": false
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.4 按钮 (Button - `BlockButton`)

对应 NX Open `BlockButton` 组件，用于触发操作的按钮控件。

> **POWER 模板参考**: `power_ui_template/BlockButton.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `button0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `No Title` | 按钮文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Localize** | `PW_Localize` | Boolean | `True` | 是否支持本地化。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **Bitmap** | `PW_Bitmap` | String | `""` | 按钮图标路径。 |
| **HighQualityBitmap** | `PW_HighQualityBitmap` | Boolean | `True` | 是否使用高质量位图。 |
| **Tooltip** | `PW_Tooltip` | String | `""` | 工具提示文本。 |
| **ToolTipID** | `PW_ToolTipID` | String | `""` | 工具提示 ID。**(POWER 特有属性)** |

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

> **POWER 模板参考**: `power_ui_template/edge_select.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `edge_select0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **AllowConvergentObject** | `PW_AllowConvergentObject` | Boolean | `False` | 是否允许选择收敛对象。 |
| **AllowInferredCurveSelection** | `PW_AllowInferredCurveSelection` | Boolean | `True` | 是否允许推断曲线选择。 |
| **AutomaticProgression** | `PW_AutomaticProgression` | Boolean | `False` | 是否自动推进到下一步。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **Bitmap** | `PW_Bitmap` | String | `""` | 图标路径。 |
| **BlendVirtualCurveOverlay** | `PW_BlendVirtualCurveOverlay` | Boolean | `False` | 是否显示混合虚拟曲线叠加。 |
| **CreateInterpartLink** | `PW_CreateInterpartLink` | Boolean | `False` | 是否创建零部件间链接。 |
| **Cue** | `PW_Cue` | String | `Select Curve` | 提示文本。 |
| **CurveRules** | `PW_CurveRules` | Integer | `799` | 曲线规则位掩码。 |
| **DefaultCurveRules** | `PW_DefaultCurveRules` | String | `Single Curve` | 默认曲线规则 (Single Curve, Connected Curves, Tangent Curves, Face Edges, Body Edges, Vertex Edges, Vertex Tangent Edges)。 |
| **EntityType** | `PW_EntityType` | Integer | `5` | 实体类型。 |
| **InferredCurveSelection** | `PW_InferredCurveSelection` | Boolean | `False` | 是否使用推断曲线选择。 |
| **InterpartSelection** | `PW_InterpartSelection` | String | `Simple` | 零部件间选择模式 (Simple, Non-associative Interpart Copy Only, Associative and Non-associative Interpart Copy, Associative Interpart Copy)。 |
| **LabelString** | `PW_LabelString` | String | `Select Curve` | 标签文本。 |
| **MaximumScope** | `PW_MaximumScope` | String | `Entire Assembly` | 最大选择范围 (Entire Assembly, Within Work Part Only, Within Work Part and Components)。 |
| **PopupMenuEnabled** | `PW_PopupMenuEnabled` | Boolean | `True` | 是否启用右键弹出菜单。 |
| **SelectMode** | `PW_SelectMode` | String | `Single` | 选择模式 (Single, Multiple)。 |
| **StepStatus** | `PW_StepStatus` | String | `Required` | 步骤状态 (Required, Optional, Satisfied)。 |
| **ToolTip** | `PW_ToolTip` | String | `Curve` | 工具提示文本。 |
| **ToolTipID** | `PW_ToolTipID` | String | `""` | 工具提示 ID。**(POWER 特有属性)** |

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
    "PW_AllowInferredCurveSelection": true,
    "PW_AutomaticProgression": false,
    "PW_BalloonTooltipImage": "",
    "PW_BalloonTooltipLayout": "Horizontal",
    "PW_BalloonTooltipText": "选择曲线",
    "PW_Bitmap": "",
    "PW_BlendVirtualCurveOverlay": false,
    "PW_CreateInterpartLink": false,
    "PW_Cue": "Select Curve",
    "PW_CurveRules": 799,
    "PW_DefaultCurveRules": "Single Curve",
    "PW_EntityType": 5,
    "PW_InferredCurveSelection": false,
    "PW_InterpartSelection": "Simple",
    "PW_LabelString": "选择曲线",
    "PW_MaximumScope": "Entire Assembly",
    "PW_PopupMenuEnabled": true,
    "PW_SelectMode": "Multiple",
    "PW_StepStatus": "Required",
    "PW_ToolTip": "Curve",
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.6 指定点 (SpecifyPoint - `BlockSpecifyPoint`)

对应 NX Open `BlockSpecifyPoint` 组件，用于选择或指定点位置。

> **POWER 模板参考**: `power_ui_template/SpecifyPoint.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `point0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `""` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **AutomaticProgression** | `PW_AutomaticProgression` | Boolean | `True` | 是否自动推进到下一步。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **CreateInterpartLink** | `PW_CreateInterpartLink` | Boolean | `False` | 是否创建零部件间链接。 |
| **EnableFaceSelection** | `PW_EnableFaceSelection` | Boolean | `False` | 是否启用面选择。 |
| **InterpartSelection** | `PW_InterpartSelection` | String | `Simple` | 零部件间选择模式 (Simple, Non-associative Interpart Copy Only, Associative and Non-associative Interpart Copy, Associative Interpart Copy)。 |
| **LabelString** | `PW_LabelString` | String | `Specify Point` | 标签字符串。 |
| **SnapPointTypesEnabled** | `PW_SnapPointTypesEnabled` | String[] | `[...]` | 启用的捕捉点类型列表。 |
| **SnapPointTypesOnByDefault** | `PW_SnapPointTypesOnByDefault` | String[] | `[...]` | 默认开启的捕捉点类型列表。 |
| **StepStatus** | `PW_StepStatus` | String | `Required` | 步骤状态 (Required, Optional, Satisfied)。 |
| **ToolTipID** | `PW_ToolTipID` | String | `""` | 工具提示 ID。**(POWER 特有属性)** |

**捕捉点类型说明** (SnapPointTypes):
| 掩码 | 类型名称 | 说明 |
| :--- | :--- | :--- |
| `0x1` | User Defined | 用户定义点 |
| `0x2` | Inferred | 推断点 |
| `0x4` | Screen Position | 屏幕位置 |
| `0x8` | End Point | 端点 |
| `0x10` | Mid Point | 中点 |
| `0x20` | Control Point | 控制点 |
| `0x40` | Intersection | 交点 |
| `0x80` | Arc Center | 圆弧中心 |
| `0x100` | Quadrant Point / Point Constructor | 象限点 / 点构造器 |
| `0x200` | Existing Point | 现有点 |
| `0x400` | Point on Curve | 曲线上的点 |
| `0x800` | Point on Surface | 曲面上的点 |
| `0x2000` | Two-curve Intersection | 双曲线交点 |
| `0x4000` | Tangent Point | 切点 |
| `0x8000` | Poles | 极点 |
| `0x10000` | Bounded Grid Point | 边界网格点 |
| `0x20000` | Facet Vertex Point | 刻面顶点 |
| `0x40000` | Defining Point | 定义点 |

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
    "PW_SnapPointTypesEnabled": [
      "End Point", "Mid Point", "Arc Center", "Intersection",
      "Control Point", "Existing Point", "Point on Curve", "Point on Surface"
    ],
    "PW_SnapPointTypesOnByDefault": [
      "End Point", "Mid Point", "Arc Center", "Intersection", "Existing Point"
    ],
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

> **POWER 模板参考**: `power_ui_template/LinearExpression.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `linear_dim0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `""` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **RetainUnits** | `PW_RetainUnits` | Boolean | `True` | 是否保留单位设置。 |
| **RetainValue** | `PW_RetainValue` | Boolean | `True` | 是否在对话框关闭后保留值。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **AutoReverseDuringDrag** | `PW_AutoReverseDuringDrag` | Boolean | `False` | 拖拽时是否自动反转方向。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **Formula** | `PW_Formula` | String | `"0"` | 表达式公式。 |
| **HandleOrientation** | `PW_HandleOrientation` | String | `"0 0 0"` | 拖拽手柄方向 (X Y Z)。 |
| **HandleOrigin** | `PW_HandleOrigin` | String | `"0 0 0"` | 拖拽手柄原点 (X Y Z)。 |
| **LimitCheckTolerance** | `PW_LimitCheckTolerance` | Double | `-1` | 限制检查容差 (-1 表示禁用)。 |
| **MaximumValue** | `PW_MaximumValue` | Double | `1.79769e+308` | 最大值。 |
| **MaxInclusive** | `PW_MaxInclusive` | Boolean | `True` | 最大值是否包含边界。 |
| **MinimumValue** | `PW_MinimumValue` | Double | `-1.79769e+308` | 最小值。 |
| **MinInclusive** | `PW_MinInclusive` | Boolean | `True` | 最小值是否包含边界。 |
| **ShowFocusHandle** | `PW_ShowFocusHandle` | Boolean | `False` | 是否显示焦点手柄。 |
| **ShowHandle** | `PW_ShowHandle` | Boolean | `False` | 是否显示拖拽手柄。 |
| **ShowSpine** | `PW_ShowSpine` | Boolean | `False` | 是否显示脊线。 |
| **Value** | `PW_Value` | Double | `0` | **当前值**。 |
| **WithScale** | `PW_WithScale` | Boolean | `False` | 是否带缩放滑块。 |
| **ToolTipID** | `PW_ToolTipID` | String | `""` | 工具提示 ID。**(POWER 特有属性)** |

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
    "PW_ShowSpine": false,
    "PW_Value": 10.0,
    "PW_WithScale": false,
    "PW_ToolTipID": ""
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.8 角度尺寸 (AngularDimension - `BlockAngularExpression`)

对应 NX Open `BlockAngularExpression` 组件，用于输入角度值，支持表达式和拖拽操作。

> **POWER 模板参考**: `power_ui_template/BlockAngularExpression.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `angular_dim0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `""` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **RetainUnits** | `PW_RetainUnits` | Boolean | `True` | 是否保留单位设置。 |
| **RetainValue** | `PW_RetainValue` | Boolean | `True` | 是否在对话框关闭后保留值。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **Formula** | `PW_Formula` | String | `"0"` | 表达式公式。 |
| **HandleFixedSizeFlag** | `PW_HandleFixedSizeFlag` | Boolean | `False` | 手柄是否固定大小。 |
| **HandleOrigin** | `PW_HandleOrigin` | String | `"0 0 0"` | 拖拽手柄原点 (X Y Z)。 |
| **HandleRadius** | `PW_HandleRadius` | Double | `0` | 手柄半径。 |
| **HandleRadiusOffset** | `PW_HandleRadiusOffset` | Double | `0` | 手柄半径偏移。 |
| **HandleXAxis** | `PW_HandleXAxis` | String | `"0 0 0"` | 手柄 X 轴方向。 |
| **HandleZAxis** | `PW_HandleZAxis` | String | `"0 0 0"` | 手柄 Z 轴方向。 |
| **LimitCheckTolerance** | `PW_LimitCheckTolerance` | Double | `-1` | 限制检查容差 (-1 表示禁用)。 |
| **MaximumValue** | `PW_MaximumValue` | Double | `1.79769e+308` | 最大值 (度)。 |
| **MaxInclusive** | `PW_MaxInclusive` | Boolean | `True` | 最大值是否包含边界。 |
| **MinimumValue** | `PW_MinimumValue` | Double | `-1.79769e+308` | 最小值 (度)。 |
| **MinInclusive** | `PW_MinInclusive` | Boolean | `True` | 最小值是否包含边界。 |
| **MinRadius** | `PW_MinRadius` | Double | `0` | 最小半径。 |
| **ShowFocusHandle** | `PW_ShowFocusHandle` | Boolean | `False` | 是否显示焦点手柄。 |
| **ShowHandle** | `PW_ShowHandle` | Boolean | `False` | 是否显示拖拽手柄。 |
| **ShowSpin** | `PW_ShowSpin` | Boolean | `False` | 是否显示旋转控制。 |
| **Value** | `PW_Value` | Double | `0` | **当前值** (度)。 |
| **WithScale** | `PW_WithScale` | Boolean | `False` | 是否带缩放滑块。 |
| **ToolTipID** | `PW_ToolTipID` | String | `""` | 工具提示 ID。**(POWER 特有属性)** |

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

> **POWER 模板参考**: `power_ui_template/JiList.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `list0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `""` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **AddNewSetLabel** | `PW_AddNewSetLabel` | String | `Add New Set` | "添加新集"按钮标签。 |
| **ColumnLabels** | `PW_ColumnLabels` | String[] | `[]` | 列标签列表。 |
| **ColumnWidths** | `PW_ColumnWidths` | String | `"-1"` | 列宽度 (-1 表示自动)。 |
| **DefaultColumnWidth** | `PW_DefaultColumnWidth` | Integer | `100` | 默认列宽度。 |
| **Layout** | `PW_Layout` | String | `Selection` | 布局模式 (Selection, Value)。 |
| **ListExpanded** | `PW_ListExpanded` | Boolean | `False` | 列表是否展开。 |
| **MarkSeedExpressionsUnusable** | `PW_MarkSeedExpressionsUnusable` | Boolean | `False` | 是否将种子表达式标记为不可用。 |
| **MaximumHeight** | `PW_MaximumHeight` | Integer | `0` | 最大高度 (0 表示无限制)。 |
| **MinimumHeight** | `PW_MinimumHeight` | Integer | `4` | 最小高度 (行数)。 |
| **MultipleEdit** | `PW_MultipleEdit` | Boolean | `False` | 是否允许多选编辑。 |
| **NumberColumnString** | `PW_NumberColumnString` | String | `""` | 序号列字符串。 |
| **NumberOfColumns** | `PW_NumberOfColumns` | Integer | `1` | 列数。 |
| **ResizeHeightWithDialog** | `PW_ResizeHeightWithDialog` | Boolean | `True` | 高度是否随对话框调整。 |
| **SeedDlxFile** | `PW_SeedDlxFile` | String | `""` | 种子 DLX 文件路径。 |
| **ShowAddNewSet** | `PW_ShowAddNewSet` | Boolean | `True` | 是否显示"添加新集"按钮。 |
| **ShowColumnHeadings** | `PW_ShowColumnHeadings` | Boolean | `False` | 是否显示列标题。 |
| **ShowRemove** | `PW_ShowRemove` | Boolean | `True` | 是否显示删除按钮。 |
| **ShowReorderControls** | `PW_ShowReorderControls` | Boolean | `False` | 是否显示重排序控制按钮。 |
| **ShowTopBottomControls** | `PW_ShowTopBottomControls` | Boolean | `False` | 是否显示置顶/置底控制按钮。 |

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

> **POWER 模板参考**: `power_ui_template/ReverseDirection.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `direction0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `Reverse Direction` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **Direction** | `PW_Direction` | String | `"0 0 1"` | 方向向量 (X Y Z)。 |
| **Flip** | `PW_Flip` | Boolean | `False` | 是否翻转方向。 |
| **Origin** | `PW_Origin` | String | `"0 0 0"` | 方向原点 (X Y Z)。 |

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
    "PW_Origin": "0 0 0"
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.11 指定平面 (SpecifyPlane - `BlockSpecifyPlane`)

对应 NX Open `BlockSpecifyPlane` 组件，用于选择或指定一个平面。

> **POWER 模板参考**: `power_ui_template/SpecifyPlane.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `plane0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `""` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **AutomaticProgression** | `PW_AutomaticProgression` | Boolean | `True` | 是否自动推进到下一步。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **CreateInterpartLink** | `PW_CreateInterpartLink` | Boolean | `False` | 是否创建零部件间链接。 |
| **InterpartSelection** | `PW_InterpartSelection` | String | `Simple` | 零部件间选择模式 (Simple, Non-associative Interpart Copy Only, Associative and Non-associative Interpart Copy, Associative Interpart Copy)。 |
| **LabelString** | `PW_LabelString` | String | `指定平面` | 标签字符串。 |
| **StepStatus** | `PW_StepStatus` | String | `Required` | 步骤状态 (Required, Optional, Satisfied)。 |

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
    "PW_StepStatus": "Required"
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.12 双精度 (Double - `BlockDouble`)

对应 NX Open `BlockDouble` 组件，用于输入双精度浮点数值。

> **POWER 模板参考**: `power_ui_template/BlockDouble.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `double0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `No Title` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Localize** | `PW_Localize` | Boolean | `True` | 是否支持本地化。 |
| **RetainValue** | `PW_RetainValue` | Boolean | `True` | 是否在对话框关闭后保留值。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **AllowUnitEdit** | `PW_AllowUnitEdit` | Boolean | `False` | 是否允许编辑单位。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **Bitmap** | `PW_Bitmap` | String | `""` | 图标路径。 |
| **Dimensionality** | `PW_Dimensionality` | String | `无单位` | 量纲/单位类型。 |
| **KeyinFieldWidth** | `PW_KeyinFieldWidth` | String | `Narrow` | 输入框宽度 (Narrow, Standard)。 |
| **LimitCheckTolerance** | `PW_LimitCheckTolerance` | Double | `-1` | 限制检查容差 (-1 表示禁用)。 |
| **MaximumValue** | `PW_MaximumValue` | Double | `1.79769e+308` | 最大值。 |
| **MaxInclusive** | `PW_MaxInclusive` | Boolean | `True` | 最大值是否包含边界。 |
| **MinimumValue** | `PW_MinimumValue` | Double | `-1.79769e+308` | 最小值。 |
| **MinInclusive** | `PW_MinInclusive` | Boolean | `True` | 最小值是否包含边界。 |
| **PresentationStyle** | `PW_PresentationStyle` | String | `Keyin` | 展示样式 (Keyin, Spin, Scale, ScaleKeyin, Combo)。 |
| **ReadOnlyValue** | `PW_ReadOnlyValue` | Boolean | `False` | 值是否只读。 |
| **RetainUnits** | `PW_RetainUnits` | Boolean | `True` | 是否保留单位设置。 |
| **ShowUnitLabel** | `PW_ShowUnitLabel` | Boolean | `False` | 是否显示单位标签。 |
| **TitleVisibility** | `PW_TitleVisibility` | Boolean | `True` | 标题是否可见。 |
| **Value** | `PW_Value` | Double | `0` | **当前值**。 |

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
    "PW_Value": 5.0
  },
  "hiddenProps": [],
  "altProperties": {},
  "children": []
}
```

### 3.13 整数 (Integer - `BlockInteger`)

对应 NX Open `BlockInteger` 组件，用于输入整数值。

> **POWER 模板参考**: `power_ui_template/BlockInteger.ui`

| UI设计器显示名称 | 导出JSON字段名称 | 类型 | 默认值 | 说明 |
| :--- | :--- | :--- | :--- | :--- |
| **BlockID** | `PW_BlockID` | String | `integer0` | 组件唯一标识符。 |
| **Enable** | `PW_Enable` | Boolean | `True` | 是否启用。 |
| **Group** | `PW_Group` | Boolean | `False` | 是否作为组显示。 |
| **Label** | `PW_Label` | String | `No Title` | 标签文本。 |
| **Show** | `PW_Show` | Boolean | `True` | 是否可见。 |
| **Localize** | `PW_Localize` | Boolean | `True` | 是否支持本地化。 |
| **RetainValue** | `PW_RetainValue` | Boolean | `True` | 是否在对话框关闭后保留值。 |
| **Bottom** | `PW_Bottom` | Attachment | `Default` | 底部附着目标。 |
| **Left** | `PW_Left` | Attachment | `Default` | 左侧附着目标。 |
| **Right** | `PW_Right` | Attachment | `Default` | 右侧附着目标。 |
| **Top** | `PW_Top` | Attachment | `Default` | 顶部附着目标。 |
| **BalloonTooltipImage** | `PW_BalloonTooltipImage` | String | `""` | 气泡提示图片路径。 |
| **BalloonTooltipLayout** | `PW_BalloonTooltipLayout` | String | `Horizontal` | 气泡提示布局 (Horizontal/Vertical)。 |
| **BalloonTooltipText** | `PW_BalloonTooltipText` | String | `""` | 气泡提示文本。 |
| **Bitmap** | `PW_Bitmap` | String | `""` | 图标路径。 |
| **KeyinFieldWidth** | `PW_KeyinFieldWidth` | String | `Narrow` | 输入框宽度 (Narrow, Standard)。 |
| **MaximumValue** | `PW_MaximumValue` | Integer | `2147483647` | 最大值。 |
| **MinimumValue** | `PW_MinimumValue` | Integer | `-2147483648` | 最小值。 |
| **PresentationStyle** | `PW_PresentationStyle` | String | `Keyin` | 展示样式 (Keyin, Spin, Scale, ScaleKeyin, Combo)。 |
| **ReadOnlyValue** | `PW_ReadOnlyValue` | Boolean | `False` | 值是否只读。 |
| **TitleVisibility** | `PW_TitleVisibility` | Boolean | `True` | 标题是否可见。 |
| **Value** | `PW_Value` | Integer | `0` | **当前值**。 |
| **ToolTipID** | `PW_ToolTipID` | String | `""` | 工具提示 ID。**(POWER 特有属性)** |

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