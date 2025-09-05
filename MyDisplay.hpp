#ifndef __MyDisplay_H__
#define __MyDisplay_H__

#include "mixr/ui/glut/GlutDisplay.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/models/player/air/Aircraft.hpp"
#include "F16.hpp"
#include "MfdDisplay.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/models/Track.hpp"
#include "DisplayRwr.hpp"
#include "mixr/models/system/Rwr.hpp"


namespace mixr {
    namespace simulation { class Simulation; class Station; }
    namespace models { class Missile; class Player; }
    namespace graphics { class SymbolLoader; }
    namespace xpanel { class DspRadar; class DspRwr; }
}

// -------------------------------------------------------------------------------
// Class: MyDisplay
//
// Description: Provides main graphic display
// -------------------------------------------------------------------------------

class MyDisplay final : public mixr::glut::GlutDisplay
{
    DECLARE_SUBCLASS(MyDisplay, mixr::glut::GlutDisplay)

public:
    MyDisplay();
    void updateData(const double dt);

    bool onShot();

    mixr::simulation::Station* getStation();
    mixr::models::Player* getOwnship();

    int getCount() { return count; }
    void setCount(int num) { count = num; }
    int getAnimationCount() { return animationCount; }
    void setAnimationCount(int num) { animationCount = num; }
    bool event(const int event, mixr::base::Object* const obj = nullptr) final;

private:
    void animation(mixr::base::Pair* p);


    int animationCount;

    mixr::base::safe_ptr<mixr::simulation::Station> myStation;
    double pitch{};           // degs
    //double pitchRate{ 10.0 };   // degs/sec
    double roll{};            // degs
    //double rollRate{ -9.0 };    // degs/sec


    double maxRate{ 10.0 };

    // tracked players from network
    const int maxTrack = 20;
    mixr::models::Track* tlist[20];
    SendData slipSD;
    //SendData pitchGhostSD;
    SendData baSD;
    SendData bascaleSD;
    SendData fDirPitchSD;
    SendData fDirBankSD;
    SendData pitchSD;
    SendData rollSD;

    SendData pitchSD2;
    SendData rollSD2;


    // heading and nav stuff
    double trueHdg{};         // degs
    double tHdgRate{ 11.0 };    // degs/sec
    double cmdHdg{};          // commanded heading (heading bug) (degs)
    double cmdHdgRate{ 3.0 };   // degs/sec

    void buttonEvent(const int b);

    MfdDisplay* getSubdisplay();

    DisplayRwr* rwrDisplay{};

private:
    //float plane2_x = -7;
    //float plane2_y = 4.65;
    // side slip
    double slip{};            // degs
    double slipRate{ 10.0 };    // degs/sec
    //void updateData(const double dt = 0.0) override;
    //bool event(const int event, mixr::base::Object* const obj = nullptr) final;

    // flight director stuff (in inches)
    double fDirBank{};
    double fDirBankRate{ 4.0 };
    double fDirPitch{};
    double fDirPitchRate{ 7.0 };

    double heading{};
    double speedKts{};
    double speedMach{};
    SendData speedKtsSD;
    SendData speedMachSD;
    double headingRate{ 10.0 };
    SendData headingSD;
    SendData headingROSD;
    SendData altSD{};

    //void changeParameters(F16* plane);


    // --- AoA Indicator support ---
    double aoa{};
    double aoaRate{ 1.0 };    // rate which are going (up or down)
    SendData aoaSD;
    SendData aoaROSD;       // aoa readout
    SendData aoaASD;        // analog readout

    int count = 0; // for animation frames

    bool onShootEvent();
    bool onTargetEvent();

};
#endif