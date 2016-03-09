#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>

struct Point {
    long long x;
    long long y;

    Point(long long x = 0, long long y = 0) : x(x), y(y) {
    }

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }

    Point operator*(long long factor) const{
        return Point(factor*x, factor*y);
    }

    Point operator/(long long factor) const{
        return Point(factor*x, factor*y);
    }

    friend std::ostream& operator<<(std::ostream& str, const Point& point) {
        str << point.x << ' ' << point.y;
        return str;
    }

    friend std::istream& operator>>(std::istream& str, Point& point) {
        str >> point.x >> point.y;
        return str;
    }

    friend long long dot(const Point& a, const Point& b) {
        return a.x*b.x + a.y*b.y;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

long long sq_distance(const Point& a, const Point& b);

struct Segment {
    Point ends[2];

    Segment(Point a = Point(), Point b = Point()) : ends{a, b} {
    }

    friend std::ostream& operator<<(std::ostream& str, const Segment& segment) {
        str << segment.ends[0] << ' ' << segment.ends[1];
        return str;
    }

    friend std::istream& operator>>(std::istream& str, Segment& segment) {
        str >> segment.ends[0] >> segment.ends[1];
        return str;
    }

    long long sq_length() const {
        return sq_distance(ends[0], ends[1]);
    }

    bool is_point() const {
        return ends[0] == ends[1];
    }

    Point vector() const {
        return ends[1] - ends[0];
    }
};

struct Line {
    long long A, B, C;

    Line(long long A = 1, long long B = 1, long long C = 0) : A(A), B(B), C(C) {
    }

    Line(const Segment& segment) : A(segment.ends[0].y - segment.ends[1].y),
    B(segment.ends[1].x - segment.ends[0].x),
    C(-A*segment.ends[0].x - B*segment.ends[1].y) {
    }

    void normalize() {
    }
};


long long sq_distance(const Point& a, const Point& b) {
    Point temp = a - b;
    return temp.x*temp.x + temp.y*temp.y;
}

double distance(const Point& a, const Point& b) {
    return sqrt(sq_distance(a, b));
}

double distance(const Point& point, const Line& line) {
    return fabs((point.x*line.A + point.y*line.B + line.C) / sqrt(line.A*line.A + line.B*line.B));
}

double distance(const Point& point, const Segment& segment) {
    if (segment.is_point()) {
        return distance(point, segment.ends[0]);
    }
    double t = dot(point - segment.ends[0], segment.vector()) / (double)segment.sq_length();
    if (t >= 1) {
        return sqrt(distance(point, segment.ends[1]));
    } else if (t <= 0) {
        return sqrt(distance(point, segment.ends[0]));
    } else {
        return distance(point, Line(segment));
    }
}

int main() {
#ifdef DEBUG
    std::istream& in = std::cin;
    std::ostream& out = std::cout;
#else
    std::ifstream in("distance4.in");
    std::ofstream out("distance4.out");
#endif
    out.setf(std::ios::fixed);
    out.precision(10);
    Point point;
    Segment line;
    in >> point >> line;
    out << distance(point, line) << std::endl;
}
