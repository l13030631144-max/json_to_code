#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class BridgeSurfaceDialog
{
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    BridgeSurfaceDialog();
    ~BridgeSurfaceDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    PWOpen::BlockStyler::Group* blockGroup_edgeGroup;
    PWOpen::BlockStyler::Enumeration* enum_firstObjectTypeEnum;
    PWOpen::BlockStyler::SelectObject* selectObject_selectFirstEdge;
    PWOpen::BlockStyler::SelectObject* selectObject_selectFirstCurve;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_reverseFirstDirection;
    PWOpen::BlockStyler::Separator* separator_lineSeparator;
    PWOpen::BlockStyler::Enumeration* enum_secondObjectTypeEnum;
    PWOpen::BlockStyler::SelectObject* selectObject_selectSecondEdge;
    PWOpen::BlockStyler::SelectObject* selectObject_selectSecondCurve;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_reverseSecondDirection;
    PWOpen::BlockStyler::Group* blockGroup_shapeGroup;
    PWOpen::BlockStyler::Enumeration* enum_firstEdgeContinuityEnum;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_reverseFirstEdgeDir;
    PWOpen::BlockStyler::Enumeration* enum_secondEdgeContinuityEnum;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_reverseSecondEdgeDir;
    PWOpen::BlockStyler::Enumeration* enum_flowDirectionEnum;
    PWOpen::BlockStyler::Enumeration* enum_tangencyControlEnum;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_firstEdgeConstantExpr;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_secondEdgeConstantExpr;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_firstEdgeStartExpr;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_firstEdgeEndExpr;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_secondEdgeStartExpr;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_secondEdgeEndExpr;
    PWOpen::BlockStyler::Group* blockGroup_edgeLimitGroup;
    PWOpen::BlockStyler::Toggle* blockToggle_linkStartHandleToggle;
    PWOpen::BlockStyler::Toggle* blockToggle_linkEndHandleToggle;
    PWOpen::BlockStyler::TabControl* tabControl_blockTabWidget0;
    PWOpen::BlockStyler::UIBlock* tabPage_blockTabPage0;
    PWOpen::BlockStyler::Toggle* blockToggle_toggle0;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression3;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression4;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression7;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression8;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression5;
    PWOpen::BlockStyler::UIBlock* tabPage_blockTabPage2;
    PWOpen::BlockStyler::Toggle* blockToggle_endpointToEndpointToggle;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression0;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression1;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression9;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression10;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression2;
    PWOpen::BlockStyler::Group* blockGroup_settingGroup;
    PWOpen::BlockStyler::TabControl* tabControl_blockTabWidget1;
    PWOpen::BlockStyler::UIBlock* tabPage_blockTabPage1;
    PWOpen::BlockStyler::Enumeration* enum_enum0;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_integer0;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_integer1;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_integer2;
    PWOpen::BlockStyler::Group* blockGroup_group0;
    PWOpen::BlockStyler::LinearDimension* blockLinear_linearDim0;
    PWOpen::BlockStyler::AngularDimension* blockAngular_angularDim0;
    PWOpen::BlockStyler::ExpressionBlock* blockExpr_expression6;
};
