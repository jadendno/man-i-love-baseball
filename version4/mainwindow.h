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

    void handleLogin();
    void goToUsersMenu();
    void goToEditMenu();
    void logoutToLogin();
    void goToAdminsMenu();

    void clearLogin();




    //map setup
    void setupMap();

    //setup up undirected lines
    void drawConnection(QPointF from, QPointF to, const QString& labeltext);
    void showStadiumInfo(const QString& stadiumName);

    void on_dijAll_clicked();
    void on_dijAmerican_clicked();
    void on_dijNational_clicked();
    void populateComboBoxes();
    void on_planTripButton_clicked();
    void on_checkDistanceButton_clicked();

    void showScrollableMessage(const QString& title, const QString& content);

};
#endif // MAINWINDOW_H
