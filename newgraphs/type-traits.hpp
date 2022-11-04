#pragma once

#include "edge.hpp"
#include "graph.hpp"
#include "directed-graph.hpp"

namespace graphs {

template <typename Graph, typename = void>
struct is_weighted : std::false_type {};

template <typename Graph>
struct is_weighted<Graph, std::void_t<weight_t<edge_t<Graph>>>> : std::true_type {};

template <typename Graph>
constexpr bool is_weighted_v = is_weighted<Graph>::value;

template <typename Graph>
struct is_directed : std::false_type {};

template <typename Edge>
struct is_directed<directed_graph<Edge>> : std::true_type {};

template <typename Graph>
constexpr bool is_directed_v = is_directed<Graph>::value;

}
