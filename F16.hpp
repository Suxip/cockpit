#pragma once
#include "mixr/models/player/air/Aircraft.hpp"
#include "mixr/models/player/Player.hpp"

class F16 final : public mixr::models::Aircraft
{
	DECLARE_SUBCLASS(F16, mixr::models::Aircraft)
public:
	F16();

	double getFuelFlow() { return fuelFlow; }
	double getFuelWt() { return fuelWt; }
	double getOilPressure() { return oilPressure; }
	double getEngTemp() { return engTemp; }

	void setFuelFlow(float ff) { fuelFlow = ff; }
	void setFuelWt(float wt) { fuelWt = wt; }
	void setOilPressure(float op) { oilPressure = op; }
	void setEngTemp(float et) { engTemp = et; }

private:
	double fuelFlow = 8000; // lbs per hr
	double fuelWt = 6970; // lb
	double oilPressure = 40; // psi
	double engTemp = 900; // C


};