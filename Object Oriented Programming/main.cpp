#include <iostream>
#include "console.h"
#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "SimpleTest.h"
using namespace std;

// You are free to edit the main in any way that works
// for your testing/debugging purposes.
// We will supply our own main() during grading

int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    cout << endl << "main() completed." << endl;
    return 0;
}


// Do not remove or edit below this line. It is here to confirm that your code
// conforms to the expected function prototypes needed for grading
void confirmFunctionPrototypes() {
    Vector<DataPoint> v;
    {
        PQArray pq;
        pq.enqueue({"", 1});
        DataPoint n = pq.peek();
        n = pq.dequeue();
        pq.size();
        pq.isEmpty();
        pq.clear();
        pq.debugConfirmInternalArray();
        pq.debugSetInternalArrayContents(v, 1);
        v = pq.debugGetInternalArrayContents();
    }
    pqSort(v);
    v = topK(cin, 1);
    {
        PQHeap pq;
        pq.enqueue({"", 1});
        DataPoint n = pq.peek();
        n = pq.dequeue();
        pq.size();
        pq.isEmpty();
        pq.clear();
        pq.debugConfirmInternalArray();
        pq.debugSetInternalArrayContents(v, 1);
        v = pq.debugGetInternalArrayContents();
    }
}
