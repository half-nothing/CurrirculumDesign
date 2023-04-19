/**
 * @author Half_nothing
 * @date 2023.02
 * @class CourseManager
 * @details �γ̹���ĺ���ʵ��
 */
#pragma once
#ifndef COURSE_MANAGER_H
#define COURSE_MANAGER_H

#include <utility>
#include "../basics/BasicOperation.h"
#include "../basics/SingletonPatternBasics.h"
#include "TeacherManager.h"

// ���ﲻ��ģ���࣬����Ҫ�̳��ػ��˵�BasicOperation��
// �ػ�������Course,��DataType::CourseInfo
class CourseManager : public BasicOperation<Course>, public SingletonPatternBasics<CourseManager> {
    friend class SingletonPatternBasics;

public:
    CourseManager() = delete;

    CourseManager(CourseManager &) = delete;

    CourseManager(CourseManager &&) = delete;

    CourseManager &operator=(const CourseManager &) = delete;

    std::string getCourseIdByName(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Course &temp) {
            return temp.usageStatus && temp.lessonName == target;
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return it->lessonId;
    }

    Course &getCourseByNameOrId(const std::string &target) {
        auto it = std::find_if(this->data.begin(), this->data.end(), [target](const Course &temp) {
            return temp.usageStatus && (temp.lessonId == target || temp.lessonName == target);
        });
        if (it == this->data.end()) {
            throw TargetNotFoundException();
        }
        return *it;
    }

    std::vector<std::string> &getCourseName() { return this->courseName; }

    inline bool checkLessonId(const std::string &target) {
        return std::any_of(this->inUse.begin(), this->inUse.end(), [target, this](const auto &item){
            return this->data[item].usageStatus && (this->data[item].lessonId == target || this->data[item].lessonName == target);
        });
    }

    void addHandler() override {
        using namespace std;
        Course temp;
        cout << "������ӿγ�(����0�˳�):" << endl
             << "�����뿪ʼʱ��(1900<=x):" << endl;
        while (true) {
            cin >> temp.startData;
            if (temp.startData == 0) { return; }
            if (temp.startData < 1900) {
                cout << "��ʼʱ�������ڻ����1900�꣬����������" << endl;
                continue;
            }
            break;
        }
        cout << "���������ʱ��(x<=y):" << endl;
        while (true) {
            cin >> temp.endData;
            if (temp.endData == 0) { return; }
            if (temp.endData < temp.startData) {
                cout << "����ʱ�����С�ڻ���ڿ�ʼʱ�䣬����������" << endl;
                continue;
            }
            break;
        }
        cout << "�����뿪��ѧ��(1/2):" << endl;
        while (true) {
            cin >> temp.openTerm;
            if (temp.openTerm == '\n') { continue; }
            if (temp.openTerm == '0') { return; }
            if (temp.openTerm != '1' && temp.openTerm != '2') {
                cout << "ѧ��ֻ����1����2������������" << endl;
                continue;
            }
            break;
        }
        cout << "������γ�����:" << endl;
        cin >> temp.lessonName;
        if (std::string(temp.lessonName) == "0") { return; }
        cout << "������γ�ID" << endl;
        while (true) {
            cin >> temp.lessonId;
            cin.ignore();
            if (string(temp.lessonId) == "0") { return; }
            if (this->checkLessonId(temp.lessonId)) {
                cout << "�˿γ�ID�ѱ�ռ�ã�����������" << endl;
                continue;
            }
            break;
        }
        CLEAR;
        std::string res = std::string(
                MenuBase(" �б� ", TeacherManager::getInstance()->getTeacherNameList(), "").get());
        cout << "������ӿγ�(����0�˳�):" << endl;
        cout << "�����뿪ʼʱ��(1900<=x):" << endl << temp.startData << endl;
        cout << "���������ʱ��(x<=y):" << endl << temp.endData << endl;
        cout << "�����뿪��ѧ��(1/2):" << endl << temp.openTerm << endl;
        cout << "������γ�����" << endl << temp.lessonName << endl;
        cout << "������γ�ID" << endl << temp.lessonId << endl;
        cout << "��ѡ���ڿν�ʦ:" << endl;
        if (res == "ȡ��ѡ��") {
            PAUSE;
            return;
        }
        ::strcpy(temp.teacherName, res.c_str());
        res = TeacherManager::getInstance()->getTeacherIdByName(res);
        ::strcpy(temp.teacherId, res.c_str());
        cout << temp.teacherName << endl;
        this->add(temp);
        PAUSE;
    }

    void delHandler() override {
        using namespace std;
        std::string target;
        cout << "����q���˳�" << endl;
        cout << "����listչ��ѡ��˵�" << endl;
        cout << "������Ҫɾ���Ŀγ�ID" << endl;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getCourseIdByName(MenuBase(" �б� ", this->courseName, "").get());
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
        cout << "��ѯ����Ŀ:" << endl;
        Course::printTableHeader();
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
        cout << "������Ҫ�޸ĵĿγ�ID:" << endl;
        std::string target;
        cin >> target;
        cin.ignore();
        if (target == "q") { return; }
        if (target == "list") {
            CLEAR;
            try {
                target = this->getCourseIdByName(MenuBase(" �б� ", this->courseName, "").get());
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
    std::vector<std::string> courseName;

    inline std::vector<Course>::iterator getIteratorById(const std::string &id) {
        return std::find_if(this->data.begin(), this->data.end(), [id](const Course &temp) {
            return temp.usageStatus && temp.lessonId == id;
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
            Course temp;
            getline(sin, field, ',');
            temp.startData = ::strtol(field.c_str(), nullptr, 10);
            getline(sin, field, ',');
            temp.endData = ::strtol(field.c_str(), nullptr, 10);
            getline(sin, field, ',');
            temp.openTerm = field.c_str()[0];
            getline(sin, field, ',');
            ::strcpy(temp.lessonName, field.c_str());
            getline(sin, field, ',');
            ::strcpy(temp.lessonId, field.c_str());
            getline(sin, field);
            ::strcpy(temp.teacherName, field.c_str());
            if (1900 > temp.startData) {
                std::cout << "��ʼ����[" << temp.startData << "]С��1900��" << std::endl;
                std::cout << "��ӿγ�[" << temp.lessonName << "]ʧ��" << std::endl;
                continue;
            }
            if (temp.endData < temp.startData) {
                std::cout << "��ʼ����[" << temp.startData << "]���ڽ�������[" << temp.endData << "]" << std::endl;
                std::cout << "��ӿγ�[" << temp.lessonName << "]ʧ��" << std::endl;
                continue;
            }
            if (temp.openTerm != '1' && temp.openTerm != '2') {
                std::cout << "ѧ��[" << temp.openTerm << "]ֻ����1��2" << std::endl;
                std::cout << "��ӿγ�[" << temp.lessonName << "]ʧ��" << std::endl;
                continue;
            }
            if (this->checkLessonId(temp.lessonId)) {
                std::cout << "�γ�ID[" << temp.lessonId << "]�ѱ�ռ��" << std::endl;
                std::cout << "��ӿγ�[" << temp.lessonName << "]ʧ��" << std::endl;
                continue;
            }
            try {
                ::strcpy(temp.teacherId, TeacherManager::getInstance()->getTeacherIdByName(temp.teacherName).c_str());
            } catch (TargetNotFoundException &_) {
                std::cout << "�޷��ҵ���ʦ[" << temp.teacherName << "]�Ľ�ʦ��" << std::endl;
                std::cout << "��ӿγ�[" << temp.lessonName << "]ʧ��" << std::endl;
                continue;
            }
            this->add(temp);
        }
        input.close();
        PAUSE;
    }

    void add(Course &target) override {
        target.generateName();
        target.usageStatus = true;
        target.storageId = this->data.size() + 1;
        this->inUse.push_back(this->data.size());
        this->data.push_back(target);
        this->calculate();
        std::cout << "��ӿγ�[" << target.lessonName << "]�ɹ�" << std::endl;
    }

    void del(std::vector<Course>::iterator target) override {
        (*target).usageStatus = false;
        this->inUse.erase(std::find(this->inUse.begin(), this->inUse.end(), (*target).storageId - 1));
        this->calculate();
        std::cout << "ɾ���γ�[" << (*target).lessonName << "]�ɹ�" << std::endl;
    }

    void change(std::vector<Course>::iterator target) override {
        CLEAR;
        ChangeInfo temp(target);
        temp.menuHandler();
    }

    void search(const std::string &target) override {
        std::vector<uint> res;
        for (auto &index: this->inUse) {
            Course &temp = this->data[index];
            if (std::string(temp.lessonName).find(target) != std::string::npos ||
                std::string(temp.lessonId).find(target) != std::string::npos ||
                std::string(temp.teacherName).find(target) != std::string::npos) {
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
        Course::printTableHeader();
        for (const auto x: res) { this->data[x - 1].printInfo(); }
        std::cout << "---------------------------------------" << std::endl;
    }

    void clear() override {
        std::cout << "�����" << this->data.size() << "������" << std::endl;
        this->data.clear();
        this->inUse.clear();
        this->courseName.clear();
    }

    explicit CourseManager(std::string_view fileName) : BasicOperation<Course>(fileName) {
        for (const auto &temp: this->data) {
            if (temp.usageStatus) { this->inUse.push_back(temp.storageId - 1); }
        }
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) { this->totalPage++; }
        this->nowPage = 1;
        this->courseName.clear();
        for (auto &temp: this->inUse) { this->courseName.emplace_back(this->data[temp].lessonName); }
        this->courseName.emplace_back("ȡ��ѡ��");
    }

    ~CourseManager() = default;

    void calculate() override {
        this->totalPage = this->inUse.size() / this->linePerPage;
        if (this->inUse.size() % this->linePerPage != 0) { this->totalPage++; }
        this->nowPage = 1;
        this->courseName.clear();
        for (auto &temp: this->inUse) { this->courseName.emplace_back(this->data[temp].lessonName); }
        this->courseName.emplace_back("ȡ��ѡ��");
    }

    class ChangeInfo : public ChangeBase<Course> {
    public:
        explicit
        ChangeInfo(std::vector<Course>::iterator target)
                : ChangeBase<Course>(target, "�޸��б�",
                                     {"1.�γ�����", "2.�γ�ID", "3.��ʦID", "4.��ʼ���", "5.�������", "6.����ѧ��",
                                      "7.�˳��޸�"}) {};

        ~ChangeInfo() = default;

    private:
        bool choiceHandler() override {
            using namespace std;
            cout << "����0������һ��" << endl;
            switch (this->line) {
                case 1: {
                    cout << "ԭ�γ�����Ϊ: " << this->target->lessonName << endl;
                    cout << "�������¿γ�����:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->lessonName) {
                        cout << "��ԭ���γ���һ��" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->lessonName, target.c_str());
                    cout << "�޸ĺ�Ŀγ�������:" << this->target->lessonName << endl;
                    PAUSE;
                    break;
                }
                case 2: {
                    cout << "ԭ�γ�IDΪ: " << this->target->lessonId << endl;
                    cout << "�������¿γ�ID:" << endl;
                    string target;
                    cin >> target;
                    if (target == "0") { break; }
                    if (target == this->target->lessonId) {
                        cout << "��ԭ�γ�IDһ��" << endl;
                        PAUSE;
                        break;
                    }
                    if (CourseManager::getInstance()->checkLessonId(target)) {
                        cout << "��ID�ѱ�ʹ��" << endl;
                        PAUSE;
                        break;
                    }
                    ::strcpy(this->target->lessonId, target.c_str());
                    this->target->generateName();
                    cout << "�¿γ�IDΪ:" << this->target->lessonId << endl;
                    PAUSE;
                    break;
                }
                case 3: {
                    CLEAR;
                    TeacherManager *instance = TeacherManager::getInstance();
                    std::string temp = instance->getTeacherIdByName(
                            MenuBase(" �б� ", instance->getTeacherNameList(), "").get());
                    cout << "ԭ�ڿν�ʦΪ: " << this->target->teacherName << endl;
                    if (temp == "ȡ��ѡ��") { break; }
                    if (temp == this->target->teacherName) {
                        cout << "��ԭ�ڿν�ʦ��ͬ" << endl;
                        PAUSE;
                        break;
                    }
                    cout << "ѡ������ڿν�ʦΪ:" << temp << endl;
                    ::strcpy(this->target->teacherName, temp.c_str());
                    ::strcpy(this->target->teacherId, instance->getTeacherIdByName(temp).c_str());
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 4: {
                    cout << "ԭ��ʼ���Ϊ: " << this->target->startData << endl;
                    cout << "�������¿�ʼ���:" << endl;
                    uint temp;
                    cin >> temp;
                    if (temp == 0) { break; }
                    if (temp > this->target->endData) {
                        cout << "��ʼ��ݲ��ܴ��ڽ������" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->startData = temp;
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 5: {
                    cout << "ԭ�������Ϊ: " << this->target->endData << endl;
                    cout << "�������½������:" << endl;
                    uint temp;
                    cin >> temp;
                    if (temp == 0) { break; }
                    if (temp < this->target->startData) {
                        cout << "������ݲ���С�ڿ�ʼ���" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->endData = temp;
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 6: {
                    cout << "ԭѧ��Ϊ: " << this->target->openTerm << endl;
                    cout << "������ѧ��:" << endl;
                    int temp;
                    cin >> temp;
                    if (temp == 0) { break; }
                    if (temp != 1 && temp != 2) {
                        cout << "ѧ��ֻ����1����2" << endl;
                        PAUSE;
                        break;
                    }
                    this->target->openTerm = (char) (temp + '0');
                    this->target->generateName();
                    PAUSE;
                    break;
                }
                case 7:
                    return true;
            }
            return false;
        }
    };
};

#endif
