#include "mainwindow.h"
#include "ui_mainwindow.h"








// class QGraphicsEllipseItem  : public QObject, public QGraphicsEllipseItem  {
//     Q_OBJECT
// public:
//     QGraphicsEllipseItem (qreal x, qreal y, qreal w, qreal h, QPen p, QBrush b)
//         : QGraphicsEllipseItem (x, y, w, h) {
//         setBrush(b);
//         setPen(p);
//     }

//     QGraphicsEllipseItem (qreal x, qreal y, qreal w, qreal h, QPen p, QBrush b, QGraphicsScene* s)
//         : QGraphicsEllipseItem (x, y, w, h) {
//         setBrush(b);
//         setPen(p);
//         s->addItem(this);
//     }

// signals:
//     void clicked();

// protected:
//     void mousePressEvent(QGraphicsSceneMouseEvent *event) override {
//         emit clicked();
//     }
// };

/**********************************************************************
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow
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
    QPixmap map(":/images/us_Map.png");

    if(map.isNull()){
        QMessageBox::warning(this, "Error", "Map image not found");
        return;
    }


    //add map to the scene
    QPixmap scaledMap = map.scaled(ui->usMap->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* mapItem = scene->addPixmap(scaledMap);


    //x lower value = left
    //x higher value = right
    //y lower value = up
    //y higher value = down

    //////////////////////////////////////////
    ///////////  WEST COAST /////////////////
    ////////////////////////////////////////




    //AMERICAN LEAGUES

    QGraphicsEllipseItem * angelStadium = scene->addEllipse(65, 275, 7, 7, QPen(QColorConstants::Svg::orange), QBrush(QColorConstants::Svg::orange));

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

    QGraphicsEllipseItem * oaklandColiseum = scene->addEllipse(27, 200, 7, 7, QPen(QColorConstants::Svg::orange), QBrush(QColorConstants::Svg::orange));
    oaklandColiseum->setToolTip(
        "O.Co Coliseum\n"
        "Oakland Athletics\n"
        "7000 Coliseum Way\n"
        "Oakland, CA 94621\n"
        "(510) 569-2121\n"
        "Opened: September 18, 1966\n"
        "Capacity: 37,090 (April - August) & 55,945 (Sept-Jan)\n"
        );

    QGraphicsEllipseItem * safeCoField = scene->addEllipse(74, 30, 7, 7, QPen(QColorConstants::Svg::orange), QBrush(QColorConstants::Svg::orange));
    safeCoField->setToolTip(
        "SafeCo Field\n"
        "Seattle Mariners\n"
        "1516 First Avenue South\n"
        "Seattle, WA 98134\n"
        "(206) 346-4000\n"
        "Opened - July 15, 1999\n"
        "Capacity - 47,476\n"
        );


    QGraphicsEllipseItem * rangersBallpark = scene->addEllipse(340, 340, 7, 7, QPen(QColorConstants::Svg::orange), QBrush(QColorConstants::Svg::orange));
    rangersBallpark->setToolTip(
        "Globe Life Park in Arlington\n"
        "Texas Rangers\n"
        "1000 Ballpark Way,\n"
        "Arlington, TX 76011\n"
        "(817) 273-5222\n"
        "Opened – April 1, 1994\n"
        "Capacity – 48,114"
        );

    QGraphicsEllipseItem * minuteMaid = scene->addEllipse(380, 380, 7, 7, QPen(QColorConstants::Svg::orange), QBrush(QColorConstants::Svg::orange));
    minuteMaid->setToolTip(
        "Minute Maid Park\n"
        "Houston Astros\n"
        "501 Crawford St,\n"
        "Houston, TX 77002\n"
        "(713) 259-8000\n"
        "Opened - March 30, 2000\n"
        "Capacity - 42,060"

        );


    //National league

    QGraphicsEllipseItem * petcoPark = scene->addEllipse(73, 297, 7, 7, QPen(QColorConstants::Svg::green), QBrush(QColorConstants::Svg::green));
    petcoPark->setToolTip(
        "Petco Park\n"
        "San Diego Padres\n"
        "19 Tony Gwynn Drive\n"
        "San Diego, CA 92101\n"
        "(619) 795-5000\n"
        "Opened - April 8, 2004\n"
        "Capacity - 42,524"

        );

    QGraphicsEllipseItem * dodger = scene->addEllipse(58, 269, 7, 7, QPen(QColorConstants::Svg::green), QBrush(QColorConstants::Svg::green));
    dodger->setToolTip(
        "Dodger Stadium\n"
        "Los Angeles Dodgers\n"
        "1000 Elysian Park Avenue\n"
        "Los Angeles, CA 90090\n"
        "(323) 224-1507\n"
        "Opened - April 10, 1962\n"
        "Capacity - 56,000"
        );

    QGraphicsEllipseItem * att = scene->addEllipse(18, 193, 7, 7, QPen(QColorConstants::Svg::green), QBrush(QColorConstants::Svg::green));
    att->setToolTip(
        "AT&T Park\n"
        "San Francisco Giants\n"
        "24 Willie Mays Plaza\n"
        "San Francisco, CA 94107\n"
        "(415) 972-2000\n"
        "Opened - April 11, 2000\n"
        "Capacity - 41,915"

        );

    QGraphicsEllipseItem * chaseFields = scene->addEllipse(148, 303, 7, 7, QPen(QColorConstants::Svg::green), QBrush(QColorConstants::Svg::green));
    chaseFields->setToolTip(
        "Chase Field\n"
        "Arizona Diamondbacks\n"
        "401 East Jefferson Street\n"
        "Phoenix, AZ 85004\n"
        "(602) 462-6500\n"
        "Opened - March 31, 1998\n"
        "Capacity - 48,633"
        );

    QGraphicsEllipseItem * coorsField = scene->addEllipse(250, 208, 7, 7, QPen(QColorConstants::Svg::green), QBrush(QColorConstants::Svg::green));
    coorsField->setToolTip(
        "Coors Field\n"
        "Colorado Rockies\n"
        "2001 Blake St\n"
        "Denver, CO 80205\n"
        "(303)292-0200\n"
        "Opened - April 26, 1995\n"
        "Capacity - 50,480"
        );

    //////////////////////////////////////////
    ///////////  CENTRAL COAST //////////////
    ////////////////////////////////////////


    //American League

    QGraphicsEllipseItem * kauffman = scene->addEllipse(390, 230, 7, 7, QPen(QColorConstants::DarkYellow), QBrush(QColorConstants::DarkYellow));
    kauffman->setToolTip(
        "Kauffman Stadium\n"
        "Kansas City Royals\n"
        "1 Royal Way\n"
        "Kansas City, MO 64129\n"
        "(816) 921-8000\n"
        "Opened – April 10, 1973\n"
        "Capacity – 37,903\n"
        );

    QGraphicsEllipseItem * target = scene->addEllipse(400, 121, 7, 7, QPen(QColorConstants::DarkYellow), QBrush(QColorConstants::DarkYellow));
    target->setToolTip(
        "Target Field\n"
        "Minnesota Twins\n"
        "353 N 5th St\n"
        "Minneapolis, MN 55403\n"
        "(800) 338-9467\n"
        "Opened - April 12, 2010\n"
        "Capacity - 39,021"
        );

    QGraphicsEllipseItem * usCellular = scene->addEllipse(470, 175, 7, 7, QPen(QColorConstants::DarkYellow), QBrush(QColorConstants::DarkYellow));
    usCellular->setToolTip(
        "US Cellular Field\n"
        "Chicago White Sox\n"
        "333 West 35th Street\n"
        "Chicago, IL 60616\n"
        "(312) 674-1000\n"
        "Opened - April 18, 1991\n"
        "Capacity - 40,615"

        );

    QGraphicsEllipseItem * comerica = scene->addEllipse(525, 158, 7, 7, QPen(QColorConstants::DarkYellow), QBrush(QColorConstants::DarkYellow));
    comerica->setToolTip(
        "Comerica Park\n"
        "Detroit Tigers\n"
        "2100 Woodward Ave,\n"
        "Detroit, MI 48201\n"
        "(313) 962-4000\n"
        "Opened – April 11, 2000\n"
        "Capacity – 41,681\n"
        );


    QGraphicsEllipseItem * progressive = scene->addEllipse(547, 175, 7, 7, QPen(QColorConstants::DarkYellow), QBrush(QColorConstants::DarkYellow));
    progressive->setToolTip(
        "Progressive Field\n"
        "Cleveland Indians\n"
        "2401 Ontario Street\n"
        "Cleveland, OH 44115\n"
        "(216) 420-4487\n"
        "Opened - April 2, 1994\n"
        "Capacity - 42,404"

        );

    //national leagues

    QGraphicsEllipseItem * busch = scene->addEllipse(440, 231, 7, 7, QPen(QColorConstants::Svg::purple), QBrush(QColorConstants::Svg::purple));
    busch->setToolTip(
        "Busch Stadium\n"
        "St. Louis Cardinals\n"
        "700 Clark Street\n"
        "St. Louis, MO 63102\n"
        "(314) 345-9600\n"
        "Opened - April 10, 2006\n"
        "Capacity - 46,861\n"
        );

    QGraphicsEllipseItem * wrigley = scene->addEllipse(465, 170, 7, 7, QPen(QColorConstants::Svg::purple), QBrush(QColorConstants::Svg::purple));
    wrigley->setToolTip(
        "Wrigley Field\n"
        "Chicago Cubs\n"
        "1060 West Addison Street\n"
        "Chicago, IL 60613\n"
        "(773) 404-2827\n"
        "Opened - April 23, 1914\n"
        "Capacity - 42,374"

        );

    QGraphicsEllipseItem * miller = scene->addEllipse(465, 150, 7, 7, QPen(QColorConstants::Svg::purple), QBrush(QColorConstants::Svg::purple));
    miller->setToolTip(
        "Miller Park\n"
        "Milwaukee Brewers\n"
        "1 Brewers Way\n"
        "Milwaukee, WI 53214\n"
        "(414) 902-4400\n"
        "Opened - April 6, 2001\n"
        "Capacity - 41,900"
        );

    QGraphicsEllipseItem * greatAmerican = scene->addEllipse(518, 218, 7, 7, QPen(QColorConstants::Svg::purple), QBrush(QColorConstants::Svg::purple));
    greatAmerican->setToolTip(
        "Great America Ball Park\n"
        "Cincinnati Reds\n"
        "100 Joe Nuxhall Way\n"
        "Cincinnati, OH 45202\n"
        "(513) 381-7337\n"
        "Opened - March 31, 2003\n"
        "Capacity - 42,319"
        );

    QGraphicsEllipseItem * pnc = scene->addEllipse(575, 185, 7, 7, QPen(QColorConstants::Svg::purple), QBrush(QColorConstants::Svg::purple));
    pnc->setToolTip(
        "PNC Park\n"
        "Pittsburgh Pirates\n"
        "115 Federal St\n"
        "Pittsburgh, PA 15212\n"
        "(412) 321-2827\n"
        "Opened - March 31, 2001\n"
        "Capacity - 38,362"
        );

    //////////////////////////////////////////
    ///////////  EAST COAST //////////////
    ////////////////////////////////////////


    //American League

    QGraphicsEllipseItem * tropicana = scene->addEllipse(575, 398, 7, 7, QPen(QColorConstants::Svg::red), QBrush(QColorConstants::Svg::red));
    tropicana->setToolTip(
        "Tropicana Field\n"
        "Tampa Bay Rays\n"
        "1 Tropicana Dr\n"
        "St. Petersburg, FL 33705\n"
        "(727) 825-3137\n"
        "Opened - March 3, 1990\n"
        "Capacity - 31,042 (Regular Season) 42,735 (Postseason)"

        );

    QGraphicsEllipseItem * rogers = scene->addEllipse(575, 120, 7, 7, QPen(QColorConstants::Svg::red), QBrush(QColorConstants::Svg::red));
    rogers->setToolTip(
        "Rogers Centre\n"
        "Toronto Blue Jays\n"
        "1 Blue Jays Way\n"
        "Toronto, Ontario, Canada M5V1J3\n"
        "+1 416-341-1000\n"
        "Opened - June 3, 1989\n"
        "Capacity - 49,282"
        );

    QGraphicsEllipseItem * camden = scene->addEllipse(620, 195, 7, 7, QPen(QColorConstants::Svg::red), QBrush(QColorConstants::Svg::red));
    camden->setToolTip(
        "Oriole Park at Camden Yards\n"
        "Baltimore Orioles\n"
        "333 West Camden Street\n"
        "Baltimore, MD 21201\n"
        "(410) 685-9800\n"
        "Opened - April 6, 1992\n"
        "Capacity - 48,187"

        );

    QGraphicsEllipseItem * yankee = scene->addEllipse(650, 160, 7, 7, QPen(QColorConstants::Svg::red), QBrush(QColorConstants::Svg::red));
    yankee->setToolTip(
        "Yankee Stadium\n"
        "New York Yankees\n"
        "1 E 161st St\n"
        "South Bronx, NY 10451\n"
        "(718) 293-4300\n"
        "Opened - April 16, 2009\n"
        "Capacity - 50,291"

        );

    QGraphicsEllipseItem * fenway = scene->addEllipse(680, 130, 7, 7, QPen(QColorConstants::Svg::red), QBrush(QColorConstants::Svg::red));
    fenway->setToolTip(
        "Fenway Park\n"
        "Boston Red Sox\n"
        "4 Yawkey Way, \n"
        "Boston, MA 02215\n"
        "(877) 733-7699 \n"
        "Opened – April 20, 1912\n"
        "Capacity – 37,499 (night) & 37,071 (day)"

        );

    //national leagues

    QGraphicsEllipseItem * marlins = scene->addEllipse(604, 435, 7, 7, QPen(QColorConstants::Svg::blue), QBrush(QColorConstants::Svg::blue));
    marlins->setToolTip(
        "Marlins Park\n"
        "Miami Marlins\n"
        "501 Marlins Way\n"
        "Miami, FL 33125\n"
        "(305)480-1300\n"
        "Opened - April 4, 2012\n"
        "Capacity - 37,442"

        );

    QGraphicsEllipseItem * turner = scene->addEllipse(528, 305, 7, 7, QPen(QColorConstants::Svg::blue), QBrush(QColorConstants::Svg::blue));
    turner->setToolTip(
        "Turner Field\n"
        "Atlanta Braves\n"
        "755 Hank Aaron Drive\n"
        "Atlanta, GA 30315\n"
        "(404) 522-7630\n"
        "Opened - March 29, 1997\n"
        "Capacity - 49,586"


        );

    QGraphicsEllipseItem * nationals = scene->addEllipse(615, 200, 7, 7, QPen(QColorConstants::Svg::blue), QBrush(QColorConstants::Svg::blue));
    nationals->setToolTip(
        "Nationals Park\n"
        "Washington Nationals\n"
        "1500 S Capitol St SE\n"
        "Washington, DC 20003\n"
        "(202) 675-6287\n"
        "Opened - March 30, 2008\n"
        "Capacity - 41,418"
        );

    QGraphicsEllipseItem * citizens = scene->addEllipse(635, 178, 7, 7, QPen(QColorConstants::Svg::blue), QBrush(QColorConstants::Svg::blue));
    citizens->setToolTip(
        "Citizens Bank Park\n"
        "Philadelphia Phillies\n"
        "1 Citizens Bank Way\n"
        "Philadelphia, PA 19148\n"
        "(215) 463-1000\n"
        "Opened - April 3, 2004\n"
        "Capacity - 43,651"

        );

    QGraphicsEllipseItem * citi = scene->addEllipse(655, 165, 7, 7, QPen(QColorConstants::Svg::blue), QBrush(QColorConstants::Svg::blue));
    citi->setToolTip(
        "Citi Field\n"
        "New York Mets\n"
        "126th St. & Roosevelt Ave.\n"
        "Queens, NY 11368\n"
        "(718) 507-6387\n"
        "Opened - April 13, 2009\n"
        "Capacity - 41,922"

        );

    //coordinates: testing att && safeco

    QPointF attCenterP(18+3.5, 193 + 3.5);
    QPointF safecoCenterP(74+3.5, 30+3.5);
    drawConnection(attCenterP, safecoCenterP, "680");

    QPointF oaklandP(27+3.5, 200+3.5);
    QPointF dodgersP(58+3.5, 269+3.5);
    drawConnection(oaklandP, dodgersP, "650");

    QPointF angelsP(65+3.5, 275+3.5);
    QPointF petcoP(73+3.5, 297+3.5);
    drawConnection(petcoP, angelsP, "110");

    QPointF coorsP(250+3.5, 208+3.5);
    QPointF chaseP(148+3.5, 303+3.5);
    QPointF targetP(400+3.5, 121+3.5);
    QPointF rogersP(575+3.5, 120+3.5);
    QPointF rangersP(340+3.5, 340+3.5);
    QPointF minuteMaidP(380+3.5, 380+3.5);
    QPointF kauffmanP(390+3.5, 230+3.5);
    QPointF buschP(440+3.5, 231+3.5);
    QPointF wrigleyP(465+3.5, 170+3.5);
    QPointF millerP(465+3.5, 150+3.5);
    QPointF greatAmericanP(518+3.5, 218+3.5);
    QPointF progressiveP(547+3.5, 175+3.5);
    QPointF comericaP(525+3.5, 158+3.5);
    QPointF turnerP(528+3.5, 305+3.5);
    QPointF marlinsP(604+3.5, 435+3.5);
    QPointF trapicanaP(575+3.5, 398+3.5);
    QPointF nationalsP(615+3.5, 200+3.5);
    QPointF pncP(575+3.5, 185+3.5);
    QPointF yankeeP(650+3.5, 160+3.5);
    QPointF citizensP(635+3.5, 178+3.5);
    QPointF citiP(655+3.5, 165+3.5);
    QPointF fenwayP(680+3.5, 130+3.5);


    drawConnection(angelsP, coorsP, "830");
    drawConnection(petcoP, chaseP, "300");
    drawConnection(oaklandP, chaseP, "650");
    drawConnection(angelsP, targetP, "1500");
    drawConnection(safecoCenterP, targetP, "1390");
    drawConnection(safecoCenterP, rogersP, "2070");
    drawConnection(chaseP, coorsP, "580");
    drawConnection(chaseP, rangersP, "870");
    drawConnection(chaseP, minuteMaidP, "1115");
    drawConnection(rangersP, minuteMaidP, "230");
    drawConnection(coorsP, rangersP, "650");
    drawConnection(coorsP, kauffmanP, "560");
    drawConnection(rangersP, kauffmanP, "460");
    drawConnection(kauffmanP, buschP, "235");
    drawConnection(kauffmanP, wrigleyP, "415");
    drawConnection(buschP, minuteMaidP, "680");
    drawConnection(buschP, targetP, "465");
    drawConnection(millerP, targetP, "300");
    drawConnection(millerP, wrigleyP, "80");
    drawConnection(wrigleyP, greatAmericanP, "250");
    drawConnection(millerP, rogersP, "430");
    drawConnection(comericaP, rogersP,"210");
    drawConnection(comericaP, wrigleyP,"240");
    drawConnection(comericaP, progressiveP,"90");
    drawConnection(greatAmericanP, progressiveP,"225");
    drawConnection(turnerP, greatAmericanP,"375");
    drawConnection(turnerP, rangersP,"740");
    drawConnection(turnerP, marlinsP,"600");
    drawConnection(greatAmericanP, trapicanaP,"790");
    drawConnection(minuteMaidP, trapicanaP,"790");
    drawConnection(minuteMaidP, marlinsP,"965");
    drawConnection(trapicanaP, marlinsP,"210");
    drawConnection(nationalsP, turnerP,"560");
    drawConnection(nationalsP, marlinsP,"930");
    drawConnection(pncP, progressiveP,"115");
    drawConnection(pncP, nationalsP,"195");
    drawConnection(pncP, rogersP,"225");
    drawConnection(nationalsP, citizensP,"90");
    drawConnection(citiP, citizensP,"80");
    drawConnection(pncP, yankeeP,"315");
    drawConnection(fenwayP, rogersP,"430");
    drawConnection(fenwayP, marlinsP,"1255");


    // connect(ellipse, &QGraphicsEllipseItem ::clicked, []() {
    //     qDebug() << "Ellipse was clicked!";
    // });


}
