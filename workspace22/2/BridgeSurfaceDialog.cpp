#include "BridgeSurfaceDialog.h"
#include <PWOpen/IncludeHeader.h>
#include <windows.h>
#include <fstream>
#include <stdexcept>
#include <string>

#ifdef CreateDialog
#undef CreateDialog
#endif

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(BridgeSurfaceDialog::theSession) = nullptr;
PWOpen::UI *(BridgeSurfaceDialog::theUI) = nullptr;

namespace
{
std::string LogPath()
{
    char tempPath[MAX_PATH] = {};
    DWORD len = GetTempPathA(MAX_PATH, tempPath);
    if (len == 0 || len > MAX_PATH)
    {
        return "BridgeSurfaceDialog.log";
    }
    return std::string(tempPath) + "BridgeSurfaceDialog.log";
}

void WriteLog(const std::string& message)
{
    std::ofstream out(LogPath(), std::ios::app);
    if (out)
    {
        out << message << std::endl;
    }
}

void ShowFatalMessage(const char* message)
{
    WriteLog(std::string("FATAL: ") + message);
    if (BridgeSurfaceDialog::theUI)
    {
        BridgeSurfaceDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, message);
        return;
    }

    MessageBoxA(nullptr, message, "BridgeSurfaceDialog", MB_OK | MB_ICONERROR);
}
}

BridgeSurfaceDialog::BridgeSurfaceDialog()
{
    WriteLog("BridgeSurfaceDialog ctor: begin");
    BridgeSurfaceDialog::theSession = PWOpen::Session::GetSession();
    BridgeSurfaceDialog::theUI = PWOpen::UI::GetUI();
    if (!BridgeSurfaceDialog::theSession || !BridgeSurfaceDialog::theUI)
    {
        throw std::runtime_error("Failed to initialize PWOpen session or UI.");
    }

    theDialog = theUI->CreateDialog("BridgeSurfaceDialog.ui");
    WriteLog(std::string("BridgeSurfaceDialog ctor: CreateDialog returned ") + (theDialog ? "non-null" : "null"));
    if (!theDialog)
    {
        throw std::runtime_error("CreateDialog failed. Check whether BridgeSurfaceDialog.ui is deployed beside the DLL.");
    }

    theDialog->AddApplyHandler([this] { return apply_cb(); });
    theDialog->AddOkHandler([this] { return ok_cb(); });
    theDialog->AddUpdateHandler([this](auto && PH1) { return update_cb(std::forward<decltype(PH1)>(PH1)); });
    theDialog->AddInitializeHandler([this] { initialize_cb(); });
    theDialog->AddDialogShownHandler([this] { dialogShown_cb(); });
    WriteLog("BridgeSurfaceDialog ctor: handlers registered");
}

BridgeSurfaceDialog::~BridgeSurfaceDialog()
{
    WriteLog("BridgeSurfaceDialog dtor");
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse BridgeSurfaceDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        if (!theDialog)
        {
            throw std::runtime_error("Dialog is not initialized.");
        }
        WriteLog("Launch: before theDialog->Launch()");
        dialogResponse = theDialog->Launch();
        WriteLog("Launch: after theDialog->Launch()");
    }
    catch(std::exception& ex)
    {
        ShowFatalMessage(ex.what());
    }
    return dialogResponse;
}

void BridgeSurfaceDialog::initialize_cb()
{
    WriteLog("initialize_cb: begin");
    try
    {
        blockGroup_edgeGroup = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_edgeGroup"));
        enum_firstObjectTypeEnum = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_firstObjectTypeEnum"));
        selectObject_selectFirstEdge = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectObject_selectFirstEdge"));
        selectObject_selectFirstCurve = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectObject_selectFirstCurve"));
        reverseDir_reverseFirstDirection = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("reverseDir_reverseFirstDirection"));
        separator_lineSeparator = dynamic_cast<PWOpen::BlockStyler::Separator*>(theDialog->TopBlock()->FindBlock("separator_lineSeparator"));
        enum_secondObjectTypeEnum = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_secondObjectTypeEnum"));
        selectObject_selectSecondEdge = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectObject_selectSecondEdge"));
        selectObject_selectSecondCurve = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectObject_selectSecondCurve"));
        reverseDir_reverseSecondDirection = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("reverseDir_reverseSecondDirection"));
        blockGroup_shapeGroup = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_shapeGroup"));
        enum_firstEdgeContinuityEnum = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_firstEdgeContinuityEnum"));
        reverseDir_reverseFirstEdgeDir = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("reverseDir_reverseFirstEdgeDir"));
        enum_secondEdgeContinuityEnum = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_secondEdgeContinuityEnum"));
        reverseDir_reverseSecondEdgeDir = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("reverseDir_reverseSecondEdgeDir"));
        enum_flowDirectionEnum = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_flowDirectionEnum"));
        enum_tangencyControlEnum = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_tangencyControlEnum"));
        blockExpr_firstEdgeConstantExpr = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_firstEdgeConstantExpr"));
        blockExpr_secondEdgeConstantExpr = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_secondEdgeConstantExpr"));
        blockExpr_firstEdgeStartExpr = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_firstEdgeStartExpr"));
        blockExpr_firstEdgeEndExpr = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_firstEdgeEndExpr"));
        blockExpr_secondEdgeStartExpr = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_secondEdgeStartExpr"));
        blockExpr_secondEdgeEndExpr = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_secondEdgeEndExpr"));
        blockGroup_edgeLimitGroup = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_edgeLimitGroup"));
        blockToggle_linkStartHandleToggle = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("blockToggle_linkStartHandleToggle"));
        blockToggle_linkEndHandleToggle = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("blockToggle_linkEndHandleToggle"));
        tabControl_blockTabWidget0 = dynamic_cast<PWOpen::BlockStyler::TabControl*>(theDialog->TopBlock()->FindBlock("tabControl_blockTabWidget0"));
        tabPage_blockTabPage0 = dynamic_cast<PWOpen::BlockStyler::UIBlock*>(theDialog->TopBlock()->FindBlock("tabPage_blockTabPage0"));
        blockToggle_toggle0 = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("blockToggle_toggle0"));
        blockExpr_expression3 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression3"));
        blockExpr_expression4 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression4"));
        blockExpr_expression7 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression7"));
        blockExpr_expression8 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression8"));
        blockExpr_expression5 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression5"));
        tabPage_blockTabPage2 = dynamic_cast<PWOpen::BlockStyler::UIBlock*>(theDialog->TopBlock()->FindBlock("tabPage_blockTabPage2"));
        blockToggle_endpointToEndpointToggle = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("blockToggle_endpointToEndpointToggle"));
        blockExpr_expression0 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression0"));
        blockExpr_expression1 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression1"));
        blockExpr_expression9 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression9"));
        blockExpr_expression10 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression10"));
        blockExpr_expression2 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression2"));
        blockGroup_settingGroup = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_settingGroup"));
        tabControl_blockTabWidget1 = dynamic_cast<PWOpen::BlockStyler::TabControl*>(theDialog->TopBlock()->FindBlock("tabControl_blockTabWidget1"));
        tabPage_blockTabPage1 = dynamic_cast<PWOpen::BlockStyler::UIBlock*>(theDialog->TopBlock()->FindBlock("tabPage_blockTabPage1"));
        enum_enum0 = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_enum0"));
        blockInteger_integer0 = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("blockInteger_integer0"));
        blockInteger_integer1 = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("blockInteger_integer1"));
        blockInteger_integer2 = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("blockInteger_integer2"));
        blockGroup_group0 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_group0"));
        blockLinear_linearDim0 = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("blockLinear_linearDim0"));
        blockAngular_angularDim0 = dynamic_cast<PWOpen::BlockStyler::AngularDimension*>(theDialog->TopBlock()->FindBlock("blockAngular_angularDim0"));
        blockExpr_expression6 = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("blockExpr_expression6"));
    }
    catch(std::exception& ex)
    {
        ShowFatalMessage(ex.what());
    }
    WriteLog("initialize_cb: end");
}

void BridgeSurfaceDialog::dialogShown_cb()
{
    WriteLog("dialogShown_cb");
    try
    {
        update_cb(theDialog->TopBlock());
    }
    catch(std::exception& ex)
    {
        ShowFatalMessage(ex.what());
    }
}

int BridgeSurfaceDialog::apply_cb()
{
    WriteLog("apply_cb");
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        ShowFatalMessage(ex.what());
    }
    return errorCode;
}

int BridgeSurfaceDialog::ok_cb()
{
    WriteLog("ok_cb");
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        ShowFatalMessage(ex.what());
    }
    return errorCode;
}

int BridgeSurfaceDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    WriteLog("update_cb");
    try
    {
        if (block == theDialog->TopBlock() || block == blockToggle_endpointToEndpointToggle || block == blockToggle_toggle0 || block == enum_enum0 || block == enum_firstObjectTypeEnum || block == enum_secondObjectTypeEnum || block == enum_tangencyControlEnum)
        {
            bool isMore = theDialog->IsMore();
            bool blockToggle_endpointToEndpointToggleValue = blockToggle_endpointToEndpointToggle->GetProperties()->GetLogical("PW_Value");
            bool blockToggle_toggle0Value = blockToggle_toggle0->GetProperties()->GetLogical("PW_Value");
            PWOpen::PWString enum_enum0Value = enum_enum0->GetProperties()->GetString("CurrentData");
            PWOpen::PWString enum_firstObjectTypeEnumValue = enum_firstObjectTypeEnum->GetProperties()->GetString("CurrentData");
            PWOpen::PWString enum_secondObjectTypeEnumValue = enum_secondObjectTypeEnum->GetProperties()->GetString("CurrentData");
            PWOpen::PWString enum_tangencyControlEnumValue = enum_tangencyControlEnum->GetProperties()->GetString("CurrentData");
            selectObject_selectFirstEdge->GetProperties()->SetLogical("Show", enum_firstObjectTypeEnumValue == "1");
            selectObject_selectFirstCurve->GetProperties()->SetLogical("Show", enum_firstObjectTypeEnumValue == "2");
            selectObject_selectSecondEdge->GetProperties()->SetLogical("Show", enum_secondObjectTypeEnumValue == "EndObjectTypeEdge");
            selectObject_selectSecondCurve->GetProperties()->SetLogical("Show", enum_secondObjectTypeEnumValue == "EndObjectTypeCurve");
            blockExpr_firstEdgeConstantExpr->GetProperties()->SetLogical("Show", enum_tangencyControlEnumValue == "TangencyControlConstant");
            blockExpr_secondEdgeConstantExpr->GetProperties()->SetLogical("Show", enum_tangencyControlEnumValue == "TangencyControlConstant");
            blockExpr_firstEdgeStartExpr->GetProperties()->SetLogical("Show", enum_tangencyControlEnumValue == "TangencyControlLinear");
            blockExpr_firstEdgeEndExpr->GetProperties()->SetLogical("Show", enum_tangencyControlEnumValue == "TangencyControlLinear");
            blockExpr_secondEdgeStartExpr->GetProperties()->SetLogical("Show", enum_tangencyControlEnumValue == "TangencyControlLinear");
            blockExpr_secondEdgeEndExpr->GetProperties()->SetLogical("Show", enum_tangencyControlEnumValue == "TangencyControlLinear");
            blockExpr_expression3->GetProperties()->SetLogical("Show", !blockToggle_toggle0Value);
            blockExpr_expression4->GetProperties()->SetLogical("Show", !blockToggle_toggle0Value);
            blockExpr_expression7->GetProperties()->SetLogical("Show", blockToggle_toggle0Value);
            blockExpr_expression8->GetProperties()->SetLogical("Show", blockToggle_toggle0Value);
            blockExpr_expression0->GetProperties()->SetLogical("Show", !blockToggle_endpointToEndpointToggleValue);
            blockExpr_expression1->GetProperties()->SetLogical("Show", !blockToggle_endpointToEndpointToggleValue);
            blockExpr_expression9->GetProperties()->SetLogical("Show", blockToggle_endpointToEndpointToggleValue);
            blockExpr_expression10->GetProperties()->SetLogical("Show", blockToggle_endpointToEndpointToggleValue);
            blockGroup_settingGroup->GetProperties()->SetLogical("Show", isMore);
            blockInteger_integer0->GetProperties()->SetLogical("Show", enum_enum0Value == "2");
            blockInteger_integer1->GetProperties()->SetLogical("Show", enum_enum0Value == "3");
            blockInteger_integer2->GetProperties()->SetLogical("Show", enum_enum0Value == "3");
        }
        else if (block == blockGroup_edgeGroup)
        {
        }
        else if (block == enum_firstObjectTypeEnum)
        {
        }
        else if (block == selectObject_selectFirstEdge)
        {
        }
        else if (block == selectObject_selectFirstCurve)
        {
        }
        else if (block == reverseDir_reverseFirstDirection)
        {
        }
        else if (block == separator_lineSeparator)
        {
        }
        else if (block == enum_secondObjectTypeEnum)
        {
        }
        else if (block == selectObject_selectSecondEdge)
        {
        }
        else if (block == selectObject_selectSecondCurve)
        {
        }
        else if (block == reverseDir_reverseSecondDirection)
        {
        }
        else if (block == blockGroup_shapeGroup)
        {
        }
        else if (block == enum_firstEdgeContinuityEnum)
        {
        }
        else if (block == reverseDir_reverseFirstEdgeDir)
        {
        }
        else if (block == enum_secondEdgeContinuityEnum)
        {
        }
        else if (block == reverseDir_reverseSecondEdgeDir)
        {
        }
        else if (block == enum_flowDirectionEnum)
        {
        }
        else if (block == enum_tangencyControlEnum)
        {
        }
        else if (block == blockExpr_firstEdgeConstantExpr)
        {
        }
        else if (block == blockExpr_secondEdgeConstantExpr)
        {
        }
        else if (block == blockExpr_firstEdgeStartExpr)
        {
        }
        else if (block == blockExpr_firstEdgeEndExpr)
        {
        }
        else if (block == blockExpr_secondEdgeStartExpr)
        {
        }
        else if (block == blockExpr_secondEdgeEndExpr)
        {
        }
        else if (block == blockGroup_edgeLimitGroup)
        {
        }
        else if (block == blockToggle_linkStartHandleToggle)
        {
        }
        else if (block == blockToggle_linkEndHandleToggle)
        {
        }
        else if (block == tabControl_blockTabWidget0)
        {
        }
        else if (block == tabPage_blockTabPage0)
        {
        }
        else if (block == blockToggle_toggle0)
        {
        }
        else if (block == blockExpr_expression3)
        {
        }
        else if (block == blockExpr_expression4)
        {
        }
        else if (block == blockExpr_expression7)
        {
        }
        else if (block == blockExpr_expression8)
        {
        }
        else if (block == blockExpr_expression5)
        {
        }
        else if (block == tabPage_blockTabPage2)
        {
        }
        else if (block == blockToggle_endpointToEndpointToggle)
        {
        }
        else if (block == blockExpr_expression0)
        {
        }
        else if (block == blockExpr_expression1)
        {
        }
        else if (block == blockExpr_expression9)
        {
        }
        else if (block == blockExpr_expression10)
        {
        }
        else if (block == blockExpr_expression2)
        {
        }
        else if (block == blockGroup_settingGroup)
        {
        }
        else if (block == tabControl_blockTabWidget1)
        {
        }
        else if (block == tabPage_blockTabPage1)
        {
        }
        else if (block == enum_enum0)
        {
        }
        else if (block == blockInteger_integer0)
        {
        }
        else if (block == blockInteger_integer1)
        {
        }
        else if (block == blockInteger_integer2)
        {
        }
        else if (block == blockGroup_group0)
        {
        }
        else if (block == blockLinear_linearDim0)
        {
        }
        else if (block == blockAngular_angularDim0)
        {
        }
        else if (block == blockExpr_expression6)
        {
        }
    }
    catch(std::exception& ex)
    {
        ShowFatalMessage(ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    WriteLog("ufusr: enter");
    BridgeSurfaceDialog *dialog = NULL;
    if (retcod)
    {
        *retcod = 0;
    }
    try
    {
        WriteLog("ufusr: before new BridgeSurfaceDialog");
        dialog = new BridgeSurfaceDialog();
        WriteLog("ufusr: before Launch");
        dialog->Launch();
        WriteLog("ufusr: after Launch");
    }
    catch(const std::exception& ex)
    {
        if (retcod)
        {
            *retcod = 1;
        }
        ShowFatalMessage(ex.what());
    }
    catch(...)
    {
        if (retcod)
        {
            *retcod = 1;
        }
        ShowFatalMessage("Unhandled exception while starting BridgeSurfaceDialog.");
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
    WriteLog("ufusr: exit");
}

extern "C" DllExport int GetUnloadOption(char* param)
{
    WriteLog("GetUnloadOption");
    return static_cast<int>(PWOpen::Session::LibraryUnloadOptionImmediately);
}

extern "C" DllExport int ufusr_ask_unload()
{
    WriteLog("ufusr_ask_unload");
    return static_cast<int>(PWOpen::Session::LibraryUnloadOptionImmediately);
}

extern "C" DllExport void ufusr_cleanup(void)
{
    WriteLog("ufusr_cleanup");
}
