---
name: generate-dialog-dev
description: "[DEV] Generate CAD dialog code for CURRENT PowerCAD (no PW_ prefix). Use this during development."
---

# CAD Dialog Code Generator (DEV - Current PowerCAD)

> **This is the DEVELOPMENT version** for the current PowerCAD which has NOT fully adapted `PW_` prefix properties.
> The generated `.ui` files use **no-prefix property names** (e.g., `LabelText`, `Value`, `Data`) that work with the current PowerCAD runtime.
> When PowerCAD completes `PW_` adaptation, switch to the long-term skill: `/generate-dialog`.

You are a CAD dialog code generator. Your task is to read a JSON file exported by the UI designer and generate three output files: `.ui` (Qt XML), `.cpp` (C++ implementation), `.h` (C++ header).

## Input

Read the JSON file at path: `$ARGUMENTS`

If `$ARGUMENTS` is empty, ask the user for the JSON file path.

## JSON Input Format

The JSON follows the spec in `json_interface_spec.md`. Key structure:
- Root has `tree` array (with one `BlockDialog` root), `previewMode`, `timestamp`
- Each component has: `id`, `type`, `props`, `hiddenProps`, `altProperties`, `children`
- `props.className` on BlockDialog determines output file names
- `hiddenProps` lists properties using default values (skip these in output to save tokens)
- `altProperties` defines dynamic interaction rules (visibility/enable conditions)

**IMPORTANT**: The JSON input uses `PW_` prefixed property names (e.g., `PW_Label`, `PW_Value`). You must **convert** these to the current no-prefix convention when writing .ui files. See the Property Name Conversion Table below.

## Output

Generate three files in the **same directory** as the input JSON:
1. `{className}.ui` - Qt XML UI definition
2. `{className}.cpp` - C++ implementation
3. `{className}.h` - C++ header

Where `{className}` comes from `tree[0].props.className`.

---

## CRITICAL RULES

### Property Name Conversion Table (PW_ → Current PowerCAD)

When writing `.ui` files, convert JSON property names as follows:

| JSON Property (PW_ prefix) | .ui Property Name (no prefix) | Notes |
|---|---|---|
| `PW_Label` | `LabelText` | **Special mapping**, NOT just "Label" |
| `PW_LabelString` | `LabelText` | Same as PW_Label |
| `PW_LabelVisibility` | `LabelVisibility` | |
| `PW_Value` | `Value` | |
| `PW_Data` | `Data` | |
| `PW_Formula` | `Formula` | |
| `PW_BlockID` | **DO NOT write to .ui** | Widget `name` attribute serves as the identifier |
| `PW_Show` | `Show` | |
| `PW_Enable` | `Enable` | |
| `PW_Expanded` | `Expanded` | |
| `PW_ShowLabel` | `ShowLabel` | |
| `PW_Column` | `Column` | |
| `PW_Left` | `Left` | |
| `PW_Right` | `Right` | |
| `PW_Top` | `Top` | |
| `PW_Bottom` | `Bottom` | |
| `PW_StepStatus` | `StepStatus` | |
| `PW_Cue` | `Cue` | |
| `PW_Xxx` (all others) | `Xxx` (remove `PW_` prefix) | General rule: strip `PW_` prefix |

**STRICTLY ENFORCED**: Never write `PW_` prefixed property names in the .ui output. Never write `PW_BlockID` properties at all.

### Control Naming Convention

The `name` attribute of each widget in the .ui file must be **semantically named** based on the control's function. Use the control's `PW_Label` and `PW_BlockID` from JSON to derive a meaningful name.

**Naming format**: `{typePrefix}_{semanticName}`

| JSON type | typePrefix | Example name |
|---|---|---|
| BlockGroup | blockGroup | `blockGroup_mainSettings` |
| BlockEnumeration | enum | `enum_holeType` |
| BlockToggle | blockToggle | `blockToggle_enableAdvanced` |
| BlockButton | blockButton | `blockButton_execute` |
| BlockCurveCollector | curveCollector | `curveCollector_edgeSelect` |
| BlockSpecifyPoint | specifyPoint | `specifyPoint_origin` |
| BlockLinearExpression | blockLinear | `blockLinear_length` |
| BlockAngularExpression | blockAngular | `blockAngular_angle` |
| BlockJiList | setList | `setList_objectList` |
| BlockReverseDirection | reverseDir | `reverseDir_direction` |
| BlockSpecifyPlane | specifyPlane | `specifyPlane_basePlane` |
| BlockDouble | blockDouble | `blockDouble_radius` |
| BlockInteger | blockInteger | `blockInteger_count` |
| BlockLabel | blockLabel | `blockLabel_info` |
| BlockString | blockString | `blockString_name` |
| BlockSeparator | separator | `separator_main` |
| BlockFaceCollector | faceCollector | `faceCollector_faceSelect` |
| BlockBodyCollector | bodyCollector | `bodyCollector_bodySelect` |
| BlockSpecifyVector | specifyVector | `specifyVector_direction` |
| BlockSpecifyAxis | specifyAxis | `specifyAxis_axis` |
| BlockRadiusExpression | blockRadius | `blockRadius_radius` |
| BlockTabWidget | tabControl | `tabControl_main` |

**STRICTLY FORBIDDEN**: Never use numeric-only suffixes like `blockToggle_1`, `enum_2`. Always use meaningful English names derived from `PW_Label` or `PW_BlockID`.

### OPEN Layer vs BLOCK Layer Class Mapping (for .h and .cpp)

| C++ Type (OPEN Layer) | JSON/UI Type (BLOCK Layer) |
|---|---|
| `PWOpen::BlockStyler::AngularDimension` | BlockAngularExpression |
| `PWOpen::BlockStyler::BodyCollector` | BlockBodyCollector |
| `PWOpen::BlockStyler::Button` | BlockButton |
| `PWOpen::BlockStyler::CurveCollector` | BlockCurveCollector |
| `PWOpen::BlockStyler::DoubleBlock` | BlockDouble |
| `PWOpen::BlockStyler::Enumeration` | BlockEnumeration |
| `PWOpen::BlockStyler::FaceCollector` | BlockFaceCollector |
| `PWOpen::BlockStyler::Group` | BlockGroup |
| `PWOpen::BlockStyler::IntegerBlock` | BlockInteger |
| `PWOpen::BlockStyler::Label` | BlockLabel |
| `PWOpen::BlockStyler::LinearDimension` | BlockLinearExpression |
| `PWOpen::BlockStyler::ReverseDirection` | BlockReverseDirection |
| `PWOpen::BlockStyler::RGBColorPicker` | BlockRGBColorPicker |
| `PWOpen::BlockStyler::SelectObject` | BlockSelectObject |
| `PWOpen::BlockStyler::Separator` | BlockSeparator |
| `PWOpen::BlockStyler::SetList` | BlockSetList (BlockJiList in JSON) |
| `PWOpen::BlockStyler::SpecifyAxis` | BlockSpecifyAxis |
| `PWOpen::BlockStyler::SpecifyPlane` | BlockSpecifyPlane |
| `PWOpen::BlockStyler::SpecifyPoint` | BlockSpecifyPoint |
| `PWOpen::BlockStyler::SpecifyVector` | BlockSpecifyVector |
| `PWOpen::BlockStyler::StringBlock` | BlockString |
| `PWOpen::BlockStyler::TabControl` | BlockTabWidget |
| `PWOpen::BlockStyler::Toggle` | BlockToggle |
| `PWOpen::BlockStyler::RadiusDimension` | BlockRadiusExpression |
| `PWOpen::BlockStyler::SuperPoint` | BlockSuperPoint |
| `PWOpen::BlockStyler::SuperSection` | BlockSuperSection |
| `PWOpen::BlockStyler::ObjectColorPicker` | BlockObjectColorPicker |

**CRITICAL**: In .h/.cpp files:
- `BlockLinearExpression` maps to `LinearDimension`, NOT `LinearExpression`
- `BlockAngularExpression` maps to `AngularDimension`, NOT `AngularExpression`
- `BlockDouble` maps to `DoubleBlock`, NOT `Double`
- `BlockInteger` maps to `IntegerBlock`, NOT `Integer`
- `BlockJiList` (JSON type) maps to `SetList` (C++ OPEN type) and `BlockSetList` (.ui widget class)

### customwidgets Declaration Mapping

Each control type used in the .ui file must be declared in `<customwidgets>`. Use these exact declarations:

**Container Blocks** (extends other than QWidget):
```xml
<customwidget>
  <class>BlockDialog</class>
  <extends>QDialog</extends>
  <header>Widgets/ContainerBlock/BlockDialog.h</header>
  <addpagemethod>InitContainerWidget</addpagemethod>
  <container>1</container>
</customwidget>
<customwidget>
  <class>BlockGroup</class>
  <extends>BlockBase</extends>
  <header>Widgets/ContainerBlock/BlockGroup.h</header>
  <addpagemethod>AddContent</addpagemethod>
</customwidget>
```

**Expression Blocks** (extends BlockExpression):
```xml
<customwidget>
  <class>BlockLinearExpression</class>
  <extends>BlockExpression</extends>
  <header>Widgets/Block/BlockLinearExpression.h</header>
</customwidget>
<customwidget>
  <class>BlockAngularExpression</class>
  <extends>BlockExpression</extends>
  <header>Widgets/Block/BlockAngularExpression.h</header>
</customwidget>
<customwidget>
  <class>BlockRadiusExpression</class>
  <extends>BlockExpression</extends>
  <header>Widgets/Block/BlockRadiusExpression.h</header>
</customwidget>
```

**Standard Blocks** (extends QWidget):
```xml
<customwidget>
  <class>BlockEnumeration</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockEnumeration.h</header>
</customwidget>
<customwidget>
  <class>BlockToggle</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockToggle.h</header>
</customwidget>
<customwidget>
  <class>BlockButton</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockButton.h</header>
</customwidget>
<customwidget>
  <class>BlockDouble</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockDouble.h</header>
</customwidget>
<customwidget>
  <class>BlockInteger</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockInteger.h</header>
</customwidget>
<customwidget>
  <class>BlockCurveCollector</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockCurveCollector.h</header>
</customwidget>
<customwidget>
  <class>BlockSpecifyPoint</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockSpecifyPoint.h</header>
</customwidget>
<customwidget>
  <class>BlockSpecifyPlane</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockSpecifyPlane.h</header>
</customwidget>
<customwidget>
  <class>BlockSpecifyVector</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockSpecifyVector.h</header>
</customwidget>
<customwidget>
  <class>BlockSpecifyAxis</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockSpecifyAxis.h</header>
</customwidget>
<customwidget>
  <class>BlockSetList</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockSetList.h</header>
</customwidget>
<customwidget>
  <class>BlockReverseDirection</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockReverseDirection.h</header>
</customwidget>
<customwidget>
  <class>BlockLabel</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockLabel.h</header>
</customwidget>
<customwidget>
  <class>BlockString</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockString.h</header>
</customwidget>
<customwidget>
  <class>BlockSeparator</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockSeparator.h</header>
</customwidget>
<customwidget>
  <class>BlockFaceCollector</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockFaceCollector.h</header>
</customwidget>
<customwidget>
  <class>BlockBodyCollector</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockBodyCollector.h</header>
</customwidget>
<customwidget>
  <class>BlockSelectObject</class>
  <extends>QWidget</extends>
  <header>Widgets/Block/BlockSelectObject.h</header>
</customwidget>
```

Only include declarations for widget types actually used in the generated .ui file. Always include `BlockDialog`.

---

## .ui File Generation Rules

### Overall Structure
```xml
<?xml version="1.0" encoding="UTF-8"?>
<ui version="4.0">
  <class>{className}</class>
  <widget class="BlockDialog" name="{className}">
    <property name="geometry">
      <rect>
        <x>0</x>
        <y>0</y>
        <width>{_w or 500}</width>
        <height>{_h or 400}</height>
      </rect>
    </property>
    <!-- Dialog-level properties here (using NO-PREFIX names) -->
    <widget class="QWidget" name="page">
      <property name="geometry">
        <rect>
          <x>19</x>
          <y>39</y>
          <width>711</width>
          <height>401</height>
        </rect>
      </property>
      <layout class="QVBoxLayout" name="verticalLayout">
        <!-- All top-level children go here as <item> elements -->
        <item>
          <spacer name="verticalSpacer">
            <property name="orientation">
              <enum>Qt::Vertical</enum>
            </property>
            <property name="sizeHint" stdset="0">
              <size>
                <width>20</width>
                <height>40</height>
              </size>
            </property>
          </spacer>
        </item>
      </layout>
    </widget>
  </widget>
  <customwidgets>
    <!-- Widget declarations here -->
  </customwidgets>
  <resources/>
  <connections/>
</ui>
```

### BlockGroup Nesting Structure

BlockGroup has a special inner structure - its children go inside a `groupContents` QWidget with its own layout:

```xml
<widget class="BlockGroup" name="blockGroup_settings">
  <!-- BlockGroup properties here (NO PW_ prefix) -->
  <property name="LabelText" stdset="0">
    <string>Settings</string>
  </property>
  <widget class="QWidget" name="groupContents">
    <layout class="QVBoxLayout" name="groupVBoxLayout">
      <item>
        <!-- Child widget 1 -->
      </item>
      <item>
        <!-- Child widget 2 -->
      </item>
    </layout>
  </widget>
</widget>
```

For nested BlockGroups (group inside group), use incrementing names: `groupContents`, `groupContents_1`, `groupContents_2`, etc. And layout names: `groupVBoxLayout`, `groupVBoxLayout_1`, etc.

### Property Value Type Mapping in XML

When writing `<property>` tags, use the correct XML type wrapper based on the JSON value type:

| JSON Value Type | XML Wrapper | Example |
|---|---|---|
| String | `<string>` | `<string>Default</string>` |
| Boolean | `<bool>` | `<bool>true</bool>` |
| Integer | `<number>` | `<number>65535</number>` |
| Double/Float | `<double>` | `<double>5.0</double>` |
| String Array | `<stringlist>` | see below |

String array example (note: property name uses NO prefix):
```xml
<property name="Value" stdset="0">
  <stringlist>
    <string>Option A</string>
    <string>Option B</string>
  </stringlist>
</property>
```

**All custom properties (non-Qt-standard) must have `stdset="0"` attribute.**

### Property Filtering (hiddenProps)

If a property name appears in the component's `hiddenProps` array, **skip it** - do not write it to the .ui file. The backend will use default values for these.

However, certain properties should ALWAYS be written regardless of hiddenProps:
- `Value` (mapped from `PW_Value`, if non-default)
- `LabelText` (mapped from `PW_Label`, if non-default)
- `Data` (mapped from `PW_Data`, if non-empty, for enumerations)

**Remember**: `PW_BlockID` is NEVER written to .ui. The widget `name` attribute is the identifier.

### Empty String Properties

For string properties that are empty (`""`) in JSON, write them as a single space in XML: `<string> </string>`. This matches the PowerCAD convention observed in the template files.

---

## .h File Generation Rules

### Template
```cpp
#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class {CLASS_NAME} {
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    {CLASS_NAME}();
    ~{CLASS_NAME}();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    // Control member variables
    {MEMBER_DECLARATIONS}
};
```

### Member Declaration Rules

For each control (NOT BlockDialog itself), declare a member variable:
```cpp
PWOpen::BlockStyler::{OPEN_TYPE}* {widgetName};
```

Where `{OPEN_TYPE}` is the OPEN layer type from the mapping table, and `{widgetName}` is the semantic name used in the .ui file.

Example:
```cpp
PWOpen::BlockStyler::Group* blockGroup_mainSettings;
PWOpen::BlockStyler::Enumeration* enum_visibilityControl;
PWOpen::BlockStyler::DoubleBlock* blockDouble_radius;
PWOpen::BlockStyler::LinearDimension* blockLinear_length;
```

---

## .cpp File Generation Rules

### Template

**IMPORTANT**: `<PWOpen/IncludeHeader.h>` does NOT include the SetList header. If the dialog uses `BlockJiList`/`SetList`, you MUST manually add `#include <PWOpen/BlockStyler_SetList.hxx>` after `IncludeHeader.h`.

```cpp
#include "{CLASS_NAME}.h"
#include <PWOpen/IncludeHeader.h>
// If SetList is used, add: #include <PWOpen/BlockStyler_SetList.hxx>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *({CLASS_NAME}::theSession) = nullptr;
PWOpen::UI *({CLASS_NAME}::theUI) = nullptr;

{CLASS_NAME}::{CLASS_NAME}()
{
    try
    {
        {CLASS_NAME}::theSession = PWOpen::Session::GetSession();
        {CLASS_NAME}::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("{CLASS_NAME}.ui");

        theDialog->AddApplyHandler([this] { return apply_cb(); });
        theDialog->AddOkHandler([this] { return ok_cb(); });
        theDialog->AddUpdateHandler([this](auto && PH1) { return update_cb(std::forward<decltype(PH1)>(PH1)); });
        theDialog->AddInitializeHandler([this] { initialize_cb(); });
        theDialog->AddDialogShownHandler([this] { dialogShown_cb(); });
    }
    catch (std::exception& ex)
    {
        return;
    }
}
{CLASS_NAME}::~{CLASS_NAME}()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse {CLASS_NAME}::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void {CLASS_NAME}::initialize_cb()
{
    try
    {
        {INITIALIZE_CODE}
    }
    catch(std::exception& ex)
    {
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void {CLASS_NAME}::dialogShown_cb()
{
    try
    {
        {DIALOG_SHOWN_CODE}
    }
    catch(std::exception& ex)
    {
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int {CLASS_NAME}::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int {CLASS_NAME}::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int {CLASS_NAME}::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        {UPDATE_CB_CODE}
    }
    catch(std::exception& ex)
    {
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    {CLASS_NAME} *dialog = NULL;
    try
    {
        dialog = new {CLASS_NAME}();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
```

### initialize_cb() Code Generation

For each control, generate a `FindBlock` + `dynamic_cast` line:
```cpp
{widgetName} = dynamic_cast<PWOpen::BlockStyler::{OPEN_TYPE}*>(theDialog->TopBlock()->FindBlock("{widgetName}"));
```

### update_cb() Code Generation - altProperties Logic

The `altProperties` field on each component defines dynamic behavior. The JSON uses `PW_Show` as the key, but in C++ code, use `"Show"` (no prefix) for GetProperties/SetLogical calls.

#### Rule: Enum Trigger - `"PW_Show": "enumBlockID(val1, val2)"`
Means: show this control when the enum with PW_BlockID=enumBlockID has a selected value matching val1 or val2.

**CRITICAL - CurrentData returns Data values, NOT display text**:
`GetString("CurrentData")` returns the value from the enum's `PW_Data` array (e.g., `"1"`, `"2"`), NOT the display text from `PW_Value` (e.g., `"对称"`, `"非对称"`). The altProperties expressions use display text names for readability, so you **must look up the corresponding PW_Data value** for each display name and compare against that.

Example: If an enum has `PW_Value: ["对称", "非对称", "偏置和角度"]` and `PW_Data: [1, 2, 3]`, and altProperties says `"PW_Show": "enum(对称, 偏置和角度)"`, the generated code must compare against Data values `"1"` and `"3"`:

```cpp
if (block == {enumWidgetName})
{
    PWOpen::PWString enumValue = {enumWidgetName}->GetProperties()->GetString("CurrentData");

    // altProperties PW_Show: "enum(对称, 偏置和角度)" → Data: 1=对称, 2=非对称, 3=偏置和角度
    {targetWidgetName}->GetProperties()->SetLogical("Show",
        enumValue == "1" || enumValue == "3");
}
```

Mapping steps:
1. Parse the display names from altProperties: `enum(对称, 偏置和角度)` → `["对称", "偏置和角度"]`
2. Find the triggering enum's `PW_Value` and `PW_Data` arrays
3. For each display name, find its index in `PW_Value`, then get the corresponding `PW_Data` value at that index
4. Use the `PW_Data` values (converted to strings) in the C++ comparison

#### Rule: Toggle Trigger - `"PW_Show": "toggleBlockID == true"`
Means: show this control when the toggle with PW_BlockID=toggleBlockID is true.

Generated code pattern:
```cpp
if (block == {toggleWidgetName})
{
    bool toggleValue = {toggleWidgetName}->GetProperties()->GetLogical("Value");

    {targetWidgetName}->GetProperties()->SetLogical("Show", toggleValue);
}
```

#### Rule: Combined Conditions with && and ||
Parse the logical operators and generate appropriate combined conditions.

Example: `"PW_Show": "toggle1 == true && enum0(1)"` (where enum0 has PW_Value=["A","B"], PW_Data=[1,2], so display name "1" doesn't apply here — the `(1)` in altProperties refers to a display name; look it up in PW_Value to find the Data value)
```cpp
if (block == {toggle1WidgetName} || block == {enum0WidgetName})
{
    bool toggle1Val = {toggle1WidgetName}->GetProperties()->GetLogical("Value");
    PWOpen::PWString enum0Val = {enum0WidgetName}->GetProperties()->GetString("CurrentData");

    // Remember: compare enum0Val against PW_Data values, not display text
    {targetWidgetName}->GetProperties()->SetLogical("Show",
        toggle1Val && (enum0Val == "{corresponding_data_value}"));
}
```

#### Important: Group all targets controlled by the same trigger

If multiple controls are affected by the same trigger (e.g., multiple controls have altProperties referencing the same enum), group them under one `if (block == trigger)` block.

After the trigger blocks, add empty `else if` blocks for all other controls:
```cpp
else if (block == {otherWidgetName})
{
}
```

**IMPORTANT**: In C++ GetProperties() calls, property names are ALWAYS without prefix:
- `"Show"` (not `"PW_Show"`)
- `"Value"` (not `"PW_Value"`)
- `"CurrentData"` (not `"PW_CurrentData"`)
- `"Enable"` (not `"PW_Enable"`)

### dialogShown_cb() Code Generation

If there are altProperties rules involving enums/toggles, **MUST** add initialization code to set the correct initial visibility state. This code must be **uncommented and active** (NOT commented out), otherwise controls that should be hidden on dialog open will all be visible.

For each enum that triggers altProperties, call `update_cb` to apply the initial visibility:
```cpp
// Enum initialization - trigger update_cb to set initial UI state
update_cb({enumWidgetName});
```

For each toggle that triggers altProperties:
```cpp
// Toggle initialization - trigger update_cb to set initial UI state
update_cb({toggleWidgetName});
```

---

## Step-by-Step Execution

1. **Read** the input JSON file
2. **Parse** the JSON tree structure, maintaining parent-child relationships
3. **Collect** all components recursively, noting:
   - Component type and props
   - hiddenProps for each component
   - altProperties for dynamic rules
   - Parent-child nesting (especially BlockGroup nesting)
4. **Generate semantic names** for each widget based on PW_Label and PW_BlockID
5. **Generate .ui file**: Walk the tree and output XML with correct nesting, **converting all PW_ property names to no-prefix equivalents** per the conversion table
6. **Generate .h file**: Declare all control member variables with correct OPEN layer types
7. **Generate .cpp file**: Generate initialize_cb, update_cb with altProperties logic, and all other callbacks
8. **Write** all three files to disk in the same directory as the input JSON

## Reference Files

For verified working output examples (using no-prefix convention), see `.claude/reference/working-example/BlockFeature/`. For property templates (note: these use PW_ prefix - you must convert), consult `.ui` files in `power_ui_template/`.
