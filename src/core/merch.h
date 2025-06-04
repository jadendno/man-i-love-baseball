#pragma once

#include <QString>
#include <vector>

/// A simple merchandise item.
struct Merch {
    int     id;
    QString name;
    double  price;
};

/// Read a JSON array from `filename` and return a vector<Merch>.
/// The JSON should look like:
/// [ { "id":1, "name":"Baseball cap", "price":25.99 }, … ]
std::vector<Merch> loadMerchFromJson(const QString &filename);

/// Write a vector<Merch> back to a JSON file at `filename`, overwriting it.
/// Each Merch is written as { "id":…, "name":…, "price":… }.
bool writeMerchToJson(const QString &filename, const std::vector<Merch> &list);
