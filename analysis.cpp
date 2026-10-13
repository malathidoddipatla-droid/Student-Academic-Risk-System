#include "analysis.h"

using namespace std;

void calculateRisk(vector<Student>& students)
{
    for (auto &s : students)
    {
        // ==============================
        // RISK SCORE
        // ==============================

        s.riskScore =
            (s.attendance * 0.40) +
            (s.assignment * 0.30) +
            (s.quiz * 0.30);


        // ==============================
        // RISK LEVEL
        // ==============================

        if (s.riskScore < 50)
        {
            s.riskLevel = "HIGH";
        }
        else if (s.riskScore < 70)
        {
            s.riskLevel = "MEDIUM";
        }
        else
        {
            s.riskLevel = "LOW";
        }


        // ==============================
        // RISK REASONS
        // ==============================

        s.riskReason = "";

        if (s.attendance < 75)
        {
            s.riskReason += "Low Attendance; ";
        }

        if (s.assignment < 50)
        {
            s.riskReason += "Low Assignment; ";
        }

        if (s.quiz < 50)
        {
            s.riskReason += "Low Quiz; ";
        }

        if (s.riskReason.empty())
        {
            s.riskReason = "Good Performance";
        }
    }
}