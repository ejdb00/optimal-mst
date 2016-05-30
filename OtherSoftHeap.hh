#ifndef SoftHeap_Included
#define SoftHeap_Included

#include <cstddef>
#include <vector>

template <typename T>
class SoftHeap {
public:

  ILCell *insert(double key, T& value);
  void meld(Node *q);
  ILCell *findMin();
  ILCell *deleteMin();


private:
  struct ILCell {
    double key;
    T value;
    ILCell *next;
  };

  struct Node {
    double ckey;
    int rank;
    Node *next;
    Node *child;
    ILCell *il;
    ILCell *ilTail;
  };

  struct Head {
    Node *queue;
    size_t suffixMinIndex;
  }

  Node *sift(Node *v);
  void fixMinList();

  std::vector<Head> mHeadList;
  size_t mSize;
  size_t rMax;
}


template <typename T>
SoftHeap<T>::ILCell *SoftHeap<T>::insert(double key, T& value) {

}

template <typename T>
SoftHeap<T>::ILCell *SoftHeap<T>::findMin() {

}

template <typename T>
SoftHeap<T>::ILCell *SoftHeap<T>::deleteMin() {

}

template <typename T>
void SoftHeap<T>::meld() {
  
}

template <typename T>

template <typename T>

#endif
