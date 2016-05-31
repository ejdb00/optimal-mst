#ifndef SoftHeap_Included
#define SoftHeap_Included

#include <vector>
#include <cassert>
#include <iostream>

template <typename T>
class SoftHeap {
  public:

    SoftHeap();
    SoftHeap(const double key, const T& value, const size_t r);
    ~SoftHeap();

    struct Entry {
      Entry(const double key, const T& value);
      ~Entry();

      double mKey;
      T mValue;
      Entry *next;
    };

    struct EntryList {
      EntryList();
      EntryList(Entry *entry);
      ~EntryList();

      size_t size;
      Entry *head;
      Entry *tail;

      void concatenate(EntryList *other);
      void add(Entry *entry);
    };

    struct Node {
      Node(const double key, const T& value);
      Node();
      ~Node();

      size_t ckey;
      Entry *ckeyEntry;
      size_t rank;
      size_t size;
      Node* left;
      Node* right;
      EntryList *entryList;
    };

    struct Tree {
      Tree(const double key, const T& value);
      ~Tree();

      Node* root;
      Tree* next;
      Tree* prev;
      Tree* suffixMin;
      size_t rank;
    };

    inline size_t getRank() const;
    inline void setRank(const size_t newRank);
    inline Tree *getFirst() const;
    inline void setFirst(Tree *const newFirst);
    inline size_t getR() const;
    inline void setR(const size_t newR);

    inline size_t getSize() const;
    inline void setSize(const size_t newSize);
    inline bool isEmpty() const;

    inline EntryList *getCorrupted(); // maybe should return constant somehow?

    Entry *extract_min(); // DON'T FREE THESE, handled by heap destructor

    static SoftHeap<T>& insert(SoftHeap<T>& heap, const double key, const T& value);
    static SoftHeap<T>& meld(SoftHeap<T>& p, SoftHeap<T>& q);

  private:


    size_t mR;
    size_t mSize;
    size_t heapRank;
    Tree* first;

    EntryList *returned;
    EntryList *corrupted;

    inline bool is_leaf(Node *node) const;

    void sift(Node *node);
    void insert_tree(Tree *tree1, Tree *tree2);
    void remove_tree(Tree *tree);
    void update_suffix_min(Tree *tree);
    Entry *pick_element(Node *node);
    void concatenate(Node *one, Node *two);
    Node *combine(Node *one, Node *two);
    void repeated_combine(size_t k);

    static void merge_into(SoftHeap<T>& p, SoftHeap<T>& q);

};


template <typename T>
SoftHeap<T>::Entry::Entry(const double key, const T& value) : mKey(key), 
  mValue(value), next(NULL) {
    // handled in initializer list
  }

template <typename T>
SoftHeap<T>::Entry::~Entry() {
  // do nothing
}

template <typename T> 
SoftHeap<T>::EntryList::EntryList() : head(NULL), tail(NULL), size(0) {
  // handled in initializer list
}

template <typename T>
SoftHeap<T>::EntryList::EntryList(Entry *entry) : head(entry), tail(entry), size(1) {
  // handled in initializer list
}

template <typename T>
SoftHeap<T>::EntryList::~EntryList() {
  while (head) {
    Entry *next = head->next;
    delete head;
    head = next;
  }
}

// concatenates the list 'other' to the end of this list
// empties 'other'
template <typename T>
void SoftHeap<T>::EntryList::concatenate(EntryList *other) {
  if (!head) {
    head = other->head;
  } else {
    tail->next = other->head;
  }
  if (other->tail) {
    tail = other->tail;
  }
  size += other->size;
  other->size = 0;
  other->head = other->tail = NULL;
}

template <typename T>
void SoftHeap<T>::EntryList::add(Entry *entry) {
  entry->next = head;
  if (!head) {
    head = tail = entry;
  }
  size++;
}

template <typename T>
SoftHeap<T>::Node::Node(const double key, const T& value) : 
  ckey(key), rank(0), size(1), left(NULL), right(NULL), 
  entryList(new EntryList(new Entry(key, value))), ckeyEntry(NULL) {
    ckeyEntry = entryList->head;
  }

template <typename T>
SoftHeap<T>::Node::Node() : ckey(0), rank(0), size(1), left(NULL), right(NULL),
  entryList(new EntryList()), ckeyEntry(NULL) {
    // handled in initializer list    
  }

template <typename T>
SoftHeap<T>::Node::~Node() {
  if (entryList) {
    delete entryList;
  }
  if (left) {
    delete left;
  }
  if (right) {
    delete right;
  }
}

template <typename T>
SoftHeap<T>::Tree::Tree(const double key, const T& value): next(NULL), 
  prev(NULL), suffixMin(this), rank(0), root(new Node(key, value)) {
    // handled in initializer list
  }

template <typename T>
SoftHeap<T>::Tree::~Tree() { 
  if (root) {
    delete root; 
  }
  if (next) {
    delete next;
  }
}

template <typename T>
SoftHeap<T>::SoftHeap(const double key, const T& value, const size_t r) : 
  heapRank(0), first(new Tree(key, value)), mR(r), mSize(1) {
    // handled in initializer list
  }

template <typename T>
SoftHeap<T>::SoftHeap() : 
  heapRank(0), first(NULL), mR(0), mSize(0) {
    // handled in initializer list
  }

template <typename T>
SoftHeap<T>::~SoftHeap() {
  delete corrupted;
  delete returned;
  delete first;
}

template <typename T>
inline size_t SoftHeap<T>::getRank() const { return heapRank; }

template <typename T>
inline void SoftHeap<T>::setRank(const size_t newRank) { heapRank = newRank; }

template <typename T>
inline typename SoftHeap<T>::Tree *SoftHeap<T>::getFirst() const { 
  return first; 
}

template <typename T>
inline void SoftHeap<T>::setFirst(Tree *const newFirst) { first = newFirst; }

template <typename T>
inline size_t SoftHeap<T>::getR() const { return mR; }

template <typename T>
inline void SoftHeap<T>::setR(const size_t newR) { mR = newR; }

template <typename T>
inline size_t SoftHeap<T>::getSize() const { return mSize; }

template <typename T>
inline void SoftHeap<T>::setSize(const size_t newSize) { mSize = newSize; }

template <typename T>
inline bool SoftHeap<T>::isEmpty() const { return mSize == 0; }

template <typename T>
inline typename SoftHeap<T>::EntryList *SoftHeap<T>::getCorrupted() {
  return corrupted;
}

// inserts an element with the specified key and value into the heap
template <typename T>
SoftHeap<T>& SoftHeap<T>::insert(SoftHeap<T>& heap, 
                                 const double key, const T& value) {
  // create a new heap of size 1 and merge it into the existing heap
  SoftHeap<T> *newHeap = new SoftHeap<T>(key, value, heap.getR());
  return meld(heap, *newHeap);
}

// Melds two heaps, returning the one which originally had greater rank
// and emptying the other
template <typename T>
SoftHeap<T>& SoftHeap<T>::meld(SoftHeap& p, SoftHeap& q) {

  SoftHeap<T>& result = *(new SoftHeap<T>());
  // merge the heap with lesser rank into the heap with greater rank 
  if(p.getRank() > q.getRank()) {

    // merge the root list of q into the root list of p, 
    // then combine trees as necessary
    merge_into(q, p);

    // combine the trees in the root list of p
    p.repeated_combine(q.getRank());
    p.setSize(p.getSize() + q.getSize());

    // transfer all members to the new heap
    result.setR(p.getR());
    result.setFirst(p.getFirst());
    result.setSize(p.getSize());
    result.setRank(p.getRank());
    result.getCorrupted()->concatenate(p.getCorrupted());

  } else { // or visa versa

    merge_into(p, q);
    q.repeated_combine(p.getRank());
    q.setSize(p.getSize() + q.getSize());

    result.setR(q.getR());
    result.setFirst(q.getFirst());
    result.setSize(q.getSize());
    result.setRank(q.getRank());
    result.getCorrupted()->concatenate(q.getCorrupted());

  }

  // clear the old heaps
  p.setSize(0);
  p.setFirst(NULL);
  p.setRank(0);

  q.setSize(0);
  q.setFirst(NULL);
  q.setRank(0);

  return result;
}

// Extracts the min element from our heap.
// Returns a pointer which should not be freed.
template <typename T>
typename SoftHeap<T>::Entry* SoftHeap<T>::extract_min() {
  assert(first);
  // decrease the overall size of the heap since we're removing an element
  mSize--;
  // find the tree whose root has the lowest ckey
  Tree *tree = first->suffixMin;
  Node *x = tree->root;
  // select an arbitrary element from this node
  Entry *entry = pick_element(x);
  if (x->entryList->size <= x->size/2) {
    // if our element list in the root node is too small, and the node isn't a leaf,
    // sift to replenish the list
    if (!is_leaf(x)) {
      sift(x);
      update_suffix_min(tree);
    } else if (x->entryList->size == 0) { 
      // if the node is a leaf and is empty, remove it
      delete x;
      remove_tree(tree);
    }
  }
  return entry;
}

// tells us whether the node pointed to by x is a leaf
template <typename T>
inline bool SoftHeap<T>::is_leaf(Node *x) const {
  return (x->left == NULL && x->right == NULL);
}

// inserts tree1 into the root list before tree2
template <typename T>
void SoftHeap<T>::insert_tree(Tree *tree1, Tree *tree2) {
  assert (tree1 != tree2);
  if (!tree1) return;
  assert(tree2);
  tree1->next = tree2;
  if(tree2->prev == NULL) {
    first = tree1;
  } else {
    tree2->prev->next = tree1;
    tree2->prev = tree1;
  }
}

// removes a tree from the root list and destroys it
template <typename T>
void SoftHeap<T>::remove_tree(Tree *tree) {

  // if the tree we're removing had the highest rank in the heap, 
  // decrease the rank of the heap
  if (tree->rank == heapRank) {
    if (tree->prev && tree->prev->rank < tree->rank && !tree->next) {
      heapRank = tree->prev->rank;
    } else if (!tree->next && !tree->prev) {
      heapRank = 0;
    }
  }

  //remove the tree from the list
  if (!tree->prev) {
    first = tree->next;
  } else {
    tree->prev->next = tree->next;
  }
  if (tree->next) {
    tree->next->prev = tree->prev;
  }

  // clean up our tree, make sure it doesn't recursively clean up others
  tree->next = NULL;
  tree->prev = NULL;
  tree->root = NULL;
  delete tree;
}

// updates the suffix min pointers of our the specified tree 
// and all trees before it in the root list
template <typename T>
void SoftHeap<T>::update_suffix_min(Tree *tree) {
  while (tree) {
    // if we are the last tree, or our root node's ckey is less than the ckey
    // of the root node of the next tree in the list, point suffix min to ourself
    if (!tree->next || tree->root->ckey <= tree->next->suffixMin->root->ckey) {
      tree->suffixMin = tree;
    } else { // otherwise point suffixMin to suffixMin of the next tree in the list
      tree->suffixMin = tree->next->suffixMin;
    }
    // repeat down the list
    tree = tree->prev;
  }
}

// merges the tree list of p into that of q
template <typename T>
void SoftHeap<T>::merge_into(SoftHeap<T>& p, SoftHeap<T>& q) {
  assert(&p != &q);
  // ensure q has rank >= p
  if (p.getRank() > q.getRank()) {
    return;
  }

  Tree* tree1 = p.getFirst();
  Tree* tree2 = q.getFirst();
  assert(tree2);

  // for each tree in p's tree list
  while(tree1) {
    // find the tree in q's list which should follow it by rank
    //  -- this tree should exist as per the rank invariant that 
    //     each heap maintain a non-decreasing rank >= the rank
    //     of its highest rank tree and per our earlier check,
    //     q is of greater or equal rank to p
    assert(tree2->next || tree2->rank >= tree1->rank);
    while (tree1->rank > tree2->rank) {
      tree2 = tree2->next;
    }
    // insert the tree from p before the tree from q
    Tree *tmpTree = tree1->next;
    q.insert_tree(tree1, tree2);
    tree1 = tmpTree;
  }
}

// choose an arbitrary element from the element list of the given node,
// in this case the first one
// if this entry was the original entry in the list, and therefore is pointed
// to by our ckeyEntry pointer, we know that it is uncorrupted, and should be 
// added to the returned set for later cleanup
template <typename T>
typename SoftHeap<T>::Entry* SoftHeap<T>::pick_element(Node *node) {
  assert(node->entryList->size > 0);
  // take the first entry off the list
  Entry *entry = node->entryList->head;
  // update the list to remove the entry
  if (node->entryList->head == node->entryList->tail) {
    node->entryList->head = node->entryList->tail = NULL;
  } else {
    node->entryList->head = node->entryList->head->next;
  }
  node->entryList->size--;
  // if this entry was never corrupted, and therefore corresponds to our ckey,
  // add it to the 'returned' list for cleanup, since it was never added to the
  // 'corrupted' list
  if (entry == node->ckeyEntry) {
    node->ckeyEntry = NULL;
    returned->add(entry);
  }
  // make sure we can't access any other entries through this one
  return entry;
}

// combines two nodes by creating a new node and inserting the two argument nodes
// as its children
template <typename T>
typename SoftHeap<T>::Node *SoftHeap<T>::combine(Node *node1, Node *node2) {
  assert(node1->rank == node2->rank);
  // create a new empty node
  Node *newNode = new Node();
  // set our arg nodes as its children
  newNode->left = node1;
  newNode->right = node2;
  // assign it rank one greater than that of the children, which must have equal rank
  newNode->rank = node1->rank + 1;
  // specify the target size for the new node's element list
  if (newNode->rank <= mR) {
    newNode->size = 1;
  } else {
    newNode->size = (3 * node1->size + 1) / 2; 
  }
  // sift to try and reach the target list size
  sift(newNode);
  return newNode;
}

// concatenates the entry list of node two onto the end of node one's entry list
template <typename T>
void SoftHeap<T>::concatenate(Node *one, Node *two) {
  one->entryList->concatenate(two->entryList);
}

// sifts entries upward in the tree until the specified node has reached
// its target size or becomes a leaf
template <typename T>
void SoftHeap<T>::sift(Node *node) {
  assert(node);
  assert(node->entryList);
  // while our node has not yet acheived its target size and is not a leaf node,
  // sift entries up to it
  while (node->entryList->size < node->size && !is_leaf(node)) {
    // if we don't have a left child, or our left child has the greater ckey of the
    // children, switch the children to maintain our heap ordering invariant
    if (!node->left || node->left->ckey > node->right->ckey) {
      Node *tmp = node->right;
      node->right = node->left;
      node->left = tmp;
    }
    // append the left child's element list to our element list, 
    // clearing the child list in the process
    concatenate(node, node->left);

    // add the entry with key 'ckey' to the corrupted entries if we haven't
    // already returned it
    // an entry may only be corrupted once, and it happens when this node takes on a 
    // child's ckey
    if (node->ckeyEntry) {
      corrupted->add(node->ckeyEntry);
    }
    // update our pointer to the entry with key ckey
    node->ckeyEntry = node->left->ckeyEntry;

    // update ckey to the ckey of the left child
    node->ckey = node->left->ckey;
    // if the left child is a leaf, remove it because it is now empty
    // otherwise, sift to fill the left child's entry list
    if (is_leaf(node->left)) {
      delete node->left;
      node->left = NULL;
    } else {
      sift(node->left);
    }
  }
}

// combines all trees of equal rank, up to rank k
// repeats until there is at most one tree of each rank in the tree list
template <typename T>
void SoftHeap<T>::repeated_combine(size_t k) {
  Tree *tree = first;
  // repeat while there are at least two trees we have note yet visited
  while (tree && tree->next) {
    // combine the two trees if they have equal rank
    if (tree->rank == tree->next->rank) {
      // if 3 trees have the same rank, combine the last two
      if (!tree->next->next || tree->rank != tree->next->next->rank) {
        // combine the trees, update the rank of the new tree, and remove the old one
        tree->root = combine(tree->root, tree->next->root);
        tree->rank = tree->root->rank;
        std::cout << "called remove from repeated_combine" << std::endl;
        remove_tree(tree->next);
      }
    } else if (tree->rank > k) {
      break;
    }
    // move on to the next tree
    tree = tree->next;
  }
  if (!tree) return;
  // update the heap rank if our last tree had greater rank than the heap
  if (tree->rank > heapRank) {
    heapRank = tree->rank;
  }
  // update the suffix min for every tree before the last one
  update_suffix_min(tree);
}

#endif
