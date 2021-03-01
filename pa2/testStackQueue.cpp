#define CATCH_CONFIG_MAIN
#include <iostream>
#include "cs221util/catch.hpp"
#include "stack.h"
#include "queue.h"
#include "deque.h"
//using namespace cs221util;
using namespace std;

// TEST_CASE("stack::basic functions","[weight=1][part=stack]"){
//     //cout << "Testing Stack..." << endl;
//     Stack<int> intStack;
//     vector<int> result;
//     vector<int> expected;
//     for (int i = 10; i > 0; i--) {
//         expected.push_back(i);
//     }
//     for (int i = 1; i <= 10; i++) {
//         intStack.push(i);
//     }
//     while (!intStack.isEmpty()) {
//         result.push_back(intStack.pop());
//     }
//     REQUIRE( result == expected);
// }

// TEST_CASE("queue::basic functions","[weight=1][part=queue]"){
//     Queue<int> intQueue;
//     vector<int> result;
//     vector<int> expected;
//     for (int i = 1; i <= 10; i++) {
//         expected.push_back(i);
//     }
//     for (int i = 1; i <= 10; i++) {
//         intQueue.enqueue(i);
//     }
//     while (!intQueue.isEmpty()) {
//         result.push_back(intQueue.dequeue());
//     }
//     REQUIRE( result == expected);
// }

TEST_CASE("deque::basic functions","[weight=1][part=deque]") {

    Deque<int> intDeque;
    vector<int> result;
    vector<int> expected;

    /* pushR 10 */
    for (int i = 0; i < 10; i++) {
        intDeque.pushR(i);
    }
    /* popR 5 */
    cout << "popR: ";
    for (int j = 0; j < 5; j++) {
        cout << intDeque.popR() << " ";
    }
    cout << endl;
    /* popL 2 */
    cout << "popL: ";
    for (int k = 0; k < 2; k++) {
        cout << intDeque.popL() << " ";
    }
    cout << endl;

    /* Expected: {0, 0, 2, 3, 4} */
    REQUIRE(intDeque.size() == 5);
    REQUIRE(intDeque.isEmpty() == 0);
    /* => {3, 4} */
    if (!intDeque.isEmpty()) {
        result.push_back(intDeque.popL());
        REQUIRE(intDeque.size() == 2);
        REQUIRE(intDeque.isEmpty() == 0);
    }
    /* => {4} */
    if (!intDeque.isEmpty()) {
        result.push_back(intDeque.popL());
        REQUIRE(intDeque.size() == 1);
        REQUIRE(intDeque.isEmpty() == 0);
    }
    /* => {} */
    if (!intDeque.isEmpty()) {
        result.push_back(intDeque.popL());
        REQUIRE(intDeque.size() == 0);
        REQUIRE(intDeque.isEmpty() == 1);
    }
    expected.push_back(2);
    expected.push_back(3);
    expected.push_back(4);
    REQUIRE(result == expected);
}