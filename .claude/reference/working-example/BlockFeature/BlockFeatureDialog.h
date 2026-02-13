#pragma once
#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>
#include <PWOpen/Features_BlockFeatureBuilder.hxx>
#include <PWOpen/Features_Block.hxx>

class BlockFeatureDialog
{
  public:
    static PWOpen::Session* theSession;
    static PWOpen::UI* theUI;

    BlockFeatureDialog();
    BlockFeatureDialog(PWOpen::Features::Block* block);
    ~BlockFeatureDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

    private:
    void EditInitialize();

    void SaveAttribute();

  private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    PWOpen::BlockStyler::Enumeration* type;  //创建类型
    PWOpen::BlockStyler::Group* group;
    PWOpen::BlockStyler::Group* group_2;
    PWOpen::BlockStyler::Group* group_3;
    PWOpen::BlockStyler::Group* group_4;
    PWOpen::BlockStyler::Group* group_5;
    PWOpen::BlockStyler::Group* group_6;
    PWOpen::BlockStyler::SpecifyPoint* originPoint;         //原点
    PWOpen::BlockStyler::SpecifyPoint* cornerPoint;         //对角点
    PWOpen::BlockStyler::LinearDimension* length;           //长
    PWOpen::BlockStyler::LinearDimension* width;            //宽
    PWOpen::BlockStyler::LinearDimension* height;           //高
    PWOpen::BlockStyler::Enumeration* boolOptType;          //布尔类型
    PWOpen::BlockStyler::BodyCollector* boolBodyCollector;  //布尔体
    PWOpen::BlockStyler::Toggle* associativityOriginPoint;  //关联

    bool is_create = true;
    PWOpen::Features::BlockFeatureBuilder* m_builder;
    PWOpen::Features::Block* m_block_feature;
    // Define over...
};