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
        // _access 用来检查指定目录是否存在，如果返回不等于0说明不存在
        if (_access("data", 0) != 0) {
            mkdir("data"); // 调用系统函数创建目录
        }
        // 定义一个path类型变量，用来处理路径，并将目录设置在./data下面
        std::filesystem::path temp(".\\data");
        // 连接目录名
        temp /= fileName;
        // 复制目录名
        this->pathName = temp;
        // 读取文件内容
        this->readFile();
    }

    ~BasicOperation() = default;

    void saveData() {
        // 打开一个输出流, 因为所有的内容都在vector里面,所以使用std::ios::out而不是std::ios::app
        std::fstream output(this->pathName, std::ios::out | std::ios::binary);
        // 判断文件是否正常打开
        if (output.is_open()) {
            // 遍历vector数组，这里也可以用迭代器或者下标但是用foreach类循环(>=C++11)会简单一点
            for (const auto &temp: this->data) {
                output.write((char *) &temp, sizeof(T));
            }
        }
        // 关闭文件流
        output.close();
    }

    virtual void addHandler() {};

    virtual void delHandler() {};

    virtual void changeHandler() {};

    void searchHandler() {
        using namespace std;
        cout << "输入q键退出" << endl;
        cout << "请输入搜索关键字: " << endl;
        string target;
        cin >> target;
        if (target == "q") { return; }
        this->search(target);
        PAUSE;
    }

    void clearHandler() {
        using namespace std;
        cout << "确定清空数据吗？(y/n)" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        if (target == "yes" || target == "y") {
            cout << "警告！确定清空所有数据？(y/n)" << endl;
            cin >> target;
            cin.ignore();
            std::transform(target.begin(), target.end(), target.begin(), ::tolower);
            if (target == "yes" || target == "y") {
                this->clear();
                PAUSE;
            } else {
                cout << "已取消操作" << endl;
                PAUSE;
            }
        }
    }

    void batchImportHandler() {
        std::string filename;
        std::cout << "请输入文件路径" << std::endl;
        std::cin >> filename;
        std::cin.ignore();
        this->batchImport(std::filesystem::path(filename));
    }

protected:
    std::filesystem::path pathName; // 文件存储路径

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
        // 打开一个输入流
        std::fstream input(this->pathName, std::ios::in | std::ios::binary);
        // 如果文件没有打开，则代表文件不存在
        if (!input.is_open()) {
            // 首先关闭文件流
            input.close();
            // 打开一个输出流，std::ios::out在没有找到文件的时候回创建一个文件
            std::fstream temp(this->pathName, std::ios::out | std::ios::binary);
            // 关闭输出流，空文件创建完成
            temp.close();
            return;
        }
        T temp;                     // 定义一个临时变量
        while (input.peek() != EOF) // 如果没有读取到文件尾
        {
            // 读取一个单元
            input.read((char *) &temp, sizeof(T));
            // 推入vector
            this->data.push_back(temp);
        }
        // 关闭输入流
        input.close();
    }
};

#endif
