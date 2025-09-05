#include "MySimulation.hpp"
#include "mixr/base/numeric/Number.hpp"

IMPLEMENT_SUBCLASS(MySimulation, "MySimulation")
EMPTY_DELETEDATA(MySimulation)
EMPTY_SLOTTABLE(MySimulation)


MySimulation::MySimulation() {
    STANDARD_CONSTRUCTOR()
}

void MySimulation::copyData(const MySimulation& org, const bool) {
    BaseClass::copyData(org);
}