#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class TestDialog {
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    TestDialog();
    ~TestDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    PWOpen::BlockStyler::SelectObject* selectObject_selection;
    PWOpen::BlockStyler::SectionBuilder* sectionBuilder_section;
    PWOpen::BlockStyler::SuperSection* superSection_section;
    PWOpen::BlockStyler::CurveCollector* curveCollector_edgeSelect;
    PWOpen::BlockStyler::FaceCollector* faceCollector_faceSelect;
    PWOpen::BlockStyler::BodyCollector* bodyCollector_bodySelect;
    PWOpen::BlockStyler::SelectFeature* selectFeature_feature;
    PWOpen::BlockStyler::SelectFacetRegion* facetSelect_facet;
    PWOpen::BlockStyler::SpecifyPoint* specifyPoint_origin;
};
