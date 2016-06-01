#include <bits/stdc++.h>

#include "graph.hxx"
#include "dsu.hxx"

using namespace std;

vector<Edge> Graph::findMst() const
{
    vector<Edge> sortedEdges = edgeList;
    sort(sortedEdges.begin(), sortedEdges.end(),
            [](const Edge &a, const Edge &b) {return a.w < b.w; });

    vector<Edge> ret;
    ret.reserve(n-1);

    DisjointSetUnion dsu(n);
    for (auto&& e : sortedEdges)
        if (dsu.join(e.from, e.to))
            ret.push_back(e);

    return ret;
}

vector<Edge> Graph::findOptimalMatchingApprox(
        unordered_set<int> ignoredVertices) const
{
    vector<Edge> sortedEdges = edgeList;
    sort(sortedEdges.begin(), sortedEdges.end(),
            [](const Edge &a, const Edge &b) {return a.w < b.w; });

    vector<Edge> ret;
    for (auto&& e : sortedEdges)
        if (ignoredVertices.count(e.from) == 0 && ignoredVertices.count(e.to) == 0)
        {
            ignoredVertices.insert(e.from);
            ignoredVertices.insert(e.to);
            ret.push_back(e);
        }
    return ret;
}

vector<vector<Edge>> Graph::getAdjList() const
{
    vector<vector<Edge>> ret(n);
    for (auto&& e : edgeList)
    {
        ret[e.from].push_back(e);
        ret[e.to].push_back(Edge{e.id, e.to, e.from, e.w});
    }
    return ret;
}

vector<int> Graph::buildEulerianTourWithShortcuts(int start)
{
    vector<int> tour = buildEulerianTour(start);
    vector<int> ret;
    unordered_set<int> met;
    for (int x : tour)
        if (met.insert(x).second)
            ret.push_back(x);
    return ret;
}

namespace
{
void buildEulerianTourRecursive(int v, vector<int> &ret,
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
}

vector<int> Graph::buildEulerianTour(int start)
{
    vector<int> ret;
    vector<vector<Edge>> adjList = getAdjList();
    unordered_set<int> edgeDeleted;
    buildEulerianTourRecursive(start, ret, adjList, edgeDeleted);
    return ret;
}

namespace
{
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
}

EuclideanGraph::EuclideanGraph(vector<Point> p):
    Graph(p.size(), buildEdges(p)),
    p(p)
{}

Edge EuclideanGraph::getEdge(int a, int b) const
{
    return {a * (int)p.size() + b, a, b, Point(p[a], p[b]).radius()};
}

