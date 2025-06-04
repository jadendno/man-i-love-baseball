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

        std::cout << "Loaded: " << s.stadiumName << " at (" << s.position.x() << ", " << s.position.y() << ")\n";
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

