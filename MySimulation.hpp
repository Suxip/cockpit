#pragma once

#include "mixr/models/WorldModel.hpp"
#include "mixr/base/numeric/Number.hpp"

class MySimulation final : public mixr::models::WorldModel
{
	DECLARE_SUBCLASS(MySimulation, mixr::models::WorldModel)

public:
	MySimulation();

private:
};