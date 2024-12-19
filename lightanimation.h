#ifndef LIGHTANIMATION_H
#define LIGHTANIMATION_H
class QVector3D;
class Grid;
#include <QObject>
#include <thread>
#include <atomic>

class LightAnimation: public QObject
{
    Q_OBJECT
public:
    LightAnimation(Grid* _grid, QVector3D* _lightPos);
    ~LightAnimation()=default;
    Grid* grid;
    std::thread timerThread;
    QVector3D* lightPos;
    std::atomic<bool> moving;
    double radius = 50;
    double angularSpeed = 0.001;
    double timerStartTime;
    double startAngle;
    void startTimer();
    void stopTimer();
    void performMovement();
signals:
    void lightPositionUpdated();

};

#endif // LIGHTANIMATION_H
