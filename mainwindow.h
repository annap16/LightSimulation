#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
class Grid;
class QString;
class QVector3D;
class QGraphicsScene;
class QCheckBox;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<QVector3D*> readFile(QString fileName);
    QVector3D* processLine(QByteArray line);
    Grid* grid;
    QGraphicsScene* scene;
    void show();
protected:
    void resizeEvent(QResizeEvent* event) override;
    void resizeFunction();
private slots:
    void onColorButtonClicked();
    void onColorLightButtonClicked();
    void onFileButtonClicked();
    void onDeleteButtonClicked();
    void onNormalMapLoadButtonClicked();
    void onNormalMapDeleteButtonClicked();
private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
