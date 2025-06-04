#ifndef MAP_H
#define MAP_H

#include <QPointF>
#include <QSizeF>
#include <QRectF>
#include <QGraphicsPixmapItem>
#include <QtMath>

// Struct for geographic bounds
struct MapBounds {
    double minLat;
    double maxLat;
    double minLon;
    double maxLon;
};

// Default bounds for contiguous US (approximate)
inline constexpr MapBounds DefaultUSBbounds = { 23.4, 51.5, -126.5, -65.3 };

class MapPlotter {
public:
    // Convert geographic coordinates to scene coordinates for a given QGraphicsPixmapItem
    // Uses Mercator projection in Y and linear in X.
    // Returns a QPointF ready to pass to scene->addEllipse() or similar.
    static QPointF latLonToScene(const QGraphicsPixmapItem *mapItem,
                                 double lat, double lon,
                                 const MapBounds &bounds = DefaultUSBbounds);

private:
    // Convert latitude (degrees) to Mercator Y-value
    static double latToMercator(double lat) {
        double phi = qDegreesToRadians(lat);
        return std::log(std::tan(M_PI / 4.0 + phi / 2.0));
    }
    // Internal: map lat/lon to local pixel coords (0..width, 0..height) of the pixmap
    static QPointF latLonToPixelMercator(double lat, double lon,
                                         const MapBounds &bounds,
                                         const QSizeF &pixelSize);
};

#endif // MAP_H

