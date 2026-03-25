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

    // enum0: type selector
    PWOpen::BlockStyler::Enumeration* enum_typeSelector;

    // group0: 要定义平面的对象
    PWOpen::BlockStyler::Group* blockGroup_objectsToDefine;
    PWOpen::BlockStyler::SelectObject* selectObject_inferredObject;

    // group1: 平面参考
    PWOpen::BlockStyler::Group* blockGroup_planeReference;
    PWOpen::BlockStyler::SelectObject* selectObject_planeObject;

    // group2: 通过轴
    PWOpen::BlockStyler::Group* blockGroup_throughAxis;
    PWOpen::BlockStyler::SelectObject* selectObject_linearObject;

    // group14: 第一平面
    PWOpen::BlockStyler::Group* blockGroup_firstPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_firstPlaneObject;

    // group15: 第二平面
    PWOpen::BlockStyler::Group* blockGroup_secondPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_secondPlaneObject;

    // group5: 曲线和点子类型
    PWOpen::BlockStyler::Group* blockGroup_curvePointSubtype;
    PWOpen::BlockStyler::Enumeration* enum_curvePointSubtype;

    // group7: 参考几何体 (CurvePoint)
    PWOpen::BlockStyler::Group* blockGroup_referenceGeometry;
    PWOpen::BlockStyler::SelectObject* selectObject_inferObject;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point0;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point1;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_point2;
    PWOpen::BlockStyler::SelectObject* selectObject_curveObject;
    PWOpen::BlockStyler::SelectObject* selectObject_parallelPlaneObject;

    // group3: 第一条直线
    PWOpen::BlockStyler::Group* blockGroup_firstLine;
    PWOpen::BlockStyler::SelectObject* selectObject_firstLinear;

    // group4: 第二条直线
    PWOpen::BlockStyler::Group* blockGroup_secondLine;
    PWOpen::BlockStyler::SelectObject* selectObject_secondLinear;

    // group6: 相切子类型
    PWOpen::BlockStyler::Group* blockGroup_tangentSubtype;
    PWOpen::BlockStyler::Enumeration* enum_tangentSubtype;

    // group23: 参考几何体 (Tangent)
    PWOpen::BlockStyler::Group* blockGroup_tangentRefGeometry;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentInferObject;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentFace;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentSecondFace;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_tangentPoint;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentFaceSel;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentLinear;
    PWOpen::BlockStyler::SelectObject* selectObject_tangentAnglePlane;

    // group22: 角度
    PWOpen::BlockStyler::Group* blockGroup_angle;
    PWOpen::BlockStyler::Enumeration* enum_angleOption;
    PWOpen::BlockStyler::AngularDimension* angularDim_angle;

    // group8: 通过对象
    PWOpen::BlockStyler::Group* blockGroup_throughObject;
    PWOpen::BlockStyler::SelectObject* selectObject_coincidentObject;

    // group9: 通过点
    PWOpen::BlockStyler::Group* blockGroup_throughPoint;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_originPoint;

    // group10: 法向
    PWOpen::BlockStyler::Group* blockGroup_normal;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_normal;

    // group11: 曲线
    PWOpen::BlockStyler::Group* blockGroup_curve;
    PWOpen::BlockStyler::CurveCollector* curveCollector_curve;
    PWOpen::BlockStyler::ReverseDirection* reverseDirection_curveDir;

    // group12: 曲线上的位置
    PWOpen::BlockStyler::Group* blockGroup_positionOnCurve;
    PWOpen::BlockStyler::Enumeration* enum_positionMethod;
    PWOpen::BlockStyler::LinearDimension* linearDim_arcLength;
    PWOpen::BlockStyler::ExpressionBlock* expression_arcLengthPercent;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_positionPoint;

    // group13: 曲线上的方位
    PWOpen::BlockStyler::Group* blockGroup_orientationOnCurve;
    PWOpen::BlockStyler::Enumeration* enum_orientationMethod;
    PWOpen::BlockStyler::SelectObject* selectObject_projectObject;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_orientVector;

    // group16: 偏置和参考
    PWOpen::BlockStyler::Group* blockGroup_offsetAndRef;
    PWOpen::BlockStyler::Enumeration* enum_csysRef;
    PWOpen::BlockStyler::LinearDimension* linearDim_distance;

    // group17: 系数
    PWOpen::BlockStyler::Group* blockGroup_coefficients;
    PWOpen::BlockStyler::Enumeration* enum_coeffCsys;
    PWOpen::BlockStyler::DoubleBlock* doubleBlock_coeffA;
    PWOpen::BlockStyler::DoubleBlock* doubleBlock_coeffB;
    PWOpen::BlockStyler::DoubleBlock* doubleBlock_coeffC;
    PWOpen::BlockStyler::LinearDimension* linearDim_coeffD;

    // group19: 平面方位 (most methods)
    PWOpen::BlockStyler::Group* blockGroup_planeOrientation;
    PWOpen::BlockStyler::ReverseDirection* reverseDirection_planeFlip;

    // group18: 偏置 (most methods)
    PWOpen::BlockStyler::Group* blockGroup_offset;
    PWOpen::BlockStyler::Toggle* toggle_offset;
    PWOpen::BlockStyler::LinearDimension* linearDim_offsetDistance;

    // group24: 偏置 (Distance method)
    PWOpen::BlockStyler::Group* blockGroup_distanceOffset;
    PWOpen::BlockStyler::LinearDimension* linearDim_distanceValue;
    PWOpen::BlockStyler::IntegerBlock* integerBlock_planeCount;

    // group20: 平面方位 (Fixed/View methods)
    PWOpen::BlockStyler::Group* blockGroup_fixedPlaneOrientation;
    PWOpen::BlockStyler::ReverseDirection* reverseDirection_fixedFlip;

    // group25: 平面方位 (Distance method)
    PWOpen::BlockStyler::Group* blockGroup_distancePlaneOrientation;
    PWOpen::BlockStyler::ReverseDirection* reverseDirection_distanceFlip;

    // group21: 设置
    PWOpen::BlockStyler::Group* blockGroup_settings;
    PWOpen::BlockStyler::Toggle* toggle_associative;
};
