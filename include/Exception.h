#include <utility>

/**
 * @author Half_nothing
 * @data 2023.04
 * @class Exception
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

class TargetNotFoundException : public std::exception{
public:
    [[nodiscard]] const char *what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "Target not found!";
    }
};

#endif
