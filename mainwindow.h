#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QObject>

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    QGraphicsEllipseItem* editEllipse;

    void handleLogin();
    void goToUsersMenu();
    void goToEditMenu();
    void logoutToLogin();
    void goToAdminsMenu();
    void searchStadium();
    void editStadiumInfo();

    void clearLogin();


    //map setup
    void setupMap();

    //setup up undirected lines
    void drawConnection(QPointF from, QPointF to, const QString& labeltext);

};
#endif // MAINWINDOW_H
