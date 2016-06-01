template <typename T>
class Prim {
public:
	static UndirectedGraph<T> mst(const UndirectedGraph<T>& graph);
};

static UndirectedGraph<T> Prim<T>::mst(const UndirectedGraph<T>& graph) {
	FibonacciHeap<const T&> pq;
	std::unordered_map<const T&, FibonacciHeap<const T&>::Entry> seen;
	UndirectedGraph<T> result;

	if (graph.isEmpty()) return result;

	const T& startNode = graph.begin()->first;
	result.addNode(startNode);
	exploreNode(startNode, graph, pq, result, seen);

	size_t numEdgesToAdd = graph.size() - 1;
	for (size_t i = 0; numEdgesToAdd; ++i) {
		const T& cheapestNode = pq.extractMin().getValue();
		const T& connection = findConnection(cheapestNode, graph, result);
		double cost = graph.edgeCost(cheapestNode, connection);

		result.addNode(cheapestNode);
		result.addEdge(cheapestNode, connection, cost);

		exploreNode(cheapestNode, graph, pq, result, seen);
	}

	return result;
}

static void exploreNode(
		const T& node,
		const UndirectedGraph<T>& graph,
		FibonacciHeap<const T&>,
		UndirectedGraph<T>& result,
		std::unordered_map<const T&, FibonacciHeap<const T&>::Entry> seen) {
	for (const auto& edge : graph.edgesFrom(node)) {
		const T& endpoint = edge.first;
		double weight = edge.second;

		if (result.containsNode(endpoint)) continue;

		auto found = seen.find(endpoint);
		if (found == seen.end()) {
			Entry& entry = pq.enqueue(endpoint, weight);
			seen[endpoint] = entry;
		} else {
			Entry& entry = found->second;
			if (weight < entry.getPriority()) {
				pq.decreaseKey(entry, weight);
			}
		}
	}
}

static const T& findConnection(const T& node, const UndirectedGraph<T>& graph,
															 const UndirectedGraph<T>& result) {
	auto begin = graph.edgesFrom(node).begin();
	auto end = graph.edgesFrom(node).end();
	T& minEndpoint = begin->first;
	double minCost = begin->second;
	for (auto it = begin + 1; it != end; ++it) {
		const T& endpoint = it->first;
		double cost = it->second;
		if (!result.containsNode(endpoint)) continue;

		if (it->second < minCost) {
			minEndpoint = endpoint;
			minCost = cost;
		}
	}

	return endpoint;
}
