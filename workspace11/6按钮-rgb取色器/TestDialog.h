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
    PWOpen::BlockStyler::Button*          blockButton_button;
    PWOpen::BlockStyler::UIBlock*         listBox_list;           // BlockListBox - missing header
    PWOpen::BlockStyler::Separator*       separator_main;
    PWOpen::BlockStyler::ObjectColorPicker* colorPicker_objectColor;
    PWOpen::BlockStyler::RGBColorPicker*  rgbColorPicker_rgb;
};
