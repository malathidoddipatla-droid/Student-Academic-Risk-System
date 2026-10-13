import streamlit as st
import pandas as pd


# --------------------------------------------------
# PAGE CONFIGURATION
# --------------------------------------------------

st.set_page_config(
    page_title="Student Academic Risk Dashboard",
    page_icon="🎓",
    layout="wide"
)


# --------------------------------------------------
# CUSTOM CSS
# --------------------------------------------------

st.markdown("""
<style>

.main {
    background-color: #f5f7fb;
}

.title {
    font-size: 38px;
    font-weight: 700;
    color: #1f2937;
}

.subtitle {
    font-size: 17px;
    color: #6b7280;
}

.card {
    padding: 20px;
    border-radius: 12px;
    background-color: white;
    box-shadow: 0px 2px 10px rgba(0,0,0,0.08);
    text-align: center;
}

.high {
    color: #dc2626;
    font-weight: bold;
}

.medium {
    color: #d97706;
    font-weight: bold;
}

.low {
    color: #16a34a;
    font-weight: bold;
}

</style>
""", unsafe_allow_html=True)


# --------------------------------------------------
# TITLE
# --------------------------------------------------

st.markdown(
    '<div class="title">🎓 Student Academic Risk Dashboard</div>',
    unsafe_allow_html=True
)

st.markdown(
    '<div class="subtitle">Early warning system for identifying students who may need academic support</div>',
    unsafe_allow_html=True
)

st.write("")


# --------------------------------------------------
# LOAD DATA
# --------------------------------------------------

try:

    attendance = pd.read_csv("data/attendance.csv")
    assignments = pd.read_csv("data/assignments.csv")
    quizzes = pd.read_csv("data/quizzes.csv")

except FileNotFoundError:

    st.error(
        "CSV files not found. Keep attendance.csv, "
        "assignments.csv and quizzes.csv in the same folder as app.py."
    )

    st.stop()


# --------------------------------------------------
# MERGE DATA
# --------------------------------------------------

df = attendance.merge(
    assignments,
    on="student_id"
)

df = df.merge(
    quizzes,
    on="student_id"
)


# --------------------------------------------------
# CALCULATE RISK SCORE
# --------------------------------------------------

df["risk_score"] = (
    df["attendance"] * 0.40
    + df["assignment"] * 0.30
    + df["quiz"] * 0.30
)


# --------------------------------------------------
# RISK CLASSIFICATION
# --------------------------------------------------

def classify_risk(score):

    if score < 50:
        return "HIGH"

    elif score < 70:
        return "MEDIUM"

    else:
        return "LOW"


df["risk_level"] = df["risk_score"].apply(classify_risk)


# --------------------------------------------------
# RISK REASON
# --------------------------------------------------

def get_reason(row):

    reasons = []

    if row["attendance"] < 75:
        reasons.append("Low Attendance")

    if row["assignment"] < 50:
        reasons.append("Low Assignment")

    if row["quiz"] < 50:
        reasons.append("Low Quiz")

    if not reasons:
        return "Good Performance"

    return ", ".join(reasons)


df["risk_reason"] = df.apply(
    get_reason,
    axis=1
)


# --------------------------------------------------
# DASHBOARD METRICS
# --------------------------------------------------

total_students = len(df)

high_risk = len(
    df[df["risk_level"] == "HIGH"]
)

medium_risk = len(
    df[df["risk_level"] == "MEDIUM"]
)

low_risk = len(
    df[df["risk_level"] == "LOW"]
)


st.subheader("📊 Risk Overview")


col1, col2, col3, col4 = st.columns(4)


with col1:

    st.metric(
        "👨‍🎓 Total Students",
        total_students
    )


with col2:

    st.metric(
        "🔴 High Risk",
        high_risk
    )


with col3:

    st.metric(
        "🟡 Medium Risk",
        medium_risk
    )


with col4:

    st.metric(
        "🟢 Low Risk",
        low_risk
    )


# --------------------------------------------------
# RISK DISTRIBUTION
# --------------------------------------------------

st.write("")

st.subheader("📈 Risk Distribution")

risk_data = pd.DataFrame(
    {
        "Risk Level": [
            "HIGH",
            "MEDIUM",
            "LOW"
        ],
        "Students": [
            high_risk,
            medium_risk,
            low_risk
        ]
    }
)

st.bar_chart(
    risk_data.set_index("Risk Level")
)


# --------------------------------------------------
# STUDENT SEARCH
# --------------------------------------------------

st.write("")

st.subheader("🔎 Student Search")

search = st.text_input(
    "Search by Student ID or Name"
)


if search:

    filtered = df[
        df["student_id"].str.contains(
            search,
            case=False,
            na=False
        )
        |
        df["name"].str.contains(
            search,
            case=False,
            na=False
        )
    ]

else:

    filtered = df


# --------------------------------------------------
# STUDENT TABLE
# --------------------------------------------------

st.subheader("👥 Student Risk Details")


display_df = filtered[
    [
        "student_id",
        "name",
        "attendance",
        "assignment",
        "quiz",
        "risk_score",
        "risk_level",
        "risk_reason"
    ]
].copy()


display_df.columns = [
    "Student ID",
    "Name",
    "Attendance %",
    "Assignment %",
    "Quiz %",
    "Risk Score",
    "Risk Level",
    "Risk Reason"
]


st.dataframe(
    display_df,
    use_container_width=True,
    hide_index=True
)


# --------------------------------------------------
# HIGH RISK STUDENTS
# --------------------------------------------------

st.write("")

st.subheader("🚨 Students Requiring Attention")


high_students = df[
    df["risk_level"] == "HIGH"
]


if len(high_students) == 0:

    st.success(
        "No high-risk students detected."
    )

else:

    for _, student in high_students.iterrows():

        st.error(
            f"**{student['student_id']} - "
            f"{student['name']}**  |  "
            f"Risk Score: {student['risk_score']:.2f}  |  "
            f"Reason: {student['risk_reason']}"
        )


# --------------------------------------------------
# DOWNLOAD REPORT
# --------------------------------------------------

st.write("")

st.subheader("📥 Download Report")


report = df[
    [
        "student_id",
        "name",
        "attendance",
        "assignment",
        "quiz",
        "risk_score",
        "risk_level",
        "risk_reason"
    ]
]


csv_data = report.to_csv(
    index=False
)


st.download_button(
    label="⬇️ Download Risk Report",
    data=csv_data,
    file_name="student_risk_report.csv",
    mime="text/csv"
)


# --------------------------------------------------
# FOOTER
# --------------------------------------------------

st.write("")

st.markdown("---")

st.caption(
    "Student Academic Risk Early Warning System | "
    "Attendance 40% + Assignments 30% + Quizzes 30%"
)