#include "ui/map.h"

QPointF MapPlotter::latLonToScene(const QGraphicsPixmapItem *mapItem,
                                  double lat, double lon,
                                  const MapBounds &bounds)
{
    // Get the drawn area of the pixmap in item coordinates
    QRectF rect = mapItem->boundingRect();
    QSizeF size = rect.size();

    // Compute pixel position within the pixmap
    QPointF pixel = latLonToPixelMercator(lat, lon, bounds, size);

    // Translate pixel into scene coordinates (mapItem at its position)
    QPointF scenePos = mapItem->pos() + rect.topLeft() + pixel;
    return scenePos;
}

QPointF MapPlotter::latLonToPixelMercator(double lat, double lon,
                                          const MapBounds &bounds,
                                          const QSizeF &pixelSize)
{
    // X: longitude linear from minLon to maxLon
    double x = (lon - bounds.minLon)
               / (bounds.maxLon - bounds.minLon)
               * pixelSize.width();

    // Y: Mercator projection
    double yMax = latToMercator(bounds.maxLat);
    double yMin = latToMercator(bounds.minLat);
    double y    = latToMercator(lat);

    // Invert so that maxLat (yMax) is at top (0)
    double py = (yMax - y) / (yMax - yMin) * pixelSize.height();

    return QPointF(x, py);
}
