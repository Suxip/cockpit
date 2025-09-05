#include "MapDisplay.hpp"
#include "MyStation.hpp"


IMPLEMENT_SUBCLASS(MapDisplay, "MapDisplay")
EMPTY_COPYDATA(MapDisplay)
EMPTY_SLOTTABLE(MapDisplay)
EMPTY_DELETEDATA(MapDisplay)


MapDisplay::MapDisplay() {
    STANDARD_CONSTRUCTOR()
}



void MapDisplay::updateData(const double dt) {

    BaseClass::updateData(dt);

    auto* page = subpage();

    if (page != nullptr) {

        mixr::base::Pair* pair{ page->findByType(typeid(mixr::graphics::SymbolLoader)) };
        if (pair != nullptr) {

            loader = dynamic_cast<mixr::graphics::SymbolLoader*>(pair->object());
            if (loader != nullptr) {
                ;                loader->ref();
            }

        }
        mixr::models::Aircraft* plane{};
        mixr::models::Aircraft* newAircraft[MAX_AIRCRAFTS];
        int numAircrafts{};

        const auto sta = static_cast<mixr::simulation::Station*>(findContainerByType(typeid(mixr::simulation::Station)));
        plane = dynamic_cast<mixr::models::Aircraft*>(sta->getOwnship());
        mixr::base::PairStream* stream = sta->getPlayers();

        if (stream != nullptr) {

            mixr::models::Aircraft* newPlayers[MAX_AIRCRAFTS]{};
            int numNewPlayers{};
            mixr::base::List::Item* item{ stream->getFirstItem() };
            while (item != nullptr) {
                const auto pair = static_cast<mixr::base::Pair*>(item->getValue());
                if (pair != nullptr) {

                    const auto player = dynamic_cast<mixr::models::Aircraft*>(pair->object());
                    if (player != nullptr) {
                        newPlayers[numNewPlayers] = player;
                        newPlayers[numNewPlayers++]->ref();
                    }
                }
                item = item->getNext();
            }


            for (int i = 0; i < MAX_AIRCRAFTS; i++) {
                if (player[i] != nullptr) {
                    bool match{};
                    for (int j = 0; j < numNewPlayers && !match; j++) {
                        if (player[i] == newPlayers[j]) {
                            // if they do match, get rid of our new player, so we don't re-add it
                            // later accidentally
                            match = true;
                            newPlayers[j]->unref();
                            newPlayers[j] = nullptr;
                        }
                    }
                    // if our player doesn't match, we remove it from our list
                    if (!match) {
                        loader->removeSymbol(playerIdx[i]);
                        player[i]->unref();
                        player[i] = nullptr;
                        playerIdx[i] = -1;
                    }
                }
            }

            for (int i = 0; i < numNewPlayers; i++) {
                // make sure this player wasn't deleted earlier
                if (newPlayers[i] != nullptr) {
                    bool found{};
                    for (int j = 0; j < MAX_AIRCRAFTS && !found; j++) {
                        if (player[j] == nullptr) {
                            found = true;
                            // found an empty player, let's set him!
                            player[j] = newPlayers[i];
                            player[j]->ref();
                            int type{ 1 };
                            if (player[j]->isSide(mixr::models::Player::RED)) type = 2;
                            playerIdx[j] = loader->addSymbol(type, "player");
                            if (player[j]->getName() != nullptr) {
                                loader->updateSymbolText(playerIdx[j], "name", player[j]->getName()->getString());
                            }
                            // now let's empty our new player list
                            newPlayers[i]->unref();
                            newPlayers[i] = nullptr;
                        }
                    }
                }
            }

            // ok, now update our symbols' positions
            for (int i = 0; i < MAX_AIRCRAFTS; i++) {
                if (player[i] != nullptr) {
                    loader->updateSymbolPositionLL(playerIdx[i], player[i]->getLatitude(), player[i]->getLongitude());
                    loader->updateSymbolHeading(playerIdx[i], player[i]->getHeadingD());
                }
            }

        }

    }

    //Update player position


    //get station
    //get player
    //


}

mixr::simulation::Station* MapDisplay::getStation()
{
    if (myStation == nullptr) {
        const auto s = dynamic_cast<mixr::simulation::Station*>(findContainerByType(typeid(mixr::simulation::Station)));
        if (s != nullptr) myStation = s;
    }
    return myStation;
}


mixr::models::Player* MapDisplay::getOwnship()
{
    mixr::models::Player* p{};
    mixr::simulation::Station* sta{ getStation() };
    if (sta != nullptr) {
        p = dynamic_cast<mixr::models::Player*>(sta->getOwnship());
    }
    return p;
}