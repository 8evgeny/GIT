/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef FRONTENDAPPLICATIONBASE_HPP
#define FRONTENDAPPLICATIONBASE_HPP

#include <mvp/MVPApplication.hpp>
#include <gui/model/Model.hpp>

class FrontendHeap;

class FrontendApplicationBase : public touchgfx::MVPApplication
{
public:
    FrontendApplicationBase(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplicationBase() { }

    // HallwayScreen
    void gotoHallwayScreenScreenNoTransition();

    void gotoHallwayScreenScreenSlideTransitionWest();

    void gotoHallwayScreenScreenSlideTransitionEast();


    // KitchenScreen
    void gotoKitchenScreenScreenSlideTransitionEast();

    void gotoKitchenScreenScreenNoTransition();


    // BathtubScreen
    void gotoBathtubScreenScreenNoTransition();


    // BathroomScreen
    void gotoBathroomScreenScreenSlideTransitionWest();

    void gotoBathroomScreenScreenNoTransition();


    // WasherScreen
    void gotoWasherScreenScreenNoTransition();


    // ExtractorHoodScreen
    void gotoExtractorHoodScreenScreenNoTransition();


    // RecipeScreen
    void gotoRecipeScreenScreenCoverTransitionSouth();

    void gotoRecipeScreenScreenNoTransition();


    // RecipeSelectedScreen
    void gotoRecipeSelectedScreenScreenCoverTransitionNorth();


    // OvenScreen
    void gotoOvenScreenScreenNoTransition();


    // HomeControlScreen
    void gotoHomeControlScreenScreenNoTransition();

protected:
    touchgfx::Callback<FrontendApplicationBase> transitionCallback;
    FrontendHeap& frontendHeap;
    Model& model;

    // HallwayScreen
    void gotoHallwayScreenScreenNoTransitionImpl();

    void gotoHallwayScreenScreenSlideTransitionWestImpl();

    void gotoHallwayScreenScreenSlideTransitionEastImpl();


    // KitchenScreen
    void gotoKitchenScreenScreenSlideTransitionEastImpl();

    void gotoKitchenScreenScreenNoTransitionImpl();


    // BathtubScreen
    void gotoBathtubScreenScreenNoTransitionImpl();


    // BathroomScreen
    void gotoBathroomScreenScreenSlideTransitionWestImpl();

    void gotoBathroomScreenScreenNoTransitionImpl();


    // WasherScreen
    void gotoWasherScreenScreenNoTransitionImpl();


    // ExtractorHoodScreen
    void gotoExtractorHoodScreenScreenNoTransitionImpl();


    // RecipeScreen
    void gotoRecipeScreenScreenCoverTransitionSouthImpl();

    void gotoRecipeScreenScreenNoTransitionImpl();


    // RecipeSelectedScreen
    void gotoRecipeSelectedScreenScreenCoverTransitionNorthImpl();


    // OvenScreen
    void gotoOvenScreenScreenNoTransitionImpl();


    // HomeControlScreen
    void gotoHomeControlScreenScreenNoTransitionImpl();
};

#endif // FRONTENDAPPLICATIONBASE_HPP
