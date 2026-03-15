#ifndef INCLUDE_TEST_H
#define INCLUDE_TEST_H


class Test {
public:
    virtual ~Test() = default;
    virtual int test() = 0;
};


#endif  // INCLUDE_TEST_H