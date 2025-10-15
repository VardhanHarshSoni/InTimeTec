#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 100
#define MAX_INPUT_LENGTH 256
#define NUMBER_OF_SUBJECTS 3
#define PARSE_SUCCESS 1
#define PARSE_FAILURE 0


struct Student {
    int rollNumber;
    char name[MAX_NAME_LEN];
    float marks[NUMBER_OF_SUBJECTS];
    float totalMarks;
    float averageMarks;
    char studentGrade;
};

typedef enum {
    GRADE_A,
    GRADE_B,
    GRADE_C,
    GRADE_D,
    GRADE_F
} Grade;


void readLine(char *buf , size_t size) {
    if (fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
}

int parseInt(const char *inputString, int *out) {
    char *endptr;
    long val;
    errno = 0;
    val = strtol(inputString, &endptr, 10);
    if (endptr == inputString) return 0;
    while (*endptr && isspace((unsigned char)*endptr))
        endptr++;
    if (*endptr != '\0')
        return 0;
    if (errno == ERANGE || val < INT_MIN || val > INT_MAX)
        return 0;
    *out = (int)val;
    return 1;
}

int parseFloat(const char *inputString, float *out) {
    char *endptr;
    errno = 0;
    float val = strtof(inputString, &endptr);
    if (endptr == inputString) return 0;
    while (*endptr && isspace((unsigned char)*endptr))
        endptr++;
    if (*endptr != '\0')
        return PARSE_FAILURE;
    if (errno == ERANGE)
        return PARSE_FAILURE;
    *out = val;
    return PARSE_SUCCESS;
}

int isValidName(const char *name) {
    if (name == NULL) return 0;
    int len = (int)strlen(name);
    if (len == 0) return 0;
    int hasLetter = 0, i;
    for (i = 0; i < len; ++i) {
        unsigned char ch = (unsigned char)name[i];
        if (isalpha(ch))
            hasLetter = 1;
        else if (ch == ' ' || ch == '\t')
            continue;
        else return 0;
    }
    return hasLetter;
}

float calculateTotal(float marks[], int numberOfSubjects) {
    float total = 0.0f;
    int i;
    for (i = 0; i < numberOfSubjects; ++i) total += marks[i];
    return total;
}

float calculateAverage(float total, int numberOfSubjects) {
    return total / numberOfSubjects;
}

Grade assignGrade(float averageMarks) {
    if (averageMarks >= 85.0f) return GRADE_A;
    else if (averageMarks >= 70.0f) return GRADE_B;
    else if (averageMarks >= 50.0f) return GRADE_C;
    else if (averageMarks >= 35.0f) return GRADE_D;
    else return GRADE_F;
}

void displayPerformance(Grade grade) {
    int stars = 0;
    switch (grade) {
        case GRADE_A: stars = 5; break;
        case GRADE_B: stars = 4; break;
        case GRADE_C: stars = 3; break;
        case GRADE_D: stars = 2; break;
        case GRADE_F: stars = 0; break;
    }
    int i;
    for (i = 0; i < stars; ++i)
        putchar('*');
    putchar('\n');
}

char getGradeLetter(Grade grade) {
    switch (grade) {
        case GRADE_A: return 'A';
        case GRADE_B: return 'B';
        case GRADE_C: return 'C';
        case GRADE_D: return 'D';
        default:      return 'F';
    }
}

void printRollNumbers(int current, int totalStudents) {
    if (current > totalStudents)
        return;
    printf("%d ", current);
    printRollNumbers(current + 1, totalStudents);
}


void inputStudents(struct Student students[], int numberOfStudents) {
    char line[MAX_INPUT_LENGTH];
    int i, j;
    for (i = 0; i < numberOfStudents; ++i) {
        printf("\nEnter details for student %d:\n", i + 1);

        while (1) {
            printf("Roll Number (>0): ");
            readLine(line, sizeof(line));
            if (parseInt(line, &students[i].rollNumber) && students[i].rollNumber > 0)
                break;
            printf("Invalid roll. Roll number must be an integer greater than 0.\n");
        }

        while (1) {
            printf("Name (letters and spaces only): ");
            readLine(students[i].name, sizeof(students[i].name));
            if (isValidName(students[i].name))
                break;
            printf("Invalid name. Use only letters and spaces, at least one letter required.\n");
        }

        for (j = 0; j < NUMBER_OF_SUBJECTS; ++j) {
            while (1) {
                printf("Marks for Subject %d (0-100): ", j + 1);
                readLine(line, sizeof(line));
                float m;
                if (parseFloat(line, &m) && m >= 0.0f && m <= 100.0f) {
                    students[i].marks[j] = m;
                    break;
                }
                printf("Invalid marks. Enter a number between 0 and 100.\n");
            }
        }

        students[i].totalMarks = calculateTotal(students[i].marks, NUMBER_OF_SUBJECTS);
        students[i].averageMarks = calculateAverage(students[i].totalMarks, NUMBER_OF_SUBJECTS);
        students[i].studentGrade = getGradeLetter(assignGrade(students[i].averageMarks));
    }
}


void printStudents(struct Student students[], int numberOfStudents) {
    int i;
    for (i = 0; i < numberOfStudents; ++i) {
        printf("\nRoll: %d\n", students[i].rollNumber);
        printf("Name: %s\n", students[i].name);

        if (fabsf(students[i].totalMarks - roundf(students[i].totalMarks)) < 1e-6f)
            printf("Total: %d\n", (int)roundf(students[i].totalMarks));
        else
            printf("Total: %.2f\n", students[i].totalMarks);

        printf("Average: %.2f\n", students[i].averageMarks);
        printf("Grade: %c\n", students[i].studentGrade);

        if (students[i].averageMarks < 35.0f)
            continue;

        printf("Performance: ");
        displayPerformance(assignGrade(students[i].averageMarks));
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbers(1, numberOfStudents);
    printf("\n");
}


int main() {
    struct Student students[MAX_STUDENTS];
    char line[MAX_INPUT_LENGTH];
    int numberOfStudents = 0;

    while (1) {
        printf("Enter number of students (1-%d): ", MAX_STUDENTS);
        readLine(line, sizeof(line));
        if (parseInt(line, &numberOfStudents) && numberOfStudents >= 1 && numberOfStudents <= MAX_STUDENTS)
            break;
        printf("Invalid input. Please enter an integer between 1 and %d.\n", MAX_STUDENTS);
    }

    inputStudents(students, numberOfStudents);
    printStudents(students, numberOfStudents);

    return 0;
}

