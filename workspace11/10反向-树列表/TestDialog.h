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
    PWOpen::BlockStyler::ReverseDirection* reverseDir_direction;
    PWOpen::BlockStyler::SetList* setList_objectList;
    PWOpen::BlockStyler::Microposition* microposition_position;
    PWOpen::BlockStyler::UIBlock* selectExpression_expr;
    PWOpen::BlockStyler::Tree* blockTree_tree;
};
