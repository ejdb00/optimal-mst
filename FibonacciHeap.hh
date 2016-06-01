/* 
 * Fibonacci Heap
 * 
 * Big thanks to Keith for his Java implementation of this structure.
 * While we couldn't use it directly (and I wanted to implement the structure)
 * it was used as a guide to implementing and debugging particularly tricky
 * operations and therefore this implementation retains a very similar 
 * structure to Keith's in a number of places.
 */

#ifndef FibonacciHeap_Included
#define FibonacciHeap_Included

#include <cstddef>
#include <vector>
#include <iostream>

template <typename T>
class FibonacciHeap {
  public:
    class Entry {
      public:
        Entry(const T& value, double priority);
        ~Entry();

        inline const T& getValue() const;
        inline void setValue(const T& newValue);
        inline double getPriority() const;
        inline void setPriority(const double newPriority);
        inline bool isMarked() const;
        inline void mark();
        inline void unmark();
        inline int getDegree() const;
        inline void increaseDegree();
        inline void decreaseDegree();

        Entry *mParent;
        Entry *mChild;
        Entry *mNext;
        Entry *mPrev;

      private:
        bool marked;
        int mDegree;

        T mValue;
        double mPriority;

        Entry(Entry const &) = delete;
        void operator=(Entry const &) = delete;
    };

    FibonacciHeap(); 
    ~FibonacciHeap();

    inline size_t size() const;
    inline bool isEmpty() const;
    inline size_t getSize() const;
    inline void setSize(const size_t newSize);
    inline void setMin(Entry *newMin);
    inline Entry& findMin() const; 

    Entry& enqueue(const T& value, double priority);
    // object returned MUST BE FREED after use
    Entry& extractMin();
    void decreaseKey(Entry& entry, double newPriority);

    // object returned MUST BE FREED after use
    static FibonacciHeap<T>& meld(FibonacciHeap<T>& first,
        FibonacciHeap<T>& second);

  private:
    Entry *mMin;
    size_t mSize;

    void cutNode(Entry &entry);
    static void mergeLists(Entry *one, Entry *two);

    FibonacciHeap(FibonacciHeap const &) = delete;
    void operator=(FibonacciHeap const &) = delete;
};

/* Entry constructor */
template <typename T>
FibonacciHeap<T>::Entry::Entry(const T& value, double priority) :
  mValue(value), mPriority(priority), mParent(NULL), mChild(NULL), 
  mNext(this), mPrev(this), marked(false), mDegree(0) {
    // Handled in initializer list.
  }

// Entry destructor, recursively deletes all children and iteratively
// and iteratively deletes all siblings. If you don't want to destroy
// the siblings, make sure this node has no siblings before deleting it.
template <typename T>
FibonacciHeap<T>::Entry::~Entry() {
  if (mChild) {
    delete mChild;
  }
  while (mNext != this) {
    Entry *next = mNext;
    mNext = mNext->mNext;
    next->mNext = next;
    delete next;
  }
}

template <typename T>
inline const T& FibonacciHeap<T>::Entry::getValue() const {
  return mValue;
}

template <typename T>
inline void FibonacciHeap<T>::Entry::setValue(const T& newValue) {
  mValue = newValue;
}

template <typename T>
inline double FibonacciHeap<T>::Entry::getPriority() const {
  return mPriority;
}

template <typename T>
inline void FibonacciHeap<T>::Entry::setPriority(const double newPriority) {
  mPriority = newPriority;
}

template <typename T>
inline bool FibonacciHeap<T>::Entry::isMarked() const {
  return marked;
}

template <typename T>
inline void FibonacciHeap<T>::Entry::mark() {
  marked = true;
}

template <typename T>
inline void FibonacciHeap<T>::Entry::unmark() {
  marked = false;
}

template <typename T>
inline int FibonacciHeap<T>::Entry::getDegree() const {
  return mDegree;
}

template <typename T>
inline void FibonacciHeap<T>::Entry::increaseDegree() {
  mDegree++;
}

template <typename T>
inline void FibonacciHeap<T>::Entry::decreaseDegree() {
  mDegree--;
}

template <typename T>
FibonacciHeap<T>::FibonacciHeap() : mSize(0), mMin(NULL) {
  // Handled in initializer list.
}

template <typename T>
FibonacciHeap<T>::~FibonacciHeap() {
  delete mMin;
}

template <typename T>
inline size_t FibonacciHeap<T>::size() const {
  return mSize;
}

template <typename T>
inline bool FibonacciHeap<T>::isEmpty() const {
  return mSize == 0;
}

template <typename T>
inline size_t FibonacciHeap<T>::getSize() const {
  return mSize;
}

template <typename T>
inline void FibonacciHeap<T>::setSize(const size_t newSize) {
  mSize = newSize;
}

template <typename T>
inline void FibonacciHeap<T>::setMin(Entry *newMin) {
  mMin = newMin;
}

template <typename T>
inline typename FibonacciHeap<T>::Entry& FibonacciHeap<T>::findMin() const {
  // doesn't work if empty
  return *mMin;
}

template <typename T>
typename FibonacciHeap<T>::Entry& FibonacciHeap<T>::extractMin() {
  // segfaults if empty
  Entry *min = mMin;

  // pull the min node out of the root list
  if (mMin->mNext == mMin) {
    mMin = NULL;
  } else {
    mMin->mPrev->mNext = mMin->mNext;
    mMin->mNext->mPrev = mMin->mPrev;
    mMin = mMin->mNext;
  }
  mSize--;

  // sever any children from the min node and promote them to the root list
  Entry *firstChild = min->mChild;
  if (firstChild) {
    Entry *cur = firstChild->mNext;
    do {
      cur->mParent = NULL;
      cur = firstChild->mNext;
    } while (cur != firstChild);

    if (mMin) {
      mergeLists(mMin, firstChild);
      mMin = mMin->getPriority() <= firstChild->getPriority() 
        ? mMin : firstChild;
    } else {
      mMin = firstChild;
    }
  }

  // clean up the min node for deletion by user
  min->mNext = min;
  min->mPrev = min;
  min->mChild = NULL;

  // if min was the last node, no merging should occur
  if (!mMin) {
    return *min;
  }

  // create a vector with a bucket for trees of each degree
  // and a vector to store the root nodes and make sure we visit them all
  std::vector<Entry *> buckets;
  std::vector<Entry *> toVisit;

  Entry *first = mMin;
  toVisit.push_back(first);
  for (Entry *cur = first->mNext; cur != first; cur = cur->mNext) {
    toVisit.push_back(cur);
  }

  // put each root node (tree) into the appropriate bucket
  // merge if necessary
  for (typename std::vector<Entry *>::iterator it = toVisit.begin(); 
      it != toVisit.end(); ++it) {
    Entry *cur = *it;
    for (;;) {
      // extend the bucket list (vector really but where's the pun in that)
      while (cur->getDegree() >= buckets.size()) {
        buckets.push_back(NULL);
      }
      // if the bucket was empty, fill it
      if (!buckets[cur->getDegree()]) {
        buckets[cur->getDegree()] = cur;
        break;
      }
      // otherwise merge
      Entry *other = buckets[cur->getDegree()];
      buckets[cur->getDegree()] = NULL;

      Entry *greater = other->getDegree() < cur->getDegree() ? cur : other;
      Entry *lesser = other->getDegree() < cur->getDegree() ? other : cur;
      greater->mNext->mPrev = greater->mPrev;
      greater->mPrev->mNext = greater->mNext;
      greater->mPrev = greater->mNext = greater;
      if (lesser->mChild) {
        mergeLists(lesser->mChild, greater);
      } else {
        lesser->mChild = greater;
      }
      greater->mParent = lesser;
      greater->unmark();
      lesser->increaseDegree();
      cur = lesser;
    }
    if (cur->getPriority() <= mMin->getPriority())
      mMin = cur;
  }

  return *min;
}

template <typename T>
void FibonacciHeap<T>::decreaseKey(Entry& entry, double newPriority) {
  entry.setPriority(newPriority);

  if (entry.mParent && entry.getPriority() <= entry.mParent->getPriority()) {
    cutNode(entry);
  }

  if (entry.getPriority() <= mMin->getPriority()) {
    mMin = &entry;
  }
}

template <typename T>
typename FibonacciHeap<T>::Entry& FibonacciHeap<T>::enqueue(const T& value, 
    const double priority) {
  Entry *newEntry = new Entry(value, priority);
  mergeLists(newEntry, mMin);
  if (!mMin || newEntry->getPriority() < mMin->getPriority()) {
    mMin = newEntry;
  }
  mSize++;
  return *newEntry;
}

template <typename T>
void FibonacciHeap<T>::cutNode(Entry &entry) {

  entry.unmark();
  if (!entry.mParent) return;

  // if the node has siblings pull it out of the siblings list and point
  // the parent to the next sibling instead
  if (entry.mNext != &entry) {
    entry.mPrev->mNext = entry.mNext;
    entry.mNext->mPrev = entry.mPrev;
    entry.mParent->mChild = entry->mNext;
  }
  if (entry.mParent->mChild == &entry) {
    entry.mParent->mChild = NULL;
  }
  entry.mParent->decreaseDegree();

  // merge the node with the root list
  entry.mNext = entry.mPrev = &entry;
  mergeLists(mMin, &entry);
  if (mMin->getPriority() < entry.getPriority()) {
    mMin = &entry;
  }

  // recursively cut the parent if it was marked
  if (entry.mParent->isMarked()) {
    cutNode(*entry.mParent);
  } else {
    entry.mParent->mark();
  }
  entry.mParent = NULL;
}

template <typename T>
void FibonacciHeap<T>::mergeLists(Entry *one, Entry *two) {
  if (!one || !two) return;

  one->mPrev->mNext = two->mNext;
  two->mNext->mPrev = one->mPrev;
  two->mNext = one;
  one->mPrev = two;
}

template <typename T>
FibonacciHeap<T>& FibonacciHeap<T>::meld(FibonacciHeap<T>& first, 
    FibonacciHeap<T>& second) {

  FibonacciHeap<T> *result = new FibonacciHeap<T>();
  Entry& minOne = first.findMin();
  Entry& minTwo = second.findMin();

  //merge the root lists of the two heaps
  mergeLists(&minOne, &minTwo);

  //set min to the lower of the two and set size
  result->setMin(minOne.getPriority() < minTwo.getPriority() 
      ? &minOne : &minTwo);
  result->setSize(first.getSize() + second.getSize());

  //empty out the old heaps
  first.setMin(NULL);
  second.setMin(NULL);
  first.setSize(0);
  second.setSize(0);

  return *result;
}

#endif
