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
#include "core/merch.h"

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

    // map from stadium ID → its scene‐coordinates
    std::unordered_map<int, QPointF> m_idToPoint;

    // items we draw in red for the “highlighted” path
    std::vector<QGraphicsLineItem*>       m_highlightedEdges;
    std::vector<QGraphicsSimpleTextItem*> m_edgeLabels;


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
    void clearHighlights();

    QString computeVisitOrderAndDistance(const std::vector<int> &mustVisitIds);
    std::vector<int> getFullVisitPath(const std::vector<int>& mustVisitIds);

    // NEW: keep the most‐recent stadium name that was clicked
    QString m_lastSelectedStadiumName;

    std::vector<Merch> m_merchList;   // holds everything loaded from merch.json

    // A small struct to keep track of what the user “bought”:
    struct Purchased {
        QString name;
        int     qty;
        double  unitPrice;
        double  totalPrice() const { return qty * unitPrice; }
    };
    std::vector<Purchased> m_cart;     // in‐memory “shopping cart”


private slots:
    // slot to handle when an ellipse is clicked
    void onStadiumEllipseClicked(const QString &stadiumName);

    void on_pqBtn_clicked();
    void on_clrBtn_clicked();
    void showViewStadiums();
    void showDataDialog();
    void fillNL();
    void fillAL();
    void addMerch();

};
#endif // MAINWINDOW_H
