#include "FibonacciHeap.hh"
#include <iostream>
#include <cassert>

int main(int argc, char *argv[]) {
  FibonacciHeap<int> heap;
  assert(heap.isEmpty());
  int testArr[200];
  for (int i = 0; i < 200; i++) {
    testArr[i] = i;
    heap.enqueue(i, i);
    assert(heap.getSize() == i + 1);
  }
  assert(heap.extractMin().getValue() == 0);
  assert(heap.findMin().getValue() == 1);
  assert(heap.extractMin().getValue() == 1);

  FibonacciHeap<int> heap2;
  heap2.enqueue(-1, -1);
  FibonacciHeap<int>& newHeap = FibonacciHeap<int>::meld(heap, heap2);
  assert(newHeap.getSize() == 199);
  assert(newHeap.findMin().getValue() == -1);
  assert(heap.isEmpty());
  assert(heap2.isEmpty());

  delete &newHeap;

  return 0;
}
