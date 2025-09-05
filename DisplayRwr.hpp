#pragma once
#include "mixr/graphics/Graphic.hpp"
#include "mixr/base/Vectors.hpp"
#include <vector>

namespace mixr {
    namespace models { class Rwr; }
}

class DisplayRwr final : public mixr::graphics::Graphic
{
    DECLARE_SUBCLASS(DisplayRwr, mixr::graphics::Graphic)

public:
    DisplayRwr();

    mixr::models::Rwr* getRwr() { return rwr; }
    const mixr::models::Rwr* getRwr() const { return rwr; }
    void setRwr(mixr::models::Rwr* s) { rwr = s; }
    void setRadar(const std::vector<mixr::base::Vec3d>& pVec, const std::vector<mixr::base::Vec3d>& mVec) { planeVec = pVec, missileVec = mVec; }
    void drawFunc() final;
    //void updateData(const double dt = 0.0) final;

private:
    mixr::models::Rwr* rwr{};     // The test RWR sensor
    std::vector<mixr::base::Vec3d> planeVec;
    std::vector<mixr::base::Vec3d> missileVec;

};