#include "FeatureBlockDialog.h"

#include "PWOpen/Expression.hxx"
#include "PWOpen/Features_FeatureCollection.hxx"
#include "PWOpen/PartCollection.hxx"

#include <PWOpen/IncludeHeader.h>
#include <magic_enum/magic_enum.hpp>

#ifndef DllExport
#define DllExport __declspec(dllexport)
#endif

PWOpen::Session *(FeatureBlockDialog::theSession) = nullptr;
PWOpen::UI *(FeatureBlockDialog::theUI) = nullptr;

FeatureBlockDialog::FeatureBlockDialog(PWOpen::Features::Block* block)
{
    try
    {
        FeatureBlockDialog::theSession = PWOpen::Session::GetSession();
        FeatureBlockDialog::theUI = PWOpen::UI::GetUI();

        if (block)  //编辑块特征
        {
            m_undo_mark_name = "重新定义特征";
        }
        else  //创建块特征
        {
            m_undo_mark_name = "块";
        }
        m_undo_mark_id = theSession->SetUndoMark(PWOpen::Session::MarkVisibility::MarkVisibilityVisible, m_undo_mark_name.c_str());

        m_builder = theSession->Parts()->Work()->Features()->CreateBlockFeatureBuilder(block);

        if (block)
            EditInitialize();

        theDialog = theUI->CreateDialog(":/Forms/FeatureBlockDialog.ui");
        theDialog->AddApplyHandler([this] { return apply_cb(); });
        theDialog->AddOkHandler([this] { return ok_cb(); });
        theDialog->AddUpdateHandler([this](auto && PH1) { return update_cb(std::forward<decltype(PH1)>(PH1)); });
        theDialog->AddInitializeHandler([this] { initialize_cb(); });
        theDialog->AddDialogShownHandler([this] { dialogShown_cb(); });
    }
    catch (std::exception& ex) {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
        return;
    }
}
FeatureBlockDialog::~FeatureBlockDialog()
{
    if (theDialog) { delete theDialog; }
}

PWOpen::BlockStyler::BlockDialog::DialogResponse FeatureBlockDialog::Launch()
{
    PWOpen::BlockStyler::BlockDialog::DialogResponse dialogResponse = PWOpen::BlockStyler::BlockDialog::DialogResponse::DialogResponseInvalid;
    try
    {
        dialogResponse = theDialog->Launch();
    }
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return dialogResponse;
}
void FeatureBlockDialog::initialize_cb()
{
    try
    {
        type = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum0"));
        blockGroup_group0 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group0"));
        originPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point0"));
        blockGroup_group1 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group1"));
        cornerPoint = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point1"));
        blockGroup_group = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group"));
        cornerPointOther = dynamic_cast<PWOpen::BlockStyler::SpecifyPoint*>(theDialog->TopBlock()->FindBlock("point"));
        blockGroup_group2 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group2"));
        blockLinear_linear_dim0 = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim0"));
        blockGroup_group3 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group3"));
        length = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim1"));
        width = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim2"));
        height = dynamic_cast<PWOpen::BlockStyler::LinearDimension*>(theDialog->TopBlock()->FindBlock("linear_dim3"));
        blockGroup_group4 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group4"));
        enum_enum1 = dynamic_cast<PWOpen::BlockStyler::Enumeration*>(theDialog->TopBlock()->FindBlock("enum1"));
        boolBodyCollector = dynamic_cast<PWOpen::BlockStyler::BodyCollector*>(theDialog->TopBlock()->FindBlock("bodySelect0"));
        blockGroup_group5 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group5"));
        blockToggle_toggle0 = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle0"));
        blockGroup_group6 = dynamic_cast<PWOpen::BlockStyler::Group*>(theDialog->TopBlock()->FindBlock("group6"));
        blockToggle_toggle1 = dynamic_cast<PWOpen::BlockStyler::Toggle*>(theDialog->TopBlock()->FindBlock("toggle1"));


        // 如果是创建新块（非编辑模式），先恢复上次使用的参数到 builder
        if (!m_block_feature && m_builder)
        {
            RestoreLastParameters();
        }

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
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void FeatureBlockDialog::dialogShown_cb()
{
    try
    {
        update_cb(type);
        update_cb(enum_enum1);
    }
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
int FeatureBlockDialog::apply_cb()
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
                origin_point = theSession->Parts()->Work()->Points()->CreatePoint(origin_position);
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

        // 保存当前参数到历史记忆
        SaveLastParameters();

        m_builder->Destroy();  // 销毁特征构建器（清理 OM 层资源）
        delete m_builder;      // 释放 builder 对象本身
        m_builder = nullptr;

        theSession->DeleteUndoMark(m_undo_mark_id, m_undo_mark_name);

        m_undo_mark_name = "块";  // 重置标记名称
        m_undo_mark_id = theSession->SetUndoMark(PWOpen::Session::MarkVisibility::MarkVisibilityVisible, m_undo_mark_name.c_str());
        m_builder = theSession->Parts()->Work()->Features()->CreateBlockFeatureBuilder(nullptr);

        // 将 UI 中当前的表达式值同步到新 Builder，并重新绑定 ExpressionObject
        // 避免 UI 控件持有已销毁的旧 Expression 指针导致访问违规
        if (m_builder)
        {
            auto length_str = length->Formula();
            if (!length_str.IsEmpty())
                m_builder->Length()->SetFormula(length_str);
            length->SetExpressionObject(m_builder->Length());

            auto width_str = width->Formula();
            if (!width_str.IsEmpty())
                m_builder->Width()->SetFormula(width_str);
            width->SetExpressionObject(m_builder->Width());

            auto height_str = height->Formula();
            if (!height_str.IsEmpty())
                m_builder->Height()->SetFormula(height_str);
            height->SetExpressionObject(m_builder->Height());
        }
    }
    catch (std::exception& ex)
    {
        errorCode = 1;
        theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int FeatureBlockDialog::ok_cb()
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
                origin_point = theSession->Parts()->Work()->Points()->CreatePoint(origin_position);
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

        // 保存当前参数到历史记忆
        SaveLastParameters();

        m_builder->Destroy();  // 销毁特征构建器（清理 OM 层资源）
        delete m_builder;      // 释放 builder 对象本身
        m_builder = nullptr;

        theSession->DeleteUndoMark(m_undo_mark_id, m_undo_mark_name);
    }
    catch (std::exception& ex)
    {
        errorCode = 1;
        theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return errorCode;
}
int FeatureBlockDialog::update_cb(PWOpen::BlockStyler::UIBlock* block)
{
    try
    {
        if (block == type)
        {
            auto currentData = type->GetProperties()->GetString("CurrentData");
            // blockGroup_group1: show when enum0 == TwoPointsAndHeight
            blockGroup_group1->GetProperties()->SetLogical("Show", currentData == "TwoPointsAndHeight");
            // blockGroup_group: show when enum0 == TwoDiagonalPoints
            blockGroup_group->GetProperties()->SetLogical("Show", currentData == "TwoDiagonalPoints");
            // blockGroup_group2: show when enum0 == TwoPointsAndHeight
            blockGroup_group2->GetProperties()->SetLogical("Show", currentData == "TwoPointsAndHeight");
            // blockGroup_group3: show when enum0 == OriginAndEdgeLength
            blockGroup_group3->GetProperties()->SetLogical("Show", currentData == "OriginAndEdgeLength");
            // blockGroup_group5: show when enum0 == OriginAndEdgeLength
            blockGroup_group5->GetProperties()->SetLogical("Show", currentData == "OriginAndEdgeLength");
            // blockGroup_group6: show when enum0 == TwoPointsAndHeight or TwoDiagonalPoints
            blockGroup_group6->GetProperties()->SetLogical("Show", currentData == "TwoPointsAndHeight" || currentData == "TwoDiagonalPoints");
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
        else if (block == length)
        {
            auto exprObj = m_builder->Length();
            auto length_string = length->Formula();
            // 拿到 PWOpen::Expression*
            if (exprObj && !length_string.IsEmpty())
            {
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
        else if (block == enum_enum1)
        {
            auto boolType = enum_enum1->GetProperties()->GetString("CurrentType");
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
        else if (block == blockToggle_toggle0)
        {
            m_builder->SetParentAssociativity(blockToggle_toggle0->GetProperties()->GetLogical("Value"));
        }
    }
    catch(std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    return 0;
}
void FeatureBlockDialog::EditInitialize()
{
    try
    {
        auto edit_type = m_builder->Type();
        if (edit_type == PWOpen::Features::BlockFeatureBuilder::TypesOriginAndEdgeLengths)
        {
            originPoint->GetProperties()->SetEnumAsString("CurrentType", m_block_feature->GetStringAttribute("OriginPointType"));
            length->SetValue(m_builder->Length()->NumberValue());
            width->SetValue(m_builder->Width()->NumberValue());
            height->SetValue(m_builder->Height()->NumberValue());
        }
        else if (edit_type == PWOpen::Features::BlockFeatureBuilder::TypesTwoPointsAndHeight)
        {
            originPoint->GetProperties()->SetEnumAsString("CurrentType", m_block_feature->GetStringAttribute("OriginPointType"));
            cornerPoint->GetProperties()->SetEnumAsString("CurrentType", m_block_feature->GetStringAttribute("CornerPointType"));
            height->SetValue(m_builder->Height()->NumberValue());
        }
        else
        {
            originPoint->GetProperties()->SetEnumAsString("CurrentType", m_block_feature->GetStringAttribute("OriginPointType"));
            cornerPointOther->GetProperties()->SetEnumAsString("CurrentType", m_block_feature->GetStringAttribute("CornerPointType"));
        }
    }
    catch (const std::exception& e)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, e.what());
    }
}
void FeatureBlockDialog::SaveAttribute()
{
    try
    {
        auto associativity = m_builder->ParentAssociativity();
        std::string edit_type = magic_enum::enum_name(m_builder->Type()).data();
        if (edit_type == "TypesOriginAndEdgeLengths")
        {
            if (associativity)
            {
                PWOpen::PWString originPointType = PWOpen::PWString("TypeFixed");
                // if (originPoint->GetProperties()->GetString("CurrentType").GetUTF8Text() != "")
                //     originPointType = PWOpen::PWString(originPoint->GetProperties()->GetString("CurrentType"));
                m_block_feature->SetAttribute("OriginPointType", originPointType);
            }
            else
                m_block_feature->SetAttribute("OriginPointType", "TypeFixed");
        }
        else
        {
            if (associativity)
            {
                PWOpen::PWString originPointType = PWOpen::PWString("TypeFixed");
                PWOpen::PWString cornerPointType = PWOpen::PWString("TypeFixed");
                // if (originPoint->GetProperties()->GetString("CurrentType") != "")
                //     originPointType = PWOpen::PWString(originPoint->GetProperties()->GetString("CurrentType"));
                // if (cornerPoint->GetProperties()->GetString("CurrentType") != "")
                //     cornerPointType = PWOpen::PWString(cornerPoint->GetProperties()->GetString("CurrentType"));
                m_block_feature->SetAttribute("OriginPointType", originPointType);
                m_block_feature->SetAttribute("CornerPointType", cornerPointType);
            }
            else
            {
                m_block_feature->SetAttribute("OriginPointType", "TypeFixed");
                m_block_feature->SetAttribute("CornerPointType", "TypeFixed");
            }
        }
    }
    catch (std::exception& ex)
    {
        theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
}
void FeatureBlockDialog::SaveLastParameters()
{
    try
    {
        // 保存创建类型
        if (m_builder)
        {
            auto create_type = m_builder->Type();
            s_last_create_type = create_type;
        }

        // 保存长宽高表达式
        if (length)
        {
            auto formula = length->Formula();
            if (!formula.IsEmpty())
            {
                s_last_length_formula = formula.GetUTF8Text();
            }
        }

        if (width)
        {
            auto formula = width->Formula();
            if (!formula.IsEmpty())
            {
                s_last_width_formula = formula.GetUTF8Text();
            }
        }

        if (height)
        {
            auto formula = height->Formula();
            if (!formula.IsEmpty())
            {
                s_last_height_formula = formula.GetUTF8Text();
            }
        }
    }
    catch (const std::exception& e)
    {
        // 忽略保存参数时的异常
    }
}

void FeatureBlockDialog::RestoreLastParameters()
{
    try
    {
        // 恢复长宽高表达式到 builder（必须在 SetExpressionObject 之前）
        if (m_builder)
        {
            if (!s_last_length_formula.empty())
            {
                m_builder->Length()->SetFormula(PWOpen::PWString(s_last_length_formula.c_str()));
            }

            if (!s_last_width_formula.empty())
            {
                m_builder->Width()->SetFormula(PWOpen::PWString(s_last_width_formula.c_str()));
            }

            if (!s_last_height_formula.empty())
            {
                m_builder->Height()->SetFormula(PWOpen::PWString(s_last_height_formula.c_str()));
            }
        }

        // 恢复创建类型
        if (m_builder && type)
        {
                m_builder->SetType(s_last_create_type);
                type->GetProperties()->SetString("CurrentData", magic_enum::enum_name(s_last_create_type).data());
        }
    }
    catch (const std::exception& e)
    {
        // 忽略恢复参数时的异常
    }
}

extern "C" DllExport void ufusr(char *param, int *retcod, int param_len)
{
    FeatureBlockDialog *dialog = NULL;
    try
    {
        dialog = new FeatureBlockDialog();
        dialog->Launch();
    }
    catch(const std::exception& ex)
    {
        FeatureBlockDialog::theUI->PWMessageBox()->Show("Block Styler", PWOpen::PWMessageBox::DialogTypeError, ex.what());
    }
    if(dialog != NULL)
    {
        delete dialog;
        dialog = NULL;
    }
}
