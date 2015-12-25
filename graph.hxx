#ifndef GRAPH_HXX
#define GRAPH_HXX

#include "dsu.hxx"
using namespace std;

struct Point
{
    double x, y;
    Point() {}
    Point(double x, double y): x(x), y(y) {}
    Point(Point a, Point b): x(b.x-a.x), y(b.y-a.y) {}

    double radius() const
    {
        return sqrt(x*x + y*y);
    }
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

    explicit Graph(int n, vector<Edge> edgeList):
        Graph::Graph(n)
    {
        this->edgeList.reserve(edgeList.size());
        for (Edge e : edgeList)
            addEdge(e);
    }

    void addEdge(Edge e)
    {
        edgeList.push_back(e);
    }

    vector<Edge> findMst() const
    {
        vector<Edge> sortedEdges = edgeList;
        sort(sortedEdges.begin(), sortedEdges.end(),
                [](Edge a, Edge b) {return a.w < b.w; });

        vector<Edge> ret;
        ret.reserve(n-1);

        DisjointSetUnion dsu(n);
        for (Edge e : sortedEdges)
            if (dsu.join(e.from, e.to))
                ret.push_back(e);

        return ret;
    }

    vector<Edge> findOptimalMatchingApprox(std::unordered_set<int> ignoredVertices = {}) const
    {
        vector<Edge> sortedEdges = edgeList;
        sort(sortedEdges.begin(), sortedEdges.end(),
                [](Edge a, Edge b) {return a.w < b.w; });

        vector<Edge> ret;
        for (Edge e : sortedEdges)
            if (ignoredVertices.count(e.from) == 0 && ignoredVertices.count(e.to) == 0)
            {
                ignoredVertices.insert(e.from);
                ignoredVertices.insert(e.to);
                ret.push_back(e);
            }
        return ret;
    }

    vector<vector<Edge>> getAdjList() const
    {
        vector<vector<Edge>> ret(n);
        for (Edge e : edgeList)
        {
            ret[e.from].push_back(e);
            ret[e.to].push_back(Edge{e.id, e.to, e.from, e.w});
        }
        return ret;
    }

    vector<int> buildEulerianTourWithShortcuts(int start = 0)
    {
        vector<int> tour = buildEulerianTour(start);
        vector<int> ret;
        unordered_set<int> met;
        for (int x : tour)
            if (met.insert(x).second)
                ret.push_back(x);
        return ret;
    }

    vector<int> buildEulerianTour(int start = 0)
    {
        vector<int> ret;
        vector<vector<Edge>> adjList = getAdjList();
        unordered_set<int> edgeDeleted;
        buildEulerianTourRecursive(start, ret, adjList, edgeDeleted);
        return ret;
    }

    int n;
    vector<Edge> edgeList;

private:
    static void buildEulerianTourRecursive(int v, vector<int> &ret,
            vector<vector<Edge>> &adjList, unordered_set<int> &edgeDeleted)
    {
        while (!adjList[v].empty())
        {
            Edge e = adjList[v].back();
            adjList[v].pop_back();
            if (edgeDeleted.insert(e.id).second)
                buildEulerianTourRecursive(e.to, ret, adjList, edgeDeleted);
        }
        ret.push_back(v);
    }
};

struct EuclideanGraph : public Graph
{
    explicit EuclideanGraph(vector<Point> p):
        Graph(p.size(), buildEdges(p)),
        p(p)
    {}

    Edge getEdge(int a, int b) const
    {
        return {a * (int)p.size() + b, a, b, Point(p[a], p[b]).radius()};
    }

    vector<Point> p;

protected:
    static Edge buildEdge(const vector<Point> &p, int a, int b)
    {
        return {a * (int)p.size() + b, a, b, Point(p[a], p[b]).radius()};
    }

    static vector<Edge> buildEdges(const vector<Point> &p)
    {
        vector<Edge> ret;
        for (size_t i = 0; i < p.size(); ++i)
            for (size_t j = i + 1; j < p.size(); ++j)
                ret.push_back(buildEdge(p, i, j));
        return ret;
    }
};

#endif

