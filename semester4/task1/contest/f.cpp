#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

template<typename T> T sq(T val) {
    return val*val;
}


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
        return GenericPoint(x/factor, y/factor);
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

    T norm() const {
        return sq(x) + sq(y);
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

    void translate(long long x, long long y) {
        C += A*x + B*y;
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

struct Circle {
    Point center;
    long long r;

    Circle(Point center = Point(0, 0), long long radius = 0) : center(center), r(radius) {
    }

    friend std::istream& operator>>(std::istream& str, Circle& circle) {
        str >> circle.center >> circle.r;
        return str;
    }

    friend bool operator==(const Circle& a, const Circle& b) {
        return a.center == b.center && a.r == b.r;
    }

    friend std::vector<PointF> intersect(const Circle& a, const Circle& b) {
        if (a.center == b.center) {
            if (a.r == b.r) {
                return std::vector<PointF>(3);
            } else {
                return std::vector<PointF>();
            }
        }
        Line line(-2*(b.center.x - a.center.x), -2*(b.center.y - a.center.y), sq(b.center.x - a.center.x) + sq(b.center.y - a.center.y) + sq(a.r) - sq(b.r));
        line.translate(a.center.x, a.center.y);
        return intersect(a, line);
    }

    friend std::vector<PointF> intersect(const Circle& a, const Line& b) {
        Circle circle(a);
        Line line(b);
        line.C -= line.A*circle.center.x + line.B*circle.center.y;
        circle.center = Point(0, 0);
        long long denom = sq(line.C) - sq(circle.r)*(sq(line.A) + sq(line.B));
        PointF intersection(-line.A*line.C / (double)(sq(line.A) + sq(line.B)) + a.center.x, -line.B*line.C / (double)(sq(line.A) + sq(line.B)) + a.center.y);
        if (denom > 0) {
            return std::vector<PointF>();
        } else if (denom == 0) {
            return std::vector<PointF>{intersection};
        } else {
            double d = sq(circle.r) - sq(line.C)/(double)(sq(line.A) + sq(line.B));
            double mult = sqrt(d / (double)(sq(line.A) + sq(line.B)));
            return std::vector<PointF>{intersection, PointF(sqrt(sq(intersection.x - a.center.x) + sq(intersection.y - a.center.y)), sqrt(d)),
                    intersection + PointF(line.B*mult, -line.A*mult), intersection + PointF(-line.B*mult, line.A*mult)};
        }
    }
};

long long vec(Point a, Point b, Point c) {
    Point A(b - a);
    Point B(c - b);
    return A.x*B.y - A.y*B.x;
}

bool cmp (Point a, Point b) {
    return a.x < b.x || a.x == b.x && a.y < b.y;
}
int main() {
#ifdef DEBUG
    std::istream& in = std::cin;
    std::ostream& out = std::cout;
#else
    std::ifstream in("hull.in");
    std::ofstream out("hull.out");
#endif
    out.setf(std::ios::fixed);
    out.precision(10);
    int n;
    in >> n;
    std::vector<Point> points;
    for (int i = 0; i < n; ++i) {
        Point cur;
        in >> cur;
        points.push_back(cur);
    }
    if (points.size() == 1)  return 0;
    std::sort(points.begin(), points.end(), &cmp);
    Point p1 = points[0],  p2 = points.back();
    std::vector<Point> up, down;
    up.push_back(p1);
    down.push_back(p1);
    for (size_t i=1; i<points.size(); ++i) {
        if (i==points.size()-1 || vec(p1, points[i], p2) < 0) {
            while (up.size()>=2 && vec(up[up.size()-2], up[up.size()-1], points[i]) >= 0)
                up.pop_back();
            up.push_back(points[i]);
        }
        if (i==points.size()-1 || vec(p1, points[i], p2) > 0) {
            while (down.size()>=2 && vec(down[down.size()-2], down[down.size()-1], points[i]) <= 0)
                down.pop_back();
            down.push_back(points[i]);
        }
    }
    points.clear();
    for (size_t i=0; i<up.size(); ++i)
        points.push_back(up[i]);
    for (size_t i=down.size()-2; i>0; --i)
        points.push_back(down[i]);
    out << points.size() << std::endl;
    for (Point p : points) {
        out << p << std::endl;
    }
}
