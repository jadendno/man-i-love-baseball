#include "utils/stadiumhelper.h"

#include <algorithm>
#include <set>
#include <QString>
#include <QDate>

namespace StadiumHelpers {

//----------------------------------------------------------------------------------------------------------------------
// Comparator: sorts two Stadiums by their `team` field (std::string) in ascending lexicographic order.
//----------------------------------------------------------------------------------------------------------------------
static bool compareByTeam(const Stadium& a, const Stadium& b) {
    return a.team < b.team;
}

//----------------------------------------------------------------------------------------------------------------------
// 1) allSortedByTeam(): just sort the entire vector by `team`
//----------------------------------------------------------------------------------------------------------------------
std::vector<Stadium> allSortedByTeam(const std::vector<Stadium>& stadiums) {
    std::vector<Stadium> result = stadiums;
    std::sort(result.begin(), result.end(), compareByTeam);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
// 2) ALSortedByTeam(): filter to a hard-coded set of American League teams, then sort by `team`.
//   See comment below for which team names are considered “American League.”
//----------------------------------------------------------------------------------------------------------------------
std::vector<Stadium> ALSortedByTeam(const std::vector<Stadium>& stadiums) {
    // Hard-coded set of official AL team names:
    static const std::set<std::string> alTeams = {
        "Baltimore Orioles",
        "Boston Red Sox",
        "New York Yankees",
        "Tampa Bay Rays",
        "Toronto Blue Jays",
        "Chicago White Sox",
        "Cleveland Guardians",
        "Detroit Tigers",
        "Kansas City Royals",
        "Minnesota Twins",
        "Houston Astros",
        "Los Angeles Angels",
        "Oakland Athletics",
        "Seattle Mariners",
        "Texas Rangers"
    };

    std::vector<Stadium> result;
    result.reserve(stadiums.size());
    for (const Stadium& s : stadiums) {
        if (alTeams.count(s.team)) {
            result.push_back(s);
        }
    }
    std::sort(result.begin(), result.end(), compareByTeam);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
// 3) NLSortedByTeam(): filter to a hard-coded set of National League teams, then sort by `team`.
//   See comment below for which team names are considered “National League.”
//----------------------------------------------------------------------------------------------------------------------
std::vector<Stadium> NLSortedByTeam(const std::vector<Stadium>& stadiums) {
    // Hard-coded set of official NL team names:
    static const std::set<std::string> nlTeams = {
        "Atlanta Braves",
        "Miami Marlins",
        "New York Mets",
        "Philadelphia Phillies",
        "Washington Nationals",
        "Chicago Cubs",
        "Cincinnati Reds",
        "Milwaukee Brewers",
        "Pittsburgh Pirates",
        "St. Louis Cardinals",
        "Arizona Diamondbacks",
        "Colorado Rockies",
        "Los Angeles Dodgers",
        "San Diego Padres",
        "San Francisco Giants"
    };

    std::vector<Stadium> result;
    result.reserve(stadiums.size());
    for (const Stadium& s : stadiums) {
        if (nlTeams.count(s.team)) {
            result.push_back(s);
        }
    }
    std::sort(result.begin(), result.end(), compareByTeam);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
// 4) grassSortedByTeam(): filter to those stadiums whose `turf == false` (i.e. grass), then sort by `team`.
//----------------------------------------------------------------------------------------------------------------------
std::vector<Stadium> grassSortedByTeam(const std::vector<Stadium>& stadiums) {
    std::vector<Stadium> result;
    result.reserve(stadiums.size());
    for (const Stadium& s : stadiums) {
        if (!s.turf) {
            result.push_back(s);
        }
    }
    std::sort(result.begin(), result.end(), compareByTeam);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
// Helper: parse a “YYYY-MM-DD” string into a QDate. If invalid, returns a null QDate.
//----------------------------------------------------------------------------------------------------------------------
static QDate parseOpenedDate(const std::string& openedStr) {
    // Try ISO yyyy-MM-dd. If that fails, return a null date.
    QString q = QString::fromStdString(openedStr);
    QDate d = QDate::fromString(q, Qt::ISODate);
    return d.isValid() ? d : QDate();
}

//----------------------------------------------------------------------------------------------------------------------
// 5) sortedByOpenedDate(): sort entire vector by the `opened` field (assumed “YYYY-MM-DD”).
//    If parsing fails for either, we treat invalid dates as “later than any valid date.” If both invalid, fallback to
//    sorting by team name just so the order is deterministic.
//----------------------------------------------------------------------------------------------------------------------
std::vector<Stadium> sortedByOpenedDate(const std::vector<Stadium>& stadiums) {
    std::vector<Stadium> result = stadiums;
    std::sort(result.begin(), result.end(),
              [](const Stadium& a, const Stadium& b) {
                  QDate da = parseOpenedDate(a.opened);
                  QDate db = parseOpenedDate(b.opened);

                  if (da.isValid() && db.isValid()) {
                      return da < db;
                  }
                  if (da.isValid()) {
                      // a has a valid date, b does not → a should come first
                      return true;
                  }
                  if (db.isValid()) {
                      // b has a valid date, a does not → b comes first
                      return false;
                  }
                  // both invalid → fallback to team name
                  return a.team < b.team;
              }
              );
    return result;
}

QString stadiumInfo(const Stadium &s)
{
    // Determine turf type string
    QString turf = s.turf ? "Artificial" : "Grass";

    // Build the multiline text exactly as you showed
    QString text = QString::fromStdString(
        s.name + "\n" +
        s.team + "\n" +
        s.address + "\n" +
        s.phone + "\n" +
        "Opened: " + s.opened + "\n" +
        "Capacity: " + s.capacity + "\n"   // add newline before Turf type
        );
    // Append the turf line (we can’t include it in the std::string above since it’s a QString)
    text += QString("Turf type: %1").arg(turf);
    return text;
}

} // namespace StadiumHelpers
