/**
 * @author Half_nothing
 * @date 2023.02
 * @details ��ʦ����ĺ���ʵ��
 */
#pragma once
#ifndef TEACHER_MANAGER_H
#define TEACHER_MANAGER_H

#include "../basics/BasicOperation.h"
#include "../basics/ChangeBase.h"
#include "../basics/SingletonPatternBasics.h"

class TeacherManager : public BasicOperation<Teacher>, public SingletonPatternBasics<TeacherManager> {
    friend class SingletonPatternBasics;

public:
    TeacherManager() = delete;

    TeacherManager(TeacherManager &) = delete;

    TeacherManager(TeacherManager &&) = delete;

    TeacherManager &operator=(const TeacherManager &) = delete;

    std::string getTeacherIdByName(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Teacher &temp) {
            return temp.usageStatus && temp.teacherName == target;
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return (*it).teacherId;
    }

    inline std::vector<std::string> &getTeacherNameList() {
        return this->teacherNameList;
    }

    void addHandler() override {
        using namespace std;
        cout << "����¼���ʦ(����0�˳�):" << endl;
        Teacher target;
        cout << "�������ʦ����:" << endl;
        cin >> target.teacherName;
        if (std::string(target.teacherName) == "0") { return; }
        cout << "�������Ա�:" << endl;
        while (true) {
            cin >> target.sex;
            if (std::string(target.sex) == "0") { return; }
            if (std::string(target.sex) != "��" && std::string(target.sex) != "Ů") {
                cout << "�Ա�ֻ�����л�Ů,����������" << endl;
                continue;
            }
            break;
        }
        cout << "�������ʦ��:" << endl;
        while (true) {
            cin >> target.teacherId;
            if (string(target.teacherId) == "0") { return; }
            auto it = this->getIteratorById(target.teacherId);
            if (it != this->data.end()) {
                cout << "�˽�ʦ���ѱ�ռ�ã�����������" << endl;
                continue;
            }
            break;
        }
        this->add(target);
        PAUSE;
    }

    void delHandler() override {
        using namespace std;
        string target;
        cout << "����q���˳�" << endl;
        cout << "����listչ��ѡ��˵�" << endl;
        cout << "������Ҫɾ���Ľ�ʦID" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getTeacherIdByName(MenuBase(" �б� ", this->teacherNameList, "").get());
            } catch (TargetNotFoundException &_) {
                cout << "ȡ��ѡ��" << endl;
                PAUSE;
                return;
            }
        }
        auto it = this->getIteratorById(target);
        if (it == this->data.end()) {
            cout << "�޷��ҵ���Ӧ��Ŀ" << endl;
            PAUSE;
            return;
        }
        cout << "��ѯ����Ŀ:" << endl;
        Teacher::printTableHeader();
        (*it).printInfo();
        cout << "ȷ��ɾ��(y/n)?" << endl;
        cin >> target;
        std::transform(target.begin(), target.end(), target.begin(), ::tolower);
        if (target == "yes" || target == "y") {
            this->del(it);
            PAUSE;
            return;
        }
        cout << "��ȡ��" << endl;
        PAUSE;
    }

    void changeHandler() override {
        using namespace std;
        cout << "����q���˳�" << endl;
        cout << "����listչ��ѡ��˵�" << endl;
        cout << "������Ҫ�޸ĵĽ�ʦID:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getTeacherIdByName(MenuBase(" �б� ", this->teacherNameList, "").get());
            } catch (TargetNotFoundException &_) {
                cout << "ȡ��ѡ��" << endl;
                PAUSE;
                return;
            }
        }
        auto it = this->getIteratorById(target);
        if (it == this->data.end()) {
            cout << "�޷��ҵ���Ӧ����Ŀ" << endl;
            PAUSE;
            return;
        }
        if (!(*it).usageStatus) {
            cout << "����Ŀ�ѱ�ɾ��" << endl;
            PAUSE;
            return;
        }
        this->change(it);
    }

private:
    std::vector<std::string> teacherNameList;

    inline bool checkTeacherId(const std::string &target) {
        return std::any_of(this->inUse.begin(), this->inUse.end(), [target, this](const auto &index){
            return this->data[index].usageStatus && this->data[index].teacherId == target;
        });
    }

    inline std::vector<Teacher>::iterator getIteratorById(const std::string &id) {
        return std::find_if(this->data.begin(), this->data.end(), [id](const Teacher &temp) {
            return temp.usageStatus && temp.teacherId == id;
        });
    }

    void batchImport(std::filesystem::path path) override {
        std::fstream input(path, std::ios::in);
        if (!input.is_open()) {
            std::cout << "�ļ�������" << std::endl;
            PAUSE;
            return;
        }
        std::string line;
        while (getline(input, line)) {
            std::string field;
            std::istringstream sin(line);
            Teacher temp;
            getline(sin, field, ',');
            ::strcpy(temp.teacherName, field.c_str());
            getline(sin, field, ',');
            ::strcpy(temp.teacherId, field.c_str());
            if (this->checkTeacherId(field)) {
                std::cout << "��ʦ��[" << field << "]�ظ�" << std::endl
                          << "��ӽ�ʦ[" << temp.teacherName << "]ʧ��" << std::endl;
                continue;
            }
            getline(sin, field);
            ::strcpy(temp.sex, field.c_str());
            if (field != "��" && field != "Ů") {
                std::cout << "�Ա�[" << temp.teacherId << "]ֻ����\"��\"��\"Ů\"" << std::endl
                          << "���ѧ��[" << temp.teacherName << "]ʧ��" << std::endl;
                continue;
            }
            this->add(temp);
        }
        input.close();
        PAUSE;
    }

    void add(Teacher &target) override {
        target.usageStatus = true;
        target.storageId = this->data.size() + 1;
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
        std::cout << "��ӽ�ʦ[" << target.teacherName << "]�ɹ�" << std::endl;
    }

    void del(std::vector<Teacher>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "ɾ����ʦ[" << (*target).teacherName << "]�ɹ�" << std::endl;
    }

    void change(std::vector<Teacher>::iterator target) override {
        CLEAR;
        ChangeInfo temp(target);
        temp.menuHandler();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            const Teacher &temp = this->data[index];
            if (std::string(temp.teacherName).find(target) != std::string::npos ||
                std::string(temp.teacherId).find(target) != std::string::npos) {
                res.push_back(temp.storageId);
            }
        }
        if (res.empty()) {
            std::cout << "δ��ѯ����¼" << std::endl;
            PAUSE;
            return;
        }
        std::cout << "�ҵ����½��: " << std::endl
                  << "---------------------------------------" << std::endl;
        Teacher::printTableHeader();
        for (const auto &temp: res) { this->data[temp - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "�����" << this->data.size() << "������" << std::endl;
        this->data.clear();
        this->inUse.clear();
        this->teacherNameList.clear();
    }

    explicit TeacherManager(std::string_view pathName) : BasicOperation<Teacher>(pathName) {
        for (uint i = 0; i < this->data.size(); i++) {
            if (this->data[i].usageStatus) {
                this->inUse.push_back(i);
            }
        }
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) {
            this->totalPage++;
        }
        this->nowPage = 1;
        this->teacherNameList.clear();
        for (auto &temp: this->inUse) {
            this->teacherNameList.emplace_back(this->data[temp].teacherName);
        }
        this->teacherNameList.emplace_back("ȡ��ѡ��");
    }

    ~TeacherManager() = default;

    void calculate() override {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) {
            this->totalPage++;
        }
        this->nowPage = 1;
        this->teacherNameList.clear();
        for (auto &temp: this->inUse) {
            this->teacherNameList.emplace_back(this->data[temp].teacherName);
        }
        this->teacherNameList.emplace_back("ȡ��ѡ��");
    }

    class ChangeInfo : public ChangeBase<Teacher> {
    public:
        explicit ChangeInfo(std::vector<Teacher>::iterator target) : ChangeBase<Teacher>(target, "�޸��б�",
                                                                                         {"1.�޸�����",
                                                                                          "2.�޸��Ա�",
                                                                                          "3.�޸Ľ�ʦ��",
                                                                                          "4.�˳��޸�"}) {}

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            cout << "����0������һ��" << endl;
            switch (this->line) {
                case 1: {
                    cout << "ԭ��������Ϊ��" << this->target->teacherName << endl;
                    cout << "�����������֣�" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->teacherName) {
                        cout << "��ԭ��ʦ��һ��" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->teacherName, target.c_str());
                    cout << "�޸ĺ������Ϊ��" << this->target->teacherName << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "ԭ�����Ա���:" << this->target->sex << endl;
                    cout << "�������Ա�" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target != "��" && target != "Ů") {
                        cout << "�Ա�ֻ�����л�Ů" << endl;
                        PAUSE;
                        break;
                    }
                    if (target == this->target->sex) {
                        cout << "��ԭ�Ա�һ��" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->sex, target.c_str());
                    cout << "���Ա�Ϊ��" << this->target->sex << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    cout << "ԭ���Ľ�ʦ����:" << this->target->teacherId << endl;
                    cout << "�������½�ʦ��:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->teacherId) {
                        cout << "��ԭ��ʦ����ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    if (TeacherManager::getInstance()->checkTeacherId(target)) {
                        cout << "�ý�ʦ���ѱ�ʹ�ã�����������" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->teacherId, target.c_str());
                    cout << "�½�ʦ��Ϊ:" << this->target->teacherId << endl;
                    PAUSE;
                    break;
                }
                case 4:
                    return true;
                default:
                    break;
            }
            return false;
        }
    };
};

#endif
