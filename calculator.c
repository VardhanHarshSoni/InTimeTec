#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define SIZE 100

int calculateResult(int numbers[] , char operators[] , int numberOfNumbers , int numberOfOperators) {
    int i , j , result = 0;
    for (i = 0 ; i < numberOfOperators ; i++) {
        if (operators[i] == '/' || operators[i] == '*') {
            if (operators[i] == '/' && numbers[i + 1] == 0) {
                printf(" Error: division by zero\n ");
                return 0;
            }

            if (operators[i] == '*')
                result = numbers[i] * numbers[i + 1];
            else
                result = numbers[i] / numbers[i + 1];

            numbers[i] = result;

            for ( j = i + 1 ; j < numberOfNumbers - 1 ; j++)
                numbers[j] = numbers[j + 1];
            for (j = i ; j < numberOfOperators - 1 ; j++)
                operators[j] = operators[j + 1];

            numberOfNumbers--;
            numberOfOperators--;
            i--;
        }
    }

    result = numbers[0];
    for (i = 0; i < numberOfOperators; i++) {
        if (operators[i] == '+')
            result += numbers[i + 1];
        else
            result -= numbers[i + 1];
    }
    return result;
}

int main() {
    char expression[SIZE];
    char clearedExpression[SIZE];
    printf("Enter the Expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';

    int j = 0 , i;
    for (i = 0 ; expression[i] != '\0' ; i++) {
        if (!isspace(expression[i])) {
            clearedExpression[j++] = expression[i];
        }
    }
    clearedExpression[j] = '\0';

    if (j == 0) {
        printf("Error: Empty Expression \n");
        return 0;
    }

    for (i = 0 ; clearedExpression[i] != '\0' ; i++) {
        if ( !isdigit(clearedExpression[i]) && clearedExpression[i] != '+' &&
            clearedExpression[i] != '-' && clearedExpression[i] != '*' &&
            clearedExpression[i] != '/') {
            printf(" Error: Invalid Expression \n ");
            return 0;
        }
    }

    if (!isdigit(clearedExpression[0]) || !isdigit(clearedExpression[j - 1])) {
        printf("Error: Expression cannot start or end with an operator \n");
        return 0;
    }

    for ( i = 0 ; clearedExpression[i] != '\0' ; i++ ) {
        if (!isdigit(clearedExpression[i]) && clearedExpression[i + 1] != '\0' &&
            !isdigit(clearedExpression[i + 1])) {
            printf("Error: Consecutive operators not allowed \n");
            return 0;
        }
    }

    int numbers[SIZE];
    char operators[SIZE];
    int numberOfNumbers = 0, numberOfOperators = 0;
    for ( i = 0 ; clearedExpression[i] != '\0';) {
        if (isdigit(clearedExpression[i])) {
            int num = 0;
            while (isdigit(clearedExpression[i])) {
                num = num * 10 + (clearedExpression[i] - '0');
                i++;
            }
            numbers[numberOfNumbers++] = num;
        } else {
            operators[numberOfOperators++] = clearedExpression[i];
            i++;
        }
    }

    int result = calculateResult(numbers, operators , numberOfNumbers , numberOfOperators);
    printf("Result: %d\n", result);
    return 0;
}


