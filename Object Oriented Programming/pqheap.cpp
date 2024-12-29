/* Author: Theo Snoey
 * This file, PQHeap, uses the PQHeap class, define in PQHeap.h and implements the methods fro that class
 * to create a coherent working PQHeap Class that can be referred to to solve ranking and sorting problems efficiently.
 */
#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;    // program-wide constant
const int NONE = -1;                // used as sentinel index

/*
 * This constructor initializes the PQHeap object and assigns _numalloactd to the initial capacity,
 * creates the array of datapoints, _elements, and sets numFilled = 0;
 */
PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated];
    _numFilled = 0;
}

/*
 * This destructor, deletes the elements array to avoid any memory allocation issues
 */
PQHeap::~PQHeap() {
    delete[] _elements;
}

// this function, expandAllocation, doubles the allocated capacity of our heap. It is
// typically called when the numfilled = _numAllocated
void PQHeap::expandAllocation(){

    _numAllocated = _numAllocated * 2;

    DataPoint *newArray = new DataPoint[_numAllocated];
    for (int i = 0; i < size(); i++){
        newArray[i] = _elements[i];
    }

    delete[] _elements;

    _elements = newArray;
}



/*
 * This method, enqueue, takes a dataPoint element, elem, and adds it to the end of the
 * array, then percolates up comparing each spot to its parent's priority to properly assign it to the queue.
 */
void PQHeap::enqueue(DataPoint elem) {

    if (size() == _numAllocated){
        expandAllocation();
    }

    _elements[size()] = elem;
    _numFilled ++;

    int thisIdx = size() - 1;
    validateIndex(thisIdx);

    while (thisIdx > 0 && _elements[getParentIndex(thisIdx)].priority > _elements[thisIdx].priority){
        swapElements(thisIdx, getParentIndex(thisIdx));
        thisIdx = getParentIndex(thisIdx);
    }

}

/*
 * This method, peek(), returns the first element in the queue (without dequeueing it).
 */
DataPoint PQHeap::peek() const {
    if (isEmpty()){
        error("Cannot peek because PQHeap is empty!");
    }
    return _elements[0];
}

/*
 * This method, dequeue, removes the highest priority item (lowest priority score) from the queue, returning it, and replacing that
 * spot with the lowest priority element, then calls percolateDown to percolate that lowest element down comparing it to its children
 * to find the proper priority spot
 */
DataPoint PQHeap::dequeue() {
    int lastIdx = size() - 1;
    int firstIdx = 0;

    DataPoint deQueuedData = peek();

    validateIndex(lastIdx);
    validateIndex(firstIdx);

    swapElements(lastIdx, firstIdx);

    // delete first data point in now last position
    _numFilled --;

    percolateDown();

    return deQueuedData;
}

/*
 * This method, isEmpty, returns true if the array/queue is empty, false otherwise
 */
bool PQHeap::isEmpty() const {
    if (size() == 0){
        return true;
    }
    return false;
}

/*
 * This method, size, returns the number of filled spots in the queue
 */
int PQHeap::size() const {
    // return the number of filled spots
    return _numFilled;
}

/*
 * this method, sets the number of filled spots in the queue/heap/array to be zero
 */
void PQHeap::clear() {
    // set the number of filled spots to zero
    _numFilled = 0;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the parent of the
 * specified child index. If this child has no parent, return
 * the sentinel value NONE.
 */
int PQHeap::getParentIndex(int child) const {
    if (child < 0){
        return NONE;
    }
    int parentIndex = (child - 1) / 2;
    return parentIndex;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the left child of the
 * specified parent index. If this parent has no left child, return
 * the sentinel value NONE.
 */
int PQHeap::getLeftChildIndex(int parent) const {

    if (parent < 0){
        return NONE;
    }

    int leftChildIndex = (2 * parent) + 1;

    if (leftChildIndex >= _numFilled){
        return NONE;
    }

    return leftChildIndex;
}

/*
 * We strongly recommend implementing this helper function, which
 * calculates the index of the element that is the right child of the
 * specified parent index. If this parent has no right child, return
 * the sentinel value NONE.
 */
int PQHeap::getRightChildIndex(int parent) const {

    if (parent < 0){
        return NONE;
    }

    int rightChildIndex = (2 * parent) + 2;

    if (rightChildIndex >= _numFilled){
        return NONE;
    }

    return rightChildIndex;
}

/*
 * We strongly recommend implementing this helper function that swaps
 * array elements. There is a swapElements member function implemented
 * in PQAarray that you start from. Be sure to include error bounds-checking
 * it is an invaluable defense against indexing mistakes!
 */
void PQHeap::swapElements(int indexA, int indexB) {
    validateIndex(indexA);
    validateIndex(indexB);
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

/*
 * To confirm the validity of the internal array, you must check that
 * the heap property holds for all elements in the array. If elements are
 * found that violate the heap property, report an error.
 */
void PQHeap::debugConfirmInternalArray() const {

    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    for (int i = _numFilled - 1; i > 0; i --){
        if (_elements[i].priority < _elements[getParentIndex(i)].priority){
            error("PQHeap index: " + integerToString(i) + ", priority: " + integerToString(_elements[i].priority) + " is out of order!");
        }
    }
}

// This method, debugGetInternalArrayContents, serves as a helper for debugging by creating a vector copy
// of the elements in the queue, returning it.
Vector<DataPoint> PQHeap::debugGetInternalArrayContents() const {
    Vector<DataPoint> v;

    for (int i = 0; i < size(); i++) {
        v.add(_elements[i]);
    }
    return v;
}

// This method, debugSdtInternalArrayContents, takes a vecotor of DataPoints and a capacity
// and creates a new element data point queue inputting vector elements into that queue and returning it.
void PQHeap::debugSetInternalArrayContents(const Vector<DataPoint>& v, int capacity) {
    if (v.size() > capacity || capacity == 0) {
        error("Invalid capacity for debugSetInternalArrayContents!");
    }
    delete[] _elements;                     // discard old memory
    _elements = new DataPoint[capacity];    // allocate new memory
    _numAllocated = capacity;
    _numFilled = v.size();
    for (int i = 0; i < v.size(); i++) {    // fill contents with copy from vector
        _elements[i] = v[i];
    }
    debugConfirmInternalArray();            // confirm contents valid
}

// This method, validate index, takes an index and raises an error if that index is invalid/out of bounds.
void PQHeap::validateIndex(int index) const {
    if (index < 0 || index >= _numFilled) error("Invalid index " + integerToString(index));
}

// this helper method, percolataeDown starts from the first index/ top stop of the heap
// and performs the percolate down process comparing an element to its children to find the proper
// priority spot.
void PQHeap::percolateDown(){
    int thisIdx = 0;

    while (thisIdx <= (size() / 2)){
        int rcIdx = getRightChildIndex(thisIdx);
        int lcIdx = getLeftChildIndex(thisIdx);
        int smallestOfChildren;

        // find smaller of both children

        if (lcIdx == -1 && lcIdx == -1){
            break;
        }

        if (lcIdx != -1 && ((_elements[lcIdx].priority < _elements[rcIdx].priority) || rcIdx == -1)){
            smallestOfChildren = lcIdx;
        }

        if (rcIdx != -1 && ((_elements[rcIdx].priority < _elements[lcIdx].priority) || lcIdx == -1)){
            smallestOfChildren = rcIdx;
        }

        // compare smallest child to current priority

        if (_elements[smallestOfChildren].priority >= _elements[thisIdx].priority){
            break;
        }

        // if we found a smaller index = keep going

        else {
            swapElements(thisIdx, smallestOfChildren);
            thisIdx = smallestOfChildren;
        }

    }

}


/* * * * * * Test Cases Below This Point * * * * * */

STUDENT_TEST("PQHeap example in writeup, confirm contents of internal array") {
    PQHeap pq;
    Vector<DataPoint> input = {
        {"R", 4}, {"A", 5}, {"B", 3}, {"K", 7}, {"G", 2},
        {"V", 9}, {"T", 1}, {"O", 8}, {"S", 6} };

    pq.debugConfirmInternalArray();
    for (DataPoint dp : input) {
        pq.enqueue(dp);
        pq.debugConfirmInternalArray();
    }

    EXPECT_EQUAL((pq.dequeue()).priority, 1);
    pq.debugConfirmInternalArray();

    EXPECT_EQUAL((pq.dequeue()).priority, 2);
    pq.debugConfirmInternalArray();
}



STUDENT_TEST("PQHeap, testing enqueue / dequeue with diff vals") {
    PQHeap pq;

    pq.enqueue({"", 30});
    pq.debugConfirmInternalArray();

    pq.enqueue({"", 81});
    pq.debugConfirmInternalArray();

    pq.enqueue({"", 68});
    pq.debugConfirmInternalArray();

    pq.enqueue({"", 1});
    pq.debugConfirmInternalArray();

    pq.enqueue({"", -54});
    pq.debugConfirmInternalArray();

    pq.enqueue({"", 206});
    pq.debugConfirmInternalArray();

    EXPECT_EQUAL(pq.size(), 6);

    EXPECT_EQUAL(pq.peek(), pq.dequeue());
    pq.debugConfirmInternalArray();

    pq.dequeue();
    pq.debugConfirmInternalArray();

    pq.dequeue();
    pq.debugConfirmInternalArray();

    pq.enqueue({"", 30});
    pq.debugConfirmInternalArray();

    pq.dequeue();
    pq.debugConfirmInternalArray();

    EXPECT_EQUAL(pq.size(), 3);
}


STUDENT_TEST("PQHeap: stress test, cycle random elements in and out") {
    setRandomSeed(42); // make test behavior deterministic
    PQHeap pq;
    DataPoint mostUrgent;
    int nOperations = 1000;
    double sumEnqueued = 0, sumDequeued = 0;

    for (int i = 0; i < nOperations; i++) {
        if (randomChance(0.9) || pq.isEmpty()) {
            DataPoint elem = {"", randomReal(-1, 1)};
            if (pq.isEmpty() || elem.priority < mostUrgent.priority) {
                mostUrgent = elem;
            }
            sumEnqueued += elem.priority;
            pq.enqueue(elem);
        } else {
            DataPoint elem = pq.dequeue();
            sumDequeued += elem.priority;
            EXPECT_EQUAL(elem, mostUrgent);
            if (!pq.isEmpty()) {
                mostUrgent = pq.peek();
            }
        }
    }
    while (!pq.isEmpty()) {
        sumDequeued += pq.dequeue().priority;
    }
    EXPECT_EQUAL(sumEnqueued, sumDequeued);
}

void fillQueue(PQHeap& pq, int n) {
    pq.clear(); // start with empty queue
    for (int i = 0; i < n; i++) {
        pq.enqueue({"", randomReal(0, 10)});
    }
}

void emptyQueue(PQHeap& pq, int n) {
    for (int i = 0; i < n; i++) {
        pq.dequeue();
    }
}

PROVIDED_TEST("PQArray: time fillQueue and emptyQueue") {
    PQHeap pq;
    int n = 20000;

    TIME_OPERATION(n, fillQueue(pq, n));
    EXPECT_EQUAL(pq.size(), n);
    TIME_OPERATION(n, emptyQueue(pq, n));
}
