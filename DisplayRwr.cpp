#include "DisplayRwr.hpp"

#include "mixr/models/system/Rwr.hpp"

using namespace mixr;

IMPLEMENT_SUBCLASS(DisplayRwr, "DisplayRwr")
EMPTY_SLOTTABLE(DisplayRwr)
EMPTY_DELETEDATA(DisplayRwr)
EMPTY_COPYDATA(DisplayRwr)


DisplayRwr::DisplayRwr()
{
    STANDARD_CONSTRUCTOR()
}


void DisplayRwr::drawFunc()
{
    // Need a RWR to draw; if not, just draw a big X
    if (rwr == nullptr) {
        glBegin(GL_LINES);
        glVertex3d(-1.0, -1.0, 0.0);
        glVertex3d(1.0, 1.0, 0.0);
        glVertex3d(-1.0, 1.0, 0.0);
        glVertex3d(1.0, -1.0, 0.0);
        glEnd();
        return;
    }

    // ---
    // Draw the RWR signal rays
    // ---

    GLdouble  ocolor[4];
    glGetDoublev(GL_CURRENT_COLOR, ocolor);

    glColor3d(0.0, 1.0, 0.0);

    unsigned int n = rwr->getNumberOfRays();
    for (unsigned int i = 0; i < n; i++) {
        GLdouble azr = (base::angle::D2RCC * rwr->getRayAzimuth(i));
        GLdouble pwr = rwr->getRay(i);

        GLdouble up = cos(azr) * pwr;
        GLdouble right = sin(azr) * pwr;
        if (right != 0) {
        }
        glBegin(GL_LINES);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(right, up, 0.0);
        glEnd();
    }

    glColor4dv(ocolor);

    // Draw dot for radar
    glColor3d(1.0, 0.0, 0.0);
    for (const mixr::base::Vec3d& pos : planeVec) {
        glBegin(GL_TRIANGLE_FAN);
        double xPos = pos.x(), yPos = pos.y();
        // get the total euclidean  dis between planes
        // x2 - x1 + y2 - y1
        double distance = std::sqrt(std::pow(xPos, 2) + std::pow(yPos, 2));

        double section;
        double dMeters;




        if (distance <= 3000) {
            section = 0;

            dMeters = (distance - 0) / 3000;

        }
        else if (distance <= 10000) {
            section = .2;

            dMeters = (distance - 3000) / 10000;

        }
        else if (distance <= 30000) {
            section = .4;
            dMeters = (distance - 10000) / 30000;

        }
        else if (distance <= 70000) {
            section = .6;
            dMeters = (distance - 30000) / 70000;
        }
        else {
            section = .8;
            dMeters = (distance - 70000) / 150000;
        }
        dMeters /= 5;
        dMeters += section;
        double angle = std::atan2(yPos, xPos);
        double screenX = std::cos(angle) * dMeters;
        double screenY = std::sin(angle) * dMeters;
        glVertex3d(screenY, screenX, 0); // center of the circle
        for (int i = 0; i < 50; ++i) {
            GLdouble theta = 2.0 * 3.141592 * i / 30;
            GLdouble x = (screenY)+.05 * cos(theta);
            GLdouble y = (screenX)+.05 * sin(theta);
            glVertex3d(x, y, 0.0);
        }
        glEnd();
    }

    // draw missiles
    glColor3d(.5, 0.5, 0.5);
    for (const mixr::base::Vec3d& pos : missileVec) {
        glBegin(GL_TRIANGLE_FAN);
        double xPos = pos.x(), yPos = pos.y();
        // get the total euclidean  dis between planes
        // x2 - x1 + y2 - y1
        double distance = std::sqrt(std::pow(xPos, 2) + std::pow(yPos, 2));

        double section;
        double dMeters;




        if (distance <= 3000) {
            section = 0;

            dMeters = (distance - 0) / 3000;

        }
        else if (distance <= 10000) {
            section = .2;

            dMeters = (distance - 3000) / 10000;

        }
        else if (distance <= 30000) {
            section = .4;
            dMeters = (distance - 10000) / 30000;

        }
        else if (distance <= 70000) {
            section = .6;
            dMeters = (distance - 30000) / 70000;
        }
        else {
            section = .8;
            dMeters = (distance - 70000) / 150000;
        }
        dMeters /= 5;
        dMeters += section;
        double angle = std::atan2(yPos, xPos);
        double screenX = std::cos(angle) * dMeters;
        double screenY = std::sin(angle) * dMeters;
        glVertex3d(screenY, screenX, 0); // center of the circle
        for (int i = 0; i < 50; ++i) {
            GLdouble theta = 2.0 * 3.141592 * i / 30;
            GLdouble x = (screenY)+.05 * cos(theta);
            GLdouble y = (screenX)+.05 * sin(theta);
            glVertex3d(x, y, 0.0);
        }
        glEnd();
    }
}