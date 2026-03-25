#include "FeatureDatumPlaneDialog.h"
#include <PWOpen/IncludeHeader.h>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(FeatureDatumPlaneDialog::theSession) = nullptr;
PWOpen::UI *(FeatureDatumPlaneDialog::theUI) = nullptr;

FeatureDatumPlaneDialog::FeatureDatumPlaneDialog()
{
    try
    {
        FeatureDatumPlaneDialog::theSession = PWOpen::Session::GetSession();
        FeatureDatumPlaneDialog::theUI = PWOpen::UI::GetUI();
        theDialog = theUI->CreateDialog("FeatureDatumPlaneDialog.ui");
        theDialog->AddApplyHandler([this] { return apply_cb(); });
        theDialog->AddOkHandler([this] { return ok_cb(); });
        theDialog->AddUpdateHandler([this](auto && PH1) { return update_cb(std::forward<decltype(PH1)>(PH1)); });
        theDialog->AddInitializeHandler([this] { initialize_cb(); });
        theDialog->AddDialogShownHandler([this] { dialogShown_cb(); });
    }
    catch (std::exception& ex) { return; }
}

FeatureDatumPlaneDialog::~FeatureDatumPlaneDialog()
{
    if (theDialog) { delete theDialog; }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse FeatureDatumPlaneDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try { dialogResponse = theDialog->Launch(); }
    catch(std::exception& ex)
    { FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what()); }
    return dialogResponse;
}

void FeatureDatumPlaneDialog::initialize_cb()
{
    try
    {
        // Type selector
        enum_typeSelector = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));

        // group0: 要定义平面的对象
        blockGroup_objectsToDefine = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        selectObject_inferredObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject16"));

        // group1: 平面参考
        blockGroup_planeReference = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        selectObject_planeObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject0"));

        // group2: 通过轴
        blockGroup_throughAxis = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        selectObject_linearObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject1"));

        // group14: 第一平面
        blockGroup_firstPlane = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group14"));
        selectObject_firstPlaneObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject2"));

        // group15: 第二平面
        blockGroup_secondPlane = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group15"));
        selectObject_secondPlaneObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject14"));

        // group5: 曲线和点子类型
        blockGroup_curvePointSubtype = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group5"));
        enum_curvePointSubtype = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum1"));

        // group7: 参考几何体 (CurvePoint)
        blockGroup_referenceGeometry = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group7"));
        selectObject_inferObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject4"));
        specifyPoint_point0 = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
        specifyPoint_point1 = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point1"));
        specifyPoint_point2 = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point2"));
        selectObject_curveObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject5"));
        selectObject_parallelPlaneObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject6"));

        // group3: 第一条直线
        blockGroup_firstLine = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));
        selectObject_firstLinear = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject15"));

        // group4: 第二条直线
        blockGroup_secondLine = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group4"));
        selectObject_secondLinear = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject3"));

        // group6: 相切子类型
        blockGroup_tangentSubtype = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group6"));
        enum_tangentSubtype = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum2"));

        // group23: 参考几何体 (Tangent)
        blockGroup_tangentRefGeometry = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group23"));
        selectObject_tangentInferObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject7"));
        selectObject_tangentFace = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject8"));
        selectObject_tangentSecondFace = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject17"));
        specifyPoint_tangentPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point5"));
        selectObject_tangentFaceSel = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject9"));
        selectObject_tangentLinear = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject10"));
        selectObject_tangentAnglePlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject18"));

        // group22: 角度
        blockGroup_angle = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group22"));
        enum_angleOption = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum7"));
        angularDim_angle = dynamic_cast<PWOpen::BlockStyler::AngularDimension*>(theDialog->TopBlock()->FindBlock("angular_dim0"));

        // group8: 通过对象
        blockGroup_throughObject = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group8"));
        selectObject_coincidentObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject19"));

        // group9: 通过点
        blockGroup_throughPoint = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group9"));
        specifyPoint_originPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point3"));

        // group10: 法向
        blockGroup_normal = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group10"));
        specifyVector_normal = dynamic_cast<PWOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("vector0"));

        // group11: 曲线
        blockGroup_curve = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group11"));
        curveCollector_curve = dynamic_cast<PWOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("section0"));
        reverseDirection_curveDir = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction0"));

        // group12: 曲线上的位置
        blockGroup_positionOnCurve = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group12"));
        enum_positionMethod = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum3"));
        linearDim_arcLength = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim0"));
        expression_arcLengthPercent = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("expression0"));
        specifyPoint_positionPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point4"));

        // group13: 曲线上的方位
        blockGroup_orientationOnCurve = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group13"));
        enum_orientationMethod = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum4"));
        selectObject_projectObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject11"));
        specifyVector_orientVector = dynamic_cast<PWOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("vector1"));

        // group16: 偏置和参考
        blockGroup_offsetAndRef = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group16"));
        enum_csysRef = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum5"));
        linearDim_distance = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim1"));

        // group17: 系数
        blockGroup_coefficients = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group17"));
        enum_coeffCsys = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum6"));
        doubleBlock_coeffA = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double0"));
        doubleBlock_coeffB = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double1"));
        doubleBlock_coeffC = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double2"));
        linearDim_coeffD = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim2"));

        // group19: 平面方位 (most methods)
        blockGroup_planeOrientation = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group19"));
        reverseDirection_planeFlip = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction2"));

        // group18: 偏置 (most methods)
        blockGroup_offset = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group18"));
        toggle_offset = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        linearDim_offsetDistance = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim3"));

        // group24: 偏置 (Distance method)
        blockGroup_distanceOffset = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group24"));
        linearDim_distanceValue = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim4"));
        integerBlock_planeCount = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("integer1"));

        // group20: 平面方位 (Fixed/View methods)
        blockGroup_fixedPlaneOrientation = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group20"));
        reverseDirection_fixedFlip = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction3"));

        // group25: 平面方位 (Distance method)
        blockGroup_distancePlaneOrientation = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group25"));
        reverseDirection_distanceFlip = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction1"));

        // group21: 设置
        blockGroup_settings = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group21"));
        toggle_associative = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle1"));
    }
    catch(std::exception& ex)
    { FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what()); }
}

void FeatureDatumPlaneDialog::dialogShown_cb()
{
    try
    {
        // Trigger update for enum0 (top-level type selector)
        update_cb(enum_typeSelector);
        // Trigger update for enum1 (curve/point subtype)
        update_cb(enum_curvePointSubtype);
        // Trigger update for enum2 (tangent subtype)
        update_cb(enum_tangentSubtype);
        // Trigger update for enum3 (position on curve)
        update_cb(enum_positionMethod);
        // Trigger update for enum4 (orientation on curve)
        update_cb(enum_orientationMethod);
        // Trigger update for enum7 (angle option)
        update_cb(enum_angleOption);
        // Trigger update for toggle0 (offset toggle)
        update_cb(toggle_offset);
    }
    catch(std::exception& ex)
    { FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what()); }
}

int FeatureDatumPlaneDialog::apply_cb()
{
    int errorCode = 0;
    try {}
    catch(std::exception& ex)
    { errorCode = 1; FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what()); }
    return errorCode;
}

int FeatureDatumPlaneDialog::ok_cb()
{
    int errorCode = 0;
    try { errorCode = apply_cb(); }
    catch(std::exception& ex)
    { errorCode = 1; FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what()); }
    return errorCode;
}

int FeatureDatumPlaneDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        // --- enum0: top-level type selector ---
        if (block == enum_typeSelector)
        {
            PWOpen::PWString enum0Val = enum_typeSelector->GetProperties()->GetString("CurrentData");

            // group0: show for MethodTypeInferred
            blockGroup_objectsToDefine->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeInferred");

            // group1: show for MethodTypeDistance or MethodTypeAngle
            blockGroup_planeReference->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeDistance" || enum0Val == "MethodTypeAngle");

            // group2: show for MethodTypeAngle
            blockGroup_throughAxis->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeAngle");

            // group14: show for MethodTypeCenter
            blockGroup_firstPlane->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCenter");

            // group15: show for MethodTypeCenter
            blockGroup_secondPlane->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCenter");

            // group5: show for MethodTypeCurvePoint
            blockGroup_curvePointSubtype->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCurvePoint");

            // group7: show for MethodTypeCurvePoint
            blockGroup_referenceGeometry->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCurvePoint");

            // group3: show for MethodTypeTwoLines
            blockGroup_firstLine->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTwoLines");

            // group4: show for MethodTypeTwoLines
            blockGroup_secondLine->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTwoLines");

            // group6: show for MethodTypeTangent
            blockGroup_tangentSubtype->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTangent");

            // group23: show for MethodTypeTangent
            blockGroup_tangentRefGeometry->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTangent");

            // group22: show for MethodTypeAngle or MethodTypeTangent
            blockGroup_angle->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeAngle" || enum0Val == "MethodTypeTangent");

            // group8: show for MethodTypeCoincident
            blockGroup_throughObject->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCoincident");

            // group9: show for MethodTypePointDir
            blockGroup_throughPoint->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypePointDir");

            // group10: show for MethodTypePointDir
            blockGroup_normal->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypePointDir");

            // group11: show for MethodTypeFrenet
            blockGroup_curve->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFrenet");

            // group12: show for MethodTypeFrenet
            blockGroup_positionOnCurve->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFrenet");

            // group13: show for MethodTypeFrenet
            blockGroup_orientationOnCurve->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFrenet");

            // group16: show for MethodTypeFixedX, MethodTypeFixedY, MethodTypeFixedZ
            blockGroup_offsetAndRef->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFixedX" || enum0Val == "MethodTypeFixedY" || enum0Val == "MethodTypeFixedZ");

            // group17: show for MethodTypeCoefficients
            blockGroup_coefficients->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCoefficients");

            // group19: show for most methods (not fixed planes, not distance)
            blockGroup_planeOrientation->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeInferred" || enum0Val == "MethodTypeAngle" ||
                enum0Val == "MethodTypeCenter" || enum0Val == "MethodTypeCurvePoint" ||
                enum0Val == "MethodTypeTwoLines" || enum0Val == "MethodTypeTangent" ||
                enum0Val == "MethodTypeCoincident" || enum0Val == "MethodTypePointDir" ||
                enum0Val == "MethodTypeFrenet" || enum0Val == "MethodTypeCoefficients");

            // group18: show for most methods (not fixed planes, not distance)
            blockGroup_offset->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeInferred" || enum0Val == "MethodTypeAngle" ||
                enum0Val == "MethodTypeCenter" || enum0Val == "MethodTypeCurvePoint" ||
                enum0Val == "MethodTypeTwoLines" || enum0Val == "MethodTypeTangent" ||
                enum0Val == "MethodTypeCoincident" || enum0Val == "MethodTypePointDir" ||
                enum0Val == "MethodTypeFrenet");

            // group24: show for MethodTypeDistance
            blockGroup_distanceOffset->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeDistance");

            // group20: show for MethodTypeFixedX, MethodTypeFixedY, MethodTypeFixedZ, MethodTypeFixedView
            blockGroup_fixedPlaneOrientation->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFixedX" || enum0Val == "MethodTypeFixedY" ||
                enum0Val == "MethodTypeFixedZ" || enum0Val == "MethodTypeFixedView");

            // group25: show for MethodTypeDistance
            blockGroup_distancePlaneOrientation->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeDistance");

            // group21: enable for most methods (not fixed planes)
            blockGroup_settings->GetProperties()->SetLogical("Enable",
                enum0Val == "MethodTypeInferred" || enum0Val == "MethodTypeDistance" ||
                enum0Val == "MethodTypeAngle" || enum0Val == "MethodTypeCenter" ||
                enum0Val == "MethodTypeCurvePoint" || enum0Val == "MethodTypeTwoLines" ||
                enum0Val == "MethodTypeTangent" || enum0Val == "MethodTypeCoincident" ||
                enum0Val == "MethodTypePointDir" || enum0Val == "MethodTypeFrenet");
        }
        // --- enum1: curve/point subtype ---
        else if (block == enum_curvePointSubtype)
        {
            PWOpen::PWString enum1Val = enum_curvePointSubtype->GetProperties()->GetString("CurrentData");

            // selectobject4: show for MethodTypePointInfer
            selectObject_inferObject->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypePointInfer");

            // point0: show for MethodTypePoint, MethodTypeTwoPoints, MethodTypeThreePoints, MethodTypeCurvePoint, MethodTypeParallelPoint
            specifyPoint_point0->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypePoint" || enum1Val == "MethodTypeTwoPoints" ||
                enum1Val == "MethodTypeThreePoints" || enum1Val == "MethodTypeCurvePoint" ||
                enum1Val == "MethodTypeParallelPoint");

            // point1: show for MethodTypeTwoPoints, MethodTypeThreePoints
            specifyPoint_point1->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeTwoPoints" || enum1Val == "MethodTypeThreePoints");

            // point2: show for MethodTypeThreePoints
            specifyPoint_point2->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeThreePoints");

            // selectobject5: show for MethodTypeCurvePoint
            selectObject_curveObject->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeCurvePoint");

            // selectobject6: show for MethodTypeParallelPoint
            selectObject_parallelPlaneObject->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeParallelPoint");
        }
        // --- enum2: tangent subtype ---
        else if (block == enum_tangentSubtype)
        {
            PWOpen::PWString enum2Val = enum_tangentSubtype->GetProperties()->GetString("CurrentData");

            // selectobject7: show for MethodTypeTangentInfer
            selectObject_tangentInferObject->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangentInfer");

            // selectobject8: show for MethodTypeTangent, MethodTypeTangentPoint, MethodTypeTangentTwoFaces, MethodTypeTangentAnglePlane
            selectObject_tangentFace->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangent" || enum2Val == "MethodTypeTangentPoint" ||
                enum2Val == "MethodTypeTangentTwoFaces" || enum2Val == "MethodTypeTangentAnglePlane");

            // selectobject17: show for MethodTypeTangentTwoFaces
            selectObject_tangentSecondFace->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangentTwoFaces");

            // point5: show for MethodTypeTangentPoint
            specifyPoint_tangentPoint->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangentPoint");

            // selectobject9: show for MethodTypeTangentLinear
            selectObject_tangentFaceSel->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangentLinear");

            // selectobject10: show for MethodTypeTangentLinear
            selectObject_tangentLinear->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangentLinear");

            // selectobject18: show for MethodTypeTangentAnglePlane
            selectObject_tangentAnglePlane->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangentAnglePlane");
        }
        // --- enum3: position on curve ---
        else if (block == enum_positionMethod)
        {
            PWOpen::PWString enum3Val = enum_positionMethod->GetProperties()->GetString("CurrentData");

            // linear_dim0: show for SetExpression
            linearDim_arcLength->GetProperties()->SetLogical("Show",
                enum3Val == "SetExpression");

            // expression0: show for SetPercent
            expression_arcLengthPercent->GetProperties()->SetLogical("Show",
                enum3Val == "SetPercent");

            // point4: show for SetPoint
            specifyPoint_positionPoint->GetProperties()->SetLogical("Show",
                enum3Val == "SetPoint");
        }
        // --- enum4: orientation on curve ---
        else if (block == enum_orientationMethod)
        {
            PWOpen::PWString enum4Val = enum_orientationMethod->GetProperties()->GetString("CurrentData");

            // selectobject11: show for FrenetSubtypeProject
            selectObject_projectObject->GetProperties()->SetLogical("Show",
                enum4Val == "FrenetSubtypeProject");

            // vector1: show for FrenetSubtypeNormalToVector, FrenetSubtypeParallelToVector, FrenetSubtypeThruAxis
            specifyVector_orientVector->GetProperties()->SetLogical("Show",
                enum4Val == "FrenetSubtypeNormalToVector" || enum4Val == "FrenetSubtypeParallelToVector" ||
                enum4Val == "FrenetSubtypeThruAxis");
        }
        // --- enum7: angle option ---
        else if (block == enum_angleOption)
        {
            PWOpen::PWString enum7Val = enum_angleOption->GetProperties()->GetString("CurrentData");

            // angular_dim0: show for Value
            angularDim_angle->GetProperties()->SetLogical("Show",
                enum7Val == "Value");
        }
        // --- toggle0: offset toggle ---
        else if (block == toggle_offset)
        {
            bool toggle0Val = toggle_offset->GetProperties()->GetLogical("Value");

            // linear_dim3: show and enable based on toggle0
            linearDim_offsetDistance->GetProperties()->SetLogical("Show", toggle0Val);
            linearDim_offsetDistance->GetProperties()->SetLogical("Enable", true);
        }
    }
    catch(std::exception& ex)
    { FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what()); }
    return 0;
}

extern "C" DllExport void ufusr(char *param, int *retcod, int param_len)
{
    FeatureDatumPlaneDialog *dialog = NULL;
    try
    {
        dialog = new FeatureDatumPlaneDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    { FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what()); }
    if(dialog != NULL) { delete dialog; dialog = NULL; }
}
