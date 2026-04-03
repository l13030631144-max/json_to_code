#include "IntersectionCurve.h"
#include <PWOpen/IncludeHeader.h>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(IntersectionCurve::theSession) = nullptr;
PWOpen::UI *(IntersectionCurve::theUI) = nullptr;

IntersectionCurve::IntersectionCurve()
{
    try
    {
        IntersectionCurve::theSession = PWOpen::Session::GetSession();
        IntersectionCurve::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("IntersectionCurve.ui");

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

IntersectionCurve::~IntersectionCurve()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse IntersectionCurve::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        IntersectionCurve::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}

void IntersectionCurve::initialize_cb()
{
    try
    {
        blockGroup_firstInput = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        faceCollector_firstFace = dynamic_cast<PWOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("face_select0"));
        specifyPlane_firstPlane = dynamic_cast<PWOpen::BlockStyler::SpecifyPlane*>(theDialog->TopBlock()->FindBlock("plane0"));
        blockToggle_keepFirstSelected = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("keepSelected"));
        blockGroup_secondInput = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group_2"));
        faceCollector_secondFace = dynamic_cast<PWOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("face_select1"));
        specifyPlane_secondPlane = dynamic_cast<PWOpen::BlockStyler::SpecifyPlane*>(theDialog->TopBlock()->FindBlock("plane1"));
        blockToggle_keepSecondSelected = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("keepSelected_2"));
        blockGroup_settings = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group_3"));
        blockToggle_associative = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("associative"));
        blockToggle_advancedCurveFitting = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("advancedCurveFitting"));
        enum_method = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("method"));
        blockInteger_degree = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("degree"));
        blockInteger_segments = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("segments"));
        blockInteger_minDegree = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("integer1"));
        blockInteger_maxDegree = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("integer2"));
        blockInteger_maxSegments = dynamic_cast<PWOpen::BlockStyler::IntegerBlock*>(theDialog->TopBlock()->FindBlock("integer0"));
        blockLinear_distanceTolerance = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim0"));
    }
    catch(std::exception& ex)
    {
        IntersectionCurve::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}

void IntersectionCurve::dialogShown_cb()
{
    try
    {
        update_cb(blockToggle_advancedCurveFitting);
        update_cb(enum_method);
        update_cb(theDialog->TopBlock()->FindBlock("More"));
    }
    catch(std::exception& ex)
    {
        IntersectionCurve::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}

int IntersectionCurve::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        IntersectionCurve::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int IntersectionCurve::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        IntersectionCurve::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}

int IntersectionCurve::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == blockToggle_advancedCurveFitting || block == enum_method)
        {
            bool advancedCurveFittingValue = blockToggle_advancedCurveFitting->GetProperties()->GetLogical("Value");
            PWOpen::PWString methodValue = enum_method->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "advancedCurveFitting == true"
            enum_method->GetProperties()->SetLogical("Show", advancedCurveFittingValue);

            // altProperties PW_Show: "advancedCurveFitting == true && method(1,2)"
            blockInteger_degree->GetProperties()->SetLogical("Show",
                advancedCurveFittingValue && (methodValue == "1" || methodValue == "2"));

            // altProperties PW_Show: "advancedCurveFitting == true && method(1)"
            blockInteger_segments->GetProperties()->SetLogical("Show",
                advancedCurveFittingValue && methodValue == "1");

            // altProperties PW_Show: "advancedCurveFitting == true && method(3)"
            blockInteger_minDegree->GetProperties()->SetLogical("Show",
                advancedCurveFittingValue && methodValue == "3");
            blockInteger_maxDegree->GetProperties()->SetLogical("Show",
                advancedCurveFittingValue && methodValue == "3");
            blockInteger_maxSegments->GetProperties()->SetLogical("Show",
                advancedCurveFittingValue && methodValue == "3");

            // altProperties PW_Show: "advancedCurveFitting == false || advancedCurveFitting == true && method(2,3)"
            blockLinear_distanceTolerance->GetProperties()->SetLogical("Show",
                !advancedCurveFittingValue || (methodValue == "2" || methodValue == "3"));
        }
        else if (block == theDialog->TopBlock()->FindBlock("More"))
        {
            // altProperties PW_Show: "Dialog == true"
            blockGroup_settings->GetProperties()->SetLogical("Show", theDialog->IsMore());
        }
        else if (block == blockGroup_firstInput)
        {
        }
        else if (block == faceCollector_firstFace)
        {
        }
        else if (block == specifyPlane_firstPlane)
        {
        }
        else if (block == blockToggle_keepFirstSelected)
        {
        }
        else if (block == blockGroup_secondInput)
        {
        }
        else if (block == faceCollector_secondFace)
        {
        }
        else if (block == specifyPlane_secondPlane)
        {
        }
        else if (block == blockToggle_keepSecondSelected)
        {
        }
        else if (block == blockGroup_settings)
        {
        }
        else if (block == blockToggle_associative)
        {
        }
        else if (block == blockInteger_degree)
        {
        }
        else if (block == blockInteger_segments)
        {
        }
        else if (block == blockInteger_minDegree)
        {
        }
        else if (block == blockInteger_maxDegree)
        {
        }
        else if (block == blockInteger_maxSegments)
        {
        }
        else if (block == blockLinear_distanceTolerance)
        {
        }
    }
    catch(std::exception& ex)
    {
        IntersectionCurve::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    IntersectionCurve *dialog = NULL;
    try
    {
        dialog = new IntersectionCurve();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        IntersectionCurve::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
