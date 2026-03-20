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

    // Method type selector
    PWOpen::BlockStyler::Enumeration* enum_methodType;

    // Inferred plane
    PWOpen::BlockStyler::Group* blockGroup_inferObject;
    PWOpen::BlockStyler::SelectObject* selectObject_inferObject;

    // Plane reference (Distance / Angle)
    PWOpen::BlockStyler::Group* blockGroup_planeReference;
    PWOpen::BlockStyler::SelectObject* selectObject_planeRef;

    // Through axis (Angle)
    PWOpen::BlockStyler::Group* blockGroup_throughAxis;
    PWOpen::BlockStyler::SelectObject* selectObject_axisLinear;

    // Center bisect planes
    PWOpen::BlockStyler::Group* blockGroup_firstPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_firstPlane;
    PWOpen::BlockStyler::Group* blockGroup_secondPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_secondPlane;

    // Curve and point subtype
    PWOpen::BlockStyler::Group* blockGroup_curvePointSubtype;
    PWOpen::BlockStyler::Enumeration* enum_curvePointSubtype;

    // Reference geometry for curve point
    PWOpen::BlockStyler::Group* blockGroup_refGeomCurvePoint;
    PWOpen::BlockStyler::SelectObject* selectObject_pointInfer;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point1;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point2;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point3;
    PWOpen::BlockStyler::SelectObject* selectObject_curveForPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_planeForPoint;

    // Two lines
    PWOpen::BlockStyler::Group* blockGroup_firstLine;
    PWOpen::BlockStyler::SelectObject* selectObject_firstLine;
    PWOpen::BlockStyler::Group* blockGroup_secondLine;
    PWOpen::BlockStyler::SelectObject* selectObject_secondLine;

    // Tangent subtype
    PWOpen::BlockStyler::Group* blockGroup_tangentSubtype;
    PWOpen::BlockStyler::Enumeration* enum_tangentSubtype;

    // Reference geometry for tangent
    PWOpen::BlockStyler::Group* blockGroup_refGeomTangent;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentInfer;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentFace;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentFace2;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_tangentPoint;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentLinearFace;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentLinear;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentAnglePlane;

    // Angle group
    PWOpen::BlockStyler::Group* blockGroup_angle;
    PWOpen::BlockStyler::Enumeration* enum_angleOption;
    PWOpen::BlockStyler::AngularDimension* blockAngular_angle;

    // Coincident object
    PWOpen::BlockStyler::Group* blockGroup_coincidentObject;
    PWOpen::BlockStyler::SelectObject* selectObject_coincidentObj;

    // Point and direction
    PWOpen::BlockStyler::Group* blockGroup_pointDir;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_pointDir;
    PWOpen::BlockStyler::Group* blockGroup_normalDir;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_normalDir;

    // Curve on path (Frenet)
    PWOpen::BlockStyler::Group* blockGroup_curve;
    PWOpen::BlockStyler::SectionBuilder* sectionBuilder_curve;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_curveDir;

    // Curve position (Frenet)
    PWOpen::BlockStyler::Group* blockGroup_curvePosition;
    PWOpen::BlockStyler::Enumeration* enum_positionMethod;
    PWOpen::BlockStyler::LinearDimension* blockLinear_arcLength;
    PWOpen::BlockStyler::ExpressionBlock* blockExpression_arcPercent;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_frenetPoint;

    // Frenet orientation
    PWOpen::BlockStyler::Group* blockGroup_frenetOrientation;
    PWOpen::BlockStyler::Enumeration* enum_frenetDirection;
    PWOpen::BlockStyler::SelectObject* selectObject_frenetProject;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_frenetVector;

    // Fixed plane offset and reference
    PWOpen::BlockStyler::Group* blockGroup_offsetReference;
    PWOpen::BlockStyler::Enumeration* enum_coordSystem;
    PWOpen::BlockStyler::LinearDimension* blockLinear_fixedDistance;

    // Coefficients
    PWOpen::BlockStyler::Group* blockGroup_coefficients;
    PWOpen::BlockStyler::Enumeration* enum_coeffCoordSystem;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_coeffA;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_coeffB;
    PWOpen::BlockStyler::DoubleBlock* blockDouble_coeffC;
    PWOpen::BlockStyler::LinearDimension* blockLinear_coeffD;

    // Plane orientation (most methods)
    PWOpen::BlockStyler::Group* blockGroup_planeOrientation;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_planeOrient;

    // Offset (most methods)
    PWOpen::BlockStyler::Group* blockGroup_offset;
    PWOpen::BlockStyler::Toggle* blockToggle_offset;
    PWOpen::BlockStyler::LinearDimension* blockLinear_offsetDist;

    // Distance offset
    PWOpen::BlockStyler::Group* blockGroup_distanceOffset;
    PWOpen::BlockStyler::LinearDimension* blockLinear_distanceDist;
    PWOpen::BlockStyler::IntegerBlock* blockInteger_planeCount;

    // Fixed plane orientation
    PWOpen::BlockStyler::Group* blockGroup_fixedPlaneOrient;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_fixedOrient;

    // Distance plane orientation
    PWOpen::BlockStyler::Group* blockGroup_distancePlaneOrient;
    PWOpen::BlockStyler::ReverseDirection* reverseDir_distanceOrient;

    // Settings
    PWOpen::BlockStyler::Group* blockGroup_settings;
    PWOpen::BlockStyler::Toggle* blockToggle_associative;
};
