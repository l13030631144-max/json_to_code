#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class TestDialog {
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    TestDialog();
    ~TestDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    PWOpen::BlockStyler::Group* blockGroup_main;
    PWOpen::BlockStyler::SpecifyCSYS* specifyCsys_main;
    PWOpen::BlockStyler::SpecifyLocation* cursorLocation_main;
    PWOpen::BlockStyler::SpecifyOrientation* manipulator_main;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_main;
    PWOpen::BlockStyler::SetList* setList_main;
};
