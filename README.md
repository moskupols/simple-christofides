Simple christofides
===================

As we know, the most difficult part of Christofides heuristic for TSP is
commonly considered to be the problem of finding the minimum weighted
perfect matching in a euclidean graph, as the algo finding exactly that is
rather complex.

But we can use some heuristic for that subproblem too! This repo was created for
a quick benchmark of two simple greedy approaches.

The 'straightforward' variant
-----------------------------

Is contained in the branch `master`. Nobody seems to know how to prove its
effectiveness.

The 'log-approximation' variant
-------------------------------

Is contained in the branch `log-approximation`. Based on the
[paper](http://dcg.ethz.ch/publications/ctw04.pdf) by Mirjam Wattenhofer and
Roger Wattenhofer. Slightly more complex. Finds slightly worse answers on my tests.
There is a simple proof that the length of a tour found by this algo is not
greater than T log_3 N, where T is the length of the optimal tour, and N is the
number of vertices in the given graph.

Tests
-----

All the tests are National TSP, taken from the
[site](http://www.math.uwaterloo.ca/tsp/world/countries.html)
by University of Waterloo.
