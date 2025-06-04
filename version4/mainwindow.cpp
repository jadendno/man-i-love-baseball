#include "mainwindow.h"
#include "graphs.h"
#include "ui_mainwindow.h"
#include "clickableellipseitem.h"
#include "dijkprimkrus.h"
#include "graphutils.h"  // <-- the helper we just created
#include <QDialog>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <climits>





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
    populateComboBoxes();

    connect(ui->dijAll, &QPushButton::clicked, this, &MainWindow::on_dijAll_clicked);
    connect(ui->dijAmerican, &QPushButton::clicked, this, &MainWindow::on_dijAmerican_clicked);
    connect(ui->dijNational, &QPushButton::clicked, this, &MainWindow::on_dijNational_clicked);
    connect(ui->planTripButton, &QPushButton::clicked, this, &MainWindow::on_planTripButton_clicked);
    connect(ui->checkDistanceButton, &QPushButton::clicked, this, &MainWindow::on_checkDistanceButton_clicked);





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

void MainWindow::showScrollableMessage(const QString& title, const QString& content) {
    QDialog* dialog = new QDialog(this);
    dialog->setWindowTitle(title);
    dialog->resize(600, 500);  // Width and height

    dialog->setStyleSheet(
        "QDialog { background-color: white; }"
        "QTextEdit { background-color: white; color: black; font-family: Consolas; font-size: 12pt; }"
        "QPushButton { background-color: lightgray; color: black; padding: 5px; }"
        );

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    QTextEdit* textEdit = new QTextEdit(dialog);
    textEdit->setReadOnly(true);
    textEdit->setPlainText(content);
    layout->addWidget(textEdit);

    QPushButton* closeButton = new QPushButton("Close", dialog);
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton);

    dialog->setLayout(layout);
    dialog->exec();
}


void MainWindow::on_dijAll_clicked() {
    qDebug() << "Dijkstra button clicked!";

    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");
    addMajorLeagueEdges(graph);  // <-- ensures connectivity

    vector<vector<int>> matrix;
    vector<string> cities;
    buildMatrixFromGraph(graph, matrix, cities);

    qDebug() << "Cities loaded:" << cities.size();
    for (const auto& name : cities)
        qDebug() << QString::fromStdString(name);

    qDebug() << "Matrix built. Size:" << matrix.size() << "x" << matrix[0].size();
    if (!matrix.empty())
        qDebug() << "Matrix size:" << matrix.size() << "x" << matrix[0].size();

    QString output;
    std::string startCity = "Angels Stadium of Anaheim";
    int startIndex = -1;

    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i] == startCity) {
            startIndex = i;
            break;
        }
    }

    qDebug() << "Start index:" << startIndex;

    if (startIndex == -1) {
        QMessageBox::warning(this, "Error", "Starting stadium not found.");
        return;
    }

    const int INF = INT_MAX;
    int n = cities.size();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[startIndex] = 0;
    pq.push({0, startIndex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 0; v < n; ++v) {
            if (matrix[u][v] != INF) {
                int alt = dist[u] + matrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                    pq.push({alt, v});
                }
            }
        }
    }

    int totalMiles = 0;
    int stadiumsVisited = 0;

    for (int i = 0; i < n; ++i) {
        if (i == startIndex || dist[i] == INF) continue;

        totalMiles += dist[i];
        ++stadiumsVisited;

        output += QString::fromStdString(cities[i]) + " - " + QString::number(dist[i]) + " miles\n";

        std::vector<std::string> path;
        int cur = i;
        while (cur != -1) {
            if (cur < 0 || cur >= cities.size()) {
                qDebug() << "Invalid index during path reconstruction: " << cur;
                break;
            }
            path.push_back(cities[cur]);
            cur = prev[cur];
        }

        std::reverse(path.begin(), path.end());

        output += "Path: ";
        for (size_t j = 0; j < path.size(); ++j) {
            output += QString::fromStdString(path[j]);
            if (j + 1 < path.size()) output += " → ";
        }
        output += "\n\n";
    }

    for (int i = 0; i < dist.size(); ++i) {
        qDebug() << QString::fromStdString(cities[i]) << ":" << dist[i];
    }


    output = "Total Stadiums Visited: " + QString::number(stadiumsVisited) + "\n" +
             "Total Miles: " + QString::number(totalMiles) + "\n\n" + output;

    qDebug() << "About to display output";
    showScrollableMessage("Dijkstra All Results", output);
}

void MainWindow::on_dijAmerican_clicked() {
    qDebug() << "Dijkstra American League clicked";

    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");
    addMajorLeagueEdges(graph);

    vector<vector<int>> matrix;
    vector<string> cities;
    buildMatrixFromGraph(graph, matrix, cities);

    std::string startCity = "Angels Stadium of Anaheim";
    int startIndex = -1;
    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i] == startCity) {
            startIndex = i;
            break;
        }
    }

    if (startIndex == -1) {
        QMessageBox::warning(this, "Error", "Starting stadium not found.");
        return;
    }

    const int INF = INT_MAX;
    int n = cities.size();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[startIndex] = 0;
    pq.push({0, startIndex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 0; v < n; ++v) {
            if (matrix[u][v] != INF) {
                int alt = dist[u] + matrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                    pq.push({alt, v});
                }
            }
        }
    }

    int totalMiles = 0;
    int stadiumsVisited = 0;
    QString output;

    for (int i = 0; i < n; ++i) {
        if (i == startIndex || dist[i] == INF) continue;

        const Stadium& s = graph.getStadiumInfo().at(cities[i]);
        if (s.league != "American League") continue;  // 👈 Filter here

        ++stadiumsVisited;
        totalMiles += dist[i];

        output += QString::fromStdString(cities[i]) + " - " + QString::number(dist[i]) + " miles\n";

        std::vector<std::string> path;
        int cur = i;
        while (cur != -1) {
            if (cur < 0 || cur >= cities.size()) break;
            path.push_back(cities[cur]);
            cur = prev[cur];
        }
        std::reverse(path.begin(), path.end());

        output += "Path: ";
        for (size_t j = 0; j < path.size(); ++j) {
            output += QString::fromStdString(path[j]);
            if (j + 1 < path.size()) output += " → ";
        }
        output += "\n\n";
    }

    output = "Total American League Stadiums Visited: " + QString::number(stadiumsVisited) + "\n"
             + "Total Miles: " + QString::number(totalMiles) + "\n\n" + output;

    showScrollableMessage("Dijkstra - American League", output);
}

void MainWindow::on_dijNational_clicked() {
    qDebug() << "Dijkstra National League clicked";

    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");
    addMajorLeagueEdges(graph);

    vector<vector<int>> matrix;
    vector<string> cities;
    buildMatrixFromGraph(graph, matrix, cities);

    std::string startCity = "Angels Stadium of Anaheim";
    int startIndex = -1;
    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i] == startCity) {
            startIndex = i;
            break;
        }
    }

    if (startIndex == -1) {
        QMessageBox::warning(this, "Error", "Starting stadium not found.");
        return;
    }

    const int INF = INT_MAX;
    int n = cities.size();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[startIndex] = 0;
    pq.push({0, startIndex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 0; v < n; ++v) {
            if (matrix[u][v] != INF) {
                int alt = dist[u] + matrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                    pq.push({alt, v});
                }
            }
        }
    }

    int totalMiles = 0;
    int stadiumsVisited = 0;
    QString output;

    for (int i = 0; i < n; ++i) {
        if (i == startIndex || dist[i] == INF) continue;

        const Stadium& s = graph.getStadiumInfo().at(cities[i]);
        if (s.league != "National League") continue;

        ++stadiumsVisited;
        totalMiles += dist[i];

        output += QString::fromStdString(cities[i]) + " - " + QString::number(dist[i]) + " miles\n";

        std::vector<std::string> path;
        int cur = i;
        while (cur != -1) {
            if (cur < 0 || cur >= cities.size()) break;
            path.push_back(cities[cur]);
            cur = prev[cur];
        }
        std::reverse(path.begin(), path.end());

        output += "Path: ";
        for (size_t j = 0; j < path.size(); ++j) {
            output += QString::fromStdString(path[j]);
            if (j + 1 < path.size()) output += " → ";
        }
        output += "\n\n";
    }

    output = "Total National League Stadiums Visited: " + QString::number(stadiumsVisited) + "\n"
             + "Total Miles: " + QString::number(totalMiles) + "\n\n" + output;

    showScrollableMessage("Dijkstra - National League", output);
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
    // Load and display the map image
    QPixmap map(":/images/us_Map.png");
    if (map.isNull()) {
        QMessageBox::warning(this, "Error", "Map image not found");
        return;
    }
    QPixmap scaledMap = map.scaled(ui->usMap->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* mapItem = scene->addPixmap(scaledMap);

    // Load graph and stadiums
    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");

    const auto& positions = graph.getStadiumPositions();

    // Add edges (manual)
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
    graph.addEdge("Marlins Park", "Turner Field", 600);
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

    // Blips + labels
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

    // Draw edges without duplication
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

    // Build matrix and city list, then run algorithms
    vector<vector<int>> matrix;
    vector<string> cities;
    buildMatrixFromGraph(graph, matrix, cities);

    //dijkstra(matrix, cities, "AT&T Park");
    //prim(matrix, cities);
    //kruskal(matrix, cities);
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

void MainWindow::populateComboBoxes() {
    ui->startComboBox->clear();
    ui->endComboBox->clear();

    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");
    const std::unordered_map<std::string, Stadium>& allStadiums = graph.getStadiumInfo();


    for (const auto& entry : allStadiums) {
        const QString name = QString::fromStdString(entry.first);
        ui->startComboBox->addItem(name);
        ui->endComboBox->addItem(name);
    }
}

void MainWindow::on_planTripButton_clicked() {
    QString startName = ui->startComboBox->currentText();
    QString endName = ui->endComboBox->currentText();

    if (startName == endName) {
        QMessageBox::information(this, "Same Stadium", "Start and end stadium cannot be the same.");
        return;
    }

    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");
    addMajorLeagueEdges(graph);

    vector<vector<int>> matrix;
    vector<string> cities;
    buildMatrixFromGraph(graph, matrix, cities);

    std::string start = startName.toStdString();
    std::string end = endName.toStdString();

    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i] == start) startIndex = i;
        if (cities[i] == end) endIndex = i;
    }

    if (startIndex == -1 || endIndex == -1) {
        QMessageBox::warning(this, "Error", "Stadium not found in graph.");
        return;
    }

    const int INF = INT_MAX;
    int n = cities.size();
    vector<int> dist(n, INF);
    vector<int> prev(n, -1);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[startIndex] = 0;
    pq.push({0, startIndex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 0; v < n; ++v) {
            if (matrix[u][v] != INF) {
                int alt = dist[u] + matrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    prev[v] = u;
                    pq.push({alt, v});
                }
            }
        }
    }

    if (dist[endIndex] == INF) {
        QMessageBox::information(this, "No Path", "There is no path between the selected stadiums.");
        return;
    }

    std::vector<std::string> path;
    for (int cur = endIndex; cur != -1; cur = prev[cur]) {
        path.push_back(cities[cur]);
    }
    std::reverse(path.begin(), path.end());

    QString output;
    output += "Number of stadiums visited: " + QString::number(path.size()) + "\n";
    output += "Total distance traveled: " + QString::number(dist[endIndex]) + " miles\n";
    output += "Order of visit:\n";

    for (const auto& name : path) {
        output += QString::fromStdString(name) + "\n";
    }

    QMessageBox::information(this, "Planned Trip", output);

}

void MainWindow::on_checkDistanceButton_clicked() {
    QString startName = ui->startComboBox->currentText();
    QString endName = ui->endComboBox->currentText();

    if (startName == endName) {
        QMessageBox::information(this, "Same Stadium", "Start and end stadium cannot be the same.");
        return;
    }

    Graphs graph(Graphs::ALL);
    graph.loadStadiumsFromFile("stadiums.txt");
    addMajorLeagueEdges(graph);

    vector<vector<int>> matrix;
    vector<string> cities;
    buildMatrixFromGraph(graph, matrix, cities);

    std::string start = startName.toStdString();
    std::string end = endName.toStdString();

    int startIndex = -1, endIndex = -1;
    for (int i = 0; i < cities.size(); ++i) {
        if (cities[i] == start) startIndex = i;
        if (cities[i] == end) endIndex = i;
    }

    if (startIndex == -1 || endIndex == -1) {
        QMessageBox::warning(this, "Error", "Stadium not found in graph.");
        return;
    }

    const int INF = INT_MAX;
    int n = cities.size();
    vector<int> dist(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[startIndex] = 0;
    pq.push({0, startIndex});

    while (!pq.empty()) {
        int u = pq.top().second;
        int d = pq.top().first;
        pq.pop();

        if (d > dist[u]) continue;

        for (int v = 0; v < n; ++v) {
            if (matrix[u][v] != INF) {
                int alt = dist[u] + matrix[u][v];
                if (alt < dist[v]) {
                    dist[v] = alt;
                    pq.push({alt, v});
                }
            }
        }
    }

    if (dist[endIndex] == INF) {
        QMessageBox::information(this, "No Path", "There is no path between the selected stadiums.");
        return;
    }

    QString output;
    output += "Total distance between \"" + startName + "\" and \"" + endName + "\" is: " + QString::number(dist[endIndex]) + " miles";
    QMessageBox::information(this, "Distance Result", output);
}

