#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class FeatureDatumPlaneDialog {
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    FeatureDatumPlaneDialog();
    ~FeatureDatumPlaneDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    PWOpen::BlockStyler::Enumeration* enum_methodType;

    PWOpen::BlockStyler::Group* blockGroup_inferredReference;
    PWOpen::BlockStyler::SelectObject* selectObject_inferredObj;

    PWOpen::BlockStyler::Group* blockGroup_planeRef;
    PWOpen::BlockStyler::SelectObject* selectObject_planeRef;

    PWOpen::BlockStyler::Group* blockGroup_axisRef;
    PWOpen::BlockStyler::SelectObject* selectObject_linearObj;

    PWOpen::BlockStyler::Group* blockGroup_firstPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_firstPlane;

    PWOpen::BlockStyler::Group* blockGroup_secondPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_secondPlane;

    PWOpen::BlockStyler::Group* blockGroup_curvePointSubtype;
    PWOpen::BlockStyler::Enumeration* enum_curvePointSubtype;

    PWOpen::BlockStyler::Group* blockGroup_curveRefGeometry;
    PWOpen::BlockStyler::SelectObject* selectObject_curveRefObj;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_first;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_second;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_third;
    PWOpen::BlockStyler::SelectObject* selectObject_curveObj;
    PWOpen::BlockStyler::SelectObject* selectObject_planeObj;

    PWOpen::BlockStyler::Group* blockGroup_firstLine;
    PWOpen::BlockStyler::SelectObject* selectObject_firstLine;

    PWOpen::BlockStyler::Group* blockGroup_secondLine;
    PWOpen::BlockStyler::SelectObject* selectObject_secondLine;

    PWOpen::BlockStyler::Group* blockGroup_tangentSubtype;
    PWOpen::BlockStyler::Enumeration* enum_tangentSubtype;

    PWOpen::BlockStyler::Group* blockGroup_tangentRefGeometry;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentObj;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentFace1;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentFace2;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_tangent;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentFaceSel;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentLinear;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentPlane;

    PWOpen::BlockStyler::Group* blockGroup_angle;
    PWOpen::BlockStyler::Enumeration* enum_angleOption;
    PWOpen::BlockStyler::AngularDimension* blockAngular_angle;

    PWOpen::BlockStyler::Group* blockGroup_throughObj;
    PWOpen::BlockStyler::SelectObject* selectObject_throughObj;

    PWOpen::BlockStyler::Group* blockGroup_throughPoint;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_pointDir;

    PWOpen::BlockStyler::Group* blockGroup_normalDir;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_normal;

    PWOpen::BlockStyler::Group* blockGroup_curve;
    PWOpen::BlockStyler::SectionBuilder* sectionBuilder_curveSelect;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_curve;

    PWOpen::BlockStyler::Group* blockGroup_positionOnCurve;
    PWOpen::BlockStyler::Enumeration* enum_curvePosition;
    PWOpen::BlockStyler::LinearDimension* blockLinear_arcLength;
    PWOpen::BlockStyler::ExpressionBlock* blockExpression_arcPercent;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_onCurve;

    PWOpen::BlockStyler::Group* blockGroup_orientOnCurve;
    PWOpen::BlockStyler::Enumeration* enum_frenetOrientation;
    PWOpen::BlockStyler::SelectObject* selectObject_orientObj;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_frenet;

    PWOpen::BlockStyler::Group* blockGroup_offsetAndRef;
    PWOpen::BlockStyler::Enumeration* enum_coordSys;
    PWOpen::BlockStyler::LinearDimension* blockLinear_offsetDist;

    PWOpen::BlockStyler::Group* blockGroup_coefficients;
    PWOpen::BlockStyler::Enumeration* enum_coeffCoordSys;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_coeffA;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_coeffB;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_coeffC;
    PWOpen::BlockStyler::LinearDimension* blockLinear_coeffD;

    PWOpen::BlockStyler::Group* blockGroup_planeOrientGeneral;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_general;

    PWOpen::BlockStyler::Group* blockGroup_offsetBias;
    PWOpen::BlockStyler::Toggle* blockToggle_offsetEnable;
    PWOpen::BlockStyler::LinearDimension* blockLinear_biasDist;

    PWOpen::BlockStyler::Group* blockGroup_distanceOffset;
    PWOpen::BlockStyler::LinearDimension* blockLinear_multiDist;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_planeCount;

    PWOpen::BlockStyler::Group* blockGroup_planeOrientFixed;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_fixed;

    PWOpen::BlockStyler::Group* blockGroup_planeOrientDistance;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_distance;

    PWOpen::BlockStyler::Group* blockGroup_settings;
    PWOpen::BlockStyler::Toggle* blockToggle_associative;
};
