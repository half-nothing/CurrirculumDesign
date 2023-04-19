#pragma once
#ifndef TOOLS_H
#define TOOLS_H

#include "menuHandler/MainMenuHandler.h"

class Tool {
public:
    static void generateInstance() {
        CourseManager::generateInstance("course.dat");
        StudentManager::generateInstance("student.dat");
        TeacherManager::generateInstance("teacher.dat");
        EvaluationIndexManager::generateInstance("evaluationIndex.dat");
        StudentCourseManager::generateInstance("studentCourse.dat");
        EvaluationManager::generateInstance("evaluation.dat");
        UserManager::generateInstance();
        CourseMenuHandler::generateInstance();
        StudentInfoMenuHandler::generateInstance();
        TeacherMenuHandler::generateInstance();
        EvaluationIndexMenuHandler::generateInstance();
        StudentCourseMenuHandler::generateInstance();
        EvaluationDataMenuHandler::generateInstance();
        StudentMenuHandler::generateInstance();
        EvaluationMenuHandler::generateInstance();
        AdminMenuHandler::generateInstance();
        UserMenuHandler::generateInstance();
        MainMenuHandler::generateInstance();
    }

    static void deleteInstance() {
        MainMenuHandler::deleteInstance();
        UserMenuHandler::deleteInstance();
        AdminMenuHandler::deleteInstance();
        EvaluationMenuHandler::deleteInstance();
        StudentMenuHandler::deleteInstance();
        EvaluationDataMenuHandler::deleteInstance();
        StudentCourseMenuHandler::deleteInstance();
        EvaluationIndexMenuHandler::deleteInstance();
        TeacherMenuHandler::deleteInstance();
        StudentInfoMenuHandler::deleteInstance();
        CourseMenuHandler::deleteInstance();
        UserManager::deleteInstance();
        EvaluationManager::deleteInstance();
        StudentCourseManager::deleteInstance();
        EvaluationIndexManager::deleteInstance();
        TeacherManager::deleteInstance();
        StudentManager::deleteInstance();
        CourseManager::deleteInstance();
    }
};

#endif
