---
name: generate-dialog
description: Read a JSON file from the UI designer and generate CAD dialog code (.ui, .cpp, .h files)
---
# CAD Dialog Code Generator

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

## Output

Generate three files in the **same directory** as the input JSON:

1. `{className}.ui` - Qt XML UI definition
2. `{className}.cpp` - C++ implementation
3. `{className}.h` - C++ header

Where `{className}` comes from `tree[0].props.className`.

---

## CRITICAL RULES

### PW_BlockID MUST ALWAYS Be Written (HIGHEST PRIORITY)

**NEVER skip `PW_BlockID` even if it appears in `hiddenProps`.** This is the #1 most common generation error. Every single control (except BlockDialog itself) MUST have its `PW_BlockID` property written to the .ui file. The `hiddenProps` filter does NOT apply to `PW_BlockID`.

```xml
<!-- CORRECT: PW_BlockID is always written -->
<widget class="BlockDouble" name="blockDouble_coeffA">
  <property name="PW_BlockID" stdset="0">
    <string>double0</string>
  </property>
  ...
</widget>

<!-- WRONG: PW_BlockID omitted because it was in hiddenProps -->
<widget class="BlockDouble" name="blockDouble_coeffA">
  <property name="PW_Label" stdset="0">
    <string>a</string>
  </property>
</widget>
```

### Control Naming Convention

The `name` attribute of each widget in the .ui file must be **semantically named** based on the control's function. Use the control's `PW_Label` and `PW_BlockID` to derive a meaningful name.

**Naming format**: `{typePrefix}_{semanticName}`

**CRITICAL RULES**:
1. **NO CHINESE CHARACTERS**: Variable names MUST NOT contain Chinese characters. Use English translations or descriptive English names.
2. **NO DUPLICATE NAMES**: All variable names MUST be unique within the class. If multiple controls have the same semantic name, append a numeric suffix (e.g., `_1`, `_2`) or use more specific descriptive names.

**Naming Strategy for Chinese Labels**:
- Translate Chinese labels to English (e.g., "原点" → "origin", "设置" → "settings")
- Use descriptive English names based on functionality
- For ambiguous terms, use context-specific names (e.g., "设置1" → "settings_origin", "设置2" → "settings_offset")
- Always prefer meaningful English names over direct translations

| JSON type                | typePrefix           | Example name                         |
| ------------------------ | -------------------- | ------------------------------------ |
| BlockGroup               | blockGroup           | `blockGroup_mainSettings`          |
| BlockEnumeration         | enum                 | `enum_holeType`                    |
| BlockToggle              | blockToggle          | `blockToggle_enableAdvanced`       |
| BlockButton              | blockButton          | `blockButton_execute`              |
| BlockDouble              | blockDouble          | `blockDouble_radius`               |
| BlockInteger             | blockInteger         | `blockInteger_count`               |
| BlockLabel               | blockLabel           | `blockLabel_info`                  |
| BlockString              | blockString          | `blockString_name`                 |
| BlockMultilineString     | multilineString      | `multilineString_description`      |
| BlockSeparator           | separator            | `separator_main`                   |
| BlockListBox             | listBox              | `listBox_items`                    |
| BlockExpression          | blockExpression      | `blockExpression_value`            |
| BlockLinearExpression    | blockLinear          | `blockLinear_length`               |
| BlockAngularExpression   | blockAngular         | `blockAngular_angle`               |
| BlockRadiusExpression    | blockRadius          | `blockRadius_radius`               |
| BlockOnPathDim           | onPathDim            | `onPathDim_distance`               |
| BlockIntegerTable        | integerTable         | `integerTable_data`                |
| BlockDoubleTable         | doubleTable          | `doubleTable_data`                 |
| BlockTable               | blockTable           | `blockTable_data`                  |
| BlockTabWidget           | tabControl           | `tabControl_main`                  |
| BlockWizard              | wizard               | `wizard_main`                      |
| BlockScrollWindow        | scrollWindow         | `scrollWindow_main`                |
| BlockExplorer            | explorer             | `explorer_main`                    |
| BlockCurveCollector      | curveCollector       | `curveCollector_edgeSelect`        |
| BlockFaceCollector       | faceCollector        | `faceCollector_faceSelect`         |
| BlockBodyCollector       | bodyCollector        | `bodyCollector_bodySelect`         |
| BlockSelectObject        | selectObject         | `selectObject_selection`           |
| BlockSelectFeature       | selectFeature        | `selectFeature_feature`            |
| BlockSelectPart          | selectPart           | `selectPart_part`                  |
| BlockSelectNodes         | selectNodes          | `selectNodes_nodes`                |
| BlockSelectElements      | selectElements       | `selectElements_elements`          |
| BlockFacetSelect         | facetSelect          | `facetSelect_facet`                |
| BlockSpecifyPoint        | specifyPoint         | `specifyPoint_origin`              |
| BlockSuperPoint          | superPoint           | `superPoint_point`                 |
| BlockSpecifyVector       | specifyVector        | `specifyVector_direction`          |
| BlockSpecifyAxis         | specifyAxis          | `specifyAxis_axis`                 |
| BlockSpecifyPlane        | specifyPlane         | `specifyPlane_basePlane`           |
| BlockSpecifyCsys         | specifyCsys          | `specifyCsys_csys`                 |
| BlockReverseDirection    | reverseDir           | `reverseDir_direction`             |
| BlockSetList             | setList              | `setList_objectList`               |
| BlockOrientXpress        | orientXpress         | `orientXpress_orient`              |
| BlockManipulator         | manipulator          | `manipulator_manip`                |
| BlockCursorLocation      | cursorLocation       | `cursorLocation_cursor`            |
| BlockMicroposition       | microposition        | `microposition_position`           |
| BlockSectionBuilder      | sectionBuilder       | `sectionBuilder_section`           |
| BlockSuperSection        | superSection         | `superSection_section`             |
| BlockObjectColorPicker   | colorPicker          | `colorPicker_objectColor`          |
| BlockRGBColorPicker      | rgbColorPicker       | `rgbColorPicker_color`             |
| BlockLayer               | blockLayer           | `blockLayer_layer`                 |
| BlockLineFont            | lineFont             | `lineFont_style`                   |
| BlockLineWidth           | lineWidth            | `lineWidth_width`                  |
| BlockLineColorFontWidth  | lineColorFontWidth   | `lineColorFontWidth_line`          |
| BlockTextColorFontWidth  | textColorFontWidth   | `textColorFontWidth_text`          |
| BlockDrawingArea         | drawingArea          | `drawingArea_preview`              |
| BlockFileSelection       | fileSelection        | `fileSelection_file`               |
| BlockFolderSelection     | folderSelection      | `folderSelection_folder`           |
| BlockSelectExpression    | selectExpression     | `selectExpression_expr`            |
| BlockTree                | blockTree            | `blockTree_tree`                   |

**STRICTLY FORBIDDEN**: Never use numeric-only suffixes like `blockToggle_1`, `enum_2`. Always use meaningful English names derived from `PW_Label` or `PW_BlockID`.

### OPEN Layer vs BLOCK Layer Class Mapping (for .h and .cpp)

| C++ Type (OPEN Layer)                          | JSON/UI Type (BLOCK Layer) |
| ---------------------------------------------- | -------------------------- |
| `PWOpen::BlockStyler::AngularDimension`      | BlockAngularExpression     |
| `PWOpen::BlockStyler::BodyCollector`         | BlockBodyCollector         |
| `PWOpen::BlockStyler::Button`                | BlockButton                |
| `PWOpen::BlockStyler::ChooseExpression`      | BlockSelectExpression      |
| `PWOpen::BlockStyler::CurveCollector`        | BlockCurveCollector        |
| `PWOpen::BlockStyler::DoubleBlock`           | BlockDouble                |
| `PWOpen::BlockStyler::DoubleTable`           | BlockDoubleTable           |
| `PWOpen::BlockStyler::DrawingArea`           | BlockDrawingArea           |
| `PWOpen::BlockStyler::Enumeration`           | BlockEnumeration           |
| `PWOpen::BlockStyler::Explorer`              | BlockExplorer              |
| `PWOpen::BlockStyler::ExpressionBlock`       | BlockExpression            |
| `PWOpen::BlockStyler::FaceCollector`         | BlockFaceCollector         |
| `PWOpen::BlockStyler::FileSelection`         | BlockFileSelection         |
| `PWOpen::BlockStyler::FolderSelection`       | BlockFolderSelection       |
| `PWOpen::BlockStyler::Group`                 | BlockGroup                 |
| `PWOpen::BlockStyler::IntegerBlock`          | BlockInteger               |
| `PWOpen::BlockStyler::IntegerTable`          | BlockIntegerTable          |
| `PWOpen::BlockStyler::Label`                 | BlockLabel                 |
| `PWOpen::BlockStyler::LayerBlock`            | BlockLayer                 |
| `PWOpen::BlockStyler::LinearDimension`       | BlockLinearExpression      |
| `PWOpen::BlockStyler::LineColorFontWidth`    | BlockLineColorFontWidth    |
| `PWOpen::BlockStyler::LineFont`              | BlockLineFont              |
| `PWOpen::BlockStyler::LineWidth`             | BlockLineWidth             |
| `PWOpen::BlockStyler::ListBox`               | BlockListBox               |
| `PWOpen::BlockStyler::Microposition`         | BlockMicroposition         |
| `PWOpen::BlockStyler::MultilineString`       | BlockMultilineString       |
| `PWOpen::BlockStyler::ObjectColorPicker`     | BlockObjectColorPicker     |
| `PWOpen::BlockStyler::OnPathDimension`       | BlockOnPathDim             |
| `PWOpen::BlockStyler::OrientXpress`          | BlockOrientXpress          |
| `PWOpen::BlockStyler::RadiusDimension`       | BlockRadiusExpression      |
| `PWOpen::BlockStyler::ReverseDirection`      | BlockReverseDirection      |
| `PWOpen::BlockStyler::RGBColorPicker`        | BlockRGBColorPicker        |
| `PWOpen::BlockStyler::ScrolledWindow`        | BlockScrollWindow          |
| `PWOpen::BlockStyler::SectionBuilder`        | BlockSectionBuilder        |
| `PWOpen::BlockStyler::SelectElement`         | BlockSelectElements        |
| `PWOpen::BlockStyler::SelectFacetRegion`     | BlockFacetSelect           |
| `PWOpen::BlockStyler::SelectFeature`         | BlockSelectFeature         |
| `PWOpen::BlockStyler::SelectNode`            | BlockSelectNodes           |
| `PWOpen::BlockStyler::SelectObject`          | BlockSelectObject          |
| `PWOpen::BlockStyler::SelectPartFromList`    | BlockSelectPart            |
| `PWOpen::BlockStyler::Separator`             | BlockSeparator             |
| `PWOpen::BlockStyler::SetList`               | BlockSetList               |
| `PWOpen::BlockStyler::SpecifyAxis`           | BlockSpecifyAxis           |
| `PWOpen::BlockStyler::SpecifyCSYS`           | BlockSpecifyCsys           |
| `PWOpen::BlockStyler::SpecifyLocation`       | BlockCursorLocation        |
| `PWOpen::BlockStyler::SpecifyOrientation`    | BlockManipulator           |
| `PWOpen::BlockStyler::SpecifyPlane`          | BlockSpecifyPlane          |
| `PWOpen::BlockStyler::SpecifyPoint`          | BlockSpecifyPoint          |
| `PWOpen::BlockStyler::SpecifyVector`         | BlockSpecifyVector         |
| `PWOpen::BlockStyler::StringBlock`           | BlockString                |
| `PWOpen::BlockStyler::SuperPoint`            | BlockSuperPoint            |
| `PWOpen::BlockStyler::SuperSection`          | BlockSuperSection          |
| `PWOpen::BlockStyler::TabControl`            | BlockTabWidget             |
| `PWOpen::BlockStyler::Table`                 | BlockTable                 |
| `PWOpen::BlockStyler::TextColorFontWidth`    | BlockTextColorFontWidth    |
| `PWOpen::BlockStyler::Toggle`                | BlockToggle                |
| `PWOpen::BlockStyler::Tree`                  | BlockTree                  |
| `PWOpen::BlockStyler::Wizard`                | BlockWizard                |

**CRITICAL**: In .h/.cpp files:

- `BlockLinearExpression` maps to `LinearDimension`, NOT `LinearExpression`
- `BlockAngularExpression` maps to `AngularDimension`, NOT `AngularExpression`
- `BlockDouble` maps to `DoubleBlock`, NOT `Double`
- `BlockInteger` maps to `IntegerBlock`, NOT `Integer`

### customwidgets Declaration Mapping

Each control type used in the .ui file must be declared in `<customwidgets>`. Use these exact declarations:

**Container Blocks** — All container blocks MUST have both `<container>1</container>` and `<addpagemethod>` tags:

```xml
<customwidget>
  <class>BlockDialog</class>
  <extends>QDialog</extends>
  <header>Widgets/ContainerBlock/BlockDialog.h</header>
  <addpagemethod>InitContainerWidget</addpagemethod>
  <container>1</container>
</customwidget>
<customwidget>
  <class>BlockFeatureDialog</class>
  <extends>QDialog</extends>
  <header>Widgets/ContainerBlock/BlockFeatureDialog.h</header>
  <addpagemethod>InitContainerWidget</addpagemethod>
  <container>1</container>
</customwidget>
<customwidget>
  <class>BlockGroup</class>
  <extends>BlockBase</extends>
  <header>Widgets/ContainerBlock/BlockGroup.h</header>
  <addpagemethod>AddContent</addpagemethod>
  <container>1</container>
</customwidget>
<customwidget>
  <class>BlockScrollWindow</class>
  <extends>QWidget</extends>
  <header>Widgets/ContainerBlock/BlockScrollWindow.h</header>
  <addpagemethod>AddContent</addpagemethod>
  <container>1</container>
</customwidget>
<customwidget>
  <class>BlockTabWidget</class>
  <extends>QWidget</extends>
  <header>Widgets/ContainerBlock/BlockTabWidget.h</header>
  <addpagemethod>AddTab</addpagemethod>
  <container>1</container>
</customwidget>
<customwidget>
  <class>BlockWizard</class>
  <extends>QWidget</extends>
  <header>Widgets/ContainerBlock/BlockWizard.h</header>
  <addpagemethod>addPage</addpagemethod>
  <container>1</container>
</customwidget>
<customwidget>
  <class>BlockExplorer</class>
  <extends>QWidget</extends>
  <header>Widgets/ContainerBlock/BlockExplorer.h</header>
  <addpagemethod>AddContent</addpagemethod>
  <container>1</container>
</customwidget>
```

**CRITICAL**: Every container block declaration MUST include both `<container>1</container>` AND `<addpagemethod>`. Missing either tag will cause the Qt Designer plugin to fail loading child widgets.

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
    <!-- BlockDialog PW_ properties here -->
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
  <!-- BlockGroup PW_ properties here -->
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

| JSON Value Type | XML Wrapper      | Example                      |
| --------------- | ---------------- | ---------------------------- |
| String          | `<string>`     | `<string>Default</string>` |
| Boolean         | `<bool>`       | `<bool>true</bool>`        |
| Integer         | `<number>`     | `<number>65535</number>`   |
| Double/Float    | `<double>`     | `<double>5.0</double>`     |
| String Array    | `<stringlist>` | see below                    |

String array example:

```xml
<property name="PW_Value" stdset="0">
  <stringlist>
    <string>Option A</string>
    <string>Option B</string>
  </stringlist>
</property>
```

**Enum stringlist properties — MUST always output as `<stringlist>` (CRITICAL)**:

The following BlockEnumeration properties MUST ALWAYS be output as `<stringlist>` with individual `<string>` entries, **NEVER** as a single comma-separated `<string>`:
- **`PW_Value`**
- **`PW_Data`**
- **`PW_Bitmaps`**
- **`PW_EnumSensitivity`**
- **`PW_EnumVisibility`**

JSON input may use **either** of two formats:

| Format | JSON Example | How to process |
|--------|-------------|----------------|
| **JSON Array** | `"PW_Value": ["A", "B"]` | Use array items directly as `<stringlist>` entries |
| **Comma-separated String** | `"PW_Data": "aa,bb,cc"` | Split by comma first, then write each as `<stringlist>` entry |

**Detection rule**: If the JSON value is an array (`[]`), use items directly. If it is a string, split by comma.

Both formats produce the same `.ui` output — always `<stringlist>`:

```xml
<property name="PW_Value" stdset="0">
  <stringlist>
    <string>A</string>
    <string>B</string>
  </stringlist>
</property>
<property name="PW_Data" stdset="0">
  <stringlist>
    <string>aa</string>
    <string>bb</string>
    <string>cc</string>
  </stringlist>
</property>
```

**WRONG** (do NOT generate this):
```xml
<!-- WRONG: single comma-separated string -->
<property name="PW_Value" stdset="0"><string>A,B</string></property>
<property name="PW_Data" stdset="0"><string>aa,bb,cc</string></property>
```

`PW_EnumSensitivity` and `PW_EnumVisibility` may appear as JSON arrays of integers (e.g., `[1, 1, 1]`) — convert each integer to a string in the `<stringlist>`.

**All PW_ properties must have `stdset="0"` attribute.**

### Property Filtering (hiddenProps)

If a property name appears in the component's `hiddenProps` array, **skip it** - do not write it to the .ui file. The backend will use default values for these.

**EXCEPTION — These properties MUST ALWAYS be written, even if they appear in hiddenProps:**

- **`PW_BlockID`** — **MANDATORY for every control** (except BlockDialog). Without it, `FindBlock()` in C++ will fail at runtime. This is the most critical property. **NEVER skip PW_BlockID.**
- `PW_Value` (if non-default, always needed)
- `PW_Label` (if non-default)
- `PW_Data` (if non-empty, for enumerations)

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

**IMPORTANT**: `<PWOpen/IncludeHeader.h>` does NOT include the SetList header. If the dialog uses `BlockSetList` (JSON type `BlockJiList`), you MUST manually add `#include <PWOpen/BlockStyler_SetList.hxx>` after `IncludeHeader.h`.

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

### initialize_cb() Code Generation — FindBlock Naming Rule

**CRITICAL — Three-Way Naming Convention**:
- `.ui` widget `name` attribute → semantic name (e.g., `specifyPoint_origin`)
- `.h` member variable name → same semantic name (e.g., `specifyPoint_origin`)
- `.cpp` `FindBlock()` parameter → **`PW_BlockID` value from JSON** (e.g., `"point0"`), NOT the widget name

This means the FindBlock string does NOT match the variable name. The variable is semantic, but FindBlock uses the raw `PW_BlockID`.

For each control, generate a `FindBlock` + `dynamic_cast` line:

```cpp
{widgetName} = dynamic_cast<PWOpen::BlockStyler::{OPEN_TYPE}*>(theDialog->TopBlock()->FindBlock("{PW_BlockID}"));
```

Example (JSON has `PW_BlockID: "point0"`, widget named `specifyPoint_origin`):

```cpp
specifyPoint_origin = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
```

### update_cb() Code Generation - altProperties Logic

**CRITICAL — String type rule**: In `.cpp` code, **NEVER use `std::string`** for values returned by `GetString()`. Always use `PWOpen::PWString`. Example:

```cpp
// CORRECT:
PWOpen::PWString enumValue = widget->GetProperties()->GetString("CurrentData");

// WRONG — do NOT use std::string:
std::string enumValue = widget->GetProperties()->GetString("CurrentData");
```

The `altProperties` field on each component defines dynamic behavior. Parse these rules and generate `if/else` logic.

#### Rule: Enum Trigger - `"PW_Show": "enumBlockID(val1, val2)"`

Means: show this control when the enum with `PW_BlockID=enumBlockID` has `CurrentData` matching `val1` or `val2`.

**CRITICAL - CurrentData and altProperties both use `PW_Data` values**:
`GetString("CurrentData")` returns the currently selected entry from enum `PW_Data`.
Based on the sample JSON in `workspace/17*/block_ui_project (46).json`, enum triggers in `PW_Show` also use `PW_Data` tokens directly.

Example (old comma-separated string format):

- `PW_Value: "aa,221,as"` or `PW_Value: ["aa","221","as"]`
- `PW_Data: "a,b,c"` or `PW_Data: ["a","b","c"]`
- `PW_Show: "enum0(a, c)"`

Generated comparison:

```cpp
if (block == {enumWidgetName})
{
    PWOpen::PWString enumValue = {enumWidgetName}->GetProperties()->GetString("CurrentData");

    // altProperties PW_Show: "enum0(a, c)"
    {targetWidgetName}->GetProperties()->SetLogical("Show",
        enumValue == "a" || enumValue == "c");
}
```

Mapping steps:

1. Parse values inside enum trigger: `enum0(a, c)` -> `["a", "c"]`
2. Read enum `PW_Data` — if it's a JSON array, use directly; if it's a comma-separated string, split by comma first
3. Treat parsed trigger values as `PW_Data` tokens and validate they exist in `PW_Data`
4. Compare `CurrentData` directly against those tokens

Enum implementation规范 (MUST follow):

1. In `update_cb`, update enum-driven visibility inside `if (block == {enumWidgetName})` (or a grouped trigger block containing that enum). Do not apply enum visibility unconditionally before trigger checks.
2. Group all targets controlled by the same enum into the same enum trigger block and set all related `Show` states together.
3. In `dialogShown_cb`, call `update_cb({enumWidgetName})` for each trigger enum so initial visibility is correct when dialog opens.

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

Example: `"PW_Show": "toggle1 == true && enum0(a)"` (where enum0 has `PW_Data: "a,b,c"`)

```cpp
if (block == {toggle1WidgetName} || block == {enum0WidgetName})
{
    bool toggle1Val = {toggle1WidgetName}->GetProperties()->GetLogical("Value");
    PWOpen::PWString enum0Val = {enum0WidgetName}->GetProperties()->GetString("CurrentData");

    // Compare against PW_Data values
    {targetWidgetName}->GetProperties()->SetLogical("Show",
        toggle1Val && (enum0Val == "a"));
}
```

#### Rule: PW_Enable - Enable/Disable Control

`altProperties` can also use `PW_Enable` (same syntax as `PW_Show`). Generated code uses `"Enable"` instead of `"Show"`:

```cpp
if (block == {enumWidgetName})
{
    PWOpen::PWString enumValue = {enumWidgetName}->GetProperties()->GetString("CurrentData");
    {targetWidgetName}->GetProperties()->SetLogical("Enable",
        enumValue == "456");
}
```

#### Rule: PW_ReadOnlyValue - Read-Only State Control

For `"PW_ReadOnlyValue"` in altProperties, use `SetLogical("ReadOnlyValue", ...)`:

```cpp
if (block == {toggleWidgetName})
{
    bool toggleValue = {toggleWidgetName}->GetProperties()->GetLogical("Value");
    {targetWidgetName}->GetProperties()->SetLogical("ReadOnlyValue", toggleValue);
}
```

#### Rule: Dialog-Level Conditions — `IsMore()` and `IsSatisfied()`

The dialog object provides two important state checks:

- `theDialog->IsMore()` — returns true when the dialog's "More" section is expanded
- `theDialog->IsSatisfied()` — returns true when all required inputs are satisfied

These can be combined with enum/toggle conditions:

```cpp
if (block == {enumWidgetName})
{
    PWOpen::PWString enumValue = {enumWidgetName}->GetProperties()->GetString("CurrentData");
    // Combined with IsMore() — show only when enum matches AND More is expanded
    {targetWidgetName}->GetProperties()->SetLogical("Show",
        enumValue == "123" && theDialog->IsMore());
}
```

#### Rule: More Button Block Handling

When altProperties involve `IsMore()`, the `update_cb` must also handle the "More" button block. The "More" block is found via `theDialog->TopBlock()->FindBlock("More")`:

```cpp
else if (block == theDialog->TopBlock()->FindBlock("More"))
{
    // Re-apply the same IsMore()-dependent logic
    {targetWidgetName}->GetProperties()->SetLogical("Show",
        {enumWidgetName}->GetProperties()->GetString("CurrentData") == "123" && theDialog->IsMore());
}
```

#### Rule: SelectObject StepStatus Check

For SelectObject/collector controls, use `StepStatusAsString()` to check selection state. Possible values: `"Optional"`, `"Required"`, `"Satisfied"`:

```cpp
else if (block == {selectObjectWidgetName})
{
    auto step_status = {selectObjectWidgetName}->StepStatusAsString();
    if (step_status != "Optional")
    {
        {targetWidgetName}->GetProperties()->SetLogical("Show", step_status == "Satisfied");
    }
}
```

#### Rule: String RequiredInput Check

For string controls with `RequiredInput`, check whether the string has a value:

```cpp
else if (block == {stringWidgetName})
{
    if ({stringWidgetName}->GetProperties()->GetLogical("RequiredInput"))
    {
        auto value = {stringWidgetName}->GetProperties()->GetString("Value");
        {targetWidgetName}->GetProperties()->SetLogical("Enable", !value.IsEmpty());
    }
}
```

#### Rule: Unconditional Post-Trigger Statements

After all `if/else if` trigger blocks, you may add unconditional statements that always execute regardless of which block triggered the update. These are placed **after** the last `else if` block but still inside the `try` block:

```cpp
    // ... all if/else if trigger blocks above ...

    // Unconditional: always update based on dialog state
    {widgetName}->GetProperties()->SetLogical("Enable", theDialog->IsSatisfied());
```

#### Important: Group all targets controlled by the same trigger

If multiple controls are affected by the same trigger (e.g., multiple controls have altProperties referencing the same enum), group them under one `if (block == trigger)` block.

After the trigger blocks, add empty `else if` blocks for all other controls:

```cpp
else if (block == {otherWidgetName})
{
}
```

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
5. **Generate .ui file**: Walk the tree and output XML with correct nesting, properties, and customwidgets
6. **Generate .h file**: Declare all control member variables with correct OPEN layer types
7. **Generate .cpp file**: Generate initialize_cb, update_cb with altProperties logic, and all other callbacks
8. **Write** all three files to disk in the same directory as the input JSON

## Reference Files

For property templates, consult the `.ui` files in `power_ui_template/` directory in this project. For verified working output examples, see `.claude/reference/working-example/BlockFeature/`.
