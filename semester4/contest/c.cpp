#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>


template<class T> struct GenericPoint {
    T x;
    T y;

    GenericPoint(T x = 0, T y = 0) : x(x), y(y) {
    }

    GenericPoint operator+(const GenericPoint& other) const {
        return GenericPoint(x + other.x, y + other.y);
    }

    GenericPoint operator-(const GenericPoint& other) const {
        return GenericPoint(x - other.x, y - other.y);
    }

    GenericPoint operator*(T factor) const{
        return GenericPoint(factor*x, factor*y);
    }

    GenericPoint operator/(T factor) const{
        return GenericPoint(factor*x, factor*y);
    }

    GenericPoint operator<(const GenericPoint& other) const {
        return (x == other.x ? (y <= other.y) : (x <= other.y));
    }

    friend std::ostream& operator<<(std::ostream& str, const GenericPoint& point) {
        str << point.x << ' ' << point.y;
        return str;
    }

    friend std::istream& operator>>(std::istream& str, GenericPoint& point) {
        str >> point.x >> point.y;
        return str;
    }

    friend T dot(const GenericPoint& a, const GenericPoint& b) {
        return a.x*b.x + a.y*b.y;
    }

    bool operator==(const GenericPoint& other) const {
        return x == other.x && y == other.y;
    }
};

typedef GenericPoint<double> PointF;
typedef GenericPoint<long long> Point;

long long sq_distance(const Point& a, const Point& b);

struct Line;

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

    template<typename PointT> bool contains(const PointT& point) const {
        if (point.x < ends[0].x && point.x < ends[1].x) {
            return false;
        }
        if (point.x > ends[0].x && point.x > ends[1].x) {
            return false;
        }
        if (point.y < ends[0].y && point.y < ends[1].y) {
            return false;
        }
        if (point.y > ends[0].y && point.y > ends[1].y) {
            return false;
        }
        return true;
    }


    Point vector() const {
        return ends[1] - ends[0];
    }
};

long long gcd(long long a, long long b) {
    if (a == 0 || b == 0) {
        return a+b;
    }
    return gcd(b, a % b);
}

struct Line {
    long long A, B, C;

    Line(long long A = 1, long long B = 1, long long C = 0) : A(A), B(B), C(C) {
    }

    Line(const Segment& segment) : A(segment.ends[0].y - segment.ends[1].y),
    B(segment.ends[1].x - segment.ends[0].x),
    C(-A*segment.ends[0].x - B*segment.ends[0].y) {
    }

    friend std::istream& operator>>(std::istream& str, Line& line) {
        str >> line.A >> line.B >> line.C;
        return str;
    }

    friend bool have_good_intersection(const Line& a, const Line& b) {
        return a.A*b.B - b.A*a.B != 0;
    }

    friend PointF intersect(const Line& a, const Line& b) {
        double zn = a.A*b.B - b.A*a.B;
        return PointF(-(a.C*b.B - b.C*a.B) / zn, -(a.A*b.C - b.A*a.C) / zn);
    }


    void normalize() {
        long long div = gcd(A, gcd(B, C));
        A /= div;
        B /= div;
        C /= div;
        if (A < 0) {
            A = -A;
            B = -B;
            C = -C;
        }
    }

    bool operator==(Line& other) {
        normalize();
        other.normalize();
        return (A == other.A) && (B == other.B) && (C == other.C);
    }
        
};

bool have_intersection(const Segment& a, const Segment& b) {
    Line line_a(a);
    Line line_b(b);
    if (a.is_point() && b.is_point()) {
        return a.ends[0] == b.ends[0];
    } else if (a.is_point() || b.is_point()) {
        if (a.is_point()) {
            return b.contains(a.ends[0]);
        }
        return a.contains(b.ends[0]);
    }
    if (have_good_intersection(line_a, line_b)) {
        PointF intersection = intersect(line_a, line_b);
        return a.contains(intersection) && b.contains(intersection);
    } else {
        if (line_a == line_b) {
            return a.contains(b.ends[0]) || a.contains(b.ends[1]) || b.contains(a.ends[0]) || b.contains(a.ends[1]);
        } else {
            return false;
        }
    }
}


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
    long long a = sq_distance(point, segment.ends[0]);
    long long b = sq_distance(point, segment.ends[1]);
    long long c = segment.sq_length();
    if (a >= b + c) {
        return sqrt(b);
    } else if (b >= a + c) {
        return sqrt(a);
    } else {
        return distance(point, Line(segment));
    }
}

int main() {
#ifdef DEBUG
    std::istream& in = std::cin;
    std::ostream& out = std::cout;
#else
    std::ifstream in("intersec2.in");
    std::ofstream out("intersec2.out");
#endif
    out.setf(std::ios::fixed);
    out.precision(10);
    Segment a, b;
    in >> a >> b;
    out << (have_intersection(a, b) ? "YES" : "NO")  << std::endl;
}
