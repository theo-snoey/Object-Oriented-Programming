/*
 * Author: Theo Snoey
 * This file, implements the client side of this project, implementing two sorting algorithms that
 * test the efficiency of our differnt classes, PQarray, PQheap.
 */
#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "vector.h"
#include "strlib.h"
#include <sstream>
#include "SimpleTest.h"
using namespace std;

/* This function, PQsort, takes a vector of data points, and enques all those elements, into
 * one of our queues then performs the inverse, dequeuing all those elements back into the vector, to
 * obtain a vector, sorted according to our sorting priorities.
 */
void pqSort(Vector<DataPoint>& v) {
    PQHeap pq;

    /* Using the Priority Queue data structure as a tool to sort, neat! */

    /* Add all the elements to the priority queue. */
    for (int i = 0; i < v.size(); i++) {
        pq.enqueue(v[i]);
    }

    /* Extract all the elements from the priority queue. Due
     * to the priority queue property, we know that we will get
     * these elements in sorted order, in order of increasing priority
     * value. Store elements back into vector, now in sorted order.
     */
    for (int i = 0; !pq.isEmpty(); i++) {
        v[i] = pq.dequeue();
    }
}

/* This function, topK takes a stream of DataPoints, and uses a queue to retain only the 10 "best"
 * DataPoints according to our set priorities. THen, is dequeues these 10 DataPoints returning them as a vector from biggest
 * to smallest.
 */
Vector<DataPoint> topK(istream& stream, int k) {
    PQHeap pq;
    DataPoint cur;
    while (stream >> cur) {
        pq.enqueue(cur);
        if (pq.size() > k){
            pq.dequeue();
        }
    }

    Vector<DataPoint> result(pq.size());

    for (int i = pq.size() - 1; i >= 0 ; i--){
        result[i] = pq.dequeue();
    }

    return result;
}



/* * * * * * Test Cases Below This Point * * * * * */

/* Helper function that, given a list of data points, produces a stream from them. */
stringstream asStream(const Vector<DataPoint>& dataPoints) {
    stringstream result;
    for (const DataPoint& pt: dataPoints) {
        result << pt;
    }
    return result;
}

/* Helper function that, given range start and stop, produces a stream of values. */
stringstream asStream(int start, int stop) {
    stringstream result;
    for (int i = start; i <= stop; i++) {
        DataPoint pt = {"", double(i) };
        result << pt;
    }
    return result;
}

/* Helper function to fill vector with n random DataPoints. */
void fillVector(Vector<DataPoint>& vec, int n) {
    vec.clear();
    for (int i = 0; i < n; i++) {
        DataPoint pt = {"", randomReal(0, 100)};
        vec.add(pt);
    }
}


// MY TESTS

STUDENT_TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}

STUDENT_TEST("topK: time trial") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input, result;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, result = topK(stream, k));
        EXPECT_EQUAL(result.size(), k);
    }
}


/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("pqSort: vector of random elements") {
    setRandomSeed(137); // why might it be a good idea to set seed here?

    Vector<DataPoint> input;
    Vector<double> expected;
    fillVector(input, 100);
    for (DataPoint dp : input) {
        expected.add(dp.priority);
    }
    expected.sort();

    pqSort(input);
    for (int i = 0; i < input.size(); i++) {
        EXPECT_EQUAL(input[i].priority, expected[i]);
    }
}

PROVIDED_TEST("pqSort: time trial") {
    int startSize = 1000;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> v;
        fillVector(v, n);
        TIME_OPERATION(n, pqSort(v));
    }
}


/* Constant used for sizing the tests below this point. */
const int kMany = 100000;

PROVIDED_TEST("topK: stream 0 elements, ask for top 1") {
    stringstream emptyStream = asStream({});
    Vector<DataPoint> expected = {};
    EXPECT_EQUAL(topK(emptyStream, 1), expected);
}

PROVIDED_TEST("topK: stream 1 element, ask for top 1") {
    stringstream stream = asStream({ {"" , 1} });
    Vector<DataPoint> expected = { {"" , 1} };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: small fixed input") {
    Vector<DataPoint> input = { {"A", 1}, {"B", 2}, {"C", 3}, {"D", 4} };

    stringstream stream = asStream(input);
    Vector<DataPoint> expected = { {"D", 4}, {"C", 3} };
    EXPECT_EQUAL(topK(stream, 2), expected);

    stream = asStream(input);
    expected = { {"D", 4}, {"C", 3}, {"B", 2}, {"A", 1} };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 1") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = { {"" , kMany} };
    EXPECT_EQUAL(topK(stream, 1), expected);
}

PROVIDED_TEST("topK: stream many elements, ask for top 5") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> expected = {
        {"" , kMany    },
        {"" , kMany - 1},
        {"" , kMany - 2},
        {"" , kMany - 3},
        {"" , kMany - 4},
    };
    EXPECT_EQUAL(topK(stream, 5), expected);
}

PROVIDED_TEST("topK: stress test, many elements, random values") {
    Vector<double> expected;
    Vector<DataPoint> points;
    fillVector(points, 10000);
    for (DataPoint dp : points) {
        expected.add(dp.priority);
    }
    sort(expected.begin(), expected.end(), greater<double>());

    stringstream stream = asStream(points);
    int k = 10;
    Vector<DataPoint> result = topK(stream, k);
    EXPECT_EQUAL(result.size(), k);
    for (int i = 0; i < k; i++) {
        EXPECT_EQUAL(result[i].priority, expected[i]);
    }
}

PROVIDED_TEST("topK: stress test - many elements, ask for top half") {
    stringstream stream = asStream(1, kMany);
    Vector<DataPoint> result = topK(stream, kMany/2);
    EXPECT_EQUAL(result.size(), kMany/2);
    EXPECT_EQUAL(result[0].priority, kMany);
    EXPECT_EQUAL(result[result.size()-1].priority, kMany - result.size() + 1);
}

PROVIDED_TEST("topK: time trial") {
    int startSize = 200000;
    int k = 10;
    for (int n = startSize; n < 10*startSize; n *= 2) {
        Vector<DataPoint> input, result;
        fillVector(input, n);
        stringstream stream = asStream(input);
        TIME_OPERATION(n, result = topK(stream, k));
        EXPECT_EQUAL(result.size(), k);
    }
}
