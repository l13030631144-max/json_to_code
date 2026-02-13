#include "BlockFeatureDialog.h"

#include <PWOpen/Expression.hxx>
#include <PWOpen/Features_Block.hxx>
#include <PWOpen/Features_FeatureCollection.hxx>
#include <PWOpen/GeometricUtilities_BooleanOperation.hxx>
#include <PWOpen/IncludeHeader.h>
#include <PWOpen/Part.hxx>
#include <PWOpen/PartCollection.hxx>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session*(BlockFeatureDialog::theSession) = nullptr;
PWOpen::UI*(BlockFeatureDialog::theUI) = nullptr;

BlockFeatureDialog::BlockFeatureDialog()
{
    try
    {
        BlockFeatureDialog::theSession = PWOpen::Session::GetSession();
        m_builder = BlockFeatureDialog::theSession->Parts()->Work()->Features()->CreateBlockFeatureBuilder(nullptr);

        BlockFeatureDialog::theUI = PWOpen::UI::GetUI();
        theDialog = theUI->CreateDialog("BlockFeatureDialog.ui");

        theDialog->AddApplyHandler([this] {
            return apply_cb();
        });
        theDialog->AddOkHandler([this] {
            return ok_cb();
        });
        theDialog->AddUpdateHandler([this](auto&& PH1) {
            return update_cb(std::forward<decltype(PH1)>(PH1));
        });
        theDialog->AddInitializeHandler([this] {
            initialize_cb();
        });
        theDialog->AddDialogShownHandler([this] {
            dialogShown_cb();
        });
    }
    catch (std::exception& ex)
    {
        return;
    }
}
BlockFeatureDialog::BlockFeatureDialog(PWOpen::Features::Block* block)
  : m_block_feature(block)
{
    BlockFeatureDialog::theSession = PWOpen::Session::GetSession();
    m_builder = BlockFeatureDialog::theSession->Parts()->Work()->Features()->CreateBlockFeatureBuilder(block);

    BlockFeatureDialog::theUI = PWOpen::UI::GetUI();
    theDialog = theUI->CreateDialog("BlockFeatureDialog.ui");

    theDialog->AddInitializeHandler([this] {
        initialize_cb();
    });

    EditInitialize();

    theDialog->AddApplyHandler([this] {
        return apply_cb();
    });
    theDialog->AddOkHandler([this] {
        return ok_cb();
    });
    theDialog->AddUpdateHandler([this](auto&& PH1) {
        return update_cb(std::forward<decltype(PH1)>(PH1));
    });
    theDialog->AddDialogShownHandler([this] {
        dialogShown_cb();
    });
}
BlockFeatureDialog::~BlockFeatureDialog()
{
    if (theDialog)
    {
        delete theDialog;
    }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse BlockFeatureDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch (std::exception& ex)
    {
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void BlockFeatureDialog::initialize_cb()
{
    try
    {
        type = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("type"));
        group = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        group_2 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group_2"));
        group_3 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group_3"));
        group_4 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group_4"));
        group_5 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group_5"));
        group_6 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group_6"));
        originPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("originPoint"));
        cornerPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("cornerPoint"));
        length = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("length"));
        width = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("width"));
        height = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("height"));
        boolOptType = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("boolOptType"));
        boolBodyCollector = dynamic_cast<PWOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("boolBodyCollector"));
        associativityOriginPoint = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("associativityOriginPoint"));

        // 为 LinearDimension 控件设置 ExpressionObject
        if (m_builder)
        {
            if (length)
            {
                auto lengthExpr = m_builder->Length();
                if (lengthExpr)
                {
                    length->SetExpressionObject(lengthExpr);
                }
            }

            if (width)
            {
                auto widthExpr = m_builder->Width();
                if (widthExpr)
                {
                    width->SetExpressionObject(widthExpr);
                }
            }

            if (height)
            {
                auto heightExpr = m_builder->Height();
                if (heightExpr)
                {
                    height->SetExpressionObject(heightExpr);
                }
            }
        }
    }
    catch (std::exception& ex)
    {
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void BlockFeatureDialog::dialogShown_cb()
{
    try
    {
    }
    catch (std::exception& ex)
    {
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int BlockFeatureDialog::apply_cb()
{
    int errorCode = 0;
    try
    {
        if (!m_builder)
            return -1;

        auto create_type = m_builder->Type();
        PWOpen::Body* nullBody = nullptr;

        if (create_type == PWOpen::Features::BlockFeatureBuilder::Types::TypesOriginAndEdgeLengths)
        {
            // 原点和边长模式
            auto origin_point = m_builder->OriginPoint();
            if (!origin_point)
            {
                PWOpen::Point3d origin_position = PWOpen::Point3d(0, 0, 0);
                origin_point = BlockFeatureDialog::theSession->Parts()->Work()->Points()->CreatePoint(origin_position);
            }

            m_builder->SetOriginPoint(origin_point);

            auto length_str = length->Formula();
            auto width_str = width->Formula();
            auto height_str = height->Formula();

            PWOpen::Point3d origin_pt = origin_point->Coordinates();
            m_builder->SetOriginAndLengths(origin_pt, length_str, width_str, height_str);

            m_builder->SetBooleanOperationAndTarget(m_builder->BooleanType(), nullBody);
        }
        else if (create_type == PWOpen::Features::BlockFeatureBuilder::Types::TypesTwoPointsAndHeight)
        {
            // 两点和高度模式
            auto origin_point = m_builder->OriginPoint();
            auto corner_point = m_builder->PointFromOrigin();

            if (origin_point && corner_point)
            {
                m_builder->SetOriginPoint(origin_point);
                m_builder->SetPointFromOrigin(corner_point);

                PWOpen::Point3d origin_pt = origin_point->Coordinates();
                PWOpen::Point3d corner_pt = corner_point->Coordinates();
                auto height_str = height->Formula();

                m_builder->SetTwoPointsAndHeight(origin_pt, corner_pt, height_str);

                m_builder->SetBooleanOperationAndTarget(m_builder->BooleanType(), nullBody);
            }
        }
        else if (create_type == PWOpen::Features::BlockFeatureBuilder::Types::TypesDiagonalPoints)
        {
            // 两个对角点模式
            auto origin_point = m_builder->OriginPoint();
            auto corner_point = m_builder->PointFromOrigin();

            if (origin_point && corner_point)
            {
                m_builder->SetOriginPoint(origin_point);
                m_builder->SetPointFromOrigin(corner_point);

                PWOpen::Point3d origin_pt = origin_point->Coordinates();
                PWOpen::Point3d corner_pt = corner_point->Coordinates();

                m_builder->SetTwoDiagonalPoints(origin_pt, corner_pt);

                m_builder->SetBooleanOperationAndTarget(m_builder->BooleanType(), nullBody);
            }
        }

        m_builder->CommitFeature();  // 提交特征

        m_block_feature = static_cast<PWOpen::Features::Block*>(m_builder->GetObject());
        SaveAttribute();

        m_builder->Destroy();  // 销毁特征构建器
        m_builder = nullptr;

        m_builder = BlockFeatureDialog::theSession->Parts()->Work()->Features()->CreateBlockFeatureBuilder(nullptr);
    }
    catch (std::exception& ex)
    {
        errorCode = 1;
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int BlockFeatureDialog::ok_cb()
{
    int errorCode = 0;
    try
    {
        if (!m_builder)
            return -1;

        auto create_type = m_builder->Type();
        PWOpen::Body* nullBody = nullptr;

        if (create_type == PWOpen::Features::BlockFeatureBuilder::Types::TypesOriginAndEdgeLengths)
        {
            // 原点和边长模式
            auto origin_point = m_builder->OriginPoint();
            if (!origin_point)
            {
                PWOpen::Point3d origin_position = PWOpen::Point3d(0, 0, 0);
                origin_point = BlockFeatureDialog::theSession->Parts()->Work()->Points()->CreatePoint(origin_position);
            }

            m_builder->SetOriginPoint(origin_point);

            auto length_str = length->Formula();
            auto width_str = width->Formula();
            auto height_str = height->Formula();

            PWOpen::Point3d origin_pt = origin_point->Coordinates();
            m_builder->SetOriginAndLengths(origin_pt, length_str, width_str, height_str);

            // m_builder->SetBooleanOperationAndTarget(m_builder->BooleanType(), nullBody);
        }
        else if (create_type == PWOpen::Features::BlockFeatureBuilder::Types::TypesTwoPointsAndHeight)
        {
            // 两点和高度模式
            auto origin_point = m_builder->OriginPoint();
            auto corner_point = m_builder->PointFromOrigin();

            if (origin_point && corner_point)
            {
                m_builder->SetOriginPoint(origin_point);
                m_builder->SetPointFromOrigin(corner_point);

                PWOpen::Point3d origin_pt = origin_point->Coordinates();
                PWOpen::Point3d corner_pt = corner_point->Coordinates();
                auto height_str = height->Formula();

                m_builder->SetTwoPointsAndHeight(origin_pt, corner_pt, height_str);

                m_builder->SetBooleanOperationAndTarget(m_builder->BooleanType(), nullBody);
            }
        }
        else if (create_type == PWOpen::Features::BlockFeatureBuilder::Types::TypesDiagonalPoints)
        {
            // 两个对角点模式
            auto origin_point = m_builder->OriginPoint();
            auto corner_point = m_builder->PointFromOrigin();

            if (origin_point && corner_point)
            {
                m_builder->SetOriginPoint(origin_point);
                m_builder->SetPointFromOrigin(corner_point);

                PWOpen::Point3d origin_pt = origin_point->Coordinates();
                PWOpen::Point3d corner_pt = corner_point->Coordinates();

                m_builder->SetTwoDiagonalPoints(origin_pt, corner_pt);

                m_builder->SetBooleanOperationAndTarget(m_builder->BooleanType(), nullBody);
            }
        }

        m_builder->CommitFeature();  // 提交特征
        m_block_feature = static_cast<PWOpen::Features::Block*>(m_builder->GetObject());
        SaveAttribute();

        m_builder->Destroy();  // 销毁特征构建器
        m_builder = nullptr;
    }
    catch (std::exception& ex)
    {
        errorCode = 1;
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int BlockFeatureDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        // Update code here
        if (block == type)
        {
            auto create_type = type->GetProperties()->GetString("CurrentData");
            //界面
            group_5->GetProperties()->SetLogical("Show", create_type != "TypesOriginAndEdgeLengths");
            length->GetProperties()->SetLogical("Show", create_type == "TypesOriginAndEdgeLengths");
            width->GetProperties()->SetLogical("Show", create_type == "TypesOriginAndEdgeLengths");
            height->GetProperties()->SetLogical("Show", create_type == "TypesOriginAndEdgeLengths" || create_type == "TypesTwoPointsAndHeight");
        }
        else if (block == originPoint)
        {
            auto selected_objects = originPoint->GetSelectedObjects();
            if (selected_objects.size() > 0)
            {
                auto point_obj = dynamic_cast<PWOpen::Point*>(selected_objects[0]);
                if (point_obj)
                {
                    m_builder->SetOriginPoint(point_obj);
                }
            }
        }
        else if (block == cornerPoint)
        {
            auto selected_objects = cornerPoint->GetSelectedObjects();
            if (selected_objects.size() > 0)
            {
                auto point_obj = dynamic_cast<PWOpen::Point*>(selected_objects[0]);
                if (point_obj)
                {
                    m_builder->SetPointFromOrigin(point_obj);
                }
            }
        }
        else if (block == length) {
            auto exprObj = m_builder->Length();
            auto length_string = length->Formula();
            // 拿到 PWOpen::Expression*
            if (exprObj && !length_string.IsEmpty()) {
                // 把公式写进去
                exprObj->SetFormula(length_string);
            }
        }
        else if (block == width)
        {
            auto width_string = width->Formula();
            if (!width_string.IsEmpty())
            {
                m_builder->SetWidth(width_string);
            }
        }
        else if (block == height)
        {
            auto height_string = height->Formula();
            if (!height_string.IsEmpty())
            {
                m_builder->SetHeight(height_string);
            }
        }
        else if (block == boolOptType)
        {
            auto boolType = boolOptType->GetProperties()->GetString("CurrentType");
            //界面
            boolBodyCollector->GetProperties()->SetLogical("Show", boolType != "BooleanTypeCreate");
            //数据
            auto string2type = [](const PWOpen::PWString& str) -> PWOpen::Features::Feature::BooleanType {
                if (str == "BooleanTypeCreate")
                    return PWOpen::Features::Feature::BooleanType::BooleanTypeCreate;
                else if (str == "BooleanTypeUnite")
                    return PWOpen::Features::Feature::BooleanType::BooleanTypeUnite;
                else if (str == "BooleanTypeSubtract")
                    return PWOpen::Features::Feature::BooleanType::BooleanTypeSubtract;
                else if (str == "BooleanTypeIntersect")
                    return PWOpen::Features::Feature::BooleanType::BooleanTypeIntersect;
                else
                    return PWOpen::Features::Feature::BooleanType::BooleanTypeCreate;
            };
            auto string2booltype = [](const PWOpen::PWString& str) -> PWOpen::GeometricUtilities::BooleanOperation::BooleanType {
                if (str == "BooleanTypeCreate")
                    return PWOpen::GeometricUtilities::BooleanOperation::BooleanType::BooleanTypeCreate;
                else if (str == "BooleanTypeUnite")
                    return PWOpen::GeometricUtilities::BooleanOperation::BooleanType::BooleanTypeUnite;
                else if (str == "BooleanTypeSubtract")
                    return PWOpen::GeometricUtilities::BooleanOperation::BooleanType::BooleanTypeSubtract;
                else if (str == "BooleanTypeIntersect")
                    return PWOpen::GeometricUtilities::BooleanOperation::BooleanType::BooleanTypeIntersect;
                else
                    return PWOpen::GeometricUtilities::BooleanOperation::BooleanType::BooleanTypeCreate;
            };
            m_builder->SetBooleanType(string2type(boolType));
            auto boolop = m_builder->BooleanOption();
            boolop->SetType(string2booltype(boolType));
        }
        else if (block == boolBodyCollector)
        {
            m_builder->BooleanOption()->SetTargetBodies({});
        }
        else if (block == associativityOriginPoint)
        {
            m_builder->SetParentAssociativity(associativityOriginPoint->GetProperties()->GetLogical("Value"));
        }
    }
    catch (std::exception& ex)
    {
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}

void BlockFeatureDialog::SaveAttribute()
{
    try
    {
        auto associativity = m_builder->ParentAssociativity();
    }
    catch (std::exception& ex)
    {
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}

void BlockFeatureDialog::EditInitialize()
{
    try
    {
        
    }
    catch (const std::exception& e)
    {
    }
}


extern "C" DllExport void  ufusr(char *param, int *retcod, int param_len)
{
    BlockFeatureDialog *dialog = NULL;
    try
    {
        dialog = new BlockFeatureDialog();
        // The following method shows the dialog immediately
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        //---- Enter your exception handling code here -----
        BlockFeatureDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}