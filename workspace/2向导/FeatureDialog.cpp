#include "FeatureDialog.h"
#include <PWOpen/IncludeHeader.h>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(FeatureDialog::theSession) = nullptr;
PWOpen::UI *(FeatureDialog::theUI) = nullptr;

FeatureDialog::FeatureDialog()
{
    try
    {
        FeatureDialog::theSession = PWOpen::Session::GetSession();
        FeatureDialog::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("FeatureDialog.ui");

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
FeatureDialog::~FeatureDialog()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse FeatureDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        FeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void FeatureDialog::initialize_cb()
{
    try
    {
        wizard_main = dynamic_cast<PWOpen::BlockStyler::Wizard*>(theDialog->TopBlock()->FindBlock("wizard0"));
        blockLabel_stepOneTitle = dynamic_cast<PWOpen::BlockStyler::Label*>(theDialog->TopBlock()->FindBlock("label0"));
        blockButton_action = dynamic_cast<PWOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));
        blockLabel_stepTwoTitle = dynamic_cast<PWOpen::BlockStyler::Label*>(theDialog->TopBlock()->FindBlock("label1"));
    }
    catch(std::exception& ex)
    {
        FeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void FeatureDialog::dialogShown_cb()
{
    try
    {
    }
    catch(std::exception& ex)
    {
        FeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int FeatureDialog::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        FeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int FeatureDialog::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        FeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int FeatureDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == wizard_main)
        {
        }
        else if (block == blockLabel_stepOneTitle)
        {
        }
        else if (block == blockButton_action)
        {
        }
        else if (block == blockLabel_stepTwoTitle)
        {
        }
    }
    catch(std::exception& ex)
    {
        FeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    FeatureDialog *dialog = NULL;
    try
    {
        dialog = new FeatureDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        FeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
