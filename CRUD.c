#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 200

typedef struct {
    int id;
    char name[SIZE];
    int age;
} User;

int createFileIfNotExists(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        file = fopen(fileName, "w");
        if (file == NULL) {
            perror("Error creating file");
            return 1;
        }
        fprintf(file, " ID , Name , Age \n");
        printf("File '%s' created successfully \n", fileName);
    } else {
        printf("File '%s' already exists \n", fileName);
    }
    fclose(file);
    return 0;
}

int userExistsOrNot( const char *fileName, int id ) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL){
    	return 0;
	} 
    char line[SIZE];
    fgets(line, sizeof(line), file); 
    while (fgets(line, sizeof(line), file)) {
        User u;
        if (sscanf(line, "%d , %99[^,] , %d", &u.id, u.name, &u.age) == 3) {
            if (u.id == id) {
                fclose(file);
                return 1;
            }
        }
    }
    fclose(file);
    return 0;
}

int validateIntegerValue(const char *prompt) {
    int num;
    char buffer[SIZE];
    while (1) {
        printf("%s", prompt);
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("Error reading input  Try again \n");
            continue;
        }
        if (sscanf(buffer, "%d", &num) == 1) {
            if (num > 0)
                return num;
            else
                printf("Invalid input Please enter a positive integer \n");
        } else {
            printf("Invalid input  Please enter a valid integer \n");
        }
    }
}

int isValidName(const char *name) {
	int i;
    for (i = 0; name[i] != '\0'; i++) {
        if (!isalpha(name[i]) && name[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int createUser(const char *fileName) {
    User user;
    user.id = validateIntegerValue("Enter ID: ");
    if (userExistsOrNot(fileName, user.id)) {
        printf("Error: User with ID %d already exists \n", user.id );
        return 1;
    }
    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';
    while (!isValidName(user.name) || strlen(user.name) == 0) {
    	printf("Invalid name Only alphabets and spaces are allowed, Try again: ");
    	fgets(user.name, sizeof(user.name), stdin);
    	user.name[strcspn(user.name, "\n")] = '\0';
	}
    user.age = validateIntegerValue("Enter Age: ");
    if (user.age <= 0) {
        printf("Invalid Age \n");
        return 1;
    }
    FILE *file = fopen(fileName, "a");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    fprintf(file, "%d,%s,%d\n", user.id, user.name, user.age);
    fclose(file);
    printf("User with ID %d added successfully \n", user.id);
    return 0;
}

void readUsers(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    char line[SIZE];
    printf("\n    Users List     \n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

void updateUser(const char *fileName) {
    int searchId = validateIntegerValue("Enter User ID to update: ");
    FILE *Ptrfile = fopen(fileName, "r");
    FILE *Ptrtemp = fopen("temp.txt", "w");
    if (Ptrfile == NULL || Ptrtemp == NULL) {
        perror("Error opening file");
        return 1;
    }
    char line[SIZE];
    int found = 0;
    if (fgets(line, sizeof(line), Ptrfile))
        fputs(line, Ptrtemp);
    while (fgets(line, sizeof(line), Ptrfile)) {
        User user;
        if (sscanf(line , "%d , %99[^,] , %d" , &user.id, user.name, &user.age) == 3) {
            if (user.id == searchId) {
                found = 1;
                printf("Enter new Name: ");
                fgets(user.name, sizeof(user.name), stdin);
                user.name[strcspn(user.name, "\n")] = '\0';
                while (!isValidName(user.name) || strlen(user.name) == 0) {
    				printf(" Invalid name! Only alphabets and spaces are allowed. Try again: ");
    				fgets(user.name, sizeof(user.name), stdin);
    				user.name[strcspn(user.name, "\n")] = '\0';
				}
                user.age = validateIntegerValue("Enter new Age: ");
                if (user.age <= 0) {
                    printf("Invalid Age \n");
                    fclose(Ptrfile);
                    fclose(Ptrtemp);
                    remove("temp.txt");
                    return;
                }
                fprintf(Ptrtemp, "%d, %s, %d \n" , user.id, user.name, user.age);
                printf("User with ID %d updated successfully \n", user.id);
            } else {
                fputs(line, Ptrtemp); 
            }
        } else {
            fputs(line, Ptrtemp);
        }
    }
    fclose(Ptrfile);
    fclose(Ptrtemp);
    remove(fileName);
    rename("temp.txt", fileName);
    if (!found)
        printf("User with ID %d not found \n", searchId);
}

void deleteUser(const char *fileName) {
    int searchId = validateIntegerValue("Enter User ID to delete: ");
    FILE *Ptrfile = fopen(fileName, "r");
    FILE *Ptrtemp = fopen("temp.txt", "w");
    if (Ptrfile == NULL || Ptrtemp == NULL) {
        perror("Error opening file");
        return;
    }
    char line[SIZE];
    int found = 0;
    if (fgets(line, sizeof(line), Ptrfile))
        fputs(line, Ptrtemp);
    while (fgets(line, sizeof(line), Ptrfile)) {
        User user;
        if (sscanf(line, "%d , %99[^,] , %d", &user.id, user.name, &user.age) == 3) {
            if (user.id == searchId) {
                found = 1;
                printf("User with ID %d deleted successfully \n ", user.id);
                continue;
            }
        }
        fputs(line, Ptrtemp);
    }
    fclose(Ptrfile);
    fclose(Ptrtemp);
    remove(fileName);
    rename("temp.txt", fileName);
    if (!found)
        printf(" User with ID %d not found \n ", searchId);
}

int main() {
    const char *fileName = "users.txt";
    int choice;
    createFileIfNotExists(fileName);
    do {
        printf("1. Create User\n");
        printf("2. Read Users\n");
        printf("3. Update User\n");
        printf("4. Delete User\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        choice = validateIntegerValue("");
        switch (choice) {
            case 1: createUser(fileName); 
					break;
            case 2: readUsers(fileName); 
					break;
            case 3: updateUser(fileName); 
					break;
            case 4: deleteUser(fileName); 
					break;
            case 5: printf(" Exiting program \n "); 
					break;
            default: printf(" Invalid choice  Try again \n ");
        }
    } while (choice != 5);
    return 0;
}

