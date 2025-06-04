#ifndef GRAPHS_H
#define GRAPHS_H

#include <QString>
#include <QPointF>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>


struct Stadium{
    std::string stadiumName;
    std::string teamName;
    std::string streetName;
    std::string cityStateZip;
    std::string phoneNum;
    std::string launch;
    std::string capacity;
    std::string league;
    QPointF position; //holds x and y
};

class Graphs {
public:
    enum LeagueType { ALL, AMERICAN, NATIONAL };

    Graphs(LeagueType type = ALL);

    void addStadium(const std::string& name, const QPointF& position, LeagueType league);
    void addEdge(const std::string& from, const std::string& to, int distance);


    const std::unordered_map<std::string, QPointF>& getStadiumPositions() const;
    const std::unordered_map<std::string, std::vector<std::pair<std::string, int>>>& getAdjList() const;

    //graph hashing and file streaming
    void loadStadiumsFromFile(const std::string& file);

    void printAllStadiums() const;

    const std::unordered_map<std::string, Stadium>& getStadiumInfo() const { return stadiumInfo; };



private:
    std::unordered_map<std::string, QPointF> stadiumPositions;
    std::unordered_map<std::string, Stadium> stadiumInfo;
    std::unordered_map<std::string, LeagueType> stadiumLeague;


    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjList;




    LeagueType activeLeague;


    void loadSampleGraph(); // for testing/demo
};

void addMajorLeagueEdges(Graphs& graph);

#endif // GRAPHS_H
