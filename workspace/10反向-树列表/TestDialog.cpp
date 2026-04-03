#include "TestDialog.h"
#include <PWOpen/IncludeHeader.h>
#include <PWOpen/BlockStyler_SetList.hxx>

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
    catch (std::exception &ex)
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
    catch (std::exception &ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}

void TestDialog::initialize_cb()
{
    try
    {
        reverseDir_direction = dynamic_cast<PWOpen::BlockStyler::ReverseDirection *>(theDialog->TopBlock()->FindBlock("direction0"));
        setList_selection = dynamic_cast<PWOpen::BlockStyler::SetList *>(theDialog->TopBlock()->FindBlock("list0"));
        orientXpress_orientation = dynamic_cast<PWOpen::BlockStyler::OrientXpress *>(theDialog->TopBlock()->FindBlock("orientXpress0"));
        microposition_position = dynamic_cast<PWOpen::BlockStyler::Microposition *>(theDialog->TopBlock()->FindBlock("micropositioning0"));
        fileSelection_fileBrowser = dynamic_cast<PWOpen::BlockStyler::FileSelection *>(theDialog->TopBlock()->FindBlock("nativeFileBrowser0"));
        folderSelection_folderBrowser = dynamic_cast<PWOpen::BlockStyler::FolderSelection *>(theDialog->TopBlock()->FindBlock("nativeFolderBrowser0"));
        selectExpression_expression = dynamic_cast<PWOpen::BlockStyler::ChooseExpression *>(theDialog->TopBlock()->FindBlock("selectExpression0"));
        blockTree_tree = dynamic_cast<PWOpen::BlockStyler::Tree *>(theDialog->TopBlock()->FindBlock("tree_control0"));
    }
    catch (std::exception &ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}

void TestDialog::dialogShown_cb()
{
    try
    {
    }
    catch (std::exception &ex)
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
    catch (std::exception &ex)
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
    catch (std::exception &ex)
    {
        errorCode = 1;
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int TestDialog::update_cb(PWOpen::BlockStyler::UIBlock *block)
{
    try
    {
        if (block == reverseDir_direction)
        {
        }
        else if (block == setList_selection)
        {
        }
        else if (block == orientXpress_orientation)
        {
        }
        else if (block == microposition_position)
        {
        }
        else if (block == fileSelection_fileBrowser)
        {
        }
        else if (block == folderSelection_folderBrowser)
        {
        }
        else if (block == selectExpression_expression)
        {
        }
        else if (block == blockTree_tree)
        {
        }
    }
    catch (std::exception &ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void ufusr(char *param, int *retcod, int param_len)
{
    TestDialog *dialog = NULL;
    try
    {
        dialog = new TestDialog();
        dialog->Launch();
    }
    catch (const std::exception &ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if (dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
