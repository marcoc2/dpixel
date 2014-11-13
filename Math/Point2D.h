#ifndef POINT2D_H
#define POINT2D_H

class Point2D
{
public:

    float x;
    float y;
    Point2D(){
        this->x = 0;
        this->y = 0;
    }
    Point2D(float x, float y){
        this->x = x;
        this->y = y;
    }
    bool operator <(const Point2D &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
    bool operator ==(const Point2D &p) const {
        return ((x == p.x) && (y == p.y));
    }
    Point2D operator *(const int t) const {
        return Point2D(this->x * t, this->y * t);
    }
    Point2D operator /(const int t) const {
        return Point2D(this->x / t, this->y / t);
    }
};

#endif // POINT2D_H
