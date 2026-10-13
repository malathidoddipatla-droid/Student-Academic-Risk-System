#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>

using namespace std;

struct Student
{
    string id;
    string name;

    double attendance;
    double assignment;
    double quiz;

    double riskScore;

    string riskLevel;
    string riskReason;
};

vector<Student> loadStudents();

#endif