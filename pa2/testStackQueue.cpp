#define CATCH_CONFIG_MAIN
#include <iostream>
#include "cs221util/catch.hpp"
#include "stack.h"
#include "queue.h"
#include "deque.h"
//using namespace cs221util;
using namespace std;

TEST_CASE("stack::basic functions","[weight=1][part=stack]"){
    //cout << "Testing Stack..." << endl;
    Stack<int> intStack;
    vector<int> result;
    vector<int> expected;
    for (int i = 10; i > 0; i--) {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        intStack.push(i);
    }
    while (!intStack.isEmpty()) {
        result.push_back(intStack.pop());
    }
    REQUIRE( result == expected);
}

TEST_CASE("queue::basic functions","[weight=1][part=queue]"){
    Queue<int> intQueue;
    vector<int> result;
    vector<int> expected;
    for (int i = 1; i <= 10; i++) {
        expected.push_back(i);
    }
    for (int i = 1; i <= 10; i++) {
        intQueue.enqueue(i);
    }
    while (!intQueue.isEmpty()) {
        result.push_back(intQueue.dequeue());
    }
    REQUIRE( result == expected);
}

TEST_CASE("deque::basic functions","[weight=1][part=deque]") {
    /* TEST: pushR 10 popR 5 popL 2 */
    Deque<int> intDeque0;
    vector<int> result0;
    vector<int> expected0;

    /* pushR 10 */
    cout << "peekR 10: ";
    for (int i = 0; i < 10; i++) {
        intDeque0.pushR(i);
        cout << intDeque0.peekR() << " ";
    }
    cout << endl;
    /* popR 5 */
    cout << "popR 5: ";
    for (int j = 0; j < 5; j++) {
        cout << intDeque0.popR() << " ";
    }
    cout << endl;
    /* popL 2 */
    cout << "popL 2: ";
    for (int k = 0; k < 2; k++) {
        cout << intDeque0.popL() << " ";
    }
    cout << endl;

    /* Expected: {0, 0, 2, 3, 4} */
    REQUIRE(intDeque0.size() == 5);
    REQUIRE(intDeque0.isEmpty() == 0);
    /* => {3, 4} */
    if (!intDeque0.isEmpty()) {
        result0.push_back(intDeque0.popL());
        REQUIRE(intDeque0.size() == 2);
        REQUIRE(intDeque0.isEmpty() == 0);
    }
    /* => {4} */
    if (!intDeque0.isEmpty()) {
        result0.push_back(intDeque0.popL());
        REQUIRE(intDeque0.size() == 1);
        REQUIRE(intDeque0.isEmpty() == 0);
    }
    /* => {} */
    if (!intDeque0.isEmpty()) {
        result0.push_back(intDeque0.popL());
        REQUIRE(intDeque0.size() == 0);
        REQUIRE(intDeque0.isEmpty() == 1);
    }
    expected0.push_back(2);
    expected0.push_back(3);
    expected0.push_back(4);
    REQUIRE(result0 == expected0);

    /* TEST: pushR 10 popR 10 */
    Deque<int> intDeque1;
    vector<int> result1;
    vector<int> expected1;

    /* pushR 10 */
    for (int i = 3; i < 13; i++) {
        intDeque1.pushR(i);
    }
    /* popR 10 */
    int expectedSize = 10;
    while (!intDeque1.isEmpty()) {
        REQUIRE(intDeque1.size() == expectedSize);
        result1.push_back(intDeque1.popR());
        expectedSize--;
    }
    for (int j = 12; j > 2; j--) {
    expected1.push_back(j);
    }
    REQUIRE(result1 == expected1);

    /* TEST: pushR 10 popR 5 popL 2 popR 1 */
    Deque<int> intDeque2;
    vector<int> result2;
    vector<int> expected2;
    
    /* pushR 10 */
    cout << "peekR 10:";
    for (int i = 0; i < 10; i++) {
        intDeque2.pushR(i);
        cout << intDeque2.peekR() << " ";
    }
    cout << endl;
    /* popR 5 */
        cout << "popR 5: ";
    for (int j = 0; j < 5; j++) {
        cout << intDeque2.popR() << " ";
    }
    cout << endl;
    /* popL 2 */
    cout << "popL 2: ";
    for (int k = 0; k < 2; k++) {
        cout << intDeque2.popL() << " ";
    }
    cout << endl;
    /* popR */
    cout << "popR: " << intDeque2.popR() << endl;

    /* Expected: {2, 3} */
    REQUIRE(intDeque2.size() == 2);
    REQUIRE(intDeque2.isEmpty() == 0);
    /* => {3} */
    if (!intDeque2.isEmpty()) {
        result2.push_back(intDeque2.popL());
        REQUIRE(intDeque2.size() == 1);
        REQUIRE(intDeque2.isEmpty() == 0);
    }
    /* => {} */
    if (!intDeque2.isEmpty()) {
        result2.push_back(intDeque2.popL());
        REQUIRE(intDeque2.size() == 0);
        REQUIRE(intDeque2.isEmpty() == 1);
    }
    expected2.push_back(2);
    expected2.push_back(3);
    REQUIRE(result2 == expected2);
}