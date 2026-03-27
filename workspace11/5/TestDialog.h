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
    PWOpen::BlockStyler::Enumeration*     enum_en;
    PWOpen::BlockStyler::Toggle*          blockToggle_t;
    PWOpen::BlockStyler::StringBlock*     blockString_s;
    PWOpen::BlockStyler::UIBlock*         multilineString_m;
    PWOpen::BlockStyler::Label*           blockLabel_l;
};
