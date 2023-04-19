/**
 * @author Half_nothing
 * @date 2023.02
 * @class template class BasicOperation
 */

#pragma once
#ifndef BASIC_OPERATION_H
#define BASIC_OPERATION_H

#include "ListBase.h"
#include "DataStructure.h"
#include <filesystem>
#include <fstream>
#include <io.h>

template<typename T>
class BasicOperation : public ListBase<T> {
public:
    explicit BasicOperation(std::string_view fileName) {
        // _access �������ָ��Ŀ¼�Ƿ���ڣ�������ز�����0˵��������
        if (_access("data", 0) != 0) {
            mkdir("data"); // ����ϵͳ��������Ŀ¼
        }
        // ����һ��path���ͱ�������������·��������Ŀ¼������./data����
        std::filesystem::path temp(".\\data");
        // ����Ŀ¼��
        temp /= fileName;
        // ����Ŀ¼��
        this->pathName = temp;
        // ��ȡ�ļ�����
        this->readFile();
    }

    ~BasicOperation() = default;

    void saveData() {
        // ��һ�������, ��Ϊ���е����ݶ���vector����,����ʹ��std::ios::out������std::ios::app
        std::fstream output(this->pathName, std::ios::out | std::ios::binary);
        // �ж��ļ��Ƿ�������
        if (output.is_open()) {
            // ����vector���飬����Ҳ�����õ����������±굫����foreach��ѭ��(>=C++11)���һ��
            for (const auto &temp: this->data) {
                output.write((char *) &temp, sizeof(T));
            }
        }
        // �ر��ļ���
        output.close();
    }

    virtual void addHandler() {};

    virtual void delHandler() {};

    virtual void changeHandler() {};

    void searchHandler() {
        using namespace std;
        cout << "����q���˳�" << endl;
        cout << "�����������ؼ���: " << endl;
        string target;
        cin >> target;
        if (target == "q") { return; }
        this->search(target);
        PAUSE;
    }

    void clearHandler() {
        using namespace std;
        cout << "ȷ�����������(y/n)" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        if (target == "yes" || target == "y") {
            cout << "���棡ȷ������������ݣ�(y/n)" << endl;
            cin >> target;
            cin.ignore();
            std::transform(target.begin(), target.end(), target.begin(), ::tolower);
            if (target == "yes" || target == "y") {
                this->clear();
                PAUSE;
            } else {
                cout << "��ȡ������" << endl;
                PAUSE;
            }
        }
    }

    void batchImportHandler() {
        std::string filename;
        std::cout << "�������ļ�·��" << std::endl;
        std::cin >> filename;
        std::cin.ignore();
        this->batchImport(std::filesystem::path(filename));
    }

protected:
    std::filesystem::path pathName; // �ļ��洢·��

    virtual void calculate() {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) {
            this->totalPage++;
        }
        this->nowPage = 1;
    }

private:
    virtual void add(T &target) = 0;

    virtual void del(typename std::vector<T>::iterator target) {};

    virtual void search(const std::string &target) = 0;

    virtual void change(typename std::vector<T>::iterator target) {};

    virtual void batchImport(std::filesystem::path path) = 0;

    virtual void clear() = 0;

    void readFile() {
        // ��һ��������
        std::fstream input(this->pathName, std::ios::in | std::ios::binary);
        // ����ļ�û�д򿪣�������ļ�������
        if (!input.is_open()) {
            // ���ȹر��ļ���
            input.close();
            // ��һ���������std::ios::out��û���ҵ��ļ���ʱ��ش���һ���ļ�
            std::fstream temp(this->pathName, std::ios::out | std::ios::binary);
            // �ر�����������ļ��������
            temp.close();
            return;
        }
        T temp;                     // ����һ����ʱ����
        while (input.peek() != EOF) // ���û�ж�ȡ���ļ�β
        {
            // ��ȡһ����Ԫ
            input.read((char *) &temp, sizeof(T));
            // ����vector
            this->data.push_back(temp);
        }
        // �ر�������
        input.close();
    }
};

#endif
