#include "MfdDisplay.hpp"
#include "mixr/graphics/readouts/NumericReadout.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/models/WorldModel.hpp"
#include "MyStation.hpp"
#include "MySimulation.hpp"
#include "mixr/models/player/air/Aircraft.hpp"
#include "mixr/models/player/Player.hpp"
#include "MyDisplay.hpp"
#include "mixr/graphics/Polygon.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/colors/Color.hpp"


IMPLEMENT_SUBCLASS(MfdDisplay, "MfdDisplay")
EMPTY_DELETEDATA(MfdDisplay)
EMPTY_SLOTTABLE(MfdDisplay)
EMPTY_COPYDATA(MfdDisplay)


MfdDisplay::MfdDisplay() {
	STANDARD_CONSTRUCTOR()
}

void MfdDisplay::updateData(const double dt) {
	BaseClass::updateData(dt);

	MyDisplay* display = dynamic_cast<MyDisplay*>(container());
	MyStation* station = dynamic_cast<MyStation*>(display->container());
	F16* plane = dynamic_cast<F16*>(station->getOwnship());

	std::string spName = subpageName();

	// if we are on playerscreen
	if (spName == plname) {
		mixr::base::PairStream* players = station->getPlayers();
		mixr::base::List::Item* playerItem = players->getFirstItem();
		int i = 0;

		while (playerItem != nullptr) {
			i++;
			std::string playerText = "player" + std::to_string(i); // text: player1, player2, etc.
			std::cout << "i: " << i << std::endl;
			if (subpage()->findByName(playerText.c_str()) != nullptr) {
				mixr::graphics::AsciiText* text = dynamic_cast<mixr::graphics::AsciiText*>(subpage()->findByName(playerText.c_str())->object());
				if (text->isVisible() == false) text->setVisibility(true);
				if (i == 1) {
					mixr::graphics::AsciiText* text1 = dynamic_cast<mixr::graphics::AsciiText*>(subpage()->findByName("player2")->object());
					mixr::graphics::AsciiText* text2 = dynamic_cast<mixr::graphics::AsciiText*>(subpage()->findByName("player3")->object());

					if (text1->isVisible() == true) text1->setVisibility(false);
					if (text2->isVisible() == true) text2->setVisibility(false);

				} if (i == 2) {
					mixr::graphics::AsciiText* text3 = dynamic_cast<mixr::graphics::AsciiText*>(subpage()->findByName("player3")->object());

					if (text3->isVisible() == true) text3->setVisibility(false);
				}
			}
			playerItem = playerItem->next;
		}


		//mixr::base::PairStream* players = station->getPlayers();
		//mixr::base::List::Item* playerItem = players->getFirstItem();
		//int i = 0;

		//// while there is another player in the playerlist (if two networked computers are on, should only loop through twice)
		//while (playerItem != nullptr) {
		//	i++;
		//	std::string playerText = "player" + std::to_string(i); // text: player1, player2, etc.
		//	mixr::base::String playerSlot(playerText.c_str()); // wait what is a cstring?
		//	
		//	if (subpage()->findByName(playerSlot) == nullptr) {
		//		mixr::graphics::AsciiText* text = new mixr::graphics::AsciiText();
		//		mixr::base::Color* color = new mixr::base::Color();
		//		color->setRed(1);
		//		std::cout << "player number: " << i << std::endl;
		//		text->setText("player1");
		//		text->setVisibility(true);
		//		text->setColor(color);
		//		//text->setChar();
		//		//text->line(1);
		//		//text->column(1);
		//		std::cout << "color of text: " << text->getColor() <<std::endl;
		//		std::cout << "char pos of text: " << text->getCharacterPos() <<std::endl;
		//		// other set functions
		//		mixr::base::Pair* pair = new mixr::base::Pair(playerSlot, text); // player1, player2, etc. these are the names of the slot with the text
		//		// do I need to translate it
		//		subpage()->addComponent(pair);
		//	}
		//	playerItem = playerItem->next;
		//}
	}


	if (spName == posname) {
		mixr::graphics::Polygon* poly = dynamic_cast<mixr::graphics::Polygon*>(subpage()->findByName("poly")->object());
		SendData sd5;
		SendData sd6;

		subpage()->send("xpos", UPDATE_VALUE, plane->getXPosition(), sd5);
		subpage()->send("ypos", UPDATE_VALUE, plane->getYPosition(), sd6);
	}

	if (plane->getXPosition() > 1000) {
		if (subpage()->findByName("plane2") != nullptr && subpage()->findByName("plane2")->object() != nullptr) {
			mixr::graphics::Polygon* plane2 = dynamic_cast<mixr::graphics::Polygon*>(subpage()->findByName("plane2")->object());
			if (plane2_x >= 5) {
				plane2->setVisibility(false);
				plane2 = nullptr; // is this how I can actually delete it?
			}
			else {
				plane2->setVisibility(true);
				plane2->lcTranslate(.05, .05);
				plane2_x = plane2_x + .05;
				plane2_y = plane2_y + .05;
			}
		}
	}

	if (spName == engname) {
		subpage()->send("fuelflow", UPDATE_VALUE, plane->getFuelFlow(), sd1);
		subpage()->send("fuelwt", UPDATE_VALUE, plane->getFuelWt(), sd2);
		subpage()->send("engpress", UPDATE_VALUE, plane->getOilPressure(), sd3);
		subpage()->send("engtemp", UPDATE_VALUE, plane->getEngTemp(), sd4);
	}

	changeParameters(plane);

	////////////////////////////////////////////////////////////////////////////////
		// Find and update the test RWR display
	   /*{
		  rwrDisplay = nullptr;
		  mixr::base::Pair* p{findByType(typeid(DisplayRwr))};
		  if (p != nullptr) rwrDisplay = dynamic_cast<DisplayRwr*>( p->object() );
	   }
	   if (rwrDisplay != nullptr && getOwnship() != nullptr) {
		  mixr::models::Rwr* rwr{};
		  mixr::base::Pair* pair{getOwnship()->getSensorByType(typeid(mixr::models::Rwr))};
		  if (pair != nullptr) rwr = static_cast<mixr::models::Rwr*>(pair->object());
		  rwrDisplay->setRwr(rwr);
	   }*/

}
////////////////////////////////////////////////////////////////////////////////
// mixr::simulation::Station* MfdDisplay::getStation()
//{
//	 
//	 MyDisplay* dis{ getMyDisplay() };
//	 //MFD Display is inside of Display so you need to get the Display then 
//	 if (dis != nullptr) {
//		 const auto sta = dynamic_cast<mixr::simulation::Station*>(findContainerByType(typeid(mixr::simulation::Station)));
//		 if (sta != nullptr) {
//			 return sta;
//		 }
//		 
//	 }
//	 
//}
//------------------------------------------------------------------------------
// Simulation access functions
//------------------------------------------------------------------------------
//mixr::models::Player* MfdDisplay::getOwnship()
//{
//	mixr::models::Player* p{};
//	mixr::simulation::Station* sta{ getStation() };
//	if (sta != nullptr) {
//		p = dynamic_cast<mixr::models::Player*>(sta->getOwnship());
//	}
//	return p;
//}
////getDisplay
//MyDisplay* MfdDisplay::getMyDisplay() {
//
//	if (myDisplay == nullptr) {
//		const auto display = dynamic_cast<MyDisplay*>(findContainerByType(typeid(MyDisplay)));
//		if (display != nullptr) myDisplay = display;
//	}
//	return myDisplay;
//
// }
////////////////////////////////////////////////////////////////////////////////



void MfdDisplay::changeParameters(F16* plane) {
	if (plane->getXPosition() > 0 || plane->getYPosition() > 0) {
		float fuelFlow = plane->getFuelFlow();
		float fuelWt = plane->getFuelWt();
		float pressure = plane->getOilPressure();
		float temp = plane->getEngTemp();
		plane->setFuelFlow(fuelFlow - .01);
		plane->setFuelWt(fuelWt - .01);
		// I chose to not change pressure during our flight
		//plane->setOilPressure(pressure - .1);
		plane->setEngTemp(temp + .001);

	}
}