/**
 * @author Half_nothing
 * @date 2023.02
 * @details ʵ��Ŀ¼�л���ģ�����
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
    std::vector<uint> inUse;        // �洢û�б�ɾ�����±꣬�������ҳ��
    uint totalPage = 1;
    uint nowPage = 1;
    uint linePerPage = LINE_PER_PAGE;

private:
    void printPage() {
        using namespace std;
        CLEAR;
        if (this->inUse.empty()) {
            cout << "δ��ѯ����¼" << endl;
            return;
        }
        // ���㿪ʼ���±�
        uint start = this->nowPage * this->linePerPage - this->linePerPage;
        cout << "---------------------------------------" << endl;
        T::printTableHeader();
        for (int i = 0; i < this->linePerPage; ++i) {
            // �������û�б�ɾ��������
            // ����ȡ�����±��������д洢������
            // ���˳�ѭ��
            if (start + i >= this->inUse.size() || this->inUse[start + i] >= this->data.size()) {
                break;
            }
            // ��������һ����Ϣ
            this->data[this->inUse[start + i]].printInfo();
        }
        std::cout << "------------�� " << this->nowPage << " ҳ �� " << this->totalPage << " ҳ------------"
                  << std::endl;
        std::cout << "------ʹ�����ҷ������ҳ,q���˳�-------" << std::endl;
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
