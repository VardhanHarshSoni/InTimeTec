#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include<stdlib.h>

typedef struct{
	int id;
	char name[100];
	int age;
} User;

void createfile(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fp = fopen(filename, "w");
        if (fp == NULL) {
            perror("Error in creating file");
            exit(1);
        }
        fprintf(fp, "ID,Name,Age \n");
        printf("File '%s' created \n", filename);
    } 
	else {
        printf("File '%s' already exists \n", filename);
    }
    fclose(fp);
}

void create(const char *filename) {
    User user;
    printf("Enter ID: ");
    scanf("%d", &user.id);
    getchar();
    printf("Enter Name: ");
    fgets(user.name, sizeof(user.name), stdin);
    user.name[strcspn(user.name, "\n")] = '\0';
	printf("Enter Age: ");
    scanf("%d", &user.age);
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }
    fprintf(fp, "%d,%s,%d\n", user.id, user.name, user.age);
    fclose(fp);
    printf("User with ID %d added\n", user.id);
}

void read(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file");
        return;
    }
    char line[200];
    printf("\n----- Users List -----\n");
    while (fgets(line, sizeof(line), fp)) {
        printf("%s", line);
    }
    printf("\n----------------------\n");
    fclose(fp);
}

void update(const char *filename) {
    int searchId;
    printf("Enter User ID to update: ");
    scanf("%d", &searchId);
    getchar();
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        perror("Error in opening file");
        exit(1);
    }
    char line[200];
    int found = 0;
    if (fgets(line, sizeof(line), fp)) {
        fputs(line, temp);
    }

    while (fgets(line, sizeof(line), fp)) {
        User user;
        sscanf(line, "%d,%99[^,],%d", &user.id, user.name, &user.age);
        if (user.id == searchId) {
            found = 1;
            printf("Enter new Name:");
            fgets(user.name, sizeof(user.name), stdin);
            user.name[strcspn(user.name, "\n")] = '\0';
            printf("Enter new Age :");
            scanf("%d", &user.age);
            fprintf(temp, "%d,%s,%d\n", user.id, user.name, user.age);
            printf("User with ID %d updated\n", user.id);
        } else {
            fputs(line, temp);
        }
    }
    fclose(fp);
    fclose(temp);
    remove(filename);
    rename("temp.txt", filename);
    if (!found) {
        printf("User with ID %d does not exist\n", searchId);
    }
}


void deleter(const char *filename) {
    int searchId;
    printf("Enter User ID to delete : ");
    scanf("%d", &searchId);
    FILE *fp = fopen(filename, "r");
    FILE *temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL) {
        perror("Error opening file");
        exit(1);
    }
    char line[200];
    int found = 0;
    if (fgets(line, sizeof(line), fp)) {
        fputs(line, temp);
    }
    while (fgets(line, sizeof(line), fp)) {
        User user;
        sscanf(line, "%d,%99[^,],%d", &user.id, user.name, &user.age);
        if (user.id == searchId) {
            found = 1;
            printf("User with ID %d deleted\n", user.id);
            continue;
        }
        fputs(line, temp);
    }
    fclose(fp);
    fclose(temp);
    remove(filename);
    rename("temp.txt", filename);
    if (!found) {
        printf("User with ID %d is not found\n", searchId);
    }
}

int main() {
    const char *filename = "users.txt";
    int choice;
    createfile(filename);
    do {
        printf("1. CREATE\n");
        printf("2. READ\n");
        printf("3. UPDATE\n");
        printf("4. DELETE\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1: create(filename);
					break;
            case 2: read(filename);
                	break;
            case 3: update(filename);
                	break;
            case 4: deleter(filename);
                	break;
            case 5: break;
            default:printf("Invalid choice. Try again.\n");
        }
    } while (choice != 5);
    return 0;
}
