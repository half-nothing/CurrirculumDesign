/**
 * @author Half_nothing
 * @date 2023.02
 * @details 存储的单元结构体
 * @namespace DataType
 */

#pragma once
#ifndef DATA_STRUCTURE_H
#define DATA_STRUCTURE_H

#include "iomanip"

// 为了方便管理统一放入DataType命名空间
namespace DataType {

    typedef
    struct DataSize {
        static constexpr int storageId = 5;
        static constexpr int openTerm = 5;
        static constexpr int startData = 10;
        static constexpr int endData = 10;
        static constexpr int teacherName = 10;
        static constexpr int teacherId = 10;
        static constexpr int lessonId = 20;
        static constexpr int lessonName = 30;
        static constexpr int sex = 5;
        static constexpr int studentId = 20;
        static constexpr int studentName = 20;
        static constexpr int evaluated = 10;
        static constexpr int score = 5;
        static constexpr int weight = 5;
        static constexpr int evaluationID = 10;
        static constexpr int level = 5;
    } DataSize;

    typedef
    struct Admin {
        char account[NAME_LEN]{'\0'};
        char password[PASSWORD_LEN]{'\0'};

        Admin() = default;

        Admin(const std::string &accout, const std::string &password) {
            ::strcpy(this->account, accout.c_str());
            ::strcpy(this->password, password.c_str());
        }
    } Admin;

    typedef
    struct CourseInfo // 教学任务
    {
        uint storageId{};                           // 存储id，用来识别和定位
        bool usageStatus{};                         // 是否删除
        char openTerm{};                            // 学期
        uint startData{};                           // 开始年份
        uint endData{};                             // 结束年份
        char teacherName[NAME_LEN]{'\0'};       // 授课教师名
        char teacherId[NAME_LEN]{'\0'};         // 授课教师ID
        char lessonId[ID_LEN]{'\0'};            // 课程ID
        char lessonName[NAME_LEN]{'\0'};        // 课程名称
        char className[FULL_NAME_LEN]{'\0'};    // 教学班全名

        // 默认构造函数
        CourseInfo() = default;

        // 拷贝构造函数，用于vector的push_back
        CourseInfo(const CourseInfo &target) {
            this->deepCopy(target);
        }

        // 重载=运算符，用来直接复制拷贝
        CourseInfo &operator=(const CourseInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        // 用来生成全名
        void generateName() {
            std::string temp =
                    "(" + std::to_string(this->startData) + "-" + std::to_string(this->endData) + "-" + this->openTerm +
                    ")-" + this->lessonId + "-" +
                    this->teacherId;
            ::memset(this->className, '\0', sizeof(this->className));
            ::strcpy(this->className, temp.c_str());
        }

        // 打印相关信息
        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::lessonId) << std::left << this->lessonId;
            cout << setw(DataSize::lessonName) << std::left << this->lessonName;
            cout << setw(DataSize::teacherName) << std::left << this->teacherName;
            cout << setw(DataSize::startData) << std::left << this->startData;
            cout << setw(DataSize::endData) << std::left << this->endData;
            cout << setw(DataSize::openTerm) << std::left << this->openTerm;
            cout << std::left << this->className << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::lessonId) << std::left << "课程ID";
            cout << setw(DataSize::lessonName) << std::left << "课程名称";
            cout << setw(DataSize::teacherName) << std::left << "授课教师";
            cout << setw(DataSize::startData) << std::left << "开始时间";
            cout << setw(DataSize::endData) << std::left << "结束时间";
            cout << setw(DataSize::openTerm) << std::left << "学期";
            cout << std::left << "教学班信息" << endl;
        }

    private:
        // 私有的深复制方法
        void deepCopy(const CourseInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->openTerm = target.openTerm;
            this->startData = target.startData;
            this->endData = target.endData;
            ::memset(this->teacherName, '\0', sizeof(this->teacherName));
            ::memset(this->teacherId, '\0', sizeof(this->teacherId));
            ::memset(this->lessonId, '\0', sizeof(this->lessonId));
            ::memset(this->lessonName, '\0', sizeof(this->lessonName));
            ::memset(this->className, '\0', sizeof(this->className));
            ::strcpy(this->teacherName, target.teacherName);
            ::strcpy(this->teacherId, target.teacherId);
            ::strcpy(this->lessonId, target.lessonId);
            ::strcpy(this->lessonName, target.lessonName);
            ::strcpy(this->className, target.className);
        }
    } CourseInfo;

    typedef
    struct TeacherInfo // 教师信息
    {
        uint storageId{};                           // 存储id，用来识别和定位
        bool usageStatus{};                         // 是否删除
        char sex[4]{'\0'};                      // 性别
        char teacherName[NAME_LEN]{'\0'};       // 姓名
        char teacherId[TEACHER_ID_LEN]{'\0'};   // ID

        TeacherInfo() = default;

        TeacherInfo(const TeacherInfo &target) {
            this->deepCopy(target);
        }

        TeacherInfo &operator=(const TeacherInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::teacherId) << left << this->teacherId;
            cout << setw(DataSize::teacherName) << left << this->teacherName;
            cout << setw(DataSize::sex) << left << this->sex << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::teacherId) << left << "教师号";
            cout << setw(DataSize::teacherName) << left << "教师名";
            cout << setw(DataSize::sex) << left << "性别" << endl;
        }

    private:
        void deepCopy(const TeacherInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            ::memset(this->teacherName, '\0', sizeof(this->teacherName));
            ::memset(this->teacherId, '\0', sizeof(this->teacherId));
            ::memset(this->sex, '\0', sizeof(this->sex));
            ::strcpy(this->sex, target.sex);
            ::strcpy(this->teacherName, target.teacherName);
            ::strcpy(this->teacherId, target.teacherId);
        }
    } TeacherInfo;

    typedef
    struct StudentInfo // 学生信息
    {
        uint storageId{};                           // 存储id，用来识别和定位
        bool usageStatus{};                         // 是否删除
        char sex[4]{'\0'};                      // 性别
        char studentName[NAME_LEN]{'\0'};       // 姓名
        char studentId[STUDENT_ID_LEN]{'\0'};   // 学号
        char password[PASSWORD_LEN]{'\0'};

        StudentInfo() = default;

        StudentInfo(const StudentInfo &target) {
            this->deepCopy(target);
        }

        StudentInfo &operator=(const StudentInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::studentId) << left << this->studentId;
            cout << setw(DataSize::studentName) << left << this->studentName;
            cout << setw(DataSize::sex) << left << this->sex << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::studentId) << left << "学号";
            cout << setw(DataSize::studentName) << left << "学生姓名";
            cout << setw(DataSize::sex) << left << "性别" << endl;
        }

    private:
        void deepCopy(const StudentInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            ::memset(this->studentName, '\0', sizeof(this->studentName));
            ::memset(this->studentId, '\0', sizeof(this->studentId));
            ::memset(this->sex, '\0', sizeof(this->sex));
            ::memset(this->password, '\0', sizeof(this->password));
            ::strcpy(this->password, target.password);
            ::strcpy(this->sex, target.sex);
            ::strcpy(this->studentName, target.studentName);
            ::strcpy(this->studentId, target.studentId);
        }
    } StudentInfo;

    typedef
    struct StudentCourseInfo // 学生课程修读信息
    {
        uint storageId{};                           // 存储id，用来识别和定位
        bool usageStatus{};                         // 是否删除
        bool evaluated{false};                      // 是否已经评价
        double score{};                             // 学生评价的分数
        char lessonId[FULL_NAME_LEN]{'\0'};     // 课程ID
        char lessonName[NAME_LEN]{'\0'};        // 课程名称
        char studentId[STUDENT_ID_LEN]{'\0'};   // 学生学号
        char studentName[NAME_LEN]{'\0'};       // 学生姓名

        StudentCourseInfo() = default;

        StudentCourseInfo(const StudentCourseInfo &target) {
            this->deepCopy(target);
        }

        StudentCourseInfo &operator=(const StudentCourseInfo &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::lessonId) << left << this->lessonId;
            cout << setw(DataSize::lessonName) << left << this->lessonName;
            cout << setw(DataSize::studentId) << left << this->studentId;
            cout << setw(DataSize::studentName) << left << this->studentName;
            cout << setw(DataSize::evaluated) << left << (this->evaluated ? "是" : "否");
            if (this->evaluated)
                cout << setw(DataSize::score) << left << this->score << endl;
            else
                cout << setw(DataSize::score) << left << "未评价" << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::lessonId) << left << "课程ID";
            cout << setw(DataSize::lessonName) << left << "课程名称";
            cout << setw(DataSize::studentId) << left << "学号";
            cout << setw(DataSize::studentName) << left << "学生姓名";
            cout << setw(DataSize::evaluated) << left << "是否评价";
            cout << setw(DataSize::score) << left << "评价分数" << endl;
        }

    private:
        void deepCopy(const StudentCourseInfo &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->evaluated = target.evaluated;
            this->score = target.score;
            ::memset(this->lessonId, '\0', sizeof(this->lessonId));
            ::memset(this->studentId, '\0', sizeof(this->studentId));
            ::memset(this->lessonName, '\0', sizeof(this->lessonName));
            ::memset(this->studentName, '\0', sizeof(this->studentName));
            ::strcpy(this->studentName, target.studentName);
            ::strcpy(this->lessonName, target.lessonName);
            ::strcpy(this->lessonId, target.lessonId);
            ::strcpy(this->studentId, target.studentId);
        }
    } StudentCourseInfo;

    typedef
    struct EvaluationIndex // 评价指标
    {
        uint storageId{};                               // 存储id，用来识别和定位
        bool usageStatus{};                             // 是否删除
        double weight{};                                // 评价权重
        char evaluationID[ID_LEN]{'\0'};            // 评价ID
        char description[DESCRIPTION_LEN]{'\0'};    // 评价描述

        EvaluationIndex() = default;

        EvaluationIndex(const EvaluationIndex &target) {
            this->deepCopy(target);
        }

        EvaluationIndex &operator=(const EvaluationIndex &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::evaluationID) << left << this->evaluationID;
            cout << setw(DataSize::weight) << left << this->weight;
            cout << left << this->description << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::evaluationID) << left << "标准ID";
            cout << setw(DataSize::weight) << left << "权重";
            cout << left << "详细描述" << endl;
        }

    private:
        void deepCopy(const EvaluationIndex &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->weight = target.weight;
            ::memset(this->description, '\0', sizeof(this->description));
            ::memset(this->evaluationID, '\0', sizeof(this->evaluationID));
            ::strcpy(this->evaluationID, target.evaluationID);
            ::strcpy(this->description, target.description);
        }
    } EvaluationIndex;

    typedef struct EvaluationData // 评价数据
    {
        uint storageId{};                           // 存储id，用来识别和定位
        bool usageStatus{};                         // 是否删除
        char studentName[NAME_LEN]{'\0'};       // 学生姓名
        char evaluationID[ID_LEN]{'\0'};        // 评价ID
        char studentId[STUDENT_ID_LEN]{'\0'};   // 学生id
        char lessonName[NAME_LEN]{'\0'};        // 课程信息
        char lessonId[FULL_NAME_LEN]{'\0'};     // 课程id
        double weight{};                            // 评价权重
        double score{};                    // 评价等级

        EvaluationData() = default;

        EvaluationData(const EvaluationData &target) {
            this->deepCopy(target);
        }

        EvaluationData &operator=(const EvaluationData &target) {
            if (&target == this) {
                return *this;
            }
            this->deepCopy(target);
            return *this;
        }

        void printInfo() const {
            using namespace std;
            cout << setw(DataSize::storageId) << left << this->storageId;
            cout << setw(DataSize::evaluationID) << left << this->evaluationID;
            cout << setw(DataSize::studentId) << left << this->studentId;
            cout << setw(DataSize::studentName) << left << this->studentName;
            cout << setw(DataSize::lessonId) << left << this->lessonId;
            cout << setw(DataSize::lessonName) << left << this->lessonName;
            cout << setw(DataSize::level) << left << this->score << endl;
        }

        static
        void printTableHeader() {
            using namespace std;
            cout << setw(DataSize::storageId) << left << "ID";
            cout << setw(DataSize::evaluationID) << left << "评价ID";
            cout << setw(DataSize::studentId) << left << "学号";
            cout << setw(DataSize::studentName) << left << "学生姓名";
            cout << setw(DataSize::lessonId) << left << "课程ID";
            cout << setw(DataSize::lessonName) << left << "课程名称";
            cout << setw(DataSize::level) << left << "评价分数" << endl;
        }

    private:
        void deepCopy(const EvaluationData &target) {
            this->storageId = target.storageId;
            this->usageStatus = target.usageStatus;
            this->score = target.score;
            this->weight = target.weight;
            ::memset(this->studentId, '\0', sizeof(this->studentId));
            ::memset(this->lessonId, '\0', sizeof(this->lessonId));
            ::memset(this->lessonName, '\0', sizeof(this->lessonName));
            ::memset(this->studentName, '\0', sizeof(this->studentName));
            ::memset(this->evaluationID, '\0', sizeof(this->evaluationID));
            ::strcpy(this->evaluationID, target.evaluationID);
            ::strcpy(this->studentName, target.studentName);
            ::strcpy(this->lessonName, target.lessonName);
            ::strcpy(this->studentId, target.studentId);
            ::strcpy(this->lessonId, target.lessonId);
        }
    } EvaluationData;


}

// 为四种结构单元定义别名
using Course = DataType::CourseInfo;
using Student = DataType::StudentInfo;
using Teacher = DataType::TeacherInfo;
using StudentCourse = DataType::StudentCourseInfo;
using EvaluationIndex = DataType::EvaluationIndex;
using Evaluation = DataType::EvaluationData;
using Admin = DataType::Admin;

#endif