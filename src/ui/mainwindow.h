#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QObject>
#include <QQueue>
#include "core/stadium.h"

QT_BEGIN_NAMESPACE
namespace Ui {

class ClickableEllipse;

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

    std::vector<Stadium> m_stadiums;
    std::vector<std::vector<Edge>> m_graph;
    std::unordered_map<QString,int> m_nameToId;
    QQueue<QString> m_stadiumQueue;

    std::pair<std::vector<double>, std::vector<int>> dijkstra(int srcIndex);

    int m_startStadiumId;

    //map setup
    void setupMap();
    void loadAllStadiums();
    void buildNameLookup();
    QString computeVisitOrderAndDistance(const std::vector<int> &mustVisitIds);

private slots:
    // slot to handle when an ellipse is clicked
    void onStadiumEllipseClicked(const QString &stadiumName);

    void on_pqBtn_clicked();
    void on_clrBtn_clicked();

};
#endif // MAINWINDOW_H
