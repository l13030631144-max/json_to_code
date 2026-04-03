#include "TestDialog.h"
#include <PWOpen/IncludeHeader.h>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(TestDialog::theSession) = nullptr;
PWOpen::UI *(TestDialog::theUI) = nullptr;

TestDialog::TestDialog()
{
    try
    {
        TestDialog::theSession = PWOpen::Session::GetSession();
        TestDialog::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("TestDialog.ui");

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
TestDialog::~TestDialog()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse TestDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void TestDialog::initialize_cb()
{
    try
    {
        blockInteger_count = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("integer0"));
        blockDouble_value = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double0"));
        blockExpression_expr = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("expression0"));
        blockLinear_length = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim0"));
        blockAngular_angle = dynamic_cast<PWOpen::BlockStyler::AngularDimension*>(theDialog->TopBlock()->FindBlock("angular_dim0"));
        blockRadius_radius = dynamic_cast<PWOpen::BlockStyler::RadiusDimension*>(theDialog->TopBlock()->FindBlock("radius_dim0"));
        onPathDim_distance = dynamic_cast<PWOpen::BlockStyler::OnPathDimension*>(theDialog->TopBlock()->FindBlock("on_path_dim0"));
        integerTable_data = dynamic_cast<PWOpen::BlockStyler::IntegerTable*>(theDialog->TopBlock()->FindBlock("integerTable0"));
        doubleTable_data = dynamic_cast<PWOpen::BlockStyler::DoubleTable*>(theDialog->TopBlock()->FindBlock("doubleTable0"));
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void TestDialog::dialogShown_cb()
{
    try
    {
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int TestDialog::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int TestDialog::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int TestDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == blockInteger_count)
        {
        }
        else if (block == blockDouble_value)
        {
        }
        else if (block == blockExpression_expr)
        {
        }
        else if (block == blockLinear_length)
        {
        }
        else if (block == blockAngular_angle)
        {
        }
        else if (block == blockRadius_radius)
        {
        }
        else if (block == onPathDim_distance)
        {
        }
        else if (block == integerTable_data)
        {
        }
        else if (block == doubleTable_data)
        {
        }
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    TestDialog *dialog = NULL;
    try
    {
        dialog = new TestDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
