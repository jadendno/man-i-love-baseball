#include "merch.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

std::vector<Merch> loadMerchFromJson(const QString &filename)
{
    std::vector<Merch> out;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open merch JSON for reading:" << filename;
        return out;
    }

    QByteArray raw = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(raw, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Failed to parse merch JSON:" << parseError.errorString();
        return out;
    }

    if (!doc.isArray()) {
        qWarning() << "Expected a JSON array in" << filename;
        return out;
    }

    QJsonArray array = doc.array();
    for (const QJsonValue &val : array) {
        if (!val.isObject()) continue;

        QJsonObject obj = val.toObject();
        Merch m;
        // id
        if (obj.contains("id") && obj["id"].isDouble())
            m.id = obj["id"].toInt();
        else
            continue; // skip if no valid id

        // name
        if (obj.contains("name") && obj["name"].isString())
            m.name = obj["name"].toString();
        else
            continue; // skip if no name

        // price
        if (obj.contains("price") && obj["price"].isDouble())
            m.price = obj["price"].toDouble();
        else
            continue; // skip if no price

        out.push_back(m);
    }

    return out;
}

bool writeMerchToJson(const QString &filename, const std::vector<Merch> &list)
{
    QJsonArray array;
    for (const Merch &m : list) {
        QJsonObject obj;
        obj["id"] = m.id;
        obj["name"] = m.name;
        obj["price"] = m.price;
        array.append(obj);
    }

    QJsonDocument doc(array);
    QByteArray bytes = doc.toJson(QJsonDocument::Indented);

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open merch JSON for writing:" << filename;
        return false;
    }
    file.write(bytes);
    file.close();
    return true;
}
