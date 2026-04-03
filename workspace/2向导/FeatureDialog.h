#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class FeatureDialog {
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    FeatureDialog();
    ~FeatureDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    // Control member variables
    PWOpen::BlockStyler::Wizard* wizard_main;
    PWOpen::BlockStyler::Label* blockLabel_stepOneTitle;
    PWOpen::BlockStyler::Button* blockButton_action;
    PWOpen::BlockStyler::Label* blockLabel_stepTwoTitle;
};
