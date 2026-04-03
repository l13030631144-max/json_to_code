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
        explorer_main = dynamic_cast<PWOpen::BlockStyler::Explorer*>(theDialog->TopBlock()->FindBlock("explorer0"));
        blockLabel_nodeOneInfo = dynamic_cast<PWOpen::BlockStyler::Label*>(theDialog->TopBlock()->FindBlock("label0"));
        blockToggle_nodeOneToggle = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        blockToggle_nodeTwoToggle = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle1"));
        scrollWindow_main = dynamic_cast<PWOpen::BlockStyler::ScrolledWindow*>(theDialog->TopBlock()->FindBlock("scrolledWindow0"));
        blockLabel_scrollFirst = dynamic_cast<PWOpen::BlockStyler::Label*>(theDialog->TopBlock()->FindBlock("label1"));
        blockLabel_scrollSecond = dynamic_cast<PWOpen::BlockStyler::Label*>(theDialog->TopBlock()->FindBlock("label2"));
        blockLabel_scrollThird = dynamic_cast<PWOpen::BlockStyler::Label*>(theDialog->TopBlock()->FindBlock("label3"));
        blockToggle_scrollToggle = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle2"));
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
        if (block == explorer_main)
        {
        }
        else if (block == blockLabel_nodeOneInfo)
        {
        }
        else if (block == blockToggle_nodeOneToggle)
        {
        }
        else if (block == blockToggle_nodeTwoToggle)
        {
        }
        else if (block == scrollWindow_main)
        {
        }
        else if (block == blockLabel_scrollFirst)
        {
        }
        else if (block == blockLabel_scrollSecond)
        {
        }
        else if (block == blockLabel_scrollThird)
        {
        }
        else if (block == blockToggle_scrollToggle)
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
