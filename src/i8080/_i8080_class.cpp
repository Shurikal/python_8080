#include <Python.h>
#include <iostream>
#include <cstring>
#include "_i8080_class.hpp"


i8080C::i8080C() : m_cnt(0) {
    std::cout << "i8080C::i8080C() called" << std::endl;
}


i8080C::~i8080C() {
    std::cout << "i8080C::~i8080C() called" << std::endl;
}

unsigned long i8080C::addOne() {
    m_cnt++;
    return m_cnt;
}