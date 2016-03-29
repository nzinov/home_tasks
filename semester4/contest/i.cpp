#include <iomanip>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <set>

#ifdef DEBUG
#define debug(s) std::cerr << s << std::endl
#else
#define debug(s)
#endif

const double PI  =3.141592653589793238463;
const double EPS = 1e-10;

template<typename T> T sq(T val) {
    return val*val;
}


template<class T> struct GenericPoint {
    T x;
    T y;

    GenericPoint(T x = 0, T y = 0) : x(x), y(y) {
    }

    GenericPoint(const GenericPoint<long long>& p) : x(p.x), y(p.y) {
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

    friend T cross(const GenericPoint& a, const GenericPoint& b) {
        return a.x*b.y - a.y*b.x;
    }

    bool operator==(const GenericPoint& other) const {
        return x == other.x && y == other.y;
    }

    T norm() const {
        return sq(x) + sq(y);
    }
    
    double dir() const {
        double a =  atan2(y, x);
        return a < 0 ? a + PI*2 : a;
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

    friend std::ostream& operator<<(std::ostream& str, Circle& circle) {
        str << circle.center << ' ' << circle.r;
        return str;
    }

    friend bool operator==(const Circle& a, const Circle& b) {
        return a.center == b.center && a.r == b.r;
    }

    bool contains(const Circle& other) const {
        return sq_distance(center, other.center) < sq(r);
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

    friend bool operator<(const Circle& a, const Circle& b) {
        if (a.center.x == b.center.x) {
            if (a.center.y == b.center.y) {
                return a.r < b.r;
            }
            return a.center.y < b.center.y;
        }
        return a.center.x < b.center.x;
    }
};

long long vec(Point a, Point b, Point c) {
    Point A(b - a);
    Point B(c - b);
    return A.x*B.y - A.y*B.x;
}

double angle(Point a, Point b, Point c) {
    return vec(a, b, c) / distance(a, b) / distance(b, c);
}

struct LineF {
    double A, B, C;

    void normalize() {
        if (A < 0) {
            A = -A;
            B = -B;
            C = -C;
        }
        if (A < EPS && B < 0) {
            B = -B;
            C = -C;
        }
        double coef = sqrt(sq(A) + sq(B));
        A /= coef;
        B /= coef;
    }

    friend std::ostream& operator<<(std::ostream& str, const LineF& line) {
        str << line.A << ' ' << line.B << ' ' << line.C;
        return str;
    }
};

struct Caliper {
    const std::vector<Point>& poly;
    int point_index;
    double angle;
    short dir;

    Caliper(const std::vector<Point>& poly, int point_index, double angle, short dir) :
        poly(poly), point_index(point_index), angle(angle), dir(dir) {}

    int next() const {
        return (point_index + dir + poly.size()) % poly.size();
    }

    const Point& next_point() const {
        return poly[next()];
    }

    const Point& current_point() const {
        return poly[point_index];
    }

    double angle_to_next() const {
        if (next() == point_index) {
            return 2*PI;
        }
        Point direction(poly[next()] - poly[point_index]);
        return direction.dir();
    }

    double rotation_to_next() const {
        double a = angle_to_next() - angle;
        if (a > 2*PI) {
            a -= 2*PI;
        }
        if (a < 0) {
            a += 2*PI;
        }
        return a;
    }
    
    void rotate(double a) {
        double rot;
        while (a >= (rot = rotation_to_next())) {
            a -= rot;
            angle += rot;
            point_index = next();
        }
        angle += a;
        if (angle > 2*PI) {
            angle -= 2*PI;
        }
    }

    LineF line() const {
        PointF v = vec();
        const Point& p = current_point();
        LineF l{v.y, -v.x, -(v.y*p.x - v.x*p.y)};
        l.normalize();
        return l;
    }

    PointF vec() const {
        return PointF(cos(angle), sin(angle));
    }
};

const double INF = 1e10;

bool cmp (Point a, Point b) {
    return a.x < b.x || (a.x == b.x && a.y < b.y);
}

int main() {
#ifdef DEBUG
    std::istream& in = std::cin;
    std::ostream& out = std::cout;
#else
    std::ifstream in("gift.in");
    std::ofstream out("gift.out");
#endif
    out.setf(std::ios::fixed);
    out.precision(3);
    int counter = 1;
    int n;
    while (in >> n, n != 0) {
        std::vector<Point> poly(n);
        for (int i = 0; i < n; ++i) {
            in >> poly[i];
        }
        if (poly.size() > 1) {
            std::sort(poly.begin(), poly.end(), &cmp);
            Point p1 = poly[0],  p2 = poly.back();
            std::vector<Point> up, down;
            up.push_back(p1);
            down.push_back(p1);
            for (size_t i=1; i<poly.size(); ++i) {
                if (i==poly.size()-1 || vec(p1, poly[i], p2) < 0) {
                    while (up.size()>=2 && vec(up[up.size()-2], up[up.size()-1], poly[i]) >= 0)
                        up.pop_back();
                    up.push_back(poly[i]);
                }
                if (i==poly.size()-1 || vec(p1, poly[i], p2) > 0) {
                    while (down.size()>=2 && vec(down[down.size()-2], down[down.size()-1], poly[i]) <= 0)
                        down.pop_back();
                    down.push_back(poly[i]);
                }
            }
            poly.clear();
            for (size_t i=0; i<up.size(); ++i)
                poly.push_back(up[i]);
            for (size_t i=down.size()-2; i>0; --i)
                poly.push_back(down[i]);
        }
        debug("");
        for (Point& p : poly) {
            debug(p);
        }
        debug("");
        int lowest = std::min_element(poly.begin(), poly.end(), [](Point a, Point b) -> bool {return a.y < b.y;}) - poly.begin();
        std::vector<Caliper> calipers(4, Caliper(poly, lowest, 0, -1));
        for (int i = 0; i < 4; ++i) {
            calipers[i].rotate(PI/2 * i);
        }
        double min_area = INF;
        double max_area = 0;
        bool middle = false;
        for (int i = 0; i < 2*n + 5; ++i) {
            double ang = INF;
            if (!middle) {
                double dirs[2];
                double angles[2];
                for (int k = 0; k < 2; ++k) {
                    debug(k << '>' << (k + 2) << ' ' << (calipers[k + 2].current_point() - calipers[k].current_point()).dir()/PI*180);
                    dirs[k] = (calipers[k + 2].current_point() - calipers[k].current_point()).dir();
                    double dir = dirs[k] - calipers[k].angle;
                    if (dir < 0) {
                        dir += 2*PI;
                    }
                    angles[k] = dir;
                }
                if (fabs(dirs[0] - dirs[1]) < EPS || fabs(fabs(dirs[0] - dirs[1]) - PI) < EPS)  {
                    debug("CRITICAL");
                    ang = fabs(dirs[0] + PI/4 - calipers[0].angle);
                    debug(ang/PI*180);
                    while (ang > PI/2) {
                        ang -= PI/2;
                    }
                } else {
                    ang = (angles[0] + angles[1] + PI) / 2;
                    while (ang > PI) {
                        ang -= PI;
                    }
                }
            } else {
                debug("skip");
            }
            debug("maximizing ang: " << ang/PI*180);
            middle = true;
            for (int j = 0; j < 4; ++j) {
                double cur_ang = calipers[j].rotation_to_next();
                if (cur_ang < ang) {
                    middle = false;
                    ang = cur_ang;
                    debug(j << ' ' << ang/PI*180);
                }
            }
            debug("rotate ang: " << ang/PI*180);
            for (int j = 0; j < 4; ++j) {
                calipers[j].rotate(ang);
                debug(calipers[j].angle/PI*180 << ": " << calipers[j].point_index);
            }
            min_area = std::min(min_area,
                    fabs(calipers[0].line().C - calipers[2].line().C)*
                    fabs(calipers[1].line().C - calipers[3].line().C)
                    );
            double new_area = 
                    fabs(calipers[0].line().C - calipers[2].line().C)*
                    fabs(calipers[1].line().C - calipers[3].line().C);
            if (new_area > max_area) {
                debug("maxed out: " << new_area);
                max_area = new_area;
                debug(calipers[0].angle/PI*180);
            }
            debug(max_area << std::endl);
        }
        out << "Gift " << counter << std::endl;
        out << "Minimum area = " << min_area << std::endl;
        out << "Maximum area = " << max_area << std::endl << std::endl;
        ++counter;
    }
}
