// Fibonacci heap class.

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

	FibonacciHeap(); // we can do an enqueue that doesn't directly call meld, so we don't need 2 constructors
	~FibonacciHeap();

	inline size_t size() const;
	inline bool isEmpty() const;
  inline size_t getSize() const;
  inline void setSize(const size_t newSize);
  inline void setMin(Entry *newMin);
	inline Entry& findMin() const; // maybe we should return const Entry&, but could be bad because client passes handle to decreaseKey and other shit

	Entry& enqueue(const T& value, double priority);
	Entry& extractMin();
	void decreaseKey(Entry& entry, double newPriority);

	static FibonacciHeap<T> meld(FibonacciHeap<T>& first,
															 FibonacciHeap<T>& second);

private:
	Entry *mMin;
	size_t mSize;

  void cutNode(Entry &entry);
  void mergeLists(Entry *one, Entry *two);

  FibonacciHeap(FibonacciHeap const &) = delete;
  void operator=(FibonacciHeap const &) = delete;
};

/* Entry constructor */
template <typename T>
FibonacciHeap<T>::Entry::Entry(const T& value, double priority) :
		mValue(value), mPriority(priority), mParent(NULL), mChild(NULL), 
    mNext(&this), mPrev(&this), marked(false), mOrder(0) {
	// Handled in initializer list.
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
FibonacciHeap::FibonacciHeap() : mSize(0), mMin(NULL) {
	// Handled in initializer list.
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
inline FibonacciHeap<T>::Entry& FibonacciHeap<T>::findMin() const {
	// doesn't work if empty
	return *mMin;
}

template <typename T>
FibonacciHeap<T>::Entry& FibonacciHeap<T>::extractMin() {
  // segfaults if empty
  min = *mMin;
  if (mMin->mNext == mMin) {
    mMin = NULL;
  } else {
    mMin->prev->next = mMin->next;
    mMin->next->prev = mMin->prev;
    mMin = mMin->next;
  }

  mSize--;
  
  Entry *firstChild = min.mChild;
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
      mMin = firstChild
    }
  }

  if (!mMin) {
    return min;
  }

  std::vector<Entry *> buckets;
  std::vector<Entry *> toVisit;
  
  Entry *first = mMin;
  toVisit.push_back(first);
  for (Entry *cur = first->mNext; cur != first; cur = cur->mNext) {
    toVisit.push_back(cur);
  }

  for (std::vector<Entry *>::iterator it = toVisit.begin(); 
      it != toVisit.end(); ++it) {
    Entry *cur = *it;
    for (;;) {
      while (cur->getDegree() < buckets.size()) {
        buckets.push_back(NULL);
      }
      if (!buckets[cur->getDegree()]) {
        buckets[cur->getDegree()] = cur;
        break;
      }
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

  return min
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
FibonacciHeap<T>::Entry& FibonacciHeap<T>::enqueue(const T& value, 
    const double priority) {
  Entry newEntry = new Entry(value, priority);
  mergeLists(&newEntry, mMin);
  if (newEntry.getPriority() < mMin->getPriority()) {
    mMin = &newEntry;
  }
  mSize++;
}

template <typename T>
void FibonacciHeap<T>::cutNode(Entry &entry) {
  entry.unmark();
  if (!entry.mParent) return;
  if (entry.mNext != &entry) {
    entry.mPrev->mNext = entry.mNext;
    entry.mNext->mPrev = entry.mPrev;
  }
  if (entry.mParent->mChild == &entry) {
    entry.mParent->mChild = entry->mNext;
  }
  if (entry.mParent->mChild == &entry) {
    entry.mParent->mChild = NULL;
  }
  entry.mParent->decreaseDegree();
  entry.mNext = entry.mPrev = &entry;
  mergeLists(mMin, &entry);
  if (mMin->getPriority() < entry.getPriority()) {
    mMin = &entry;
  }
  if (entry.mParent->isMarked()) {
    cutNode(*entry.mParent);
  } else {
    entry.mParent->mark()
  }
  entry.mParent = NULL;
}

template <typename T>
void FibonacciHeap<T>::mergeLists(Entry *one, Entry *two) {
  one->mPrev->mNext = two->mNext;
  two->mNext->mPrev = one->mPrev;
  two->mNext = one;
  one->mPrev = two;
}

template <typename T>
static FibonacciHeap<T> FibonacciHeap<T>::meld(FibonacciHeap<T>& first, 
    FibonacciHeap<T>& second) {

  FibonacciHeap<T> result = new FibonacciHeap<T>();
  Entry minOne = first.findMin();
  Entry minTwo = second.findMin();

  //merge the root lists of the two heaps
  mergeLists(&minOne, &minTwo);

  //set min to the lower of the two and set size
  result.setMin(minOne.getPriority() < minTwo.getPriority() 
      ? &minOne : &minTwo);
  result.setSize(first.getSize() + second.getSize());

  //empty out the old heaps
  first.setMin(NULL);
  second.setMin(NULL);
  first.setSize(0);
  second.setSize(0);

  return result;
}
