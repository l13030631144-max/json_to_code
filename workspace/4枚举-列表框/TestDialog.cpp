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
        enum_selector = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enumeration0"));
        blockString_text = dynamic_cast<PWOpen::BlockStyler::StringBlock*>(theDialog->TopBlock()->FindBlock("string0"));
        multilineString_content = dynamic_cast<PWOpen::BlockStyler::MultilineString*>(theDialog->TopBlock()->FindBlock("multiline_string0"));
        blockButton_action = dynamic_cast<PWOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("button0"));
        listBox_items = dynamic_cast<PWOpen::BlockStyler::ListBox*>(theDialog->TopBlock()->FindBlock("list_box0"));
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
        update_cb(enum_selector);
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
        if (block == enum_selector)
        {
            PWOpen::PWString enumValue = enum_selector->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "enumeration0(1)"
            blockString_text->GetProperties()->SetLogical("Show",
                enumValue == "1");

            // altProperties PW_Show: "enumeration0(2)"
            multilineString_content->GetProperties()->SetLogical("Show",
                enumValue == "2");

            // altProperties PW_Show: "enumeration0(3)"
            blockButton_action->GetProperties()->SetLogical("Show",
                enumValue == "3");

            // altProperties PW_Show: "enumeration0(4)"
            listBox_items->GetProperties()->SetLogical("Show",
                enumValue == "4");
        }
        else if (block == blockString_text)
        {
        }
        else if (block == multilineString_content)
        {
        }
        else if (block == blockButton_action)
        {
        }
        else if (block == listBox_items)
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
