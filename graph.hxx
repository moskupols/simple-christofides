#ifndef GRAPH_HXX
#define GRAPH_HXX

#include <vector>
#include <unordered_set>
#include <cmath>

struct Point
{
    double x, y;
    Point() {}
    Point(double x, double y): x(x), y(y) {}
    Point(Point a, Point b): x(b.x-a.x), y(b.y-a.y) {}

    double radius() const { return sqrt(x*x + y*y); }
};

struct Edge
{
    int id, from, to;
    double w;
};

struct Graph
{
    explicit Graph(int n):
        n(n)
    {}

    explicit Graph(int n, std::vector<Edge> edgeList):
        n(n),
        edgeList(edgeList)
    {}

    void addEdge(Edge e)
    {
        edgeList.push_back(e);
    }

    std::vector<Edge> findMst() const;


    std::vector<std::vector<Edge>> getAdjList() const;

    std::vector<int> buildEulerianTourWithShortcuts(
            int start = 0, bool doubleEdges = false) const;
    std::vector<int> buildEulerianTour(
            int start = 0, bool doubleEdges = false) const;

    int n;
    std::vector<Edge> edgeList;

private:
};

struct EuclideanGraph : public Graph
{
    explicit EuclideanGraph(std::vector<Point> p);

    Edge getEdge(int a, int b) const;

    std::vector<Edge> findOptimalMatchingApprox(
            std::unordered_set<int> ignoredVertices = {}) const;

    std::vector<Point> p;
};

#endif

