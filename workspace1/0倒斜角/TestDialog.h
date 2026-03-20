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

    PWOpen::BlockStyler::Group* blockGroup_edge;
    PWOpen::BlockStyler::CurveCollector* curveCollector_edgeSelect;
    PWOpen::BlockStyler::Enumeration* enum_crossSection;
    PWOpen::BlockStyler::LinearDimension* blockLinear_distance;
    PWOpen::BlockStyler::LinearDimension* blockLinear_distance1;
    PWOpen::BlockStyler::LinearDimension* blockLinear_distance2;
    PWOpen::BlockStyler::AngularDimension* blockAngular_angle;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_direction;
    PWOpen::BlockStyler::SetList* setList_edgeList;
    PWOpen::BlockStyler::Group* blockGroup_lengthLimit;
    PWOpen::BlockStyler::Enumeration* enum_limitObject;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point;
    PWOpen::BlockStyler::SpecifyPlane* specifyPlane_plane;
    PWOpen::BlockStyler::FaceCollector* faceCollector_faceSelect;
    PWOpen::BlockStyler::CurveCollector* curveCollector_edgeSelect1;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_limitDirection;
    PWOpen::BlockStyler::Toggle* blockToggle_trimByLimit;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_trimPoint;
    PWOpen::BlockStyler::SetList* setList_limitList;
    PWOpen::BlockStyler::Group* blockGroup_settings;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_tolerance;
    PWOpen::BlockStyler::Enumeration* enum_offsetMethod;
    PWOpen::BlockStyler::Toggle* blockToggle_preview;
    PWOpen::BlockStyler::Button* blockButton_showResult;
};
