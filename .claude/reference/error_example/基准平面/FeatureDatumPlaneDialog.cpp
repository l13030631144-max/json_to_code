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
        enum_methodType        = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));

        blockGroup_inferObject = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        selectObject_inferObject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject16"));

        blockGroup_planeReference = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        selectObject_planeRef  = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject0"));

        blockGroup_throughAxis = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        selectObject_axisLinear = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject1"));

        blockGroup_firstPlane  = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group14"));
        selectObject_firstPlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject2"));

        blockGroup_secondPlane = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group15"));
        selectObject_secondPlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject14"));

        blockGroup_curvePointSubtype = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group5"));
        enum_curvePointSubtype = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum1"));

        blockGroup_refGeomCurvePoint = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group7"));
        selectObject_pointInfer = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject4"));
        specifyPoint_point1    = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
        specifyPoint_point2    = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point1"));
        specifyPoint_point3    = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point2"));
        selectObject_curveForPlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject5"));
        selectObject_planeForPoint = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject6"));

        blockGroup_firstLine   = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));
        selectObject_firstLine = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject15"));

        blockGroup_secondLine  = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group4"));
        selectObject_secondLine = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject3"));

        blockGroup_tangentSubtype = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group6"));
        enum_tangentSubtype    = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum2"));

        blockGroup_refGeomTangent = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group23"));
        selectObject_tangentInfer = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject7"));
        selectObject_tangentFace  = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject8"));
        selectObject_tangentFace2 = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject17"));
        specifyPoint_tangentPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point5"));
        selectObject_tangentLinearFace = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject9"));
        selectObject_tangentLinear = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject10"));
        selectObject_tangentAnglePlane = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject18"));

        blockGroup_angle       = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group22"));
        enum_angleOption       = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum7"));
        blockAngular_angle     = dynamic_cast<PWOpen::BlockStyler::AngularDimension*>(theDialog->TopBlock()->FindBlock("angular_dim0"));

        blockGroup_coincidentObject = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group8"));
        selectObject_coincidentObj  = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject19"));

        blockGroup_pointDir    = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group9"));
        specifyPoint_pointDir  = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point3"));

        blockGroup_normalDir   = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group10"));
        specifyVector_normalDir = dynamic_cast<PWOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("vector0"));

        blockGroup_curve       = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group11"));
        sectionBuilder_curve   = dynamic_cast<PWOpen::BlockStyler::SectionBuilder*>(theDialog->TopBlock()->FindBlock("section0"));
        reverseDir_curveDir    = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction0"));

        blockGroup_curvePosition = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group12"));
        enum_positionMethod    = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum3"));
        blockLinear_arcLength  = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim0"));
        blockExpression_arcPercent = dynamic_cast<PWOpen::BlockStyler::ExpressionBlock*>(theDialog->TopBlock()->FindBlock("expression0"));
        specifyPoint_frenetPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point4"));

        blockGroup_frenetOrientation = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group13"));
        enum_frenetDirection   = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum4"));
        selectObject_frenetProject = dynamic_cast<PWOpen::BlockStyler::SelectObject*>(theDialog->TopBlock()->FindBlock("selectobject11"));
        specifyVector_frenetVector = dynamic_cast<PWOpen::BlockStyler::SpecifyVector*>(theDialog->TopBlock()->FindBlock("vector1"));

        blockGroup_offsetReference = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group16"));
        enum_coordSystem       = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum5"));
        blockLinear_fixedDistance = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim1"));

        blockGroup_coefficients = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group17"));
        enum_coeffCoordSystem  = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum6"));
        blockDouble_coeffA     = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double0"));
        blockDouble_coeffB     = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double1"));
        blockDouble_coeffC     = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("double2"));
        blockLinear_coeffD     = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim2"));

        blockGroup_planeOrientation = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group19"));
        reverseDir_planeOrient = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction2"));

        blockGroup_offset      = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group18"));
        blockToggle_offset     = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        blockLinear_offsetDist = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim3"));

        blockGroup_distanceOffset = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group24"));
        blockLinear_distanceDist  = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim4"));
        blockInteger_planeCount   = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("integer1"));

        blockGroup_fixedPlaneOrient = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group20"));
        reverseDir_fixedOrient  = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction3"));

        blockGroup_distancePlaneOrient = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group25"));
        reverseDir_distanceOrient = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("direction1"));

        blockGroup_settings    = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group21"));
        blockToggle_associative = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle1"));

        // Static initial states
        reverseDir_planeOrient->GetProperties()->SetLogical("Enable", false);
        blockLinear_distanceDist->GetProperties()->SetLogical("Enable", true);
        blockInteger_planeCount->GetProperties()->SetLogical("Enable", true);
        reverseDir_distanceOrient->GetProperties()->SetLogical("Enable", true);
        blockLinear_offsetDist->GetProperties()->SetLogical("Enable", true);
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
        // Apply initial visibility based on current enum/toggle states
        update_cb(enum_methodType);
        update_cb(enum_curvePointSubtype);
        update_cb(enum_tangentSubtype);
        update_cb(enum_positionMethod);
        update_cb(enum_frenetDirection);
        update_cb(enum_angleOption);
        update_cb(blockToggle_offset);
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
            PWOpen::PWString enumValue = enum_methodType->GetProperties()->GetString("CurrentData");

            blockGroup_inferObject->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeInferred");

            blockGroup_planeReference->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeDistance" || enumValue == "MethodTypeAngle");

            blockGroup_throughAxis->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeAngle");

            blockGroup_firstPlane->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeCenter");

            blockGroup_secondPlane->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeCenter");

            blockGroup_curvePointSubtype->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeCurvePoint");

            blockGroup_refGeomCurvePoint->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeCurvePoint");

            blockGroup_firstLine->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTwoLines");

            blockGroup_secondLine->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTwoLines");

            blockGroup_tangentSubtype->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangent");

            blockGroup_refGeomTangent->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangent");

            blockGroup_angle->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeAngle" || enumValue == "MethodTypeTangent");

            blockGroup_coincidentObject->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeCoincident");

            blockGroup_pointDir->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypePointDir");

            blockGroup_normalDir->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypePointDir");

            blockGroup_curve->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeFrenet");

            blockGroup_curvePosition->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeFrenet");

            blockGroup_frenetOrientation->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeFrenet");

            blockGroup_offsetReference->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeFixedX" || enumValue == "MethodTypeFixedY" || enumValue == "MethodTypeFixedZ");

            blockGroup_coefficients->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeCoefficients");

            blockGroup_planeOrientation->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeInferred" || enumValue == "MethodTypeAngle" ||
                enumValue == "MethodTypeCenter" || enumValue == "MethodTypeCurvePoint" ||
                enumValue == "MethodTypeTwoLines" || enumValue == "MethodTypeTangent" ||
                enumValue == "MethodTypeCoincident" || enumValue == "MethodTypePointDir" ||
                enumValue == "MethodTypeFrenet" || enumValue == "MethodTypeCoefficients");

            blockGroup_offset->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeInferred" || enumValue == "MethodTypeAngle" ||
                enumValue == "MethodTypeCenter" || enumValue == "MethodTypeCurvePoint" ||
                enumValue == "MethodTypeTwoLines" || enumValue == "MethodTypeTangent" ||
                enumValue == "MethodTypeCoincident" || enumValue == "MethodTypePointDir" ||
                enumValue == "MethodTypeFrenet");

            blockGroup_distanceOffset->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeDistance");

            blockGroup_fixedPlaneOrient->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeFixedX" || enumValue == "MethodTypeFixedY" ||
                enumValue == "MethodTypeFixedZ" || enumValue == "MethodTypeFixedView");

            blockGroup_distancePlaneOrient->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeDistance");

            reverseDir_distanceOrient->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeDistance");

            blockGroup_settings->GetProperties()->SetLogical("Enable",
                enumValue == "MethodTypeInferred" || enumValue == "MethodTypeDistance" ||
                enumValue == "MethodTypeAngle" || enumValue == "MethodTypeCenter" ||
                enumValue == "MethodTypeCurvePoint" || enumValue == "MethodTypeTwoLines" ||
                enumValue == "MethodTypeTangent" || enumValue == "MethodTypeCoincident" ||
                enumValue == "MethodTypePointDir" || enumValue == "MethodTypeFrenet");
        }
        else if (block == enum_curvePointSubtype)
        {
            PWOpen::PWString enumValue = enum_curvePointSubtype->GetProperties()->GetString("CurrentData");

            selectObject_pointInfer->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypePointInfer");

            specifyPoint_point1->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypePoint" || enumValue == "MethodTypeTwoPoints" ||
                enumValue == "MethodTypeThreePoints" || enumValue == "MethodTypeCurvePoint" ||
                enumValue == "MethodTypeParallelPoint");

            specifyPoint_point2->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTwoPoints" || enumValue == "MethodTypeThreePoints");

            specifyPoint_point3->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeThreePoints");

            selectObject_curveForPlane->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeCurvePoint");

            selectObject_planeForPoint->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeParallelPoint");
        }
        else if (block == enum_tangentSubtype)
        {
            PWOpen::PWString enumValue = enum_tangentSubtype->GetProperties()->GetString("CurrentData");

            selectObject_tangentInfer->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangentInfer");

            selectObject_tangentFace->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangent" || enumValue == "MethodTypeTangentPoint" ||
                enumValue == "MethodTypeTangentTwoFaces" || enumValue == "MethodTypeTangentAnglePlane");

            selectObject_tangentFace2->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangentTwoFaces");

            specifyPoint_tangentPoint->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangentPoint");

            selectObject_tangentLinearFace->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangentLinear");

            selectObject_tangentLinear->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangentLinear");

            selectObject_tangentAnglePlane->GetProperties()->SetLogical("Show",
                enumValue == "MethodTypeTangentAnglePlane");
        }
        else if (block == enum_positionMethod)
        {
            PWOpen::PWString enumValue = enum_positionMethod->GetProperties()->GetString("CurrentData");

            blockLinear_arcLength->GetProperties()->SetLogical("Show",
                enumValue == "SetExpression");

            blockExpression_arcPercent->GetProperties()->SetLogical("Show",
                enumValue == "SetPercent");

            specifyPoint_frenetPoint->GetProperties()->SetLogical("Show",
                enumValue == "SetPoint");
        }
        else if (block == enum_frenetDirection)
        {
            PWOpen::PWString enumValue = enum_frenetDirection->GetProperties()->GetString("CurrentData");

            selectObject_frenetProject->GetProperties()->SetLogical("Show",
                enumValue == "FrenetSubtypeProject");

            specifyVector_frenetVector->GetProperties()->SetLogical("Show",
                enumValue == "FrenetSubtypeNormalToVector" ||
                enumValue == "FrenetSubtypeParallelToVector" ||
                enumValue == "FrenetSubtypeThruAxis");
        }
        else if (block == enum_angleOption)
        {
            PWOpen::PWString enumValue = enum_angleOption->GetProperties()->GetString("CurrentData");

            blockAngular_angle->GetProperties()->SetLogical("Show",
                enumValue == "Value");
        }
        else if (block == blockToggle_offset)
        {
            bool toggleValue = blockToggle_offset->GetProperties()->GetLogical("Value");

            blockLinear_offsetDist->GetProperties()->SetLogical("Show", toggleValue);
        }
        else if (block == selectObject_inferObject)
        {
        }
        else if (block == selectObject_planeRef)
        {
        }
        else if (block == selectObject_axisLinear)
        {
        }
        else if (block == selectObject_firstPlane)
        {
        }
        else if (block == selectObject_secondPlane)
        {
        }
        else if (block == selectObject_pointInfer)
        {
        }
        else if (block == specifyPoint_point1)
        {
        }
        else if (block == specifyPoint_point2)
        {
        }
        else if (block == specifyPoint_point3)
        {
        }
        else if (block == selectObject_curveForPlane)
        {
        }
        else if (block == selectObject_planeForPoint)
        {
        }
        else if (block == selectObject_firstLine)
        {
        }
        else if (block == selectObject_secondLine)
        {
        }
        else if (block == selectObject_tangentInfer)
        {
        }
        else if (block == selectObject_tangentFace)
        {
        }
        else if (block == selectObject_tangentFace2)
        {
        }
        else if (block == specifyPoint_tangentPoint)
        {
        }
        else if (block == selectObject_tangentLinearFace)
        {
        }
        else if (block == selectObject_tangentLinear)
        {
        }
        else if (block == selectObject_tangentAnglePlane)
        {
        }
        else if (block == blockAngular_angle)
        {
        }
        else if (block == selectObject_coincidentObj)
        {
        }
        else if (block == specifyPoint_pointDir)
        {
        }
        else if (block == specifyVector_normalDir)
        {
        }
        else if (block == sectionBuilder_curve)
        {
        }
        else if (block == reverseDir_curveDir)
        {
        }
        else if (block == blockLinear_arcLength)
        {
        }
        else if (block == blockExpression_arcPercent)
        {
        }
        else if (block == specifyPoint_frenetPoint)
        {
        }
        else if (block == selectObject_frenetProject)
        {
        }
        else if (block == specifyVector_frenetVector)
        {
        }
        else if (block == enum_coordSystem)
        {
        }
        else if (block == blockLinear_fixedDistance)
        {
        }
        else if (block == enum_coeffCoordSystem)
        {
        }
        else if (block == blockDouble_coeffA)
        {
        }
        else if (block == blockDouble_coeffB)
        {
        }
        else if (block == blockDouble_coeffC)
        {
        }
        else if (block == blockLinear_coeffD)
        {
        }
        else if (block == reverseDir_planeOrient)
        {
        }
        else if (block == blockLinear_offsetDist)
        {
        }
        else if (block == blockLinear_distanceDist)
        {
        }
        else if (block == blockInteger_planeCount)
        {
        }
        else if (block == reverseDir_fixedOrient)
        {
        }
        else if (block == reverseDir_distanceOrient)
        {
        }
        else if (block == blockToggle_associative)
        {
        }
    }
    catch(std::exception& ex)
    {
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
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
    {
        FeatureDatumPlaneDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
