/**
 * @author Half_nothing
 * @date 2023.02
 * @details 实现目录切换的模板基类
 */
#pragma once
#ifndef LIST_BASE_H
#define LIST_BASE_H

#include "Definitions.h"
#include "../Exception.h"
#include <iostream>
#include <utility>
#include <vector>
#include <string>

template<typename T>
class ListBase {
public:
    ListBase() = default;

    ~ListBase() = default;

    void listHandler() {
        this->printPage();
        while (true) {
            char temp;
            while (true) {
                if ((temp = (char) getch()) == 'q') {
                    return;
                }
                temp = (char) getch();
                if (temp == LEFT_KEY) {
                    this->prevPage();
                }
                if (temp == RIGHT_KEY) {
                    this->nextPage();
                }
            }
        }
    }

protected:
    std::vector<T> data;
    std::vector<uint> inUse;        // 存储没有被删除的下标，用来输出页码
    uint totalPage = 1;
    uint nowPage = 1;
    uint linePerPage = LINE_PER_PAGE;

private:
    void printPage() {
        using namespace std;
        CLEAR;
        if (this->inUse.empty()) {
            cout << "未查询到记录" << endl;
            return;
        }
        // 计算开始的下标
        uint start = this->nowPage * this->linePerPage - this->linePerPage;
        cout << "---------------------------------------" << endl;
        T::printTableHeader();
        for (int i = 0; i < this->linePerPage; ++i) {
            // 如果超过没有被删除的数量
            // 或者取出的下表超过了所有存储的数量
            // 则退出循环
            if (start + i >= this->inUse.size() || this->inUse[start + i] >= this->data.size()) {
                break;
            }
            // 否则就输出一遍信息
            this->data[this->inUse[start + i]].printInfo();
        }
        std::cout << "------------第 " << this->nowPage << " 页 共 " << this->totalPage << " 页------------"
                  << std::endl;
        std::cout << "------使用左右方向键换页,q键退出-------" << std::endl;
    }

    void nextPage() {
        if (this->nowPage < this->totalPage) {
            this->nowPage++;
            this->printPage();
        }
    }

    void prevPage() {
        if (this->nowPage > 1) {
            this->nowPage--;
            this->printPage();
        }
    }
};

#endif
