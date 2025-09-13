#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Subject {
    char name[100];
    char code[50];
    float credit;
    float gradePoint;
    char gradeLetter[3];
};

struct Student {
    char name[100];
    char roll[20];
    struct Subject subjects[50];
    float totalCredits;
    float cgpa;
    char gradeLetter[3];
    int numSubjects;
};

void getGradeLetter(float gradePoint, char gradeLetter[]) {
    if (gradePoint >= 4.00) strcpy(gradeLetter, "A+");
    else if (gradePoint >= 3.75) strcpy(gradeLetter, "A");
    else if (gradePoint >= 3.50) strcpy(gradeLetter, "A-");
    else if (gradePoint >= 3.25) strcpy(gradeLetter, "B+");
    else if (gradePoint >= 3.00) strcpy(gradeLetter, "B");
    else if (gradePoint >= 2.75) strcpy(gradeLetter, "B-");
    else if (gradePoint >= 2.50) strcpy(gradeLetter, "C+");
    else if (gradePoint >= 2.25) strcpy(gradeLetter, "C");
    else if (gradePoint >= 2.00) strcpy(gradeLetter, "D");
    else strcpy(gradeLetter, "F");
}

void calculateCGPA(struct Student *s) {
    float totalWeightedPoints = 0;
    s->totalCredits = 0;

    for (int i = 0; i < s->numSubjects; i++) {
        s->totalCredits += s->subjects[i].credit;
        totalWeightedPoints += s->subjects[i].credit * s->subjects[i].gradePoint;
    }

    s->cgpa = totalWeightedPoints / s->totalCredits;
    getGradeLetter(s->cgpa, s->gradeLetter);
}

void printStudentReport(struct Student s, FILE *fp) {
    fprintf(fp, "\n----- Student CGPA Report -----\n");
    fprintf(fp, "Name         : %s", s.name);
    fprintf(fp, "Roll         : %s", s.roll);
    fprintf(fp, "Total Credit : %.2f\n", s.totalCredits);
    fprintf(fp, "CGPA         : %.2f\n", s.cgpa);
    fprintf(fp, "Grade Letter : %s\n", s.gradeLetter);

    fprintf(fp, "\nSubject Details:\n");
    for (int i = 0; i < s.numSubjects; i++) {
        fprintf(fp, "Subject %d:\n", i + 1);
        fprintf(fp, "  Name         : %s", s.subjects[i].name);
        fprintf(fp, "  Code         : %s", s.subjects[i].code);
        fprintf(fp, "  Credit       : %.2f\n", s.subjects[i].credit);
        fprintf(fp, "  Grade Point  : %.2f\n", s.subjects[i].gradePoint);
        fprintf(fp, "  Grade Letter : %s\n", s.subjects[i].gradeLetter);
    }
}

int main() {
    struct Student students[100];
    int studentCount = 0;
    char choice[5];

    int numSubjects;
    struct Subject subjectList[50];

    printf("Enter number of subjects: ");
    scanf("%d", &numSubjects);
    getchar();

    // Subject info once
    for (int i = 0; i < numSubjects; i++) {
        printf("\nSubject %d:\n", i + 1);
        printf("Enter subject name: ");
        fgets(subjectList[i].name, sizeof(subjectList[i].name), stdin);

        printf("Enter subject code: ");
        fgets(subjectList[i].code, sizeof(subjectList[i].code), stdin);

        printf("Enter credit for subject: ");
        scanf("%f", &subjectList[i].credit);
        getchar();
    }

    FILE *fp = fopen("cgpa_report.txt", "a"); // Append mode
    if (!fp) {
        printf("Error opening file!\n");
        return 1;
    }

    // Student input loop
    do {
        printf("\n------Student Information------");
        printf("\nEnter student name: ");
        fgets(students[studentCount].name, sizeof(students[studentCount].name), stdin);

        printf("Enter student roll number: ");
        fgets(students[studentCount].roll, sizeof(students[studentCount].roll), stdin);

        students[studentCount].numSubjects = numSubjects;

        for (int j = 0; j < numSubjects; j++) {
            students[studentCount].subjects[j] = subjectList[j];
            printf("Enter grade point for %s: ", students[studentCount].subjects[j].name);
            scanf("%f", &students[studentCount].subjects[j].gradePoint);
            getchar();
            getGradeLetter(students[studentCount].subjects[j].gradePoint, students[studentCount].subjects[j].gradeLetter);
        }

        calculateCGPA(&students[studentCount]);

        // Show report immediately
        printStudentReport(students[studentCount], stdout);

        // Save report to same file
        printStudentReport(students[studentCount], fp);

        studentCount++;

        printf("\nDo you want to add another student? (yes/no): ");
        fgets(choice, sizeof(choice), stdin);

    } while (choice[0] == 'y' || choice[0] == 'Y');

    fclose(fp);

    // Search mode loop
    char searchRoll[20];
    while (1) {
        printf("\nEnter roll number to search student CGPA (or type exit to quit): ");
        fgets(searchRoll, sizeof(searchRoll), stdin);

        if (strncmp(searchRoll, "exit", 4) == 0) {
            printf("Exiting search mode...\n");
            break;
        }

        int found = 0;
        for (int i = 0; i < studentCount; i++) {
            if (strcmp(students[i].roll, searchRoll) == 0) {
                printStudentReport(students[i], stdout);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("Student with roll %s not found!\n", searchRoll);
        }
    }

    return 0;
}
