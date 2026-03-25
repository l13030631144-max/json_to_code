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
    catch (std::exception& ex)
    {
        return;
    }
}

FeatureDatumPlaneDialog::~FeatureDatumPlaneDialog()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse FeatureDatumPlaneDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}

void FeatureDatumPlaneDialog::initialize_cb()
{
    try
    {
        enum_methodType = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));

        blockGroup_inferredReference = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        selectObject_inferredObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject16"));

        blockGroup_planeRef = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        selectObject_planeRef = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject0"));

        blockGroup_axisRef = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        selectObject_linearObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject1"));

        blockGroup_firstPlane = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group14"));
        selectObject_firstPlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject2"));

        blockGroup_secondPlane = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group15"));
        selectObject_secondPlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject14"));

        blockGroup_curvePointSubtype = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group5"));
        enum_curvePointSubtype = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum1"));

        blockGroup_curveRefGeometry = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group7"));
        selectObject_curveRefObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject4"));
        specifyPoint_first = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
        specifyPoint_second = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point1"));
        specifyPoint_third = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point2"));
        selectObject_curveObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject5"));
        selectObject_planeObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject6"));

        blockGroup_firstLine = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));
        selectObject_firstLine = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject15"));

        blockGroup_secondLine = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group4"));
        selectObject_secondLine = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject3"));

        blockGroup_tangentSubtype = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group6"));
        enum_tangentSubtype = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum2"));

        blockGroup_tangentRefGeometry = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group23"));
        selectObject_tangentObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject7"));
        selectObject_tangentFace1 = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject8"));
        selectObject_tangentFace2 = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject17"));
        specifyPoint_tangent = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point5"));
        selectObject_tangentFaceSel = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject9"));
        selectObject_tangentLinear = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject10"));
        selectObject_tangentPlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject18"));

        blockGroup_angle = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group22"));
        enum_angleOption = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum7"));
        blockAngular_angle = dynamic_cast<PWOpen::BlockStyler::AngularDimension*>(theDialog->TopBlock()->FindBlock("angular_dim0"));

        blockGroup_throughObj = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group8"));
        selectObject_throughObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject19"));

        blockGroup_throughPoint = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group9"));
        specifyPoint_pointDir = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point3"));

        blockGroup_normalDir = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group10"));
        specifyVector_normal = dynamic_cast<PWOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("vector0"));

        blockGroup_curve = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group11"));
        sectionBuilder_curveSelect = dynamic_cast<PWOpen::BlockStyler::SectionBuilder*>(theDialog->TopBlock()->FindBlock("section0"));
        reverseDir_curve = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction0"));

        blockGroup_positionOnCurve = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group12"));
        enum_curvePosition = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum3"));
        blockLinear_arcLength = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim0"));
        blockExpression_arcPercent = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("expression0"));
        specifyPoint_onCurve = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point4"));

        blockGroup_orientOnCurve = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group13"));
        enum_frenetOrientation = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum4"));
        selectObject_orientObj = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject11"));
        specifyVector_frenet = dynamic_cast<PWOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("vector1"));

        blockGroup_offsetAndRef = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group16"));
        enum_coordSys = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum5"));
        blockLinear_offsetDist = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim1"));

        blockGroup_coefficients = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group17"));
        enum_coeffCoordSys = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum6"));
        blockDouble_coeffA = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double0"));
        blockDouble_coeffB = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double1"));
        blockDouble_coeffC = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double2"));
        blockLinear_coeffD = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim2"));

        blockGroup_planeOrientGeneral = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group19"));
        reverseDir_general = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction2"));

        blockGroup_offsetBias = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group18"));
        blockToggle_offsetEnable = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        blockLinear_biasDist = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim3"));

        blockGroup_distanceOffset = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group24"));
        blockLinear_multiDist = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim4"));
        blockInteger_planeCount = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("integer1"));

        blockGroup_planeOrientFixed = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group20"));
        reverseDir_fixed = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction3"));

        blockGroup_planeOrientDistance = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group25"));
        reverseDir_distance = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction1"));

        blockGroup_settings = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group21"));
        blockToggle_associative = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle1"));
    }
    catch(std::exception& ex)
    {
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}

void FeatureDatumPlaneDialog::dialogShown_cb()
{
    try
    {
        update_cb(enum_methodType);
        update_cb(enum_curvePointSubtype);
        update_cb(enum_tangentSubtype);
        update_cb(enum_curvePosition);
        update_cb(enum_frenetOrientation);
        update_cb(enum_angleOption);
        update_cb(blockToggle_offsetEnable);
    }
    catch(std::exception& ex)
    {
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}

int FeatureDatumPlaneDialog::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int FeatureDatumPlaneDialog::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int FeatureDatumPlaneDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == enum_methodType)
        {
            PWOpen::PWString enum0Val = enum_methodType->GetProperties()->GetString("CurrentData");

            blockGroup_inferredReference->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeInferred");

            blockGroup_planeRef->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeDistance" || enum0Val == "MethodTypeAngle");

            blockGroup_axisRef->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeAngle");

            blockGroup_firstPlane->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCenter");

            blockGroup_secondPlane->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCenter");

            blockGroup_curvePointSubtype->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCurvePoint");

            blockGroup_curveRefGeometry->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCurvePoint");

            blockGroup_firstLine->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTwoLines");

            blockGroup_secondLine->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTwoLines");

            blockGroup_tangentSubtype->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTangent");

            blockGroup_tangentRefGeometry->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeTangent");

            blockGroup_angle->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeAngle" || enum0Val == "MethodTypeTangent");

            blockGroup_throughObj->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCoincident");

            blockGroup_throughPoint->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypePointDir");

            blockGroup_normalDir->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypePointDir");

            blockGroup_curve->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFrenet");

            blockGroup_positionOnCurve->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFrenet");

            blockGroup_orientOnCurve->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFrenet");

            blockGroup_offsetAndRef->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFixedX" || enum0Val == "MethodTypeFixedY" || enum0Val == "MethodTypeFixedZ");

            blockGroup_coefficients->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeCoefficients");

            blockGroup_planeOrientGeneral->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeInferred" || enum0Val == "MethodTypeAngle" ||
                enum0Val == "MethodTypeCenter" || enum0Val == "MethodTypeCurvePoint" ||
                enum0Val == "MethodTypeTwoLines" || enum0Val == "MethodTypeTangent" ||
                enum0Val == "MethodTypeCoincident" || enum0Val == "MethodTypePointDir" ||
                enum0Val == "MethodTypeFrenet" || enum0Val == "MethodTypeCoefficients");

            blockGroup_offsetBias->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeInferred" || enum0Val == "MethodTypeAngle" ||
                enum0Val == "MethodTypeCenter" || enum0Val == "MethodTypeCurvePoint" ||
                enum0Val == "MethodTypeTwoLines" || enum0Val == "MethodTypeTangent" ||
                enum0Val == "MethodTypeCoincident" || enum0Val == "MethodTypePointDir" ||
                enum0Val == "MethodTypeFrenet");

            blockGroup_distanceOffset->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeDistance");

            blockGroup_planeOrientFixed->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeFixedX" || enum0Val == "MethodTypeFixedY" ||
                enum0Val == "MethodTypeFixedZ" || enum0Val == "MethodTypeFixedView");

            blockGroup_planeOrientDistance->GetProperties()->SetLogical("Show",
                enum0Val == "MethodTypeDistance");

            blockGroup_settings->GetProperties()->SetLogical("Enable",
                enum0Val == "MethodTypeInferred" || enum0Val == "MethodTypeDistance" ||
                enum0Val == "MethodTypeAngle" || enum0Val == "MethodTypeCenter" ||
                enum0Val == "MethodTypeCurvePoint" || enum0Val == "MethodTypeTwoLines" ||
                enum0Val == "MethodTypeTangent" || enum0Val == "MethodTypeCoincident" ||
                enum0Val == "MethodTypePointDir" || enum0Val == "MethodTypeFrenet");
        }
        else if (block == enum_curvePointSubtype)
        {
            PWOpen::PWString enum1Val = enum_curvePointSubtype->GetProperties()->GetString("CurrentData");

            selectObject_curveRefObj->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypePointInfer");

            specifyPoint_first->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypePoint" || enum1Val == "MethodTypeTwoPoints" ||
                enum1Val == "MethodTypeThreePoints" || enum1Val == "MethodTypeCurvePoint" ||
                enum1Val == "MethodTypeParallelPoint");

            specifyPoint_second->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeTwoPoints" || enum1Val == "MethodTypeThreePoints");

            specifyPoint_third->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeThreePoints");

            selectObject_curveObj->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeCurvePoint");

            selectObject_planeObj->GetProperties()->SetLogical("Show",
                enum1Val == "MethodTypeParallelPoint");
        }
        else if (block == enum_tangentSubtype)
        {
            PWOpen::PWString enum2Val = enum_tangentSubtype->GetProperties()->GetString("CurrentData");

            specifyPoint_tangent->GetProperties()->SetLogical("Show",
                enum2Val == "MethodTypeTangentPoint");
        }
        else if (block == enum_curvePosition)
        {
            PWOpen::PWString enum3Val = enum_curvePosition->GetProperties()->GetString("CurrentData");

            blockLinear_arcLength->GetProperties()->SetLogical("Show",
                enum3Val == "SetExpression");

            blockExpression_arcPercent->GetProperties()->SetLogical("Show",
                enum3Val == "SetPercent");

            specifyPoint_onCurve->GetProperties()->SetLogical("Show",
                enum3Val == "SetPoint");
        }
        else if (block == enum_frenetOrientation)
        {
            PWOpen::PWString enum4Val = enum_frenetOrientation->GetProperties()->GetString("CurrentData");

            specifyVector_frenet->GetProperties()->SetLogical("Show",
                enum4Val == "FrenetSubtypeNormalToVector" || enum4Val == "FrenetSubtypeParallelToVector" ||
                enum4Val == "FrenetSubtypeThruAxis");
        }
        else if (block == enum_angleOption)
        {
            PWOpen::PWString enum7Val = enum_angleOption->GetProperties()->GetString("CurrentData");

            blockAngular_angle->GetProperties()->SetLogical("Show",
                enum7Val == "Value");
        }
        else if (block == blockToggle_offsetEnable)
        {
            bool toggle0Val = blockToggle_offsetEnable->GetProperties()->GetLogical("Value");

            blockLinear_biasDist->GetProperties()->SetLogical("Show", toggle0Val);
        }
    }
    catch(std::exception& ex)
    {
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    FeatureDatumPlaneDialog *dialog = NULL;
    try
    {
        dialog = new FeatureDatumPlaneDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
