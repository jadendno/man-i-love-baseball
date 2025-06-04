#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

#include "ui/map.h"
#include "ui/clickableellipse.h"
#include "core/stadium.h"

#include <QGraphicsItem>
#include <QListWidgetItem>

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
    //ui->stackedWidget->setCurrentWidget(ui->users_menu);

    loadAllStadiums();
    m_graph = buildGraph(m_stadiums);
    buildNameLookup();
    m_startStadiumId = 1;

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

    connect(ui->pqBtn, &QPushButton::clicked,
            this, &MainWindow::on_pqBtn_clicked);

    connect(ui->clrBtn, &QPushButton::clicked,
            this, &MainWindow::on_clrBtn_clicked);
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
    QPixmap map(":/images/map.png");

    if(map.isNull()){
        QMessageBox::warning(this, "Error", "Map image not found");
        return;
    }


    //add map to the scene
    QPixmap scaledMap = map.scaled(ui->usMap->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem* mapItem = scene->addPixmap(scaledMap);

    std::vector<Stadium> stadiums = load_stadiums_from_resource(":/data/stadiums.json");

    auto graph = buildGraph(stadiums);

    // 3) Build a direct lookup: id → (lat, lon), or id → QPointF
    //    So we don’t recalculate lat/lon every time we draw.
    std::unordered_map<int, QPointF> idToPoint;
    idToPoint.reserve(stadiums.size());
    for (const Stadium& s : stadiums) {
        double lat = s.location.first;
        double lon = s.location.second;
        QPointF pt = MapPlotter::latLonToScene(mapItem, lat, lon);
        idToPoint[s.id] = pt;


        ClickableEllipse *ellipse =
            new ClickableEllipse(pt.x() - 4, pt.y() - 4, 7, 7);

        ellipse->setBrush(Qt::blue);
        ellipse->setPen(QPen(Qt::black, 1));

        // Store the stadium’s name as user data (index 0)
        QString qname = QString::fromStdString(s.name);
        ellipse->setData(0, QVariant(qname));
        ellipse->setFlag(QGraphicsItem::ItemIsSelectable, true);


        scene->addItem(ellipse);


        // Capture “info.name” inside a lambda, or read ellipse->data(0) in the slot:
        connect(ellipse, &ClickableEllipse::clicked, this, [this, ellipse]() {
            QString stadiumName = ellipse->data(0).toString();
            onStadiumEllipseClicked(stadiumName);
        });

        /*QGraphicsEllipseItem* item = scene->addEllipse(
            pt.x() - 4, pt.y() - 4, 7, 7,
            QPen(QColorConstants::Svg::orange),
            QBrush(QColorConstants::Svg::orange)
            );

        item->setFlag(QGraphicsItem::ItemIsSelectable, true);
        item->setData(0, s.id);   // store stadium ID under key 0*/

        // Format tooltip text using QString for easier number/string concat
        QString tooltip = QString::fromStdString(
            s.name + "\n" +
            s.team + "\n" +
            s.address + "\n" +
            s.phone + "\n" +
            "Opened: " + s.opened + "\n" +
            "Capacity: " + s.capacity
            );
        ellipse->setToolTip(tooltip);
    }

    // 4) Draw each undirected edge exactly once:
    QPen edgePen(QColorConstants::Svg::black);
    edgePen.setWidthF(1.0);

    for (const Stadium& u : stadiums) {
        int u_id = u.id;
        int u_idx = u_id - 1;

        for (const Edge& e : graph[u_idx]) {
            if (e.weight == 0)
                continue;

            int v_id = e.to;
            // Only draw if u_id < v_id (prevents drawing each line twice).
            if (u_id < v_id) {
                // lookup points:
                QPointF pt_u = idToPoint[u_id];
                QPointF pt_v = idToPoint[v_id];

                // Draw a simple line (connection) on the scene:
                scene->addLine(
                    pt_u.x(), pt_u.y(),
                    pt_v.x(), pt_v.y(),
                    edgePen
                    );
            }
        }
    }
}


void MainWindow::onStadiumEllipseClicked(const QString &stadiumName)
{
    // If the queue is non-empty and the last queued stadium == stadiumName,
    // we interpret this as a “deselect” and remove that entry.
    if (!m_stadiumQueue.isEmpty() && m_stadiumQueue.back() == stadiumName) {
        // 1) Remove from our internal queue (removeLast() inherited from QList)
        m_stadiumQueue.removeLast();

        // 2) Remove the last row from the QListWidget
        int rowCount = ui->listWidget->count();
        if (rowCount > 0) {
            // takeItem(rowCount - 1) removes the QListWidgetItem* from the view,
            // but does not delete it, so we delete it manually
            delete ui->listWidget->takeItem(rowCount - 1);
        }

        // 3) If the queue is now empty, clear our “start” stadium ID (or reset as needed)
        if (m_stadiumQueue.isEmpty()) {
            m_startStadiumId = 0;
            // (you can choose to set this back to some default/invalid value)
        }
        return;
    }

    // Otherwise, this is a fresh click → enqueue as normal:

    // 1) Add to our internal queue
    m_stadiumQueue.enqueue(stadiumName);

    // 2) If this is the very first stadium in the queue, set it as the “start” ID
    int position = m_stadiumQueue.size(); // 1-based
    if (position == 1) {
        m_startStadiumId = m_nameToId[stadiumName];
    }

    // 3) Build the “N. StadiumName” string and append it to QListWidget
    QString displayText = QString("%1. %2").arg(position).arg(stadiumName);
    ui->listWidget->addItem(new QListWidgetItem(displayText));
}


void MainWindow::loadAllStadiums()
{
    // Example: load from a JSON resource (you already have a helper for this)
    // Suppose you embedded /resources/data/stadiums.json in Qt’s qrc.
    try {
        m_stadiums = load_stadiums_from_resource(":/data/stadiums.json");
    } catch (std::exception &ex) {
        qWarning() << "Failed to load stadiums:" << ex.what();
    }
}

void MainWindow::buildNameLookup()
{
    m_nameToId.clear();
    for (const Stadium &s : m_stadiums) {
        // Convert std::string → QString
        QString qname = QString::fromStdString(s.name);
        m_nameToId[qname] = s.id;   // note: IDs are 1‐based
    }
}


// A helper struct for Dijkstra’s priority‐queue
struct DijkstraNode {
    int idx;
    double dist;
    bool operator<(const DijkstraNode &other) const {
        // we want min‐heap, so invert here
        return dist > other.dist;
    }
};

std::pair<std::vector<double>, std::vector<int>> MainWindow::dijkstra(int srcIndex)
{
    int N = static_cast<int>(m_graph.size());
    qDebug() << "→ entering dijkstra with srcIndex =" << srcIndex << " N =" << N;
    const double INF = std::numeric_limits<double>::infinity();

    std::vector<double> dist(N, INF);
    std::vector<int> prev(N, -1);
    std::priority_queue<DijkstraNode> pq;

    if (srcIndex < 0 || srcIndex >= N) {
        qWarning() << "dijkstra: srcIndex out of range!" << srcIndex;
        return { dist, prev };
    }

    dist[srcIndex] = 0.0;
    pq.push(DijkstraNode{srcIndex, 0.0});

    while (!pq.empty()) {
        DijkstraNode top = pq.top();
        pq.pop();
        int u = top.idx;
        double d = top.dist;
        qDebug() << "   visiting u =" << u << " with tentative dist =" << d;
        if (d > dist[u]) {
            qDebug() << "   skipping stale entry for" << u;
            continue;
        }

        // BEFORE indexing m_graph[u], check range
        if (u < 0 || u >= N) {
            qWarning() << "   ERROR: u out of range:" << u;
            break;
        }
        for (const Edge &e : m_graph[u]) {
            int v = e.to - 1;
            if (v < 0 || v >= N) {
                qWarning() << "   ERROR: neighbor index v out of range:" << v
                           << "(edge.to =" << e.to << ")";
                continue;
            }
            double w = e.weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push(DijkstraNode{v, dist[v]});
            }
        }
    }

    return { dist, prev };
}


QString MainWindow::computeVisitOrderAndDistance(const std::vector<int> &mustVisitIds)
{
    // Copy the must-visit IDs so we can remove them as we go:
    std::vector<int> toVisit = mustVisitIds;
    std::vector<int> visitOrder;                 // will hold the chosen must-visit IDs
    visitOrder.reserve(toVisit.size());

    int N = static_cast<int>(m_stadiums.size());
    double totalDistance = 0.0;

    // Track which stadium-indices (0-based) have been picked:
    std::vector<bool> isPicked(N, false);

    // Start from the fixed “home” ID:
    int currId  = m_startStadiumId;
    int currIdx = currId - 1;

    // Build the output string:
    QString output;
    QTextStream ts(&output);

    ts << "Number of stadiums to visit: " << mustVisitIds.size() << "\n\n";
    ts << "Detailed visit path (including intermediate hops):\n\n";

    // Print the “Start at: HomeStadium” line once:
    {
        QString homeName = QString::fromStdString(m_stadiums[currIdx].name);
        ts << "Start at: " << homeName << "\n\n";
    }

    int legNumber = 1;  // we’ll increment this for each actual travel

    // While there remain unvisited must-visit stadiums:
    while (!toVisit.empty()) {
        // 1) Run Dijkstra from currIdx → get distances & predecessors
        auto [dist, prev] = dijkstra(currIdx);

        // 2) Pick the closest ID among toVisit:
        double bestDist = std::numeric_limits<double>::infinity();
        int bestNextId  = -1;
        int bestNextIdx = -1;

        for (int id : toVisit) {
            int idx = id - 1;
            if (dist[idx] < bestDist) {
                bestDist   = dist[idx];
                bestNextId = id;
                bestNextIdx= idx;
            }
        }

        // If none is reachable, stop early:
        if (bestNextId < 0) {
            ts << "Warning: some stadiums are unreachable from here.\n\n";
            break;
        }

        // 3) Reconstruct the full chain of 0-based indices from currIdx → bestNextIdx:
        std::vector<int> segment;
        for (int v = bestNextIdx; v != currIdx; v = prev[v]) {
            // (In theory prev[v] should never be -1 here, since bestNextId was reachable)
            if (v < 0 || v >= N) break;
            segment.push_back(v);
        }
        segment.push_back(currIdx);
        std::reverse(segment.begin(), segment.end());
        // Now: segment[0] == currIdx, segment.back() == bestNextIdx

        // 4) Print just the “Leg X: From [curr] to [dest] (distance = bestDist)” header:
        ts << "Leg " << legNumber << ": From ["
           << QString::fromStdString(m_stadiums[currIdx].name) << "] to ["
           << QString::fromStdString(m_stadiums[bestNextIdx].name) << "] "
           << "(distance = " << bestDist << ")\n";

        // 5) Print every node in 'segment', **skipping** segment[0].
        //    That way, we list only the true intermediate hops and final destination.
        for (int i = 1; i < static_cast<int>(segment.size()); ++i) {
            int idx0   = segment[i];
            QString name0 = QString::fromStdString(m_stadiums[idx0].name);
            ts << "   → " << name0 << "\n";
        }
        ts << "\n";

        // 6) Accumulate the distance:
        totalDistance += bestDist;

        // 7) Mark bestNextIdx as visited and record it:
        isPicked[bestNextIdx] = true;
        visitOrder.push_back(bestNextId);

        // 8) Remove bestNextId from toVisit:
        toVisit.erase(
            std::remove(toVisit.begin(), toVisit.end(), bestNextId),
            toVisit.end()
            );

        // 9) Advance current and leg counter:
        currId   = bestNextId;
        currIdx  = bestNextIdx;
        ++legNumber;
    }

    // 10) Finally, print a summary block:
    ts << "======== Summary ========\n";
    ts << "Number of stadiums visited: " << visitOrder.size() << "\n\n";

    ts << "Visit order (you must visit the stadiums in the given order):\n";
    for (int i = 0; i < static_cast<int>(visitOrder.size()); ++i) {
        int id = visitOrder[i];
        QString name = QString::fromStdString(m_stadiums[id - 1].name);
        ts << (i + 1) << ". " << name << "\n";
    }
    ts << "\nTotal distance traveled: " << totalDistance << " units\n";

    return output;
}

void MainWindow::on_pqBtn_clicked()
{
    // 1) If nothing’s queued, bail out:
    if (ui->listWidget->count() == 0) {
        ui->textBrowser->append("No stadiums in the queue.\n");
        return;
    }

    // 2) Build a vector<int> of stadium IDs from the queue
    std::vector<int> mustVisitIds;
    mustVisitIds.reserve(ui->listWidget->count() - 1);

    for (int row = 1; row < ui->listWidget->count(); ++row) {
        QListWidgetItem *item = ui->listWidget->item(row);
        QString qname = item->text();
        // If you prefixed “1. “ in your display, strip up to the first space:
        //    e.g. “3. Yankee Stadium” → “Yankee Stadium”.
        //    Otherwise, if your displayText was exactly the stadiumName, skip this.
        if (qname.contains(". ")) {
            // split on “. ” and take the second half
            qname = qname.section(". ", 1, 1);
        }

        // Lookup ID
        auto it = m_nameToId.find(qname);
        if (it == m_nameToId.end()) {
            qWarning() << "Unknown stadium name in queue:" << qname;
            continue; // skip invalid
        }
        mustVisitIds.push_back(it->second);
    }

    // If for some reason the queue became empty after skipping unknown names:
    if (mustVisitIds.empty()) {
        ui->textBrowser->append("No valid stadium IDs found in the queue.\n");
        return;
    }

    // 3) Now call a helper that computes the “best path” visiting them all,
    //    starting from m_startStadiumId.
    QString result = computeVisitOrderAndDistance(mustVisitIds);
    ui->textBrowser->clear();
    ui->textBrowser->append(result);
}


void MainWindow::on_clrBtn_clicked()
{
    // Example action: just dequeue everything and clear the list
    while (!m_stadiumQueue.isEmpty()) {
        QString s = m_stadiumQueue.dequeue();
        qDebug() << "Dequeued stadium:" << s;
    }
    ui->listWidget->clear();
}



