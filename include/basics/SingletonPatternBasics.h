/**
 * @author Half_nothing
 * @data 2023.03
 * @class SingletonPatternBasics
 */
#pragma once
#ifndef BASE_H
#define BASE_H

#include "string_view"

template<class T>
class SingletonPatternBasics {
public:
    static T *getInstance() {
        return T::instance;
    }

    static void generateInstance(std::string_view fileName) {
        if (T::instance == nullptr) {
            T::instance = new T(fileName);
        }
    }

    static void generateInstance() {
        if (T::instance == nullptr) {
            T::instance = new T();
        }
    }

    static void deleteInstance() {
        // É¾³ýnewµÄÊµÀý·ÀÖ¹ÄÚ´æÐ¹Â©
        delete T::instance;
        T::instance = nullptr;
    }
private:
    static T *instance;
};

template<class T>
T *SingletonPatternBasics<T>::instance = nullptr;

#endif
