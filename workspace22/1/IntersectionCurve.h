#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class IntersectionCurve
{
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    IntersectionCurve();
    ~IntersectionCurve();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    PWOpen::BlockStyler::Group* blockGroup_firstInput;
    PWOpen::BlockStyler::FaceCollector* faceCollector_firstFace;
    PWOpen::BlockStyler::SpecifyPlane* specifyPlane_firstPlane;
    PWOpen::BlockStyler::Toggle* blockToggle_keepFirstSelected;
    PWOpen::BlockStyler::Group* blockGroup_secondInput;
    PWOpen::BlockStyler::FaceCollector* faceCollector_secondFace;
    PWOpen::BlockStyler::SpecifyPlane* specifyPlane_secondPlane;
    PWOpen::BlockStyler::Toggle* blockToggle_keepSecondSelected;
    PWOpen::BlockStyler::Group* blockGroup_settings;
    PWOpen::BlockStyler::Toggle* blockToggle_associative;
    PWOpen::BlockStyler::Toggle* blockToggle_advancedCurveFitting;
    PWOpen::BlockStyler::Enumeration* enum_method;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_degree;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_segments;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_minDegree;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_maxDegree;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_maxSegments;
    PWOpen::BlockStyler::LinearDimension* blockLinear_distanceTolerance;
};
