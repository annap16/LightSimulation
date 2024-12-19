#include "lightanimation.h"
#include "grid.h"
//#include <thread>
#include <QDateTime>
#include <chrono>

LightAnimation::LightAnimation(Grid* _grid, QVector3D* _lightPos)
{
    lightPos = _lightPos;
    grid = _grid;
    moving = false;
    startAngle = 0;
}

void LightAnimation::startTimer()
{
    timerStartTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    timerThread = std::thread(&LightAnimation::performMovement, this);
}

void LightAnimation::stopTimer()
{
    if (timerThread.joinable())
    {
        timerThread.join();
    }
}

void LightAnimation::performMovement()
{
    double theta = 0;
    while(moving)
    {
        double currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();

        double elapsedTime = currentTime - timerStartTime;

        theta = fmod(angularSpeed * elapsedTime, 2 * M_PI);
        theta += startAngle;

        double x =   radius * cos(theta);
        double y =  -radius * sin(theta);
        lightPos->setX(x);
        lightPos->setY(y);
        emit lightPositionUpdated();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    startAngle = theta;
}

