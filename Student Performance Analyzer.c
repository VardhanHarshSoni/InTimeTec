#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <math.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LEN 100
#define LINE_BUF 256
#define MARKS 3

struct Student {
    int roll;
    char name[MAX_NAME_LEN];
    float marks[MARKS];
    float total;
    float average;
    char grade;
};


void readLine(char *buf , size_t size) {
    if ( fgets(buf, (int)size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    buf[strcspn(buf, "\r\n")] = '\0';
}

int parseInt(const char *s, int *out) {
    char *endptr;
    long val;
    errno = 0;
    val = strtol(s, &endptr, 10);
    if (endptr == s) return 0;            
    while (*endptr && isspace((unsigned char)*endptr)) 
		endptr++;
    if (*endptr != '\0') 
		return 0;        
    if (errno == ERANGE || val < INT_MIN || val > INT_MAX) 
		return 0;
    *out = (int)val;
    return 1;
}

int parseFloat(const char *s, float *out) {
    char *endptr;
    errno = 0;
    float val = strtof(s, &endptr);
    if (endptr == s) return 0;            
    while (*endptr && isspace((unsigned char)*endptr)) 
		endptr++;
    if (*endptr != '\0') 
		return 0;        
    if (errno == ERANGE) 
		return 0;
    *out = val;
    return 1;
}


int isValidName(const char *s) {
    if (s == NULL) return 0;
    int len = (int)strlen(s);
    if (len == 0) return 0;
    int hasLetter = 0 , i;
    for ( i = 0; i < len; ++i) {
        unsigned char ch = (unsigned char)s[i];
        if (isalpha(ch)) 
			hasLetter = 1;
        else if (ch == ' ' || ch == '\t') 
			continue;
        else return 0; 
    }
    return hasLetter;
}


float calculateTotal(float marks[], int n) {
    float total = 0.0f;
    int i;
    for ( i = 0; i < n; ++i) total += marks[i];
    return total;
}

float calculateAverage(float total, int n) {
    return total / n;
}

char assignGrade(float average) {
    if (average >= 85.0f) return 'A';
    else if (average >= 70.0f) return 'B';
    else if (average >= 50.0f) return 'C';
    else if (average >= 35.0f) return 'D';
    else return 'F';
}

void displayPerformance(char grade) {
    int stars = 0;
    switch (grade) {
        case 'A': stars = 5; 
				break;
        case 'B': stars = 4; 
				break;
        case 'C': stars = 3; 
				break;
        case 'D': stars = 2; 
				break;
        default: stars = 0; 
				break;
    }
    int i;
    for ( i = 0 ; i < stars ; ++i ) 
		putchar('*');
    putchar('\n'); 
}

void printRollNumbersRecursively(int current, int totalStudents) {
    if (current > totalStudents) 
		return;
    printf("%d ", current);
    printRollNumbersRecursively(current + 1, totalStudents);
}

int main(void) {
    struct Student students[MAX_STUDENTS];
    char line[LINE_BUF];
    int n = 0;
    
    while (1) {
        printf("Enter number of students (1-%d): ", MAX_STUDENTS);
        readLine(line, sizeof(line));
        if (parseInt(line, &n) && n >= 1 && n <= MAX_STUDENTS) break;
        printf("Invalid input. Please enter an integer between 1 and %d.\n", MAX_STUDENTS);
    }
	int i;
    for ( i = 0; i < n; ++i) {
        printf("\nEnter details for student %d:\n", i + 1);

        
        while (1) {
            printf("Roll Number (>0): ");
            readLine(line, sizeof(line));
            if (parseInt(line, &students[i].roll) && students[i].roll > 0) 
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

       
        int j;
        for ( j = 0; j < 3; ++j) {
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
        students[i].total = calculateTotal(students[i].marks, 3);
        students[i].average = calculateAverage(students[i].total, 3);
        students[i].grade = assignGrade(students[i].average);
    }

    
    //printf("\n--- Student Performance Report ---\n");
    for ( i = 0; i < n; ++i) {
        printf("\nRoll: %d\n", students[i].roll);
        printf("Name: %s\n", students[i].name);

        if (fabsf(students[i].total - roundf(students[i].total)) < 1e-6f) 
            printf("Total: %d\n", (int)roundf(students[i].total));
        else 
            printf("Total: %.2f\n", students[i].total);

        printf("Average: %.2f\n", students[i].average);
        printf("Grade: %c\n", students[i].grade);

        if (students[i].average < 35.0f) {
            continue;
        }
        printf("Performance: ");
        displayPerformance(students[i].grade);
    }

    printf("\nList of Roll Numbers (via recursion): ");
    printRollNumbersRecursively(1, n);
    printf("\n");

    return 0;
}
