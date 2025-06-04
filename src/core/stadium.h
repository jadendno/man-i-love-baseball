#pragma once
#include <string>
#include <vector>
#include <utility>
#include <QFile>
#include <QByteArray>
#include <nlohmann/json.hpp>

struct Stadium {
    int id;
    std::string name;
    std::pair<double, double> location; // latitude, longitude
    std::string team;
    std::string address;
    std::string phone;
    std::string opened;
    std::string capacity;
    struct Adjacent {
        int id;
        double distance;
    };
    std::vector<Adjacent> adjacent;
    bool turf;
};

struct Edge {
    int to;
    double weight;
};

void to_json(nlohmann::json& j, const Stadium::Adjacent& a);
void from_json(const nlohmann::json& j, Stadium::Adjacent& a);
void to_json(nlohmann::json& j, const Stadium& s);
void from_json(const nlohmann::json& j, Stadium& s);
std::vector<Stadium> load_stadiums(const std::string& filename);
std::vector<Stadium> load_stadiums_from_resource(const QString& resource_path);
void save_stadiums(const std::string& filename, const std::vector<Stadium>& stadiums);

std::vector<std::vector<Edge>> buildGraph(const std::vector<Stadium>& stadiums);
