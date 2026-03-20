#include "TestDialog.h"
#include <PWOpen/IncludeHeader.h>
#include <PWOpen/BlockStyler_SetList.hxx>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(TestDialog::theSession) = nullptr;
PWOpen::UI *(TestDialog::theUI) = nullptr;

TestDialog::TestDialog()
{
    try
    {
        TestDialog::theSession = PWOpen::Session::GetSession();
        TestDialog::theUI = PWOpen::UI::GetUI();

        theDialog = theUI->CreateDialog("TestDialog.ui");

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
TestDialog::~TestDialog()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse TestDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void TestDialog::initialize_cb()
{
    try
    {
        blockGroup_edge = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_edge"));
        curveCollector_edgeSelect = dynamic_cast<PWOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("curveCollector_edgeSelect"));
        enum_crossSection = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_crossSection"));
        blockLinear_distance = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("blockLinear_distance"));
        blockLinear_distance1 = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("blockLinear_distance1"));
        blockLinear_distance2 = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("blockLinear_distance2"));
        blockAngular_angle = dynamic_cast<PWOpen::BlockStyler::AngularDimension*>(theDialog->TopBlock()->FindBlock("blockAngular_angle"));
        reverseDir_direction = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("reverseDir_direction"));
        setList_edgeList = dynamic_cast<PWOpen::BlockStyler::SetList*>(theDialog->TopBlock()->FindBlock("setList_edgeList"));
        blockGroup_lengthLimit = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_lengthLimit"));
        enum_limitObject = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_limitObject"));
        specifyPoint_point = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("specifyPoint_point"));
        specifyPlane_plane = dynamic_cast<PWOpen::BlockStyler::SpecifyPlane*>(theDialog->TopBlock()->FindBlock("specifyPlane_plane"));
        faceCollector_faceSelect = dynamic_cast<PWOpen::BlockStyler::FaceCollector*>(theDialog->TopBlock()->FindBlock("faceCollector_faceSelect"));
        curveCollector_edgeSelect1 = dynamic_cast<PWOpen::BlockStyler::CurveCollector*>(theDialog->TopBlock()->FindBlock("curveCollector_edgeSelect1"));
        reverseDir_limitDirection = dynamic_cast<PWOpen::BlockStyler::ReverseDirection*>(theDialog->TopBlock()->FindBlock("reverseDir_limitDirection"));
        blockToggle_trimByLimit = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("blockToggle_trimByLimit"));
        specifyPoint_trimPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("specifyPoint_trimPoint"));
        setList_limitList = dynamic_cast<PWOpen::BlockStyler::SetList*>(theDialog->TopBlock()->FindBlock("setList_limitList"));
        blockGroup_settings = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("blockGroup_settings"));
        blockDouble_tolerance = dynamic_cast<PWOpen::BlockStyler::DoubleBlock*>(theDialog->TopBlock()->FindBlock("blockDouble_tolerance"));
        enum_offsetMethod = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum_offsetMethod"));
        blockToggle_preview = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("blockToggle_preview"));
        blockButton_showResult = dynamic_cast<PWOpen::BlockStyler::Button*>(theDialog->TopBlock()->FindBlock("blockButton_showResult"));
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void TestDialog::dialogShown_cb()
{
    try
    {
        // Enum initialization - trigger update_cb to set initial UI state
        update_cb(enum_crossSection);
        update_cb(enum_limitObject);
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int TestDialog::apply_cb()
{
    int errorCode = 0;
    try
    {
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int TestDialog::ok_cb()
{
    int errorCode = 0;
    try
    {
        errorCode = apply_cb();
    }
    catch(std::exception& ex)
    {
        errorCode = 1;
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int TestDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == enum_crossSection)
        {
            PWOpen::PWString enumValue = enum_crossSection->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "enum(对称, 偏置和角度)" → Data: 1=对称, 2=非对称, 3=偏置和角度
            blockLinear_distance->GetProperties()->SetLogical("Show",
                enumValue == "1" || enumValue == "3");

            // altProperties PW_Show: "enum(非对称)" → Data: 2=非对称
            blockLinear_distance1->GetProperties()->SetLogical("Show",
                enumValue == "2");

            // altProperties PW_Show: "enum(非对称)" → Data: 2=非对称
            blockLinear_distance2->GetProperties()->SetLogical("Show",
                enumValue == "2");

            // altProperties PW_Show: "enum(偏置和角度)" → Data: 3=偏置和角度
            blockAngular_angle->GetProperties()->SetLogical("Show",
                enumValue == "3");

            // altProperties PW_Show: "enum(非对称, 偏置和角度)" → Data: 2=非对称, 3=偏置和角度
            reverseDir_direction->GetProperties()->SetLogical("Show",
                enumValue == "2" || enumValue == "3");
        }
        else if (block == enum_limitObject)
        {
            PWOpen::PWString enumValue = enum_limitObject->GetProperties()->GetString("CurrentData");

            // altProperties PW_Show: "enum1(点)" → Data: 1=点
            specifyPoint_point->GetProperties()->SetLogical("Show",
                enumValue == "1");

            // altProperties PW_Show: "enum1(平面)" → Data: 2=平面
            specifyPlane_plane->GetProperties()->SetLogical("Show",
                enumValue == "2");

            // altProperties PW_Show: "enum1(面)" → Data: 3=面
            faceCollector_faceSelect->GetProperties()->SetLogical("Show",
                enumValue == "3");

            // altProperties PW_Show: "enum1(边)" → Data: 4=边
            curveCollector_edgeSelect1->GetProperties()->SetLogical("Show",
                enumValue == "4");

            // altProperties PW_Show: "enum1(面, 边)" → Data: 3=面, 4=边
            reverseDir_limitDirection->GetProperties()->SetLogical("Show",
                enumValue == "3" || enumValue == "4");

            // altProperties PW_Show: "enum1(平面, 面)" → Data: 2=平面, 3=面
            blockToggle_trimByLimit->GetProperties()->SetLogical("Show",
                enumValue == "2" || enumValue == "3");

            // altProperties PW_Show: "enum1(平面)" → Data: 2=平面
            specifyPoint_trimPoint->GetProperties()->SetLogical("Show",
                enumValue == "2");
        }
        else if (block == curveCollector_edgeSelect)
        {
        }
        else if (block == blockLinear_distance)
        {
        }
        else if (block == blockLinear_distance1)
        {
        }
        else if (block == blockLinear_distance2)
        {
        }
        else if (block == blockAngular_angle)
        {
        }
        else if (block == reverseDir_direction)
        {
        }
        else if (block == setList_edgeList)
        {
        }
        else if (block == specifyPoint_point)
        {
        }
        else if (block == specifyPlane_plane)
        {
        }
        else if (block == faceCollector_faceSelect)
        {
        }
        else if (block == curveCollector_edgeSelect1)
        {
        }
        else if (block == reverseDir_limitDirection)
        {
        }
        else if (block == blockToggle_trimByLimit)
        {
        }
        else if (block == specifyPoint_trimPoint)
        {
        }
        else if (block == setList_limitList)
        {
        }
        else if (block == blockDouble_tolerance)
        {
        }
        else if (block == enum_offsetMethod)
        {
        }
        else if (block == blockToggle_preview)
        {
        }
        else if (block == blockButton_showResult)
        {
        }
    }
    catch(std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    TestDialog *dialog = NULL;
    try
    {
        dialog = new TestDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        TestDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
