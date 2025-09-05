#ifndef __KeyHandler_H__
#define __KeyHandler_H__

#include "mixr/linkage/IoHandler.hpp"
#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/models/player/air/Aircraft.hpp"
#include "mixr/models/dynamics/JSBSimModel.hpp"
#include "mixr/simulation/station.hpp"
#include <windows.h>

class KeyHandler final : public mixr::linkage::IoHandler
{
    DECLARE_SUBCLASS(KeyHandler, mixr::linkage::IoHandler)


public:
    KeyHandler();

    //void startAsyncProcessingImpl();

    void inputDevicesImpl(const double dt) final;
    void outputDevicesImpl(const double dt) final;

};

#endif