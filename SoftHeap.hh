#include <vector>


template <typename T>
class SoftHeap {
public:

	SoftHeap(const double key, const T& value);
	~SoftHeap();

	struct Entry {
		Entry(const double key, const T& value);
		double mKey;
		T mValue;

	};


	struct Node {
		Node(const double key, const T& value);
		size_t ckey;
		size_t rank;
		size_t size;
		Node* left;
		Node* right;
		std::vector<Entry*> list;

	};


	struct Tree {
		Tree(const double key, const T& value);
		Node* mRoot;
		Tree* next;
		Tree* prev;
		Tree* suffix_min;
		size_t tree_rank;

	};

private:

	size_t heap_rank;
	Tree* first;

};


template <typename T>
SoftHeap<T>::Entry::Entry(const double key, const T& value) : mKey(key), mValue(value) {

}

template <typename T>
SoftHeap<T>::Node::Node(const double key, const T& value) : ckey(key), rank(0), size(1), left(NULL), right(NULL) {

	list.push_back(new Entry(key, value));

}


template <typename T>
SoftHeap<T>::Tree::Tree(const double key, const T& value): next(NULL), prev(NULL), suffix_min(this), tree_rank(0), mRoot(new Node(key, value)) {


}

template <typename T>
SoftHeap<T>::SoftHeap(const double key, const T& value) : heap_rank(0), first(new Tree(key, value)) {

	
}

template <typename T>
SoftHeap<T>::~SoftHeap() {

}


template <typename T>
inline bool SoftHeap<T>::isLeaf(Node x) const {
	return(x->left == NULL && x->right == NULL)
	
}

template <typename T>
SoftHeap<T>::insert_tree(Tree* T1, Tree* T2) {
	T1->next = T2;
	if(T2->prev == NULL) {
		first = T1;

	} else {
		T2->next = T1;
	}
}





template <typename T>
static SoftHeap<T>& SoftHeap<T>::merge_into(SoftHeap<T>& P, SoftHeap<T>& Q) {
	if(P.rank > Q.rank) {
		return;
	}

	Tree* T1 = P.first;
	Tree* T2 = Q.first;

	while(T1 != NULL) {
		while //T1->rank > T2->rank {

			//T2 = T2->next

		//next_T1 = T1->next;
			//insert_tree(Q, T1, T2)
			//T1 = next_T1

	}
}


/*
template <typename T>
SoftHeap<T>::meld(SoftHeap P, SoftHeap Q) {
	if(P.heap_rank > Q.heap_rank) {
		merge_into(Q, P);
		repeated_combine(P, Q.heap_rank);
	} else {
		merge_into(P, Q);
		repeated_combine(Q, P.heap_rank);
	}
}

template <typename T>
SoftHeap<T>::insert(SoftHeap P, Edge e) {
	return meld(P, make_heap(e));
}



template <typename T>
SoftHeap<T>::repeated_combine(SoftHeap Q, size_t k) {
	Tree T = &(Q->first);

}


*/









