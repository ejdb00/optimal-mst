// Fibonacci heap class.

template <typename T>
class FibonacciHeap {
public:
	class Entry {
	public:
		Entry(); // we'll need a default constructor for the Fibonacci heap constructor
		Entry(const T& value, double priority);
		~Entry();

		inline const T& getValue() const;
		inline void setValue(const T& newValue);
		inline double getPriority() const;

	private:
		// other stuff to store inside here...
		T mValue;
		double mPriority;
	};

	FibonacciHeap(); // we can do an enqueue that doesn't directly call meld, so we don't need 2 constructors
	~FibonacciHeap();

	inline size_t size() const;
	inline bool isEmpty() const;
	inline Entry& findMin() const; // maybe we should return const Entry&, but could be bad because client passes handle to decreaseKey and other shit

	Entry& enqueue(const T& value, double priority);
	Entry& extractMin();
	void decreaseKey(Entry& entry, double newPriority);
	void remove(Entry& entry);

	static FibonacciHeap<T> meld(FibonacciHeap<T>& first,
															 FibonacciHeap<T>& second);

private:
	Entry mMin; // if things get difficult, can make this a pointer but would prefer not to
	size_t mSize;
};

template <typename T>
FibonacciHeap<T>::Entry::Entry(const T& value, double priority) :
		mValue(value), mPriority(priority) {
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
FibonacciHeap::FibonacciHeap() : mSize(0) {
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
inline FibonacciHeap<T>::Entry& FibonacciHeap::findMin() const {
	// doesn't work if empty
	return mMin;
}
