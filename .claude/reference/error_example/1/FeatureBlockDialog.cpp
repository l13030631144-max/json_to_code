#include "FeatureBlockDialog.h"
#include <PWOpen/IncludeHeader.h>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(FeatureBlockDialog::theSession) = nullptr;
PWOpen::UI *(FeatureBlockDialog::theUI) = nullptr;

FeatureBlockDialog::FeatureBlockDialog()
{
    try
    {
        FeatureBlockDialog::theSession = PWOpen::Session::GetSession();
        FeatureBlockDialog::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("FeatureBlockDialog.ui");

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
FeatureBlockDialog::~FeatureBlockDialog()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse FeatureBlockDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void FeatureBlockDialog::initialize_cb()
{
    try
    {
        enum_enum0 = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));
        blockGroup_原点 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        specifyPoint_point0 = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
        blockGroup_从原点出发的点_XC__YC = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        specifyPoint_point1 = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point1"));
        blockGroup_从原点出发的点_XC__YC__ZC = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        specifyPoint_point = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point"));
        blockGroup_维度 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        blockLinear_高度_ZC_ = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim0"));
        blockGroup_尺寸 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));
        blockLinear_长度__XC_ = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim1"));
        blockLinear_宽度__YC_ = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim2"));
        blockLinear_高度__ZC_ = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim3"));
        blockGroup_布尔 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group4"));
        enum_布尔 = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum1"));
        bodyCollector_bodySelect0 = dynamic_cast<PWOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("bodySelect0"));
        blockGroup_设置 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group5"));
        blockToggle_关联原点 = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        blockGroup_设置 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group6"));
        blockToggle_关联原点和偏置 = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle1"));
    }
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void FeatureBlockDialog::dialogShown_cb()
{
    try
    {
        // Initialize enum_布尔 trigger
        update_cb(enum_布尔);
        // Initialize enum_enum0 trigger
        update_cb(enum_enum0);
    }
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int FeatureBlockDialog::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int FeatureBlockDialog::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int FeatureBlockDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == enum_enum0)
        {
            PWOpen::PWString enumValue = enum_enum0->GetProperties()->GetString("CurrentData");

            blockGroup_从原点出发的点_XC__YC->GetProperties()->SetLogical("PW_Show", enumValue == "TwoPointsAndHeight");
            blockGroup_从原点出发的点_XC__YC__ZC->GetProperties()->SetLogical("PW_Show", enumValue == "TwoDiagonalPoints");
            blockGroup_维度->GetProperties()->SetLogical("PW_Show", enumValue == "TwoPointsAndHeight");
            blockGroup_尺寸->GetProperties()->SetLogical("PW_Show", enumValue == "OriginAndEdgeLength");
            blockGroup_设置->GetProperties()->SetLogical("PW_Show", enumValue == "OriginAndEdgeLength");
            blockGroup_设置->GetProperties()->SetLogical("PW_Show", enumValue == "TwoPointsAndHeight" || enumValue == "TwoDiagonalPoints");
        }
        if (block == enum_布尔)
        {
            PWOpen::PWString enumValue = enum_布尔->GetProperties()->GetString("CurrentData");

            bodyCollector_bodySelect0->GetProperties()->SetLogical("PW_Show", enumValue == "Unite" || enumValue == "Subtract" || enumValue == "Intersect");
        }
        else if (block == enum_enum0)
        {
        }
        else if (block == blockGroup_原点)
        {
        }
        else if (block == specifyPoint_point0)
        {
        }
        else if (block == blockGroup_从原点出发的点_XC__YC)
        {
        }
        else if (block == specifyPoint_point1)
        {
        }
        else if (block == blockGroup_从原点出发的点_XC__YC__ZC)
        {
        }
        else if (block == specifyPoint_point)
        {
        }
        else if (block == blockGroup_维度)
        {
        }
        else if (block == blockLinear_高度_ZC_)
        {
        }
        else if (block == blockGroup_尺寸)
        {
        }
        else if (block == blockLinear_长度__XC_)
        {
        }
        else if (block == blockLinear_宽度__YC_)
        {
        }
        else if (block == blockLinear_高度__ZC_)
        {
        }
        else if (block == blockGroup_布尔)
        {
        }
        else if (block == enum_布尔)
        {
        }
        else if (block == bodyCollector_bodySelect0)
        {
        }
        else if (block == blockGroup_设置)
        {
        }
        else if (block == blockToggle_关联原点)
        {
        }
        else if (block == blockGroup_设置)
        {
        }
        else if (block == blockToggle_关联原点和偏置)
        {
        }
    }
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    FeatureBlockDialog *dialog = NULL;
    try
    {
        dialog = new FeatureBlockDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
