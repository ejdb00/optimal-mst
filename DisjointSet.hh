#ifndef DisjointSet_Included
#define DisjointSet_Included

template <typename T>
class DisjointSet {
public:
  DisjointSet(const T& value);
  ~DisjointSet();

  inline T& getValue() const;
  inline void setParent(const DisjointSet<T> newParent);
  inline size_t getRank() const;
  inline void increaseRank();

  DisjointSet<T>& find();
  static DisjointSet<T>& unionSets(DisjointSet<T>& one, DisjointSet<T>& two);
  
private:
  DisjointSet& mParent;
  size_t mRank;
  T mValue;

}

template <typename T>
DisjointSet<T>::DisjointSet(const T& value) : 
  mParent(this), mRank(0), mValue(value) {
  // handled in initializer list
}

template <typename T>
DisjointSet<T>::~DisjointSet() {
  // do nothing
}

template <typename T>
inline T& getValue() const {
  return mValue;
}

template <typename T>
inline void setParent(const DisjointSet<T> newParent) {
  mParent = newParent;
}

template <typename T>
inline size_t getRank() const {
  return mRank;
}

template <typename T>
inline void increaseRank() {
  mRank++;
}

template <typename T>
DisjointSet<T>& DisjointSet<T>::find() {
  if (mParent != this) {
    mParent = find();
  }
  return mParent;
}

template <typename T>
static DisjointSet<T>& DisjointSet<T>::unionSets(DisjointSet<T>& one, 
                                                 DisjointSet<T>& two) {
  DisjointSet<T>& rootOne = one.find();
  DisjointSet<T>& rootTwo = two.find();
  if (rootOne == rootTwo) {
    return rootOne;
  }
  if (rootOne.getRank() < rootTwo.getRank()) {
    rootOne.setParent(rootTwo);
    return rootTwo;
  } else if (rootOne.getRank() > rootTwo.getRank()) {
    rootTwo.setParent(rootOne);
    return rootOne;
  } else {
    rootTwo.setParent(rootOne);
    rootOne.increaseRank();
    return rootOne;
  }
}

#endif
