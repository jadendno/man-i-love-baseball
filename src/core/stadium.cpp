#include "core/stadium.h"
#include <fstream>
#include <stdexcept>

void to_json(nlohmann::json& j, const Stadium::Adjacent& a) {
    j = nlohmann::json{{"id", a.id}, {"distance", a.distance}};
}
void from_json(const nlohmann::json& j, Stadium::Adjacent& a) {
    j.at("id").get_to(a.id);
    j.at("distance").get_to(a.distance);
}

void to_json(nlohmann::json& j, const Stadium& s) {
    j = nlohmann::json{
        {"id", s.id},
        {"name", s.name},
        {"location", {s.location.first, s.location.second}},
        {"team", s.team},
        {"address", s.address},
        {"phone", s.phone},
        {"opened", s.opened},
        {"capacity", s.capacity},
        {"adjacent", s.adjacent}
    };
}

void from_json(const nlohmann::json& j, Stadium& s) {
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    std::vector<double> loc = j.at("location").get<std::vector<double>>();
    s.location = std::make_pair(loc[0], loc[1]);
    j.at("team").get_to(s.team);
    j.at("address").get_to(s.address);
    j.at("phone").get_to(s.phone);
    j.at("opened").get_to(s.opened);
    j.at("capacity").get_to(s.capacity);
    if (j.contains("adjacent"))
        j.at("adjacent").get_to(s.adjacent);
    else
        s.adjacent.clear();
}

std::vector<Stadium> load_stadiums(const std::string& filename) {
    std::ifstream in(filename);
    nlohmann::json j;
    in >> j;
    return j.get<std::vector<Stadium>>();
}

std::vector<Stadium> load_stadiums_from_resource(const QString& resource_path) {
    QFile file(resource_path);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Could not open resource: " + resource_path.toStdString());
    }
    QByteArray data = file.readAll();
    nlohmann::json j = nlohmann::json::parse(data.constData());
    return j.get<std::vector<Stadium>>();
}

void save_stadiums(const std::string& filename, const std::vector<Stadium>& stadiums) {
    nlohmann::json j = stadiums;
    std::ofstream out(filename);
    out << j.dump(2);
}

// Assume stadium IDs run from 1..N with no gaps.
// If not, switch to unordered_map<int,vector<Edge>> instead.

std::vector<std::vector<Edge>> buildGraph(const std::vector<Stadium>& stadiums) {
    // 1) Find out how many stadiums we have (N)
    int N = static_cast<int>(stadiums.size());

    // 2) Prepare an N‐length vector of adjacency‐lists.
    //    We’ll index by (id - 1). So stadium with id=1 lives at index 0.
    std::vector<std::vector<Edge>> graph(N);

    // 3) We also want a quick way to look up a Stadium by its id.
    //    Here we build an index array "byId", where byId[id-1] == pointer/index into stadiums
    std::vector<const Stadium*> byId(N, nullptr);
    for (const Stadium& s : stadiums) {
        int idx = s.id - 1;
        if (idx < 0 || idx >= N) {
            throw std::runtime_error("Stadium id out of range in buildGraph()");
        }
        byId[idx] = &s;
    }

    // 4) Now, for each stadium, walk its adjacent[] list:
    //    - Insert an Edge (u → v) into graph[u_id - 1].
    //    - Because the map is undirected, also insert (v → u) into graph[v_id - 1].
    for (const Stadium& s : stadiums) {
        int u_id = s.id;
        int u_idx = u_id - 1;
        for (auto& adj : s.adjacent) {
            int v_id = adj.id;
            double dist = adj.distance;

            int v_idx = v_id - 1;
            if (v_idx < 0 || v_idx >= N) {
                throw std::runtime_error("Adjacent id out of range in buildGraph()");
            }

            // Add edge u → v
            graph[u_idx].push_back(Edge{v_id, dist});

            // Add edge v → u (since undirected).
            // But be careful not to double‐insert if the JSON already stored both directions.
            // If you know the JSON only lists each pair once, do this:
            graph[v_idx].push_back(Edge{u_id, dist});
        }
    }

    return graph;
}
