#include <bits/stdc++.h>

#include "graph.hxx"

using namespace std;

vector<int> findEuclideanTspTour(const EuclideanGraph &g)
{
    Graph eulerian(g.n, g.findMst());
    // eulerian.addEdge({-1, s, t, 0}); // fixed endpoints case
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

    return eulerian.buildEulerianTourWithShortcuts();
}

double tourLength(const vector<int> &p, const EuclideanGraph &g)
{
    double ret = 0;
    for (size_t i = 0; i < p.size(); ++i)
        ret += g.getEdge(p[i], p[(i+1)%p.size()]).w;
    return ret;
}

int main()
{
    int n;
    double optimal;
    cin >> n >> optimal;
    vector<Point> p(n);
    for (int i = 0; i < n; ++i)
        cin >> p[i].x >> p[i].y;

    EuclideanGraph g(p);
    vector<int> tour = findEuclideanTspTour(g);

    if (tour.size() < 50)
    {
        for (int x : tour)
            cout << x << ' ';
        cout << endl;
    }

    double len = tourLength(tour, g);
    cout << "Tour length: " << len << endl;
    cout << "Best known:  " << optimal << endl;
    cout << "ratio:       " << len / optimal << endl;
}

