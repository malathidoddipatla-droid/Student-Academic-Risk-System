#include "student.h"

#include <fstream>
#include <sstream>

using namespace std;

vector<Student> loadStudents()
{
    vector<Student> students;

    ifstream attendanceFile("data/attendance.csv");
    ifstream assignmentFile("data/assignments.csv");
    ifstream quizFile("data/quizzes.csv");

    if (!attendanceFile.is_open() ||
        !assignmentFile.is_open() ||
        !quizFile.is_open())
    {
        return students;
    }

    string line;

    // Skip headers
    getline(attendanceFile, line);
    getline(assignmentFile, line);
    getline(quizFile, line);


    // ==============================
    // READ ATTENDANCE
    // ==============================

    while (getline(attendanceFile, line))
    {
        stringstream ss(line);

        Student s;
        string attendance;

        getline(ss, s.id, ',');
        getline(ss, s.name, ',');
        getline(ss, attendance, ',');

        s.attendance = stod(attendance);

        s.assignment = 0;
        s.quiz = 0;
        s.riskScore = 0;

        students.push_back(s);
    }


    // ==============================
    // READ ASSIGNMENTS
    // ==============================

    while (getline(assignmentFile, line))
    {
        stringstream ss(line);

        string id;
        string assignment;

        getline(ss, id, ',');
        getline(ss, assignment, ',');

        for (auto &student : students)
        {
            if (student.id == id)
            {
                student.assignment = stod(assignment);
            }
        }
    }


    // ==============================
    // READ QUIZZES
    // ==============================

    while (getline(quizFile, line))
    {
        stringstream ss(line);

        string id;
        string quiz;

        getline(ss, id, ',');
        getline(ss, quiz, ',');

        for (auto &student : students)
        {
            if (student.id == id)
            {
                student.quiz = stod(quiz);
            }
        }
    }

    return students;
}