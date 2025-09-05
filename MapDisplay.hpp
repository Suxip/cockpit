#pragma once
#include "mixr/ui/glut/GlutDisplay.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/models/player/air/Aircraft.hpp"
#include "mixr/simulation/Station.hpp"
#include "mixr/graphics/Page.hpp"
#include "mixr/graphics/SymbolLoader.hpp"
#include "F16.hpp"
#include "MfdDisplay.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/List.hpp"



class MapDisplay : public mixr::glut::GlutDisplay
{
	DECLARE_SUBCLASS(MapDisplay, mixr::glut::GlutDisplay)

public:
	MapDisplay();
	void updateData(const double dt);

	double x{};
	double y{};

	mixr::models::Player* getOwnship();
	mixr::simulation::Station* getStation();
private:
	mixr::base::safe_ptr<mixr::simulation::Station> myStation;
	mixr::graphics::SymbolLoader* loader{};
	static const int MAX_AIRCRAFTS{ 200 };


	std::array<mixr::models::Aircraft*, MAX_AIRCRAFTS> player{};   // player pointer
	std::array<int, MAX_AIRCRAFTS> playerIdx{};
};