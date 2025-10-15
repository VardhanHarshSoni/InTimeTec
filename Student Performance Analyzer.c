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
    A,
    B,
    C,
    D,
    F
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
    int hasLetter = 0, index;
    for (index = 0; index < len; ++index) {
        unsigned char ch = (unsigned char)name[index];
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
    int index;
    for (index = 0; index < numberOfSubjects; ++index) total += marks[index];
    return total;
}

float calculateAverage(float total, int numberOfSubjects) {
    return total / numberOfSubjects;
}

Grade assignGrade(float averageMarks) {
    if (averageMarks >= 85.0f) return A;
    else if (averageMarks >= 70.0f) return B;
    else if (averageMarks >= 50.0f) return C;
    else if (averageMarks >= 35.0f) return D;
    else return F;
}

void displayPerformance(Grade grade) {
    int stars = 0;
    switch (grade) {
        case A: stars = 5; break;
        case B: stars = 4; break;
        case C: stars = 3; break;
        case D: stars = 2; break;
        case F: stars = 0; break;
    }
    int index;
    for (index = 0; index < stars; ++index)
        putchar('*');
    putchar('\n');
}

char getGradeLetter(Grade grade) {
    switch (grade) {
        case A: return 'A';
        case B: return 'B';
        case C: return 'C';
        case D: return 'D';
        default:return 'F';
    }
}

void printRollNumbers(int currentRollNumber , int numberOfStudents) {
    if (currentRollNumber > numberOfStudents)
        return;
    printf("%d ", currentRollNumber);
    printRollNumbers(currentRollNumber + 1, numberOfStudents);
}


void inputStudents(struct Student students[], int numberOfStudents) {
    char line[MAX_INPUT_LENGTH];
    int index, j;
    for (index = 0; index < numberOfStudents; ++index) {
        printf("\nEnter details for student %d:\n", index + 1);

        while (1) {
            printf("Roll Number (>0): ");
            readLine(line, sizeof(line));
            if (parseInt(line, &students[index].rollNumber) && students[index].rollNumber > 0)
                break;
            printf("Invalid roll. Roll number must be an integer greater than 0.\n");
        }

        while (1) {
            printf("Name (letters and spaces only): ");
            readLine(students[index].name, sizeof(students[index].name));
            if (isValidName(students[index].name))
                break;
            printf("Invalid name. Use only letters and spaces, at least one letter required.\n");
        }

        for (j = 0; j < NUMBER_OF_SUBJECTS; ++j) {
            while (1) {
                printf("Marks for Subject %d (0-100): ", j + 1);
                readLine(line, sizeof(line));
                float marksEntered;
                if (parseFloat(line, &marksEntered) && marksEntered >= 0.0f && marksEntered <= 100.0f) {
                    students[index].marks[j] = marksEntered;
                    break;
                }
                printf("Invalid marks. Enter a number between 0 and 100.\n");
            }
        }

        students[index].totalMarks = calculateTotal(students[index].marks, NUMBER_OF_SUBJECTS);
        students[index].averageMarks = calculateAverage(students[index].totalMarks, NUMBER_OF_SUBJECTS);
        students[index].studentGrade = getGradeLetter(assignGrade(students[index].averageMarks));
    }
}


void printStudents(struct Student students[], int numberOfStudents) {
    int index;
    for (index = 0; index < numberOfStudents; ++index) {
        printf("\nRoll No.: %d\n", students[index].rollNumber);
        printf("Name: %s\n", students[index].name);

        if (fabsf(students[index].totalMarks - roundf(students[index].totalMarks)) < 1e-6f)
            printf("Total: %d\n", (int)roundf(students[index].totalMarks));
        else
            printf("Total: %.2f\n", students[index].totalMarks);

        printf("Average Marks of student in the three subjects: %.2f\n", students[index].averageMarks);
        printf("Grade of the student: %c\n", students[index].studentGrade);

        if (students[index].averageMarks < 35.0f)
            continue;

        printf("Performance: ");
        displayPerformance(assignGrade(students[index].averageMarks));
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbers(1, numberOfStudents);
    printf("\n");
}


int main() {
    char line[MAX_INPUT_LENGTH];
    int numberOfStudents = 0;

    while (1) {
        printf("Enter number of students whose details you want to enter  (1-%d): ", MAX_STUDENTS);
        readLine(line, sizeof(line));
        if (parseInt(line, &numberOfStudents) && numberOfStudents >= 1 && numberOfStudents <= MAX_STUDENTS)
            break;
        printf("Invalid input. Please enter an integer between 1 and %d.\n", MAX_STUDENTS);
    }
    
    struct Student *students = (struct Student *)calloc(numberOfStudents, sizeof(struct Student));
    if (students == NULL) {
        printf("Memory Allocation Failed\n");
        return 1;
    } 

    inputStudents(students, numberOfStudents);
    printStudents(students, numberOfStudents);
	free(students);
    return 0;
}
