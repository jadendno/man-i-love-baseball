#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    ui->stackedWidget->setCurrentWidget(ui->loginPage);

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
    QPixmap map(":/images/us_Map.png");

    if(map.isNull()){
        QMessageBox::warning(this, "Error", "Map image not found");
        return;
    }


    //add map to the scene
    QPixmap scaledMap = map.scaled(ui->usMap->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* mapItem = scene->addPixmap(scaledMap);

    // Approximate coordinates on your map (adjust as needed)
    int x = 50;  // X position in pixels
    int y = 250;  // Y position in pixels

    QGraphicsEllipseItem* angelStadium = scene->addEllipse(x, y, 12, 12, QPen(Qt::red), QBrush(Qt::red));

    // Tooltip with full info
    angelStadium->setToolTip(
        "Angel Stadium of Anaheim\n"
        "Los Angeles Angels of Anaheim\n"
        "2000 E Gene Autry Way\n"
        "Anaheim, CA 92806\n"
        "(714) 940-2000\n"
        "Opened: April 19, 1966\n"
        "Capacity: 45,483"
        );


}
