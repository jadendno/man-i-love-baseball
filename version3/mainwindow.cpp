#include "mainwindow.h"
#include "graphs.h"
#include "ui_mainwindow.h"
#include "clickableellipseitem.h"


/**********************************************************************
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
***********************************************************************
sets up entire application
***********************************************************************
Pre conditions:
- established set of classes and set of functions

Post conditions:
- sets up every buttons, edit box, and functions
*************************************************************************/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->users_menu);
    //ui->stackedWidget->setCurrentWidget(ui->users_menu);

    scene = new QGraphicsScene(this);
    ui->usMap->setScene(scene);
    setupMap();

    connect(ui->passinput, &QLineEdit::returnPressed, this, &MainWindow::handleLogin);
    connect(ui->btnLogin, &QPushButton::clicked, this, &MainWindow::handleLogin);
    connect(ui->usersMenuBtn, &QPushButton::clicked, this, &MainWindow::goToUsersMenu);
    connect(ui->editBtn, &QPushButton::clicked, this, &MainWindow::goToEditMenu);
    connect(ui->logoutBtn1, &QPushButton::clicked, this, &MainWindow::logoutToLogin);
    connect(ui->logoutBtn2, &QPushButton::clicked, this, &MainWindow::logoutToLogin);
    connect(ui->logoutBtn3, &QPushButton::clicked, this, &MainWindow::logoutToLogin);
    connect(ui->goBackBtn, &QPushButton::clicked, this, &MainWindow::goToAdminsMenu);


}

MainWindow::~MainWindow()
{
    delete ui;
}


/**********************************************************************
void MainWindow::handleLogin()
***********************************************************************
Handles login upon startup of application
***********************************************************************
Pre conditions:
- established set of classes
- requires user input
- verify user input

Post conditions:
proceeds to specific pages based on whether the user is a client or admin
*************************************************************************/
void MainWindow::handleLogin(){

    QString username = ui->userinput->text();
    QString password = ui->passinput->text();

    if(username == "admin" && password == "password"){
        goToAdminsMenu();
        clearLogin();
    }
    else if(username == "user" && password == "travel"){
        goToUsersMenu();
        clearLogin();
    }else
        QMessageBox::warning(this, "login Failed", "Invalid credentials.");

}

/**********************************************************************
void MainWindow::goToAdminsMenu()
***********************************************************************
transitions to admin's page
***********************************************************************
Pre conditions:
- established set of classes

Post conditions:
proceeds to admin's page
*************************************************************************/
void MainWindow::goToAdminsMenu(){
    ui->stackedWidget->setCurrentWidget(ui->admin_menu);
}

/**********************************************************************
void MainWindow::goToUsersMenu()
***********************************************************************
transitions to main page where user/client edit their trips
***********************************************************************
Pre conditions:
- established set of classes

Post conditions:
proceeds to main page
*************************************************************************/
void MainWindow::goToUsersMenu(){
    ui->stackedWidget->setCurrentWidget(ui->users_menu);
}

/**********************************************************************
void MainWindow::goToUsersMenu()
***********************************************************************
transitions to admin specific page for editing info for stadiums
***********************************************************************
Pre conditions:
- established set of classes

Post conditions:
proceeds to edit page
*************************************************************************/
void MainWindow::goToEditMenu(){
    ui->stackedWidget->setCurrentWidget(ui->editInfo_menu);
}

/**********************************************************************
void MainWindow::logoutToLogin()
***********************************************************************
transitions to login page
***********************************************************************
Pre conditions:
- established set of classes

Post conditions:
proceeds to login page
*************************************************************************/
void MainWindow::logoutToLogin(){
    ui->stackedWidget->setCurrentWidget(ui->loginPage);
}

/**********************************************************************
void MainWindow::clearLogin()
***********************************************************************
clears out text when logging out and trnasition to login page
***********************************************************************
Pre conditions:
- established set of classes

Post conditions:
clears text within text box on login page
*************************************************************************/
void MainWindow::clearLogin(){
    ui->userinput->clear();
    ui->passinput->clear();
}


void MainWindow::drawConnection(QPointF from, QPointF to, const QString& labeltext)
{
    QPen linePen(Qt::black);
    linePen.setWidth(2);

    //draw line
    QGraphicsLineItem* line = scene->addLine(QLineF(from, to), linePen);

    //add label
    QPointF midpoint = (from + to)/2;
    QGraphicsTextItem* label = scene->addText(labeltext);
    label->setDefaultTextColor(Qt::black);
    label->setZValue(10);
    label->setPos(midpoint.x(), midpoint.y()-10);

}

/**********************************************************************
void MainWindow::setupMap()
***********************************************************************
Generates map
***********************************************************************
Pre conditions:
- established set of classes
- valid image of the us map

Post conditions:
Successfully generates map on user's page
*************************************************************************/
void MainWindow::setupMap()
{
    // Load the map image
    QPixmap map(":/images/us_Map.png");

    if (map.isNull()) {
        QMessageBox::warning(this, "Error", "Map image not found");
        return;
    }

    // Add scaled map to scene
    QPixmap scaledMap = map.scaled(ui->usMap->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* mapItem = scene->addPixmap(scaledMap);

    // Load stadiums
    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");

    const auto& positions = graph.getStadiumPositions();

    // Add test edges manually (replace with real data or file-based later)
    graph.addEdge("SafeCo Field", "AT&T Park", 680);
    graph.addEdge("SafeCo Field", "Target Field", 1390);
    graph.addEdge("SafeCo Field", "Rogers Centre", 2070);
    graph.addEdge("AT&T Park", "O.co Coliseum", 0);
    graph.addEdge("O.co Coliseum", "Chase Field", 650);
    graph.addEdge("O.co Coliseum", "Dodger Stadium", 340);
    graph.addEdge("Angels Stadium of Anaheim", "Dodger Stadium", 0);
    graph.addEdge("Target Field", "Dodger Stadium", 1500);
    graph.addEdge("Angels Stadium of Anaheim", "Petco Park", 110);
    graph.addEdge("Chase Field", "Petco Park", 300);
    graph.addEdge("Coors Field", "Petco Park", 830);
    graph.addEdge("Chase Field", "Globe Life Park in Arlington", 870);
    graph.addEdge("Chase Field", "Minute Maid Park", 1115);
    graph.addEdge("Globe Life Park in Arlington", "Minute Maid Park", 230);
    graph.addEdge("Globe Life Park in Arlington", "Coors Field", 650);
    graph.addEdge("Globe Life Park in Arlington", "Turner Field", 740);
    graph.addEdge("Busch Stadium", "Minute Maid Park", 680);
    graph.addEdge("Marlins Park", "Minute Maid Park", 965);
    graph.addEdge("Tropicana Field", "Minute Maid Park", 790);
    graph.addEdge("Tropicana Field", "Marlins Park", 210);
    graph.addEdge("Tropicana Field", "Great America Ball Park", 790);
    graph.addEdge("Marlins Park", "Turner Field", 600); ///////
    graph.addEdge("Marlins Park", "Nationals Park", 930);
    graph.addEdge("Marlins Park", "Fenway Park", 1255);
    graph.addEdge("Citi Field", "Fenway Park", 195);
    graph.addEdge("Rogers Centre", "Fenway Park", 430);
    graph.addEdge("Rogers Centre", "Comerica Park", 210);
    graph.addEdge("Rogers Centre", "Miller Park", 430);
    graph.addEdge("Rogers Centre", "PNC Park", 225);
    graph.addEdge("Nationals Park", "PNC Park", 195);
    graph.addEdge("Yankee Stadium", "PNC Park", 315);
    graph.addEdge("Great America Ball Park", "PNC Park", 260);
    graph.addEdge("Progressive Field", "PNC Park", 115);
    graph.addEdge("Progressive Field", "Great America Ball Park", 225);
    graph.addEdge("Progressive Field", "Comerica Park", 90);
    graph.addEdge("Wrigley Field", "Comerica Park", 240);
    graph.addEdge("Wrigley Field", "US Cellular Field", 0);
    graph.addEdge("Wrigley Field", "Miller Park", 80);
    graph.addEdge("Wrigley Field", "Kauffman Stadium", 415);
    graph.addEdge("Coors Field", "Kauffman Stadium", 560);
    graph.addEdge("Globe Life Park in Arlington", "Kauffman Stadium", 460);
    graph.addEdge("Busch Stadium", "Kauffman Stadium", 235);
    graph.addEdge("Busch Stadium", "Great America Ball Park", 310);
    graph.addEdge("US Cellular Field", "Great America Ball Park", 250);
    graph.addEdge("Turner Field", "Great America Ball Park", 375);
    graph.addEdge("Oriole Park at Camden Yards", "Nationals Park", 0);
    graph.addEdge("Oriole Park at Camden Yards", "Citizens Bank Park", 90);
    graph.addEdge("Citi Field", "Citizens Bank Park", 80);
    graph.addEdge("Citi Field", "Yankee Stadium", 0);
    graph.addEdge("Turner Field", "Nationals Park", 560);
    graph.addEdge("Busch Stadium", "Target Field", 465);
    graph.addEdge("Coors Field", "Chase Field", 580);
    graph.addEdge("Target Field", "Miller Park", 300);






    // Add red blips and labels for each stadium
    for (const auto& [name, pos] : positions) {
        ClickableEllipseItem* dot = new ClickableEllipseItem(QString::fromStdString(name));
        dot->setRect(pos.x(), pos.y(), 8, 8);
        dot->setBrush(QBrush(Qt::red));
        dot->setPen(QPen(Qt::red));
        dot->setZValue(5);
        scene->addItem(dot);

        connect(dot, &ClickableEllipseItem::clicked, this, &MainWindow::showStadiumInfo);

        QGraphicsTextItem* label = scene->addText(QString::fromStdString(name));
        label->setDefaultTextColor(Qt::black);
        label->setZValue(6);
        label->setPos(pos.x() + 5, pos.y() - 15);
    }


    // Draw edges
    const auto& edges = graph.getAdjList();
    QSet<QString> drawnEdges;

    for (const auto& [from, connections] : edges) {
        for (const auto& [to, dist] : connections) {
            QString key = QString::fromStdString(from < to ? from + to : to + from);
            if (!drawnEdges.contains(key)) {
                if (positions.count(from) && positions.count(to)) {
                    QPointF p1 = positions.at(from);
                    QPointF p2 = positions.at(to);
                    drawConnection(p1, p2, QString::number(dist));
                    drawnEdges.insert(key);
                }
            }
        }
    }

}


void MainWindow::showStadiumInfo(const QString& stadiumName) {
    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");

    const auto& infoMap = graph.getStadiumInfo();
    const std::string nameStd = stadiumName.toStdString();

    if (infoMap.count(nameStd)) {
        const Stadium& s = infoMap.at(nameStd);

        QString details;
        details += "Stadium: " + QString::fromStdString(s.stadiumName) + "\n";
        details += "Team: " + QString::fromStdString(s.teamName) + "\n";
        details += "Address: " + QString::fromStdString(s.streetName + ", " + s.cityStateZip) + "\n";
        details += "Phone: " + QString::fromStdString(s.phoneNum) + "\n";
        details += "Opened: " + QString::fromStdString(s.launch) + "\n";
        details += "Capacity: " + QString::fromStdString(s.capacity);

        // Styled QMessageBox
        QMessageBox msgBox;
        msgBox.setWindowTitle("Stadium Info");
        msgBox.setText(details);
        msgBox.setStyleSheet(
            "QLabel { color: white; }"
            "QMessageBox { background-color: #2d2d2d; }"
            "QPushButton { color: white; background-color: #444444; }"
            );
        msgBox.exec();
    } else {
        QMessageBox::warning(this, "Error", "Stadium info not found.");
    }
}

