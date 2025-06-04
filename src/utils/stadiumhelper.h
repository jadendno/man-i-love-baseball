#pragma once

#include "core/stadium.h"
#include <vector>

namespace StadiumHelpers {

/// Returns all stadiums sorted by their team name (lexicographically).
std::vector<Stadium> allSortedByTeam(const std::vector<Stadium>& stadiums);

/// Returns only those stadiums whose `team` is in the American League,
/// sorted by team name. (See implementation for the hard-coded AL list.)
std::vector<Stadium> ALSortedByTeam(const std::vector<Stadium>& stadiums);

/// Returns only those stadiums whose `team` is in the National League,
/// sorted by team name. (See implementation for the hard-coded NL list.)
std::vector<Stadium> NLSortedByTeam(const std::vector<Stadium>& stadiums);

/// Returns only those stadiums where `turf == false` (i.e. grass surface),
/// sorted by team name.
std::vector<Stadium> grassSortedByTeam(const std::vector<Stadium>& stadiums);

/// Returns all stadiums sorted in ascending order by their `opened` date.
/// We assume that `Stadium::opened` is in “YYYY-MM-DD” form. If parsing fails,
/// that stadium is treated as “invalid date” and placed after any valid date.
std::vector<Stadium> sortedByOpenedDate(const std::vector<Stadium>& stadiums);

/// Returns a multiline QString containing all the key fields of `s`,
/// formatted exactly like your tooltip (with newlines and labels).
/// The turf‐type line will read either “Turf type: Grass” or “Turf type: Artificial”.
QString stadiumInfo(const Stadium &s);

} // namespace StadiumHelpers
