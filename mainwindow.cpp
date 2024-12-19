#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "grid.h"
#include <QGraphicsView>
#include <QFile>
#include <QString>
#include <QVector3D>
#include <QSlider>
#include <QLabel>
#include <QCheckBox>
#include <QGraphicsScene>
#include<QResizeEvent>
#include <QPushButton>
#include <QColorDialog>
#include <QFileDialog>
#include "paintingparams.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    ui->slider1->setRange(2, 30);
    ui->slider1->setValue(18);

    ui->slider2->setRange(0, 90);
    ui->slider2->setValue(45);

    ui->slider3->setRange(0, 10);
    ui->slider3->setValue(0);

    ui->sliderKd->setRange(0, 100);
    ui->sliderKd->setValue(80);

    ui->sliderKs->setRange(0, 100);
    ui->sliderKs->setValue(50);

    ui->sliderM->setRange(1, 100);
    ui->sliderM->setValue(50);

    ui->sliderZ->setRange(30, 200);
    ui->sliderZ->setValue(100);

    ui->checkBox1->setChecked(true);

    ui->checkBoxTurnOffLight->setChecked(false);
    ui->checkBoxRefectors->setChecked(false);

    ui->horizontalSliderML->setRange(1,10);
    ui->horizontalSliderML->setValue(5);

    QString filePath = QDir::currentPath() + QDir::separator() + "data";
    QVector<QVector3D*> samplePoints = readFile(filePath);
    grid = new Grid(scene, samplePoints);

    QObject::connect(ui->slider1, &QSlider::valueChanged, grid, &Grid::onSliderChangedPrecision);
    QObject::connect(ui->slider2, &QSlider::valueChanged, grid, &Grid::onSliderChangedAlpha);
    QObject::connect(ui->slider3, &QSlider::valueChanged, grid, &Grid::onSliderChangedBetha);
    QObject::connect(ui->sliderKd, &QSlider::valueChanged, grid, &Grid::onSliderChangedKd);
    QObject::connect(ui->sliderKs, &QSlider::valueChanged, grid, &Grid::onSliderChangedKs);
    QObject::connect(ui->sliderM, &QSlider::valueChanged, grid, &Grid::onSliderChangedM);
    QObject::connect(ui->sliderZ, &QSlider::valueChanged, grid, &Grid::onSliderChangedZ);
    QObject::connect(ui->horizontalSliderML, &QSlider::valueChanged, grid, &Grid::onSliderChangedML);


    QObject::connect(ui->checkBox1, &QCheckBox::checkStateChanged, grid, &Grid::onCheckBoxStateChanged);
    QObject::connect(ui->checkBox2, &QCheckBox::checkStateChanged, grid, &Grid::onCheckBox2StateChanged);
    QObject::connect(ui->checkBox3, &QCheckBox::checkStateChanged, grid, &Grid::onCheckBox3StateChanged);
    QObject::connect(ui->checkBoxNormalMap, &QCheckBox::checkStateChanged, grid, &Grid::onCheckBoxNormalMapStateChanged);
    QObject::connect(ui->checkBoxLight, &QCheckBox::checkStateChanged, grid, &Grid::onCheckBoxLightStateChanged);
    QObject::connect(ui->checkBoxTurnOffLight, &QCheckBox::checkStateChanged, grid, &Grid::onCheckBoxTurnOffLightStateChanged);
    QObject::connect(ui->checkBoxRefectors, &QCheckBox::checkStateChanged, grid, &Grid::onCheckBoxReflectorsStateChanged);

    QObject::connect(ui->colorButton, &QPushButton::clicked, this, &MainWindow::onColorButtonClicked);
    QObject::connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
    QObject::connect(ui->colorLightButton, &QPushButton::clicked, this, &MainWindow::onColorLightButtonClicked);
    QObject::connect(ui->fileButton, &QPushButton::clicked, this, &MainWindow::onFileButtonClicked);
    QObject::connect(ui->normalMapLoadButton, &QPushButton::clicked, this, &MainWindow::onNormalMapLoadButtonClicked);
    QObject::connect(ui->deleteNormalMapButton, &QPushButton::clicked, this, &MainWindow::onNormalMapDeleteButtonClicked);
}

MainWindow::~MainWindow()
{
    delete grid;
    delete scene;
    delete ui;
}

QVector<QVector3D*> MainWindow::readFile(QString fileName)
{
    QFile file(fileName);
    QVector<QVector3D*> listControlPoints;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Cant open file";
        exit(1);
    }

    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        listControlPoints.append(processLine(line));
    }
    return listControlPoints;
}

QVector3D* MainWindow::processLine(QByteArray line)
{
    line = line.trimmed();
    QList<QByteArray> listSplited =  line.split(' ');
    if(listSplited.size()!=3)
    {
        qDebug()<<"Wrong input data";
        exit(1);
    }
    return new QVector3D(listSplited[0].toInt(), listSplited[1].toInt(), listSplited[2].toInt());
}
void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    resizeFunction();
}

void MainWindow::resizeFunction()
{
    scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    grid->resizeImage(ui->graphicsView->width(), ui->graphicsView->height());
}


void MainWindow::show()
{
    QMainWindow::show();
    resizeFunction();
}

void MainWindow::onColorButtonClicked()
{
    bool awakeLightMovement=false;
    if(grid->lightAnimation->moving)
    {
        awakeLightMovement = true;
        grid->lightAnimation->moving = false;
        grid->lightAnimation->stopTimer();
    }
    QColor color = QColorDialog::getColor(grid->paintParams->gridColor, this, "Select Color");
    if (color.isValid()) {
        grid->paintParams->gridColor = color;
        grid->update();
    }
    if(awakeLightMovement)
    {
        grid->lightAnimation->moving = true;
        grid->lightAnimation->startTimer();
    }
}



void MainWindow::onColorLightButtonClicked()
{
    bool awakeLightMovement=false;
    if(grid->lightAnimation->moving)
    {
        awakeLightMovement = true;
        grid->lightAnimation->moving = false;
        grid->lightAnimation->stopTimer();
    }
    QColor color = QColorDialog::getColor(grid->paintParams->lightColor, this, "Select Color");
    if (color.isValid()) {
        grid->paintParams->lightColor = color;
        grid->update();
    }
    if(awakeLightMovement)
    {
        grid->lightAnimation->moving = true;
        grid->lightAnimation->startTimer();
    }
}

void MainWindow::onFileButtonClicked()
{

    bool awakeLightMovement=false;
    if(grid->lightAnimation->moving)
    {
        awakeLightMovement = true;
        grid->lightAnimation->moving = false;
        grid->lightAnimation->stopTimer();
    }
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Images (*.jpg *.png);;All Files (*)");

    if (!fileName.isEmpty())
    {
        if(grid->paintParams->image)
        {
            delete grid->paintParams->image;
            grid->paintParams->image = nullptr;
        }

        grid->paintParams->image = new QImage(fileName);
        if (grid->paintParams->image ->isNull())
        {
            qDebug() << "Unable to read a texture from a file:" << fileName;
            delete grid->paintParams->image ;
            grid->paintParams->image  = nullptr;
        }
        else
        {
            ui->checkBox3->setChecked(false);
        }
    }
    if(awakeLightMovement)
    {
        grid->lightAnimation->moving = true;
        grid->lightAnimation->startTimer();
    }
    grid->update();
}

void MainWindow::onNormalMapLoadButtonClicked()
{
    bool awakeLightMovement=false;
    if(grid->lightAnimation->moving)
    {
        awakeLightMovement = true;
        grid->lightAnimation->moving = false;
        grid->lightAnimation->stopTimer();
    }

    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Images (*.jpg *.png);;All Files (*)");

    if (!fileName.isEmpty())
    {
        if(grid->paintParams->normalMap)
        {
            delete grid->paintParams->normalMap;
            grid->paintParams->normalMap = nullptr;
        }
        grid->paintParams->normalMap = new QImage(fileName);
        if (grid->paintParams->normalMap ->isNull())
        {
            qDebug() << "Unable to read a normal map from a file:" << fileName;
            delete grid->paintParams->normalMap;
            grid->paintParams->normalMap = nullptr;
        }
        else
        {
            ui->checkBoxNormalMap->setChecked(false);
        }
        grid->update();
    }
    if(awakeLightMovement)
    {
        grid->lightAnimation->moving = true;
        grid->lightAnimation->startTimer();
    }
}


void MainWindow::onDeleteButtonClicked()
{
    bool awakeLightMovement=false;
    if(grid->lightAnimation->moving)
    {
        awakeLightMovement = true;
        grid->lightAnimation->moving = false;
        grid->lightAnimation->stopTimer();
    }
    if(grid->paintParams->image)
    {
        delete grid->paintParams->image;
        grid->paintParams->image = nullptr;
        ui->checkBox3->setChecked(false);
        grid->update();
    }
    if(awakeLightMovement)
    {
        grid->lightAnimation->moving = true;
        grid->lightAnimation->startTimer();
    }
}

void MainWindow::onNormalMapDeleteButtonClicked()
{
    bool awakeLightMovement=false;
    if(grid->lightAnimation->moving)
    {
        awakeLightMovement = true;
        grid->lightAnimation->moving = false;
        grid->lightAnimation->stopTimer();
    }
    if(grid->paintParams->normalMap)
    {
        delete grid->paintParams->normalMap;
        grid->paintParams->normalMap = nullptr;
        ui->checkBoxNormalMap->setChecked(false);
        grid->update();
    }
    if(awakeLightMovement)
    {
        grid->lightAnimation->moving = true;
        grid->lightAnimation->startTimer();
    }
}


