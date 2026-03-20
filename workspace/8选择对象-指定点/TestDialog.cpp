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
        selectObject_selection = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject0"));
        sectionBuilder_curve = dynamic_cast<PWOpen::BlockStyler::SectionBuilder*>(theDialog->TopBlock()->FindBlock("section0"));
        superSection_section = dynamic_cast<PWOpen::BlockStyler::SuperSection*>(theDialog->TopBlock()->FindBlock("section1"));
        curveCollector_edgeSelect = dynamic_cast<PWOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("edge_select0"));
        faceCollector_faceSelect = dynamic_cast<PWOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("face_select0"));
        bodyCollector_bodySelect = dynamic_cast<PWOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("bodySelect0"));
        specifyPoint_origin = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
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
        if (block == selectObject_selection)
        {
        }
        else if (block == sectionBuilder_curve)
        {
        }
        else if (block == superSection_section)
        {
        }
        else if (block == curveCollector_edgeSelect)
        {
        }
        else if (block == faceCollector_faceSelect)
        {
        }
        else if (block == bodyCollector_bodySelect)
        {
        }
        else if (block == specifyPoint_origin)
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
