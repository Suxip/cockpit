#pragma once
#include "mixr/ui/glut/GlutDisplay.hpp"
#include "F16.hpp"
#include "mixr/graphics/Page.hpp"
#include <cmath>
#include "mixr/graphics/readouts/AsciiText.hpp"
#include "mixr/interop/dis/NetIO.hpp"




class MfdDisplay : public mixr::glut::GlutDisplay
{
	DECLARE_SUBCLASS(MfdDisplay, mixr::glut::GlutDisplay)

public:
	MfdDisplay();

	void updateData(const double dt);

	////////////////////////////////////////////////////////////////////////////////
	//mixr::simulation::Station* getStation();
	//mixr::models::Player* getOwnship();
	//MyDisplay* getMyDisplay();
	//////////////////////////////////////////////////////////////////////////////////
private:
	float plane2_x = -7;
	float plane2_y = 4.65;

	SendData sd1;
	SendData sd2;
	SendData sd3;
	SendData sd4;

	void changeParameters(F16* plane);

	std::string posname = "posscreen";
	std::string engname = "enginescreen";
	std::string plname = "playerscreen";

};

