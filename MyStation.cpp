#include "MyStation.hpp"
#include "MyDisplay.hpp"
#include "mixr/base/Component.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/ui/glut/GlutDisplay.hpp"

IMPLEMENT_SUBCLASS(MyStation, "MyStation")

BEGIN_SLOTTABLE(MyStation)
"display",
"mapDisplay",
END_SLOTTABLE(MyStation)

BEGIN_SLOT_MAP(MyStation)
    ON_SLOT(1, setSlotDisplay, MyDisplay)
    ON_SLOT(2, setSlotMapDisplay, MapDisplay)
END_SLOT_MAP()


MyStation::MyStation() {
    STANDARD_CONSTRUCTOR()
        displayInit = false;
    mapDisplayInit = false;
}

// initializes display and displayInit 
void MyStation::copyData(const MyStation& org, const bool)
{
    BaseClass::copyData(org);

    if (display != nullptr) {
        display->unref();
        display = nullptr;
    }

    if (org.display != nullptr) {
        display = org.display;
        display->ref();
    }

    if (mapDisplay != nullptr) {
        mapDisplay->unref();
        mapDisplay = nullptr;
    }

    if (org.mapDisplay != nullptr) {
        mapDisplay = org.mapDisplay;
        mapDisplay->ref();
    }



    mapDisplayInit = org.mapDisplayInit;
    displayInit = org.displayInit;
}

// delete member data
void MyStation::deleteData()
{
    if (display != nullptr) {
        display->unref();
        display = nullptr;
    }

    if (mapDisplay != nullptr) {
        mapDisplay->unref();
        mapDisplay = nullptr;
    }



}

void MyStation::reset()
{

    if (display != nullptr && !displayInit) {
        display->createWindow();
        display->focus(display);
        displayInit = true;
    }

    if (mapDisplay != nullptr && !mapDisplayInit) {
        mapDisplay->createWindow();
        mapDisplay->focus(mapDisplay);
        mapDisplayInit = true;
    }

    BaseClass::reset();
}

bool MyStation::setSlotDisplay(MyDisplay* dis)
{
    bool ok{};

    if (display != nullptr) {
        display->unref();
        display = nullptr;

    }
    if (dis != nullptr) {
        display = dis;
        display->ref();

        display->container(this);
        ok = true;

    }
    return ok;
}

bool MyStation::setSlotMapDisplay(MapDisplay* dis)
{
    bool ok{};

    if (mapDisplay != nullptr) {
        mapDisplay->unref();
        mapDisplay = nullptr;

    }
    if (dis != nullptr) {
        mapDisplay = dis;
        mapDisplay->ref();

        mapDisplay->container(this);
        ok = true;

    }
    return ok;
}



MyDisplay* MyStation::getDisplay()
{
    return display;
}

MapDisplay* MyStation::getMapDisplay()
{
    return mapDisplay;
}

