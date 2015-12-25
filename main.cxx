#include <bits/stdc++.h>

#include "graph.hxx"

using namespace std;


vector<Edge> buildEulerianTour(const Graph &g, int start = 0)
{
    vector<vector<Edge>> adjList = g.getAdjList();
    unordered_set<int> deletedEdges;
    vector<Edge> ret;
    int v = start;
    while (adjList[v].size())
    {
        Edge e = adjList[v].back();
        adjList[v].pop_back();
        if (!deletedEdges.count(e.id))
            ret.push_back(e);
        deletedEdges.insert(e.id);
        v = e.to;
    }
    assert(ret.size() == g.edgeList.size());
    return ret;
}

vector<int> findEuclideanTspTour(const EuclideanGraph &g, int start = 0)
{
    Graph eulerian(g.n, g.findMst());
    {
        std::unordered_set<int> evenDegree;
        {
            auto mstAdjList = eulerian.getAdjList();
            for (int i = 0; i < eulerian.n; ++i)
                if (mstAdjList[i].size() % 2 == 0)
                    evenDegree.insert(i);
        }

        {
            vector<Edge> matching = g.findOptimalMatchingApprox(evenDegree);
            assert(matching.size() * 2 + evenDegree.size() == (size_t)g.n);

            for (Edge e : matching)
            {
                e.id += g.n * g.n;
                eulerian.addEdge(e);
            }
        }
    }

    vector<int> ret(1, start);
    {
        vector<Edge> eulerianTour = buildEulerianTour(eulerian, start);
        unordered_set<int> usedVertices;
        usedVertices.insert(start);
        for (Edge e : eulerianTour)
            if (!usedVertices.count(e.to))
            {
                ret.push_back(e.to);
                usedVertices.insert(e.to);
            }
    }
    return ret;
}

int main()
{
    int n;
    cin >> n;
    vector<Point> p(n);
    for (int i = 0; i < n; ++i)
        cin >> p[i].x >> p[i].y;

    EuclideanGraph g(p);
    vector<int> tour = findEuclideanTspTour(g);
    for (int x : tour)
        cout << x << ' ';
    cout << endl;
}

