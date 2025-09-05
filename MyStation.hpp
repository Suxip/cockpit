#ifndef __MyStation_H__
#define __MyStation_H__

#include "mixr/simulation/Station.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "MyDisplay.hpp"
#include "MapDisplay.hpp"

// -------------------------------------------------------------------------------
// Class: MyStation
//
// Description:
//      Derived Station that will know it has a display and can be accessed through
//      it.  This also runs the simulation, and acts as the interface between the
//      graphics and simulation.
// -------------------------------------------------------------------------------

class MyStation final : public mixr::simulation::Station
{
	DECLARE_SUBCLASS(MyStation, mixr::simulation::Station)

public:
	MyStation();

	void reset() final;

	MyDisplay* getDisplay();
	MapDisplay* getMapDisplay();


private:


	MyDisplay* display{};
	MapDisplay* mapDisplay{};

	bool displayInit{};               // true or false - is our display created?
	bool mapDisplayInit{};

	bool setSlotDisplay(MyDisplay* dis);
	bool setSlotMapDisplay(MapDisplay* dis);
};

#endif