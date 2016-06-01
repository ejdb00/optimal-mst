#ifndef DisjointSetForest_Included
#define DisjointSetForest_Included

#include "UndirectedGraph.hh"
#include "DisjointSet.hh"

#include <unordered_map>
#include <limits>

template <typename T>
class DisjointSetForest {
public:
	DisjointSetForest();
  //TODO: implement this
  //DisjointSetForest(UndirectedGraph<T>& graph);
	~DisjointSetForest();

	inline size_t size() const;
	inline bool isEmpty() const;
	inline bool containsNode(const T& value) const;

	DisjointSet<T>& addNode(const T& value); // can consider making this return a bool

	void addEdge(DisjointSet<T>& first, DisjointSet<T>& second, double weight);
	void removeEdge(DisjointSet<T>& first, DisjointSet<T>& second);

	inline double edgeCost(DisjointSet<T>& first, DisjointSet<T>& second) const;
	inline const std::unordered_map<DisjointSet<T>&, double>& 
    edgesFrom(DisjointSet<T>& set) const;

	const T& contractEdge(DisjointSet<T>& first, DisjointSet<T>& second);

  const bool sameSuperNode(const T& first, const T& second);
  inline DisjointSet<T>& superNodeOf(const T& value);

	typedef std::unordered_map<DisjointSet<T>&, double>::iterator iterator;
	typedef std::unordered_map<DisjointSet<T>&, double>::const_iterator const_iterator;

	inline iterator begin();
	inline iterator end();
	inline const_iterator begin() const;
	inline const_iterator end() const;
	inline const_iterator cbegin() const;
	inline const_iterator cend() const;

private:
  std::unordered_map<const T&, DisjointSet<T>&> nodes;
	std::unordered_map<DisjointSet<T>&, std::unordered_map<DisjointSet<T>&, double>> mForest;
};

template <typename T>
DisjointSetForest<T>::DisjointSetForest() {
	// Does nothing.
}

/*
template <typename T>
DisjointSetForest<T>::DisjointSetForest(UndirectedGraph<T>& graph) {
  TODO: implement
}
*/

template <typename T>
DisjointSetForest<T>::~DisjointSetForest() {
	// Does nothing.
}

template <typename T>
inline size_t DisjointSetForest<T>::size() const {
	return mForest.size();
}

template <typename T>
inline bool DisjointSetForest<T>::isEmpty() const {
	return mForest.empty();
}

template <typename T>
inline bool DisjointSetForest<T>::containsNode(const T& value) const {
	return mForest.find(nodes[value]) != mForest.end();
}

template <typename T>
inline double DisjointSetForest<T>::edgeCost(DisjointSet<T>& first,
																				 DistjointSet<T>& second) const {
	// maybe include checks?
  if (mForest.find(first) == mForest.end() ||
      mForest[first].find(second) == mForest[first].end()) {
    return std::numeric_limits<double>::quiet_NaN();
  }
	return mForest[first][second];
}

template <typename T>
inline const std::unordered_map<DisjointSet<T>&, double>&
DisjointSetForest<T>::edgesFrom(DisjointSet<T>& value) const {
	return mForest[value];
}

template <typename T>
inline DisjointSet<T>& DisjointSetForest<T>::superNodeOf(const T& value) {
  return nodes[value];
}

template <typename T>
DisjointSet<T>& DisjointSetForest<T>::addNode(const T& value) {
  nodes[value] = new DisjointSet<T>(value);
	mForest[nodes[value]];
  return nodes[value];
}

template <typename T>
void DisjointSetForest<T>::addEdge(DisjointSet<T>& first, 
    DisjointSet<T>& second, double weight) {
  double cost = edgeCost(first, second);
  if (!std::isnan(cost) && weight > cost) return;
	mForest[first.find()][second.find()] = weight;
	mForest[second.find()][first.find()] = weight;
}

template <typename T>
void DisjointSetForest<T>::removeEdge(DisjointSet<T>& first, 
    DisjointSet<T>& second) {
  if (first.find() == second.find()) return;
	mForest[first.find()].erase(second.find());
	mForest[second.find()].erase(first.find());
}

// THESE MUST BE VALID REPRESENTATIVES!
// We also assume simple graph (no self loops or multi-edges)
template <typename T>
DisjointSet<T>& DisjointSetForest<T>::contractEdge(DisjointSet& first, 
    DisjointSet<T>& second) {
  if (first.find() == second.find()) return;
	removeEdge(first, second);
  const std::unordered_map<DisjointSet<T>&, double>& fromFirst;
  const std::unordered_map<DisjointSet<T>&, double>& fromSecond;
  std::unordered_map<DisjointSet<T>&, double> fromNew();
  for (iterator it = fromFirst.begin(); it != fromFirst.end(); ++it) {
    fromNew[it->first] = it->second;
    removeEdge(first, it->first);
  }
  for (iterator it = fromSecond.begin(); it != fromSecond.end(); ++it) {
    if (fromNew.find(it->first) == fromNew.end() && 
        fromNew.find(it->first)->second > it->second) {
      fromNew[it->first] = it->second;
    }
    removeEdge(second, it->first);
  }
  DisjointSet<T>& super = unionSets(first, second);
	for (iterator it = fromNew.begin(); it != fromNew.end(); ++it) {
    addEdge(super, it->first, it->second);
  }
}

template <typename T>
const bool DisjointSetForest<T>::sameSuperNode(const T& first, const T& second) {
  return nodes[first].find() == nodes[second].find()
}

template <typename T>
void DisjointSetForest<T>::iterator begin() {
	return mForest.begin();
}

template <typename T>
void DisjointSetForest<T>::iterator end() {
	return mForest.end();
}

void DisjointSetForest<T>::const_iterator begin() const {
	return mForest.begin();
}

template <typename T>
void DisjointSetForest<T>::const_iterator end() const {
	return mForest.end();
}

template <typename T>
void DisjointSetForest<T>::iterator cbegin() {
	return mForest.cbegin();
}

template <typename T>
void DisjointSetForest<T>::iterator cend() {
	return mForest.cend();
}

#endif
