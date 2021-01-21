cmake_minimum_required(VERSION 3.16)
project(Library)

set(CMAKE_CXX_STANDARD 20)

#combinatorics
add_executable(Choose Combinatorics/Choose.cpp)

#custom datatypes
add_executable(Signed128 "Custom Datatypes/Signed128.cpp")
add_executable(Tensor "Custom Datatypes/Tensor.cpp")
add_executable(Triple "Custom Datatypes/Triple.cpp")
add_executable(Unsigned128 "Custom Datatypes/Unsigned128.cpp")

#data structures
add_executable(FenwickTree "Data Structures/FenwickTree.cpp")
add_executable(FenwickTree2D "Data Structures/FenwickTree2D.cpp")
add_executable(SegmentTree "Data Structures/SegmentTree.cpp")
add_executable(SegmentTreeI "Data Structures/SegmentTreeI.cpp")
add_executable(SegmentTreeL "Data Structures/SegmentTreeL.cpp")
add_executable(SegmentTreeM "Data Structures/SegmentTreeM.cpp")
add_executable(SegmentTreeP "Data Structures/SegmentTreeP.cpp")
add_executable(SegmentTreePL "Data Structures/SegmentTreePL.cpp")
add_executable(SparseTable2D "Data Structures/SparseTable2D.cpp")
add_executable(Treap "Data Structures/Treap.cpp")
add_executable(UnionFindRB "Data Structures/UnionFindRB.cpp")
add_executable(UnionFindSL "Data Structures/UnionFindSL.cpp")

#flows
add_executable(Dinic Flows/Dinic.cpp)
add_executable(EdmondsKarp Flows/EdmondsKarp.cpp)
add_executable(HungarianAlgorithm Flows/HungarianAlgorithm.cpp)
add_executable(Matching Flows/Matching.cpp)
add_executable(MinCostFlow Flows/MinCostFlow.cpp)
add_executable(PushRelabel Flows/PushRelabel.cpp)

#geometry
add_executable(ClosestPairOfPoints Geometry/ClosestPairOfPoints.cpp)
add_executable(ConvexHull Geometry/ConvexHull.cpp)
add_executable(Polygon Geometry/Polygon.cpp)

#graphs
add_executable(2SAT Graphs/2SAT.cpp)
add_executable(BellmanFord Graphs/BellmanFord.cpp)
add_executable(Diameter Graphs/Diameter.cpp)
add_executable(Dijkstra Graphs/Dijkstra.cpp)
add_executable(Eulerian Graphs/Eulerian.cpp)
add_executable(FloydWarshall Graphs/FloydWarshall.cpp)
add_executable(HeavyLight Graphs/HeavyLight.cpp)
add_executable(Kruskal Graphs/Kruskal.cpp)
add_executable(LCA Graphs/LCA.cpp)
add_executable(PathQuery Graphs/PathQuery.cpp)
add_executable(SCC Graphs/SCC.cpp)
add_executable(TopSort Graphs/TopSort.cpp)

#miscellaneous
add_executable(CHT Miscellaneous/CHT.cpp)
add_executable(Misc Miscellaneous/Misc.cpp)

#number theory
add_executable(ExtendedEuclidean "Number Theory/ExtendedEuclidean.cpp")
add_executable(Factorisation "Number Theory/Factorisation.cpp")
add_executable(ModularArithmetic "Number Theory/ModularArithmetic.cpp")
add_executable(Sieve "Number Theory/Sieve.cpp")

#numeric
add_executable(DynamicMint Numeric/DynamicMint.cpp)
add_executable(FFT Numeric/FFT.cpp)
add_executable(GaussianElimination Numeric/GaussianElimination.cpp)
add_executable(NTT Numeric/NTT.cpp)
add_executable(Polynomial Numeric/Polynomial.cpp)

#strings
add_executable(AhoCorasick Strings/AhoCorasick.cpp)
add_executable(Manacher Strings/Manacher.cpp)
add_executable(PatternMatching Strings/PatternMatching.cpp)
add_executable(StringHashing Strings/StringHashing.cpp)
add_executable(SuffixArray Strings/SuffixArray.cpp)
add_executable(SuffixAutomaton Strings/SuffixAutomaton.cpp)
add_executable(SuffixTrie Strings/SuffixTrie.cpp)
