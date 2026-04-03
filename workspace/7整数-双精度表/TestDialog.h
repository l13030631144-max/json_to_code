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

    PWOpen::BlockStyler::IntegerBlock* blockInteger_count;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_value;
    PWOpen::BlockStyler::ExpressionBlock* blockExpression_expr;
    PWOpen::BlockStyler::LinearDimension* blockLinear_length;
    PWOpen::BlockStyler::AngularDimension* blockAngular_angle;
    PWOpen::BlockStyler::RadiusDimension* blockRadius_radius;
    PWOpen::BlockStyler::OnPathDimension* onPathDim_distance;
    PWOpen::BlockStyler::IntegerTable* integerTable_data;
    PWOpen::BlockStyler::DoubleTable* doubleTable_data;
};
