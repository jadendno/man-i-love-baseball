#include "ui/mainwindow.h"
#include "ui_mainwindow.h"

#include "ui/map.h"
#include "ui/clickableellipse.h"
#include "core/stadium.h"
#include "utils/stadiumhelper.h"

#include <QGraphicsItem>
#include <QListWidgetItem>
#include <QInputDialog>

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

    m_merchList = loadMerchFromJson(":/data/merch.json");
    if (m_merchList.empty()) {
        QMessageBox::warning(this,
                             tr("Merch Load Error"),
                             tr("No merchandise was loaded from merch.json."));
    }

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

    connect(ui->viewStadiums, &QPushButton::clicked,
            this, &MainWindow::showViewStadiums);

    connect(ui->viewData, &QPushButton::clicked,
            this, &MainWindow::showDataDialog);

    connect(ui->natBtn, &QPushButton::clicked,
            this, &MainWindow::fillNL);

    connect(ui->amBtn, &QPushButton::clicked,
            this, &MainWindow::fillAL);

    connect(ui->addMerch,
            &QPushButton::clicked,
            this,
            &MainWindow::addMerch);
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
    if (map.isNull()) {
        QMessageBox::warning(this, "Error", "Map image not found");
        return;
    }

    QPixmap scaledMap = map.scaled(
        ui->usMap->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );
    QGraphicsPixmapItem* mapItem = scene->addPixmap(scaledMap);

    // Clear any previous points (just in case):
    m_idToPoint.clear();

    // 1) Draw each stadium ellipse, store its scene point:
    for (const Stadium& s : m_stadiums) {
        double lat = s.location.first;
        double lon = s.location.second;
        QPointF pt = MapPlotter::latLonToScene(mapItem, lat, lon);
        m_idToPoint[s.id] = pt;  // <--- store it here

        ClickableEllipse* ellipse =
            new ClickableEllipse(pt.x() - 4, pt.y() - 4, 7, 7);
        ellipse->setBrush(Qt::blue);
        ellipse->setPen(QPen(Qt::black, 1));
        ellipse->setFlag(QGraphicsItem::ItemIsSelectable, true);

        QString qname = QString::fromStdString(s.name);
        ellipse->setData(0, QVariant(qname));

        std::string turf = "turf";
        if (s.turf)
                turf = "grass";
        QString tooltip =
            QString::fromStdString(
                s.name + "\n" +
                s.team + "\n" +
                s.address + "\n" +
                s.phone + "\n" +
                "Opened: " + s.opened + "\n" +
                "Capacity: " + s.capacity +
                "Turf type: " + turf
                );
        ellipse->setToolTip(tooltip);

        scene->addItem(ellipse);

        connect(ellipse, &ClickableEllipse::clicked, this, [this, ellipse]() {
            QString stadiumName = ellipse->data(0).toString();
            onStadiumEllipseClicked(stadiumName);
        });
    }

    // 2) Draw every undirected edge (thin, black) exactly once:
    QPen edgePen(Qt::black);
    edgePen.setWidthF(1.0);

    for (const Stadium& u : m_stadiums) {
        int u_id  = u.id;
        int u_idx = u_id - 1;
        for (const Edge& e : m_graph[u_idx]) {
            if (e.weight == 0.0) continue;

            int v_id = e.to;
            // draw each undirected edge once (u_id < v_id):
            if (u_id < v_id) {
                QPointF pu = m_idToPoint[u_id];
                QPointF pv = m_idToPoint[v_id];
                scene->addLine(pu.x(), pu.y(), pv.x(), pv.y(), edgePen);
            }
        }
    }
}

std::vector<int> MainWindow::getFullVisitPath(const std::vector<int>& mustVisitIds)
{
    std::vector<int> toVisit = mustVisitIds;    // copy so we can erase
    std::vector<int> fullPath;                  // final output

    int N = static_cast<int>(m_stadiums.size());
    int currId  = m_startStadiumId;
    int currIdx = currId - 1;

    // always start by “standing at” the home stadium:
    fullPath.push_back(currId);

    while (!toVisit.empty()) {
        // run Dijkstra from currIdx
        auto [dist, prev] = dijkstra(currIdx);

        // pick the nearest ‘mustVisit’ among toVisit
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

        if (bestNextId < 0) {
            // none reachable → bail out
            break;
        }

        // Reconstruct the entire chain of 0-based indices from currIdx → bestNextIdx
        std::vector<int> segment;
        for (int v = bestNextIdx; v != currIdx; v = prev[v]) {
            if (v < 0 || v >= N) break;
            segment.push_back(v);
        }
        segment.push_back(currIdx);
        std::reverse(segment.begin(), segment.end());
        // Now: segment[0]==currIdx, segment.back()==bestNextIdx

        // Append all but segment[0], converting each 0-based idx → 1-based ID:
        for (int i = 1; i < static_cast<int>(segment.size()); ++i) {
            fullPath.push_back(segment[i] + 1);
        }

        // move on
        currId   = bestNextId;
        currIdx  = bestNextIdx;
        toVisit.erase(
            std::remove(toVisit.begin(), toVisit.end(), bestNextId),
            toVisit.end()
            );
    }

    return fullPath;
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
    m_lastSelectedStadiumName = stadiumName;
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

// In mainwindow.cpp, implement clearHighlights():
void MainWindow::clearHighlights()
{
    // Remove and delete all red LineItems
    for (auto *lineItem : m_highlightedEdges) {
        scene->removeItem(lineItem);
        delete lineItem;
    }
    m_highlightedEdges.clear();

    // Remove and delete all red label items
    for (auto *labelItem : m_edgeLabels) {
        scene->removeItem(labelItem);
        delete labelItem;
    }
    m_edgeLabels.clear();
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
    clearHighlights();

    // (B) Gather the mustVisit IDs from your QListWidget:
    std::vector<int> mustVisitIds;
    mustVisitIds.reserve(ui->listWidget->count());
    for (int row = 0; row < ui->listWidget->count(); ++row) {
        QString qtext = ui->listWidget->item(row)->text();
        // strip off “N. StadiumName” → “StadiumName”
        if (qtext.contains(". ")) {
            qtext = qtext.section(". ", 1, 1);
        }
        auto it = m_nameToId.find(qtext);
        if (it != m_nameToId.end()) {
            mustVisitIds.push_back(it->second);
        }
    }
    if (mustVisitIds.empty()) {
        ui->textBrowser->setText("No stadiums to visit.\n");
        return;
    }

    // (C) Compute the full sequence (including intermediates):
    std::vector<int> fullPath = getFullVisitPath(mustVisitIds);
    // fullPath is a list of 1-based IDs: [home, X, Y, … finalMustVisit]

    // (D) Draw red, thick lines for every consecutive pair:
    QPen redPen(Qt::red);
    redPen.setWidthF(3.0);
    redPen.setCapStyle(Qt::RoundCap);
    redPen.setJoinStyle(Qt::RoundJoin);

    for (size_t i = 0; i + 1 < fullPath.size(); ++i) {
        int u_id = fullPath[i];
        int v_id = fullPath[i + 1];
        QPointF pu = m_idToPoint[u_id];
        QPointF pv = m_idToPoint[v_id];

        // Draw red line:
        QGraphicsLineItem* lineItem =
            scene->addLine(pu.x(), pu.y(), pv.x(), pv.y(), redPen);
        // Put it “on top” of existing edges:
        lineItem->setZValue(2.0);
        m_highlightedEdges.push_back(lineItem);

        // (E) Look up the weight of edge (u_id→v_id):
        //     We know graph is 0-based in m_graph[u_id-1].to == v_id
        double weight = 0.0;
        int u_idx = u_id - 1;
        for (const Edge& e : m_graph[u_idx]) {
            if (e.to == v_id) {
                weight = e.weight;
                break;
            }
        }

        // Place a text label at the midpoint:
        QPointF mid((pu.x() + pv.x()) / 2.0, (pu.y() + pv.y()) / 2.0);
        QString wText = QString::number(weight);
        QGraphicsSimpleTextItem* label =
            scene->addSimpleText(wText);
        // center it roughly:
        QRectF r = label->boundingRect();
        label->setPos(mid.x() - r.width() / 2, mid.y() - r.height() / 2);
        label->setZValue(3.0);
        label->setBrush(Qt::red);
        m_edgeLabels.push_back(label);
    }

    // (F) Finally, build the same textual summary you had before:
    //     (Number of stadiums visited, the must-visit order, total distance…)
    //     You can still call computeVisitOrderAndDistance(mustVisitIds)
    //     if you want that exact string, or re-build here.

    QString summary = computeVisitOrderAndDistance(mustVisitIds);
    ui->textBrowser->setText(summary);
}

void MainWindow::on_clrBtn_clicked()
{
    // Example action: just dequeue everything and clear the list
    while (!m_stadiumQueue.isEmpty()) {
        QString s = m_stadiumQueue.dequeue();
        qDebug() << "Dequeued stadium:" << s;
    }
    clearHighlights();
    ui->listWidget->clear();
}

void MainWindow::showViewStadiums()
{
    // 1) Build a list of choices
    QStringList options;
    options << tr("All stadiums (A–Z)")
            << tr("American League (A–Z)")
            << tr("National League (A–Z)")
            << tr("Grass-surface stadiums (A–Z)")
            << tr("By opening date");

    // 2) Pop up a simple input‐dialog to let the user choose one
    bool ok = false;
    QString choice = QInputDialog::getItem(
        this,
        tr("View Stadiums"),
        tr("Select which list to display:"),
        options,
        0,           // default index
        false,       // do not allow typing arbitrary text
        &ok
        );

    if (!ok || choice.isEmpty()) {
        // User canceled or closed the dialog
        return;
    }

    // 3) Depending on which option they picked, call the right helper:
    std::vector<Stadium> filtered;
    if (choice == options[0]) {                   // "All stadiums (A–Z)"
        filtered = StadiumHelpers::allSortedByTeam(m_stadiums);
    }
    else if (choice == options[1]) {              // "American League (A–Z)"
        filtered = StadiumHelpers::ALSortedByTeam(m_stadiums);
    }
    else if (choice == options[2]) {              // "National League (A–Z)"
        filtered = StadiumHelpers::NLSortedByTeam(m_stadiums);
    }
    else if (choice == options[3]) {              // "Grass-surface stadiums (A–Z)"
        filtered = StadiumHelpers::grassSortedByTeam(m_stadiums);
    }
    else if (choice == options[4]) {              // "By opening date"
        filtered = StadiumHelpers::sortedByOpenedDate(m_stadiums);
    }

    // 4) Build a multiline QString to show each stadium on its own line.
    //    You can include whichever fields you like; here we do “Team – StadiumName (Opened)”
    QString text;
    text.reserve(filtered.size() * 50);  // pre‐reserve some room
    for (const Stadium &s : filtered) {
        QString line;
        if (choice == options[4]) {
            // If “By opening date,” show “YYYY-MM-DD – Team – StadiumName”
            line = QString("%1 – %2 – %3")
                       .arg(QString::fromStdString(s.opened))
                       .arg(QString::fromStdString(s.team))
                       .arg(QString::fromStdString(s.name));
        } else {
            // Otherwise just show “Team – StadiumName”
            line = QString("%1 – %2")
                       .arg(QString::fromStdString(s.team))
                       .arg(QString::fromStdString(s.name));
        }
        text += line;
        text += "\n";
    }

    // 5) Finally, pop up a message box with the result
    //    (If the list is very long, you might consider using a QTextEdit in a custom QDialog instead,
    //     but for moderate‐sized lists, a QMessageBox is fine.)
    if (filtered.empty()) {
        QMessageBox::information(
            this,
            tr("No Stadiums"),
            tr("No stadiums matched your criteria.")
            );
    }
    else {
        QMessageBox::information(
            this,
            tr("Stadium List"),
            text
            );
    }
}

void MainWindow::showDataDialog()
{
    // If no stadium was ever clicked
    if (m_lastSelectedStadiumName.isEmpty()) {
        QMessageBox::information(
            this,
            tr("No Selection"),
            tr("Please click a stadium on the map first.")
            );
        return;
    }

    // Look up its ID
    auto it = m_nameToId.find(m_lastSelectedStadiumName);
    if (it == m_nameToId.end()) {
        // Should never happen, but guard just in case
        QMessageBox::warning(
            this,
            tr("Error"),
            tr("Could not find data for stadium: %1")
                .arg(m_lastSelectedStadiumName)
            );
        return;
    }
    int stadiumId = it->second;      // 1-based
    int idx       = stadiumId - 1;   // convert to 0-based index

    // Extract the Stadium struct
    const Stadium &s = m_stadiums.at(idx);

    // Build the info string using our helper
    QString info = StadiumHelpers::stadiumInfo(s);

    // Show it in a dialog
    QMessageBox::information(
        this,
        tr("Stadium Details"),
        info
        );
}

// --------------------------------------------------------------------------------------
// Slot: clear the queue and fill it with all National League stadiums (sorted A–Z)
// --------------------------------------------------------------------------------------
void MainWindow::fillNL()
{
    // 1) Clear the internal QQueue and the QListWidget UI
    m_stadiumQueue.clear();
    ui->listWidget->clear();

    // 2) Get all NL stadiums (already sorted by team name) from the helper
    std::vector<Stadium> nlList = StadiumHelpers::NLSortedByTeam(m_stadiums);

    // 3) Loop over them, enqueue each name, and add a “N. StadiumName” row
    int position = 0;
    for (const Stadium &s : nlList) {
        QString name = QString::fromStdString(s.name);
        m_stadiumQueue.enqueue(name);

        ++position; // 1-based index
        QString displayText = QString("%1. %2").arg(position).arg(name);
        ui->listWidget->addItem(new QListWidgetItem(displayText));
    }
}

// --------------------------------------------------------------------------------------
// Slot: clear the queue and fill it with all American League stadiums (sorted A–Z)
// --------------------------------------------------------------------------------------
void MainWindow::fillAL()
{
    // 1) Clear the internal QQueue and the QListWidget UI
    m_stadiumQueue.clear();
    ui->listWidget->clear();

    // 2) Get all AL stadiums (already sorted by team name) from the helper
    std::vector<Stadium> alList = StadiumHelpers::ALSortedByTeam(m_stadiums);

    // 3) Loop over them, enqueue each name, and add a “N. StadiumName” row
    int position = 0;
    for (const Stadium &s : alList) {
        QString name = QString::fromStdString(s.name);
        m_stadiumQueue.enqueue(name);

        ++position; // 1-based index
        QString displayText = QString("%1. %2").arg(position).arg(name);
        ui->listWidget->addItem(new QListWidgetItem(displayText));
    }
}

void MainWindow::addMerch()
{
    if (m_merchList.empty()) {
        QMessageBox::information(this,
                                 tr("No Merchandise"),
                                 tr("There is no merchandise to buy."));
        return;
    }

    // 1) Build a QStringList of all merch names
    QStringList names;
    for (const Merch &m : m_merchList) {
        names << m.name;
    }

    // 2) Let the user pick one (non-editable dropdown)
    bool ok = false;
    QString chosenName = QInputDialog::getItem(
        this,
        tr("Select Merchandise"),
        tr("Choose an item:"),
        names,
        0,      // default index
        false,  // do not allow typing arbitrary text
        &ok
        );
    if (!ok || chosenName.isEmpty()) {
        // user canceled
        return;
    }

    // 3) Find that Merch in m_merchList to get its price
    const Merch *selectedMerch = nullptr;
    for (const Merch &m : m_merchList) {
        if (m.name == chosenName) {
            selectedMerch = &m;
            break;
        }
    }
    if (!selectedMerch) {
        // Should never happen if names matched exactly
        QMessageBox::warning(
            this,
            tr("Lookup Error"),
            tr("Selected item not found.")
            );
        return;
    }

    // 4) Ask the user for a quantity (1..100)
    int qty = QInputDialog::getInt(
        this,
        tr("Quantity"),
        tr("Enter quantity for \"%1\":").arg(chosenName),
        1,    // default value
        1,    // min value
        100,  // max value
        1,    // step
        &ok
        );
    if (!ok) {
        // user canceled
        return;
    }

    // 5) Create a Purchased record and append to m_cart
    Purchased p;
    p.name       = chosenName;
    p.qty        = qty;
    p.unitPrice  = selectedMerch->price;
    // totalPrice() is computed on the fly
    m_cart.push_back(p);

    // 6) Add one line to cartListWidget: “Name | Qty | Unit | Total”
    QString line = QString("%1 | Qty: %2 | Unit: $%3 | Total: $%4")
                       .arg(p.name)
                       .arg(p.qty)
                       .arg(QString::number(p.unitPrice, 'f', 2))
                       .arg(QString::number(p.totalPrice(), 'f', 2));
    ui->merchList->addItem(new QListWidgetItem(line));
}
