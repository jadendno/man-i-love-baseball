#include "graphs.h"

//default constructor to generate graphs
Graphs::Graphs(LeagueType type): activeLeague(type){
    loadStadiumsFromFile("stadiums.txt");
}

void Graphs::addStadium(const std::string& name, const QPointF& position, LeagueType league) {
    if (activeLeague == ALL || league == activeLeague) {
        stadiumPositions[name] = position;
        stadiumLeague[name] = league;
    }
}

void Graphs::addEdge(const std::string& from, const std::string& to, int distance) {
    if (stadiumPositions.find(from) != stadiumPositions.end() &&
        stadiumPositions.find(to) != stadiumPositions.end())
     {
        adjList[from].push_back({to, distance});
        adjList[to].push_back({from, distance}); // undirected
    }
}


const std::unordered_map<std::string, QPointF>& Graphs::getStadiumPositions() const {
    return stadiumPositions;
}

const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& Graphs::getAdjList() const {
    return adjList;
}

void Graphs::loadSampleGraph() {


    addStadium("Phambussy's Stadium", QPointF(65, 275), LeagueType::ALL);
    addStadium("Scottea's Stadium", QPoint(65, 400), LeagueType::ALL);

    addEdge("Phambussy's Stadium", "Scottea's Stadium", 500);
    // Add sample stadiums (QPointF(x, y) values should match your map)
    //addStadium("Phambussy's Stadium", QPointF(65, 275), ALL);
    //addStadium("Dodger Stadium", QPointF(100, 450), NATIONAL);
    //addStadium("Minute Maid Park", QPointF(350, 500), AMERICAN);
    //addStadium("Wrigley Field", QPointF(500, 250), NATIONAL);


    /*

    // Add sample edges (distance in miles)
    addEdge("Yankee Stadium", "Wrigley Field", 790);
    addEdge("Wrigley Field", "Dodger Stadium", 1745);
    addEdge("Dodger Stadium", "Minute Maid Park", 1540);
    addEdge("Minute Maid Park", "Yankee Stadium", 1620);*/
}

void Graphs::loadStadiumsFromFile(const std::string& file) {
    std::ifstream inFile(file);
    if (!inFile) {
        std::cerr << "Failed to open " << file << "\n";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        Stadium s;
        s.stadiumName = line;
        std::getline(inFile, s.teamName);
        std::getline(inFile, s.streetName);
        std::getline(inFile, s.cityStateZip);
        std::getline(inFile, s.phoneNum);
        std::getline(inFile, s.launch);
        std::getline(inFile, s.capacity);
        std::getline(inFile, s.league);

        std::string xStr, yStr;
        std::getline(inFile, xStr);
        std::getline(inFile, yStr);

        try {
            double x = std::stod(xStr);
            double y = std::stod(yStr);
            s.position = QPointF(x, y);
        } catch (...) {
            std::cerr << "Invalid coordinates for stadium: " << s.stadiumName << "\n";
            continue;
        }

        // Store info
        stadiumPositions[s.stadiumName] = s.position;
        stadiumLeague[s.stadiumName] = (s.league == "American League") ? AMERICAN : NATIONAL;
        stadiumInfo[s.stadiumName] = s;
        adjList[s.stadiumName] = {};

        //std::cout << "Loaded: " << s.stadiumName << " at (" << s.position.x() << ", " << s.position.y() << ")\n";
    }

    std::cout << "\nTotal stadiums loaded: " << stadiumInfo.size() << "\n";
}

void Graphs::printAllStadiums() const {
    for (const auto& [name, s] : stadiumInfo) {
        std::cout << "Stadium Name: " << s.stadiumName << "\n"
                  << "Team Name: " << s.teamName << "\n"
                  << "Street Address: " << s.streetName << "\n"
                  << "City/State/Zip: " << s.cityStateZip << "\n"
                  << "Phone: " << s.phoneNum << "\n"
                  << "Opened: " << s.launch << "\n"
                  << "Capacity: " << s.capacity << "\n"
                  << "League: " << s.league << "\n"
                  << "Position: (" << s.position.x() << ", " << s.position.y() << ")\n"
                  << "---------------------------------------------\n";
    }

    std::cout << "\n\n";
}

void addMajorLeagueEdges(Graphs& graph) {
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
}

