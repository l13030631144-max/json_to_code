# .h头文件模板
HEADER_TEMPLATE = """#ifndef {CLASS_NAME}_H
#define {CLASS_NAME}_H
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

    // Define member variables here...    
    PWOpen::BlockStyler::Enumeration* enum0;
    PWOpen::BlockStyler::Enumeration* enum1;
    PWOpen::BlockStyler::Toggle* toggle0;
    PWOpen::BlockStyler::DoubleBlock* double0;
    PWOpen::BlockStyler::StringBlock* string0;
    PWOpen::BlockStyler::SelectObject* selectObject0;
    // Define over...
};

#endif
"""

# .cpp实现文件模板
CPP_TEMPLATE = """#include "{CLASS_NAME}.h"

#include <PWOpen/IncludeHeader.h>

PWOpen::Session *({CLASS_NAME}::theSession) = nullptr;
PWOpen::UI *({CLASS_NAME}::theUI) = nullptr;

{CLASS_NAME}::{CLASS_NAME}()
{
    try
    {
        {CLASS_NAME}::theSession = PWOpen::Session::GetSession();
        {CLASS_NAME}::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("{UI_FILE_PATH}");

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
    auto dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
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
        // Initialize...
        enum0 = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));
        enum1 = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum1"));
        toggle0 = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        double0 = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double0"));
        string0 = dynamic_cast<PWOpen::BlockStyler::StringBlock*>(theDialog->TopBlock()->FindBlock("string0"));
        selectObject0 = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectObject0"));
        // Initialize over
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
        // Update code here
        if (block == enum0)
        {
            toggle0->GetProperties()->SetLogical("Show", enum0->GetProperties()->GetString("CurrentData") == "123" && theDialog->IsMore());
        }
        else if (block == enum1)
        {
            toggle0->GetProperties()->SetLogical("Enable", enum1->GetProperties()->GetString("CurrentData") == "456");
        }
        else if (block == toggle0)
        {
            double0->GetProperties()->SetLogical("ReadOnlyValue", toggle0->GetProperties()->GetLogical("Value"));
        }
        else if (block == string0) 
        {
            if (string0->GetProperties()->GetLogical("RequiredInput")) 
            {
                auto value = string0->GetProperties()->GetString("Value");
                double0->GetProperties()->SetLogical("Enable", !value.IsEmpty());
            }
        }
        else if (block == selectObject0) 
        {
            auto step_status = selectObject0->StepStatusAsString();
            if (step_status != "Optional") 
            {
                double0->GetProperties()->SetLogical("Show", step_status == "Satisfied");
            }
        }
        else if (block == theDialog->TopBlock()->FindBlock("More"))
        {
            toggle0->GetProperties()->SetLogical("Show", enum0->GetProperties()->GetString("CurrentData") == "123" && theDialog->IsMore());
        }

        enum1->GetProperties()->SetLogical("Enable", theDialog->IsSatisfied());
        // Update code over
    }
    catch(std::exception& ex)
    {
        {CLASS_NAME}::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}
"""