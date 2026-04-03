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

    // Control member variables
    PWOpen::BlockStyler::Explorer* explorer_main;
    PWOpen::BlockStyler::Label* blockLabel_nodeOneInfo;
    PWOpen::BlockStyler::Toggle* blockToggle_nodeOneToggle;
    PWOpen::BlockStyler::Toggle* blockToggle_nodeTwoToggle;
    PWOpen::BlockStyler::ScrolledWindow* scrollWindow_main;
    PWOpen::BlockStyler::Label* blockLabel_scrollFirst;
    PWOpen::BlockStyler::Label* blockLabel_scrollSecond;
    PWOpen::BlockStyler::Label* blockLabel_scrollThird;
    PWOpen::BlockStyler::Toggle* blockToggle_scrollToggle;
};
