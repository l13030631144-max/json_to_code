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
    PWOpen::BlockStyler::Group* blockGroup_main;
    PWOpen::BlockStyler::Table* blockTable_layout;
    PWOpen::BlockStyler::Label* blockLabel_title;
    PWOpen::BlockStyler::TabControl* tabControl_main;
    PWOpen::BlockStyler::Toggle* blockToggle_toggle;
    PWOpen::BlockStyler::Label* blockLabel_info;
};
