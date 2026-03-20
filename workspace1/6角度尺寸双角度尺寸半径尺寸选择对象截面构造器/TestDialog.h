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
    PWOpen::BlockStyler::AngularDimension* blockAngular_main;
    PWOpen::BlockStyler::UIBlock* doubleAngular_main;
    PWOpen::BlockStyler::RadiusDimension* blockRadius_main;
    PWOpen::BlockStyler::SelectObject* selectObject_main;
    PWOpen::BlockStyler::SuperSection* superSection_main;
};
