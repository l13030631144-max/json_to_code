#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class FeatureBlockDialog {
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    FeatureBlockDialog();
    ~FeatureBlockDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    // Control member variables
    PWOpen::BlockStyler::Enumeration* enum_enum0;
    PWOpen::BlockStyler::Group* blockGroup_原点;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point0;
    PWOpen::BlockStyler::Group* blockGroup_从原点出发的点_XC__YC;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point1;
    PWOpen::BlockStyler::Group* blockGroup_从原点出发的点_XC__YC__ZC;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point;
    PWOpen::BlockStyler::Group* blockGroup_维度;
    PWOpen::BlockStyler::LinearDimension* blockLinear_高度_ZC_;
    PWOpen::BlockStyler::Group* blockGroup_尺寸;
    PWOpen::BlockStyler::LinearDimension* blockLinear_长度__XC_;
    PWOpen::BlockStyler::LinearDimension* blockLinear_宽度__YC_;
    PWOpen::BlockStyler::LinearDimension* blockLinear_高度__ZC_;
    PWOpen::BlockStyler::Group* blockGroup_布尔;
    PWOpen::BlockStyler::Enumeration* enum_布尔;
    PWOpen::BlockStyler::BodyCollector* bodyCollector_bodySelect0;
    PWOpen::BlockStyler::Group* blockGroup_设置;
    PWOpen::BlockStyler::Toggle* blockToggle_关联原点;
    PWOpen::BlockStyler::Group* blockGroup_设置;
    PWOpen::BlockStyler::Toggle* blockToggle_关联原点和偏置;
};
