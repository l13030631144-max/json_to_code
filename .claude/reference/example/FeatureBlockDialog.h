#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>
#include <PWOpen/Features_BlockFeatureBuilder.hxx>
#include <PWOpen/Features_Block.hxx>
#include <PWOpen/Session.hxx>

class FeatureBlockDialog
{
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    FeatureBlockDialog(PWOpen::Features::Block* block = nullptr);
    ~FeatureBlockDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    void EditInitialize();
    void SaveAttribute();
    void SaveLastParameters();      // 保存当前参数到静态变量
    void RestoreLastParameters();   // 从静态变量恢复参数

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    // Control member variables
    PWOpen::BlockStyler::Enumeration*       type;           // 创建类型
    PWOpen::BlockStyler::Group*             blockGroup_group0;    // 原点
    PWOpen::BlockStyler::SpecifyPoint*      originPoint;  // 指定点
    PWOpen::BlockStyler::Group*             blockGroup_group1;    // 从原点出发的点 XC, YC
    PWOpen::BlockStyler::SpecifyPoint*      cornerPoint;  // 指定点
    PWOpen::BlockStyler::Group*             blockGroup_group;     // 从原点出发的点 XC, YC, ZC
    PWOpen::BlockStyler::SpecifyPoint*      cornerPointOther;   // 指定点
    PWOpen::BlockStyler::Group*             blockGroup_group2;    // 维度
    PWOpen::BlockStyler::LinearDimension*   blockLinear_linear_dim0; // 高度（ZC）
    PWOpen::BlockStyler::Group*             blockGroup_group3;    // 尺寸
    PWOpen::BlockStyler::LinearDimension*   length; // 长度 (XC)
    PWOpen::BlockStyler::LinearDimension*   width; // 宽度 (YC)
    PWOpen::BlockStyler::LinearDimension*   height; // 高度 (ZC)
    PWOpen::BlockStyler::Group*             blockGroup_group4;    // 布尔
    PWOpen::BlockStyler::Enumeration*       enum_enum1;           // 布尔
    PWOpen::BlockStyler::BodyCollector*     boolBodyCollector; // 选择体
    PWOpen::BlockStyler::Group*             blockGroup_group5;    // 设置
    PWOpen::BlockStyler::Toggle*            blockToggle_toggle0;  // 关联原点
    PWOpen::BlockStyler::Group*             blockGroup_group6;    // 设置
    PWOpen::BlockStyler::Toggle*            blockToggle_toggle1;  // 关联原点和偏置

private:
    PWOpen::Features::BlockFeatureBuilder* m_builder;
    PWOpen::Features::Block* m_block_feature;

    PWOpen::Session::UndoMarkId m_undo_mark_id;
    std::string m_undo_mark_name = "";

    std::string s_last_length_formula = "100";
    std::string s_last_width_formula = "100";
    std::string s_last_height_formula = "100";
    PWOpen::Features::BlockFeatureBuilder::Types s_last_create_type
    = PWOpen::Features::BlockFeatureBuilder::TypesOriginAndEdgeLengths;
};
