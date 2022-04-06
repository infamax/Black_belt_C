#include "test_runner.h"

/*
 * Реализуйте шаблонную функцию EqualsToOneOf,
 * возвращающую true тогда и только тогда, когда
 * ее первый аргумент равен хотя бы одному из
 * последующих аргументов. Примеры:
 * EqualsToOneOf(1, 2, 3, 4) == false — первый параметр 1 не равен ни одному из последующих параметров;
 * EqualsToOneOf(1, 3, 1) == true — первый параметр 1 равен третьему параметру.
 */


template<typename T>
bool EquasToOneOf(const T& first, const T& second) {
    return first == second;
}

template<typename T, typename ...Args>
bool EqualsToOneOf(const T& first, const Args& ...args) {
    if (sizeof...(args) == 0) {
        return false;
    }
    bool res = false;
    ((res |= first == args), ...);
    return res;
}

template<typename T>
T Sum(const T& x) {
    return x;
}

template<typename T, typename ...Args>
T Sum(const T& x, const Args&... args) {
    return x + Sum(args...);
}

void Test() {
    auto x = "pear";
    ASSERT(EqualsToOneOf(x, "pear"));
    ASSERT(!EqualsToOneOf(x, "apple"));
    ASSERT(EqualsToOneOf(x, "apple", "pear"));
    ASSERT(!EqualsToOneOf(x, "apple", "banana"));
    ASSERT(EqualsToOneOf(x, "apple", "banana", "pear"));
    ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach"));
    ASSERT(EqualsToOneOf(x, "apple", "banana", "pear", "orange"));
    ASSERT(!EqualsToOneOf(x, "apple", "banana", "peach", "orange"));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);
    //std::cout << Sum(1, 2, 3, 4) << std::endl;
    return 0;
}