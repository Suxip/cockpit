#include "MyDisplay.hpp"
#include "mixr/graphics/readouts/NumericReadout.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/models/WorldModel.hpp"
#include "MyStation.hpp"
#include "MySimulation.hpp"
#include "mixr/models/player/air/Aircraft.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/graphics/BmpTexture.hpp"

#include "mixr/graphics/Polygon.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/instruments/adi/Adi.hpp"
#include "mixr/models/player/air/AirVehicle.hpp"
#include "mixr/base/list.hpp"
#include "mixr/graphics/Page.hpp"
#include "F16.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/sensor/Tws.hpp"
#include "mixr/instruments/gauges/AoaIndexer.hpp"
#include "mixr/models/system/trackmanager/AirTrkMgr.hpp"
#include "mixr/base/safe_queue.hpp"
#include "mixr/models/system/StoresMgr.hpp"

#include "mixr/models/player/weapon/AbstractWeapon.hpp"
#include "DisplayRwr.hpp"
#include "mixr/models/system/Radar.hpp"

#include <cmath>

IMPLEMENT_SUBCLASS(MyDisplay, "MyDisplay")
EMPTY_DELETEDATA(MyDisplay)
EMPTY_SLOTTABLE(MyDisplay)

BEGIN_EVENT_HANDLER(MyDisplay)
ON_EVENT(SPACE_BAR, onShootEvent)
ON_EVENT('r', onTargetEvent)
ON_EVENT(KILL_EVENT, onShot);

END_EVENT_HANDLER()


MyDisplay::MyDisplay() {
    STANDARD_CONSTRUCTOR()
}


void MyDisplay::copyData(const MyDisplay& org, const bool)
{
    BaseClass::copyData(org);

    // pitch and roll
    /*  pitch = org.pitch;
    pitchRate = org.pitchRate;
    roll = org.roll;
    rollRate = org.rollRate;*/

    aoa = org.aoa;
    aoaSD.empty();
    aoaRate = org.aoaRate;
    aoaROSD.empty();
    aoaASD.empty();

}

bool MyDisplay::onShot() {
    std::cout << "killed";
    return true;
}
mixr::simulation::Station* MyDisplay::getStation()
{
    if (myStation == nullptr) {
        const auto s = dynamic_cast<mixr::simulation::Station*>(findContainerByType(typeid(mixr::simulation::Station)));
        if (s != nullptr) myStation = s;
    }
    return myStation;
}
//------------------------------------------------------------------------------
// Simulation access functions
//------------------------------------------------------------------------------
mixr::models::Player* MyDisplay::getOwnship()
{
    mixr::models::Player* p{};
    mixr::simulation::Station* sta{ getStation() };
    if (sta != nullptr) {
        p = dynamic_cast<mixr::models::Player*>(sta->getOwnship());
    }
    return p;
}



void MyDisplay::updateData(const double dt) {
    //maintainAirTrackSymbols();
    BaseClass::updateData(dt);

    // handles background gif animation
    mixr::base::Pair* p = subpage()->findByName("sky");
    animation(p);

    mixr::models::Aircraft* plane{};

    const auto sta = static_cast<mixr::simulation::Station*>(findContainerByType(typeid(mixr::simulation::Station)));
    plane = dynamic_cast<mixr::models::Aircraft*>(sta->getOwnship());
    mixr::graphics::Page* page = subpage();

    if (plane != nullptr) {
        heading = plane->getHeadingD();
        speedKts = plane->getTotalVelocityKts();
        speedMach = speedKts / 666.739;
        send("hsi", UPDATE_VALUE6, heading, headingSD);
        send("ASMach", UPDATE_INSTRUMENTS, speedKts, speedKtsSD);
    }
    //get plane altitude and send to altimeter
    double alt = plane->getAltitude();
    auto* altDisplay = dynamic_cast<mixr::glut::GlutDisplay*>(subDisplays()->findByName("altDisplay")->object());
    altDisplay->send("alt", UPDATE_INSTRUMENTS, alt, altSD);

    pitch = plane->getPitchD();
    roll = plane->getRollD();


    if (page != nullptr) {

        page->send("adi", UPDATE_INSTRUMENTS, pitch, pitchSD);
        page->send("adi", UPDATE_VALUE, roll, rollSD);

        page->send("adiBackground", UPDATE_INSTRUMENTS, pitch, pitchSD2);
        page->send("adiBackground", UPDATE_VALUE, roll, rollSD2);
    }

    auto* station = dynamic_cast<mixr::simulation::Station*>(container());
    auto* aircraft = (station != nullptr) ? dynamic_cast<mixr::models::Aircraft*>(station->getOwnship()) : nullptr;
    if (aircraft != nullptr) {
        aoa = aircraft->getAngleOfAttackD();
    }

    send("aoa", UPDATE_VALUE, aoa, aoaROSD);
    // digital version of the aoa indexer (F16)
    send("aoagauge", UPDATE_INSTRUMENTS, aoa, aoaSD);
    // analog version of the aoa
    send("analogaoa", UPDATE_INSTRUMENTS, aoa, aoaASD);

    //Rwr update
    rwrDisplay = nullptr;

    auto* mfd = dynamic_cast<mixr::glut::GlutDisplay*>(subDisplays()->findByName("mfd")->object());
    if (mfd != nullptr) {
        auto* radarPage = dynamic_cast<mixr::graphics::Page*>(mfd->findSubpageByName("radarscreen")->object());
        if (radarPage != nullptr) {
            mixr::base::Pair* p{ radarPage->findByType(typeid(DisplayRwr)) };
            if (p != nullptr) rwrDisplay = dynamic_cast<DisplayRwr*>(p->object());
            if (rwrDisplay != nullptr && getOwnship() != nullptr) {
                mixr::models::Rwr* rwr{};
                mixr::base::Pair* pair{ getOwnship()->getSensorByType(typeid(mixr::models::Rwr)) };
                if (pair != nullptr) rwr = static_cast<mixr::models::Rwr*>(pair->object());
                if (rwr != nullptr) {
                }
                rwrDisplay->setRwr(rwr);
                // radar logic
                auto obc = plane->getOnboardComputer();
                mixr::models::AirTrkMgr* trackmanager = dynamic_cast<mixr::models::AirTrkMgr*>(obc->getTrackManagerByName("twsTrkMgr"));
                trackmanager->setMaxTrackAge(1);

                int trackingCount = trackmanager->getTrackList(tlist, trackmanager->getMaxTracks());
                std::vector<mixr::base::Vec3d> planeVec;
                std::vector<mixr::base::Vec3d> missileVec;
                for (int i = 0; i < trackingCount; i++) {
                    if (tlist[i] != nullptr) {
                        if (tlist[i]->getTarget() != nullptr) {
                            const auto* tgt = tlist[i]->getTarget();
                            // Example check (depends on your Player class hierarchy)
                            if (tgt->isClassType(typeid(mixr::models::AbstractWeapon))) {
                                missileVec.push_back(tlist[i]->getPosition());
                            }
                            else if (tgt->isClassType(typeid(mixr::models::Aircraft))) {
                                planeVec.push_back(tlist[i]->getPosition());
                                //tlist[i]->get
                            }
                        }
                    }
                }
                for (int i = trackingCount; i < trackmanager->getMaxTracks(); i++) {
                    if (tlist[i] != nullptr) {
                        tlist[i] == nullptr;
                    }
                }
                rwrDisplay->setRadar(planeVec, missileVec);
            }
        }
    }
}


void MyDisplay::buttonEvent(const int b) {
    switch (b) {
    case 3001:
        getSubdisplay()->newSubpage("mfdscreen", nullptr);
        break;
    case 3002:
        getSubdisplay()->newSubpage("radarscreen", nullptr);
        break;
    case 3003:
        getSubdisplay()->newSubpage("routescreen", nullptr);
        break;
    case 3004:
        getSubdisplay()->newSubpage("obstaclescreen", nullptr);
        break;
    case 3005:
        getSubdisplay()->newSubpage("lightscreen", nullptr);
        break;
        //case 3010:
        //    getSubdisplay()->newSubpage("networkscreen", nullptr);
        //    break;
    case 3014:
        getSubdisplay()->newSubpage("posscreen", nullptr);
        break;
    case 3015:
        getSubdisplay()->newSubpage("enginescreen", nullptr);
        break;
    case 3016:
        getSubdisplay()->newSubpage("playerscreen", nullptr);
        break;
    }
}


MfdDisplay* MyDisplay::getSubdisplay() {
    mixr::base::PairStream* pairstream = subDisplays();
    mixr::base::Pair* pair = pairstream->findByName("mfd");
    MfdDisplay* mfd = dynamic_cast<MfdDisplay*>(pair->object());
    return mfd;
}

bool MyDisplay::onShootEvent()
{
    if (getOwnship() != nullptr) {
        getOwnship()->event(WPN_REL_EVENT);
    }
    return true;
}

bool MyDisplay::onTargetEvent()
{
    if (getOwnship() != nullptr) {
        mixr::models::StoresMgr* sms{ getOwnship()->getStoresManagement() };
        if (sms != nullptr) {
            sms->setWeaponDeliveryMode(mixr::models::StoresMgr::A2A);
            std::cout << "Set A/A Weapon Mode!" << std::endl;
        }
        getOwnship()->event(TGT_STEP_EVENT);
    }
    return true;
}


void MyDisplay::animation(mixr::base::Pair* p) {
    if (p != nullptr) {

        mixr::graphics::Polygon* poly = dynamic_cast<mixr::graphics::Polygon*>(p->object());

        if (animationCount == 0) {

            mixr::graphics::BmpTexture* texture = new mixr::graphics::BmpTexture();

            //texture->setTexturePath("../../mixr-data/textures/space");
            texture->setTexturePath("space");

            std::string filename = "space" + std::to_string(count) + ".bmp";
            mixr::base::String name(filename.c_str());
            texture->setTextureFileName(name);

            mixr::base::PairStream* pairstream = getTextures();    // Returns a ptr to the list of textures

            std::string slotname = "tex" + std::to_string(count + 1); // bc we now have tex1
            mixr::base::String slot(slotname.c_str());

            mixr::base::Pair* pair = new mixr::base::Pair(slot, texture);    // Returns a ptr to the list of textures

            pairstream->put(pair);

            loadTextures();

            poly->setTexture(count + 1); // bc we now have tex1
        }
        else {

            poly->setTexture(count + 1); // bc we now have tex1
        }
    }

}
