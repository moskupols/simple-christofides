#include <bits/stdc++.h>

#include "graph.hxx"
#include "dsu.hxx"

using namespace std;

vector<Edge> Graph::findMst() const
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

vector<vector<Edge>> Graph::getAdjList() const
{
    vector<vector<Edge>> ret(n);
    for (Edge e : edgeList)
    {
        ret[e.from].push_back(e);
        ret[e.to].push_back(Edge{e.id, e.to, e.from, e.w});
    }
    return ret;
}

vector<int> Graph::buildEulerianTourWithShortcuts(int start, bool doubleEdges) const
{
    vector<int> tour = buildEulerianTour(start, doubleEdges);
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
        vector<vector<Edge>> &adjList,
        unordered_set<int> &edgeDeleted,
        bool doubleEdges)
{
    while (!adjList[v].empty())
    {
        Edge e = adjList[v].back();
        adjList[v].pop_back();
        if (doubleEdges || edgeDeleted.insert(e.id).second)
            buildEulerianTourRecursive(e.to, ret, adjList, edgeDeleted, doubleEdges);
    }
    ret.push_back(v);
}
}

vector<int> Graph::buildEulerianTour(int start, bool doubleEdges) const
{
    vector<int> ret;
    vector<vector<Edge>> adjList = getAdjList();
    unordered_set<int> edgeDeleted;
    buildEulerianTourRecursive(start, ret, adjList, edgeDeleted, doubleEdges);
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
    return {min(a,b) * (int)p.size() + max(a,b), a, b, Point(p[a], p[b]).radius()};
}

namespace
{

vector<Edge> findOptimalMatchingOnTree(const EuclideanGraph &eg, const Graph &tree)
{
    vector<int> tour = tree
        .buildEulerianTourWithShortcuts(tree.edgeList.back().from, true);
    assert(tour.size() % 2 == 0);
    pair<double, vector<Edge>> ms[2];
    for (int shift = 0; shift < 2; ++shift)
    {
        for (size_t i = shift; i < tour.size(); i += 2)
        {
            Edge e = eg.getEdge(tour[i], tour[(i+1)%tour.size()]);
            ms[shift].first += e.w;
            ms[shift].second.push_back(e);
        }
    }
    return (ms[0].first < ms[1].first ? ms[0] : ms[1]).second;
}

void relax(unordered_map<int, pair<double, int>> &mp, int a, pair<double, int> wb)
{
    auto it = mp.insert({a, wb});
    if (!it.second)
        it.first->second = min(it.first->second, wb);
}

}

// see http://dcg.ethz.ch/publications/ctw04.pdf
vector<Edge> EuclideanGraph::findOptimalMatchingApprox(
        unordered_set<int> ignoredVertices) const
{
    vector<Edge> sortedEdges = edgeList;
    sortedEdges.erase(remove_if(sortedEdges.begin(), sortedEdges.end(),
            [&ignoredVertices](Edge e)
            {
                return ignoredVertices.count(e.to) | ignoredVertices.count(e.from);
            }),
        sortedEdges.end());
    sort(sortedEdges.begin(), sortedEdges.end(),
            [](Edge a, Edge b) {return a.w < b.w; });

    DisjointSetUnion dsu(n);
    vector<Edge> markedEdges;

    bool needed = true;
    while (needed)
    {
        needed = false;

        unordered_map<int, pair<double, int>> nearest;
        for (Edge e : sortedEdges)
        {
            int a = dsu.getRepresentative(e.from), b = dsu.getRepresentative(e.to);
            if (a == b || (dsu.getSetSize(a) & dsu.getSetSize(b) & 1) == 0)
                continue;

            relax(nearest, a, make_pair(e.w, b));
            relax(nearest, b, make_pair(e.w, a));
            needed = true;
        }
        for (pair<int, pair<double, int>> p : nearest)
        {
            int a = p.first;
            int b = p.second.second;
            if (dsu.join(a, b))
                markedEdges.push_back(getEdge(a, b));
        }
    }

    vector<vector<Edge>> trees(n);
    for (Edge e : markedEdges)
        trees[dsu.getRepresentative(e.from)].push_back(e);

    vector<Edge> ret;
    for (auto& tree : trees)
        if (!tree.empty())
        {
            vector<Edge> submatch = findOptimalMatchingOnTree(
                    *this,
                    Graph(n, move(tree)));
            copy(submatch.begin(), submatch.end(), back_inserter(ret));
        }

    return ret;
}

