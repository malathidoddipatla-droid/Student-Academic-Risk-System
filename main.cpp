#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cstdlib>

#include "student.h"
#include "analysis.h"

using namespace std;


// ======================================================
// ANSI COLORS
// ======================================================

#define RESET   "\033[0m"

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

#define CYAN    "\033[36m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"

#define BOLD    "\033[1m"


// ======================================================
// CLEAR SCREEN
// ======================================================

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


// ======================================================
// HEADER
// ======================================================

void showHeader()
{
    cout << CYAN << BOLD;

    cout << "\n";
    cout << "==============================================================\n";
    cout << "        STUDENT ACADEMIC RISK EARLY WARNING SYSTEM\n";
    cout << "==============================================================\n";

    cout << RESET;

    cout << "  Data-driven academic performance monitoring system\n";
    cout << "  Attendance + Assignments + Quizzes\n\n";
}


// ======================================================
// RISK COLOR
// ======================================================

string riskColor(string risk)
{
    if (risk == "HIGH")
        return RED;

    if (risk == "MEDIUM")
        return YELLOW;

    return GREEN;
}


// ======================================================
// DASHBOARD
// ======================================================

void showDashboard(const vector<Student>& students)
{
    int high = 0;
    int medium = 0;
    int low = 0;

    for (const auto &s : students)
    {
        if (s.riskLevel == "HIGH")
            high++;

        else if (s.riskLevel == "MEDIUM")
            medium++;

        else
            low++;
    }


    cout << CYAN << BOLD;

    cout << "\n";
    cout << "+------------------------------------------------------------+\n";
    cout << "|                     RISK OVERVIEW                          |\n";
    cout << "+------------------------------------------------------------+\n";

    cout << RESET;


    cout << "\n";

    cout << "  Total Students : "
         << WHITE << BOLD
         << students.size()
         << RESET << "\n";


    cout << "  "
         << RED
         << "HIGH RISK"
         << RESET
         << "        : "
         << RED
         << high
         << RESET << "\n";


    cout << "  "
         << YELLOW
         << "MEDIUM RISK"
         << RESET
         << "      : "
         << YELLOW
         << medium
         << RESET << "\n";


    cout << "  "
         << GREEN
         << "LOW RISK"
         << RESET
         << "         : "
         << GREEN
         << low
         << RESET << "\n\n";
}


// ======================================================
// STUDENT TABLE
// ======================================================

void showStudents(const vector<Student>& students)
{
    cout << CYAN << BOLD;

    cout << "\n";
    cout << "==========================================================================\n";
    cout << "                         STUDENT RISK TABLE\n";
    cout << "==========================================================================\n";

    cout << RESET;


    cout << left
         << setw(8) << "ID"
         << setw(15) << "NAME"
         << setw(12) << "ATTEND."
         << setw(12) << "ASSIGN."
         << setw(10) << "QUIZ"
         << setw(12) << "SCORE"
         << setw(12) << "RISK"
         << "\n";


    cout << "--------------------------------------------------------------------------\n";


    for (const auto &s : students)
    {
        cout << left
             << setw(8) << s.id
             << setw(15) << s.name
             << setw(12) << s.attendance
             << setw(12) << s.assignment
             << setw(10) << s.quiz
             << setw(12)
             << fixed << setprecision(2)
             << s.riskScore;


        cout << riskColor(s.riskLevel)
             << setw(12)
             << s.riskLevel
             << RESET
             << "\n";
    }


    cout << "==========================================================================\n";
}


// ======================================================
// RISK ANALYSIS
// ======================================================

void showRiskReasons(const vector<Student>& students)
{
    cout << CYAN << BOLD;

    cout << "\n";
    cout << "==============================================================\n";
    cout << "                       RISK ANALYSIS\n";
    cout << "==============================================================\n";

    cout << RESET;


    for (const auto &s : students)
    {
        cout << "\n";

        cout << BOLD
             << s.id
             << " - "
             << s.name
             << RESET;


        cout << "\n  Risk Level : "
             << riskColor(s.riskLevel)
             << s.riskLevel
             << RESET;


        cout << "\n  Reason     : "
             << s.riskReason
             << "\n";


        if (s.riskLevel == "HIGH")
        {
            cout << "  Recommendation : "
                 << RED
                 << "Immediate faculty intervention"
                 << RESET
                 << "\n";
        }

        else if (s.riskLevel == "MEDIUM")
        {
            cout << "  Recommendation : "
                 << YELLOW
                 << "Monitor and provide additional guidance"
                 << RESET
                 << "\n";
        }

        else
        {
            cout << "  Recommendation : "
                 << GREEN
                 << "Continue regular monitoring"
                 << RESET
                 << "\n";
        }
    }
}


// ======================================================
// SAVE REPORT
// ======================================================

void saveReport(const vector<Student>& students)
{
    ofstream file("output/risk_report.csv");

    file << "Student ID,Name,Attendance,Assignment,Quiz,"
         << "Risk Score,Risk Level,Risk Reason\n";


    for (const auto &s : students)
    {
        file << s.id << ","
             << s.name << ","
             << s.attendance << ","
             << s.assignment << ","
             << s.quiz << ","
             << s.riskScore << ","
             << s.riskLevel << ","
             << s.riskReason
             << "\n";
    }

    file.close();
}


// ======================================================
// MAIN
// ======================================================

int main()
{
    clearScreen();

    showHeader();


    // Load data

    vector<Student> students = loadStudents();


    // Check data

    if (students.empty())
    {
        cout << RED
             << "\nERROR: Student data could not be loaded.\n"
             << RESET;

        cout << "\nPlease check these files:\n";

        cout << "  data/attendance.csv\n";
        cout << "  data/assignments.csv\n";
        cout << "  data/quizzes.csv\n";

        return 1;
    }


    // Calculate academic risk

    calculateRisk(students);


    // Generate report

    saveReport(students);


    // Display dashboard

    showDashboard(students);

    showStudents(students);

    showRiskReasons(students);


    cout << "\n";

    cout << GREEN << BOLD
         << "✔ Risk report generated successfully!"
         << RESET
         << "\n";

    cout << "\nReport location: ";

    cout << CYAN
         << "output/risk_report.csv"
         << RESET
         << "\n";


    cout << "\nPress ENTER to exit...";

    cin.get();

    return 0;
}