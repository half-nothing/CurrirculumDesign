/**
 * @author Half_nothing
 * @date 2023.02
 * @class ChangeBase
 * @details 修改信息的抽象基类
 * @include MenuBase
 */
#pragma once
#ifndef CHANGE_BASE_H
#define CHANGE_BASE_H

#include "MenuBase.h"

template<typename T>
class ChangeBase : public MenuBase {
public:
    explicit
    ChangeBase(typename std::vector<T>::iterator target,
               const std::string &title,
               std::vector<std::string> menuList)
            : target(target), MenuBase(title, menuList) {}

    ~ChangeBase() = default;

protected:
    typename std::vector<T>::iterator target;
};

#endif
