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
    PWOpen::BlockStyler::IntegerBlock*      blockInteger_integer;
    PWOpen::BlockStyler::DoubleBlock*       blockDouble_double;
    PWOpen::BlockStyler::ExpressionBlock*   blockExpression_expression;
    PWOpen::BlockStyler::LinearDimension*   blockLinear_linearDim;
    PWOpen::BlockStyler::AngularDimension*  blockAngular_angularDim;
    PWOpen::BlockStyler::RadiusDimension*   blockRadius_radiusDim;
};
