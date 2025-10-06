#include <stdio.h>
#include <ctype.h>
#include <string.h>
#define SIZE 100

int calculateResult(int numbers[] , char operators[] , int n , int o) {
    int i , j , result;
    for (i = 0 ; i < o ; i++) {
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

            for ( j = i + 1 ; j < n - 1 ; j++)
                numbers[j] = numbers[j + 1];
            for (j = i ; j < o - 1 ; j++)
                operators[j] = operators[j + 1];

            n--;
            o--;
            i--;
        }
    }

    result = numbers[0];
    for (i = 0; i < o; i++) {
        if (operators[i] == '+')
            result += numbers[i + 1];
        else
            result -= numbers[i + 1];
    }
    return result;
}

int main() {
    char expression[SIZE];
    char clearedexp[SIZE];
    printf("Enter the Expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = '\0';

    int j = 0 , i;
    for (i = 0 ; expression[i] != '\0' ; i++) {
        if (!isspace(expression[i])) {
            clearedexp[j++] = expression[i];
        }
    }
    clearedexp[j] = '\0';

    if (j == 0) {
        printf("Error: Empty Expression \n");
        return 0;
    }

    for (i = 0 ; clearedexp[i] != '\0' ; i++) {
        if ( !isdigit(clearedexp[i]) && clearedexp[i] != '+' &&
            clearedexp[i] != '-' && clearedexp[i] != '*' &&
            clearedexp[i] != '/') {
            printf(" Error: Invalid Expression \n ");
            return 0;
        }
    }

    if (!isdigit(clearedexp[0]) || !isdigit(clearedexp[j - 1])) {
        printf("Error: Expression cannot start or end with an operator \n");
        return 0;
    }

    for ( i = 0 ; clearedexp[i] != '\0' ; i++ ) {
        if (!isdigit(clearedexp[i]) && clearedexp[i + 1] != '\0' &&
            !isdigit(clearedexp[i + 1])) {
            printf("Error: Consecutive operators not allowed \n");
            return 0;
        }
    }

    int numbers[SIZE];
    char operators[SIZE];
    int n = 0, o = 0;
    for ( i = 0 ; clearedexp[i] != '\0';) {
        if (isdigit(clearedexp[i])) {
            int num = 0;
            while (isdigit(clearedexp[i])) {
                num = num * 10 + (clearedexp[i] - '0');
                i++;
            }
            numbers[n++] = num;
        } else {
            operators[o++] = clearedexp[i];
            i++;
        }
    }

    int result = calculateResult(numbers, operators , n , o);
    printf("Result: %d\n", result);
    return 0;
}
