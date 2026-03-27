#include "PMILinearDimensionDialog.h"
#include <PWOpen/IncludeHeader.h>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(PMILinearDimensionDialog::theSession) = nullptr;
PWOpen::UI *(PMILinearDimensionDialog::theUI) = nullptr;

PMILinearDimensionDialog::PMILinearDimensionDialog()
{
    try
    {
        PMILinearDimensionDialog::theSession = PWOpen::Session::GetSession();
        PMILinearDimensionDialog::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("PMILinearDimensionDialog.ui");

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
PMILinearDimensionDialog::~PMILinearDimensionDialog()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse PMILinearDimensionDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        PMILinearDimensionDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void PMILinearDimensionDialog::initialize_cb()
{
    try
    {
        blockGroup_reference = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        selectObject_firstObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject0"));
        selectObject_secondObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject1"));
        blockGroup_origin = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        cursorLocation_position = dynamic_cast<PWOpen::BlockStyler::SpecifyLocation*>(theDialog->TopBlock()->FindBlock("cursorLocation1"));
        blockToggle_autoPlace = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        blockGroup_direction = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group6"));
        blockToggle_lockPlane = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle1"));
        selectObject_alternativePlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject2"));
        selectObject_specifyPlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject4"));
        blockToggle_projectToPlane = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle2"));
        blockGroup_associatedObject = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        selectObject_associatedObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject3"));
        blockGroup_dimensionSet = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));
        enum_dimensionSetMethod = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enumeration0"));
        selectObject_dimensionSetObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject5"));
        blockGroup_measurement = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group4"));
        enum_measurementMethod = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enumeration1"));
        blockToggle_useMeasureMethod = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle3"));
        specifyVector_measureDirection = dynamic_cast<PWOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("vector0"));
        blockButton_alternateEndpoint = dynamic_cast<PWOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));
        blockToggle_useBaseline = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle4"));
        blockGroup_settings = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group5"));
        blockButton_settings = dynamic_cast<PWOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button2"));
        blockButton_inheritDimension = dynamic_cast<PWOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button1"));
    }
    catch(std::exception& ex)
    {
        PMILinearDimensionDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void PMILinearDimensionDialog::dialogShown_cb()
{
    try
    {
        // Toggle initialization - trigger update_cb to set initial UI state
        update_cb(blockToggle_lockPlane);

        // Enum initialization - trigger update_cb to set initial UI state
        update_cb(enum_measurementMethod);
        update_cb(enum_dimensionSetMethod);

        // More button initialization - trigger update_cb to set initial More-dependent visibility
        update_cb(theDialog->TopBlock()->FindBlock("More"));
    }
    catch(std::exception& ex)
    {
        PMILinearDimensionDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int PMILinearDimensionDialog::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        PMILinearDimensionDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int PMILinearDimensionDialog::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        PMILinearDimensionDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int PMILinearDimensionDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        // toggle1 (lockPlane) triggers: selectobject2 Show, selectobject4 Show, toggle2 Show
        if (block == blockToggle_lockPlane)
        {
            bool lockPlaneValue = blockToggle_lockPlane->GetProperties()->GetLogical("Value");

            // altProperties PW_Show: "toggle1 == false"
            selectObject_alternativePlane->GetProperties()->SetLogical("Show", !lockPlaneValue);
            selectObject_specifyPlane->GetProperties()->SetLogical("Show", !lockPlaneValue);

            // altProperties PW_Show: "toggle1 == true"
            blockToggle_projectToPlane->GetProperties()->SetLogical("Show", lockPlaneValue);
        }
        // enumeration1 (measurementMethod) triggers: toggle3 Show, vector0 Show, button0 Show, toggle4 Show, group3 Show
        else if (block == enum_measurementMethod)
        {
            PWOpen::PWString enumValue = enum_measurementMethod->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "enumeration1(PointToPoint)"
            blockToggle_useMeasureMethod->GetProperties()->SetLogical("Show",
                enumValue == "PointToPoint");

            // altProperties PW_Show: "enumeration1(PointToPoint) && toggle3 == true"
            specifyVector_measureDirection->GetProperties()->SetLogical("Show",
                enumValue == "PointToPoint" && blockToggle_useMeasureMethod->GetProperties()->GetLogical("Value"));

            // altProperties PW_Show: "enumeration1(AutoDetect,Horizontal,Vertical,PointToPoint)"
            blockButton_alternateEndpoint->GetProperties()->SetLogical("Show",
                enumValue == "AutoDetect" || enumValue == "Horizontal" || enumValue == "Vertical" || enumValue == "PointToPoint");

            // altProperties PW_Show: "enumeration1(Cylindrical)"
            blockToggle_useBaseline->GetProperties()->SetLogical("Show",
                enumValue == "Cylindrical");

            // altProperties PW_Show: "Dialog0 == true && enumeration1(AutoDetect,Horizontal,Vertical,PointToPoint,Perpendicular)"
            blockGroup_dimensionSet->GetProperties()->SetLogical("Show",
                theDialog->IsMore() && (enumValue == "AutoDetect" || enumValue == "Horizontal" || enumValue == "Vertical" || enumValue == "PointToPoint" || enumValue == "Perpendicular"));
        }
        // toggle3 (useMeasureMethod) triggers: vector0 Show
        else if (block == blockToggle_useMeasureMethod)
        {
            bool useMeasureValue = blockToggle_useMeasureMethod->GetProperties()->GetLogical("Value");
            PWOpen::PWString enumValue = enum_measurementMethod->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "enumeration1(PointToPoint) && toggle3 == true"
            specifyVector_measureDirection->GetProperties()->SetLogical("Show",
                enumValue == "PointToPoint" && useMeasureValue);
        }
        // enumeration0 (dimensionSetMethod) triggers: selectobject5 Show
        else if (block == enum_dimensionSetMethod)
        {
            PWOpen::PWString enumValue = enum_dimensionSetMethod->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "enumeration0(Chain,Baseline)"
            selectObject_dimensionSetObj->GetProperties()->SetLogical("Show",
                enumValue == "Chain" || enumValue == "Baseline");
        }
        // More button triggers: group3 Show, group5 Show
        else if (block == theDialog->TopBlock()->FindBlock("More"))
        {
            PWOpen::PWString enumValue = enum_measurementMethod->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "Dialog0 == true && enumeration1(AutoDetect,Horizontal,Vertical,PointToPoint,Perpendicular)"
            blockGroup_dimensionSet->GetProperties()->SetLogical("Show",
                theDialog->IsMore() && (enumValue == "AutoDetect" || enumValue == "Horizontal" || enumValue == "Vertical" || enumValue == "PointToPoint" || enumValue == "Perpendicular"));

            // altProperties PW_Show: "Dialog0 == true"
            blockGroup_settings->GetProperties()->SetLogical("Show", theDialog->IsMore());
        }
        else if (block == selectObject_firstObject)
        {
        }
        else if (block == selectObject_secondObject)
        {
        }
        else if (block == cursorLocation_position)
        {
        }
        else if (block == blockToggle_autoPlace)
        {
        }
        else if (block == selectObject_alternativePlane)
        {
        }
        else if (block == selectObject_specifyPlane)
        {
        }
        else if (block == blockToggle_projectToPlane)
        {
        }
        else if (block == selectObject_associatedObj)
        {
        }
        else if (block == selectObject_dimensionSetObj)
        {
        }
        else if (block == blockButton_alternateEndpoint)
        {
        }
        else if (block == blockToggle_useBaseline)
        {
        }
        else if (block == blockButton_settings)
        {
        }
        else if (block == blockButton_inheritDimension)
        {
        }
    }
    catch(std::exception& ex)
    {
        PMILinearDimensionDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    PMILinearDimensionDialog *dialog = NULL;
    try
    {
        dialog = new PMILinearDimensionDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        PMILinearDimensionDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
