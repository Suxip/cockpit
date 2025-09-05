#include "mixr/base/Pair.hpp"
#include "mixr/base/Timers.hpp"
#include "mixr/base/edl_parser.hpp"
#include "mixr/base/Component.hpp"

#include "mixr/ui/glut/GlutDisplay.hpp"
#include <GL/glut.h>
#include "mixr/base/util/system_utils.hpp"


// factories
#include "mixr/graphics/factory.hpp"
#include "mixr/base/factory.hpp"
#include "mixr/ui/glut/factory.hpp"
#include "mixr/instruments/buttons/Button.hpp"
#include "mixr/instruments/factory.hpp"
#include "mixr/simulation/factory.hpp"
#include "mixr/models/factory.hpp"
#include "mixr/terrain/factory.hpp"
#include "mixr/interop/dis/factory.hpp"
#include "mixr/ighost/cigi/factory.hpp"
#include "mixr/ighost/pov/factory.hpp"
#include "mixr/instruments/factory.hpp"

#include "mixr/models/dynamics/JSBSimModel.hpp"
#include "mixr/models/player/air/Aircraft.hpp"
#include "mixr/instruments/gauges/AoaIndexer.hpp"
#include "MyDisplay.hpp"
#include "MySimulation.hpp"
#include "MyStation.hpp"
#include "KeyHandler.hpp"
#include "F16.hpp"
#include "MfdDisplay.hpp"
#include "MapDisplay.hpp"

///////////////////////////////////////
#include "DisplayRwr.hpp"
///////////////////////////////////////

#include "mixr/instruments/dials/DialTickMarks.hpp"
#include "mixr/instruments/dials/DialPointer.hpp"
#include "mixr/instruments/dials/AnalogDial.hpp"

#include "mixr/instruments/dials/altitudedial.hpp"
#include "mixr/instruments/dials/DialPointer.hpp"
#include "mixr/instruments/gauges/GaugeSlider.hpp"
#include "mixr/instruments/dials/DialTickMarks.hpp"
#include "mixr/instruments/adi/GhostHorizon.hpp"
#include "mixr/instruments/adi/Adi.hpp"
#include "mixr/instruments/gauges/TickMarks.hpp"

#include "../shared/xzmq/factory.hpp"
#include "mixr/interop/dis/factory.hpp"
#include "mixr/linkage/factory.hpp"
#include "mixr/ighost/cigi/factory.hpp"
#include "mixr/models/factory.hpp"
#include "mixr/terrain/factory.hpp"
#include "mixr/simulation/factory.hpp"
#include "../shared/xpanel/factory.hpp"



#include <string>
#include <cstdlib>

//initialize a global frameRate variable that will be used throughout other functions in main 
const int frameRate{ 150 };
MyStation* station{};

// timer function, in this case, the background (updateData) function
void timerFunc(int)
{
    const double dt0{ 1.0 / static_cast<double>(frameRate) };
    const int millis{ static_cast<int>(dt0 * 1000) };
    glutTimerFunc(millis, timerFunc, 1);

    // Current time
    const double time{ mixr::base::getComputerTime() };

    // N-1 Time
    static double time0{ time };

    // Compute delta time
    const double dt{ static_cast<double>(time - time0) };
    time0 = time;

    int oldCount = station->getDisplay()->getCount();
    // replace with number of textures
    if (oldCount < 118) {
        int newCount = oldCount + 1;
        station->getDisplay()->setCount(newCount);
    }
    else {
        station->getDisplay()->setCount(1);
        station->getDisplay()->setAnimationCount(1); // when animation count is 0, it will load all the textures
        // when animation count is 1, it will just set the textures from 1 to however many frames there are, but will not load again
    }

    mixr::base::Timer::updateTimers(dt);
    mixr::graphics::Graphic::flashTimer(dt);
    station->updateData(dt);
}

mixr::base::Object* factory(const std::string & name)
{
    mixr::base::Object* obj{};

    // we need to add our object to the factory so mixr can identify it in the epp
    if (name == MyStation::getFactoryName()) {
        obj = new MyStation;
    }
    else if (name == MyDisplay::getFactoryName()) {
        obj = new MyDisplay;
    }
    else if (name == MySimulation::getFactoryName()) {
        obj = new MySimulation;
    }
    else if (name == KeyHandler::getFactoryName()) {
        obj = new KeyHandler;
    }

    else if (name == mixr::models::Aircraft::getFactoryName()) {
        obj = new mixr::models::Aircraft();
    }
    else if (name == mixr::instruments::DialPointer::getFactoryName()) {
        obj = new mixr::instruments::DialPointer();
    }
    else if (name == mixr::instruments::GaugeSlider::getFactoryName()) {
        obj = new mixr::instruments::GaugeSlider();
    }
    else if (name == mixr::instruments::DialTickMarks::getFactoryName()) {
        obj = new mixr::instruments::DialTickMarks();
    }
    else if (name == mixr::instruments::GhostHorizon::getFactoryName()) {
        obj = new mixr::instruments::GhostHorizon();
    }
    else if (name == mixr::instruments::Adi::getFactoryName()) {
        obj = new mixr::instruments::Adi();
    }
    else if (name == mixr::instruments::TickMarks::getFactoryName()) {
        obj = new mixr::instruments::TickMarks();
    }
    else if (name == mixr::instruments::AltitudeDial::getFactoryName()) {
        obj = new mixr::instruments::AltitudeDial();
    }
    else if (name == F16::getFactoryName()) {
        obj = new F16();
    }
    else if (name == MfdDisplay::getFactoryName()) {
        obj = new MfdDisplay();
    }

    else if (name == mixr::models::JSBSimModel::getFactoryName()) {
        obj = new mixr::models::JSBSimModel();
    }
    else if (name == mixr::instruments::DialTickMarks::getFactoryName()) {
        obj = new mixr::instruments::DialTickMarks();
    }
    else if (name == mixr::instruments::AnalogDial::getFactoryName()) {
        obj = new mixr::instruments::AnalogDial();
    }
    else if (name == mixr::instruments::DialPointer::getFactoryName()) {
        obj = new mixr::instruments::DialPointer();
    }
    else if (name == MapDisplay::getFactoryName()) {
        obj = new MapDisplay();
    }
    else if (name == DisplayRwr::getFactoryName()) {
        obj = new DisplayRwr();
    }



    // platform libraries
    if (obj == nullptr) { obj = mixr::graphics::factory(name); }
    if (obj == nullptr) { obj = mixr::glut::factory(name); }
    if (obj == nullptr) { obj = mixr::base::factory(name); }
    if (obj == nullptr) { obj = mixr::cigi::factory(name); }
    if (obj == nullptr) { obj = mixr::pov::factory(name); }
    if (obj == nullptr) { obj = mixr::instruments::factory(name); }
    if (obj == nullptr) { obj = mixr::simulation::factory(name); }
    if (obj == nullptr) { obj = mixr::models::factory(name); }
    if (obj == nullptr) { obj = mixr::terrain::factory(name); }
    if (obj == nullptr) { obj = mixr::dis::factory(name); }
    if (obj == nullptr) { obj = mixr::instruments::factory(name); }

    return obj;
}

// station builder
MyStation* builder(const std::string & filename)
{
    // read configuration file
    int num_errors{};
    mixr::base::Object* obj{ mixr::base::edl_parser(filename, factory, &num_errors) };
    if (num_errors > 0) {
        std::cerr << "File: " << filename << ", number of errors: " << num_errors << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // test to see if an object was created
    if (obj == nullptr) {
        std::cerr << "Invalid configuration file, no objects defined!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // do we have a base::Pair, if so, point to object in Pair, not Pair itself
    const auto pair = dynamic_cast<mixr::base::Pair*>(obj);
    if (pair != nullptr) {
        obj = pair->object();
        obj->ref();
        pair->unref();
    }

    // try to cast to proper object, and check
    const auto station = dynamic_cast<MyStation*>(obj);
    if (station == nullptr) {
        std::cerr << "Invalid configuration file!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return station;
}

//
int main(int argc, char* argv[])
{
    glutInit(&argc, argv);

    std::system("make-edl.cmd");

    // default configuration filename
    std::string configFilename{ "test0.edl" };

    // build a station
    station = builder(configFilename);

    // reset the Simulation
    station->event(mixr::base::Component::RESET_EVENT);

    // set timer for the background tasks
    const double dt{ 1.0 / static_cast<double>(frameRate) };
    const int millis{ static_cast<int>(dt * 1000) };

    // ensure everything is reset
    station->updateData(dt);
    station->updateTC(dt);
    station->event(mixr::base::Component::RESET_EVENT);

    glutTimerFunc(millis, timerFunc, 1);

    // create the Time Critical Thread (updateTC())
    station->createTimeCriticalProcess();

    glutMainLoop();
    return 0;
}

