template <typename T>
class UndirectedGraph {
public:
	UndirectedGraph();
	~UndirectedGraph();

	inline size_t size() const;
	inline bool isEmpty() const;
	inline bool containsNode(const T& value) const;

	inline void edgeCost(const T& first, const T& second) const;
	inline const std::unordered_map<const T&, double>& edgesFrom(const T& value)
			const;

	void addNode(const T& value); // can consider making this return a bool
	void addEdge(const T& first, const T& second, double weight);
	void removeEdge(const T& first, const T& second);

	typedef std::unordered_map<const T&, double>::iterator iterator;
	typedef std::unordered_map<const T&, double>::const_iterator const_iterator;

	inline iterator begin();
	inline iterator end();
	inline const_iterator begin() const;
	inline const_iterator end() const;
	inline const_iterator cbegin() const;
	inline const_iterator cend() const;

private:
	std::unordered_map<const T&, std::unordered_map<const T&, double>> mGraph;
};

template <typename T>
UndirectedGraph<T>::UndirectedGraph() {
	// Does nothing.
}

template <typename T>
UndirectedGraph<T>::~UndirectedGraph() {
	// Does nothing.
}

template <typename T>
inline size_t UndirectedGraph<T>::size() const {
	return mGraph.size();
}

template <typename T>
inline bool UndirectedGraph<T>::isEmpty() const {
	return mGraph.empty();
}

template <typename T>
inline bool UndirectedGraph<T>::containsNode(const T& value) const {
	return mGraph.find(value) != mGraph.end();
}

template <typename T>
inline void UndirectedGraph<T>::edgeCost(const T& first,
																				 const T& second) const {
	// maybe include checks?
	return mGraph[first][second];
}

template <typename T>
inline const std::unordered_map<const T&, double>&
UndirectedGraph<T>::edgesFrom(const T& value) const {
	return mGraph[value];
}

template <typename T>
void UndirectedGraph<T>::addNode(const T& value) {
	mGraph[value];
}

template <typename T>
void UndirectedGraph<T>::addEdge(const T& first, const T& second,
																 double weight) {
	mGraph[first][second] = weight;
	mGraph[second][first] = weight;
}

template <typename T>
void UndirectedGraph<T>::removeEdge(const T& first, const T& second) {
	mGraph[first].erase(second);
	mGraph[second].erase(first);
}

template <typename T>
void UndirectedGraph<T>::iterator begin() {
	return mGraph.begin();
}

template <typename T>
void UndirectedGraph<T>::iterator end() {
	return mGraph.end();
}

void UndirectedGraph<T>::const_iterator begin() const {
	return mGraph.begin();
}

template <typename T>
void UndirectedGraph<T>::const_iterator end() const {
	return mGraph.end();
}

template <typename T>
void UndirectedGraph<T>::iterator cbegin() {
	return mGraph.cbegin();
}

template <typename T>
void UndirectedGraph<T>::iterator cend() {
	return mGraph.cend();
}
