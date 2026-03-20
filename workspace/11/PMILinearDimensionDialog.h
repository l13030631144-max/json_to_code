#pragma once

#include <PWOpen/BlockStyler_BlockDialog.hxx>
#include <PWOpen/ForwardDeclaration.h>

class PMILinearDimensionDialog {
public:
    static PWOpen::Session *theSession;
    static PWOpen::UI *theUI;

    PMILinearDimensionDialog();
    ~PMILinearDimensionDialog();

    PWOpen::BlockStyler::BlockDialog::DialogResponse Launch();

    void initialize_cb();
    void dialogShown_cb();
    int apply_cb();
    int ok_cb();
    int update_cb(PWOpen::BlockStyler::UIBlock* block);

private:
    PWOpen::BlockStyler::BlockDialog* theDialog{};

    // Control member variables
    PWOpen::BlockStyler::Group* blockGroup_reference;
    PWOpen::BlockStyler::SelectObject* selectObject_firstObject;
    PWOpen::BlockStyler::SelectObject* selectObject_secondObject;
    PWOpen::BlockStyler::Group* blockGroup_origin;
    PWOpen::BlockStyler::SpecifyLocation* cursorLocation_position;
    PWOpen::BlockStyler::Toggle* blockToggle_autoPlace;
    PWOpen::BlockStyler::Group* blockGroup_direction;
    PWOpen::BlockStyler::Toggle* blockToggle_lockPlane;
    PWOpen::BlockStyler::SelectObject* selectObject_alternativePlane;
    PWOpen::BlockStyler::SelectObject* selectObject_specifyPlane;
    PWOpen::BlockStyler::Toggle* blockToggle_projectToPlane;
    PWOpen::BlockStyler::Group* blockGroup_associatedObject;
    PWOpen::BlockStyler::SelectObject* selectObject_associatedObj;
    PWOpen::BlockStyler::Group* blockGroup_dimensionSet;
    PWOpen::BlockStyler::Enumeration* enum_dimensionSetMethod;
    PWOpen::BlockStyler::SelectObject* selectObject_dimensionSetObj;
    PWOpen::BlockStyler::Group* blockGroup_measurement;
    PWOpen::BlockStyler::Enumeration* enum_measurementMethod;
    PWOpen::BlockStyler::Toggle* blockToggle_useMeasureMethod;
    PWOpen::BlockStyler::SpecifyVector* specifyVector_measureDirection;
    PWOpen::BlockStyler::Button* blockButton_alternateEndpoint;
    PWOpen::BlockStyler::Toggle* blockToggle_useBaseline;
    PWOpen::BlockStyler::Group* blockGroup_settings;
    PWOpen::BlockStyler::Button* blockButton_settings;
    PWOpen::BlockStyler::Button* blockButton_inheritDimension;
};
