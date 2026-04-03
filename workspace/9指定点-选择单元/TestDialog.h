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

    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_origin;
    PWOpen::BlockStyler::SuperPoint* superPoint_point;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_direction;
    PWOpen::BlockStyler::SpecifyAxis* specifyAxis_axis;
    PWOpen::BlockStyler::SpecifyPlane* specifyPlane_basePlane;
    PWOpen::BlockStyler::SpecifyCSYS* specifyCsys_csys;
    PWOpen::BlockStyler::SpecifyLocation* cursorLocation_cursor;
    PWOpen::BlockStyler::SpecifyOrientation* manipulator_manip;
    PWOpen::BlockStyler::SelectPartFromList* selectPart_part;
    PWOpen::BlockStyler::SelectNode* selectNodes_nodes;
    PWOpen::BlockStyler::SelectElement* selectElements_elements;
};
