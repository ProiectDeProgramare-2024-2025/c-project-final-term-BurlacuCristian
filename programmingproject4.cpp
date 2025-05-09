#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_APPLICATIONS 100

// ANSI color codes
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

typedef struct {
    char name[50];
    char surname[50];
    int age;
    char highSchool[100];
    float baccalaureateAverage;
    char university[100];
    char faculty[100];
    char specialization[100];
} Application;

Application applications[MAX_APPLICATIONS];
int applicationCount = 0;

// ❗❗ Replace this with a valid path on your computer
const char *FILE_PATH = "applications.txt";  // Example: "C:\\Users\\YourName\\Desktop\\applications.txt"

void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printColored(const char* color, const char* text) {
    printf("%s%s%s", color, text, COLOR_RESET);
}

void getValidString(const char* prompt, char* buffer, int maxLength) {
    while(1) {
        printf("%s: ", prompt);
        fflush(stdout);
        if(fgets(buffer, maxLength, stdin) != NULL) {
            size_t len = strlen(buffer);
            if(len > 0 && buffer[len-1] == '\n') {
                buffer[len-1] = '\0';
                if(strlen(buffer) > 0) return;
            }
        }
        printColored(COLOR_RED, "Invalid input! Please try again.\n");
    }
}

int getValidInt(const char* prompt, int min, int max) {
    int value;
    while(1) {
        printf("%s (%d-%d): ", prompt, min, max);
        if(scanf("%d", &value) == 1 && value >= min && value <= max) {
            getchar(); // Consume newline
            return value;
        }
        while(getchar() != '\n');
        printColored(COLOR_RED, "Invalid input! Please enter a valid number.\n");
    }
}

float getValidFloat(const char* prompt, float min, float max) {
    float value;
    while(1) {
        printf("%s (%.2f - %.2f): ", prompt, min, max);
        if(scanf("%f", &value) == 1 && value >= min && value <= max) {
            getchar(); // Consume newline
            return value;
        }
        while(getchar() != '\n');
        printColored(COLOR_RED, "Invalid input! Please enter a valid float number.\n");
    }
}

void loadApplicationsFromFile() {
    FILE *file = fopen(FILE_PATH, "r");
    if (file == NULL) return;

    while (fscanf(file, " %49[^;];%49[^;];%d;%99[^;];%f;%99[^;];%99[^;];%99[^\n]",
                  applications[applicationCount].name,
                  applications[applicationCount].surname,
                  &applications[applicationCount].age,
                  applications[applicationCount].highSchool,
                  &applications[applicationCount].baccalaureateAverage,
                  applications[applicationCount].university,
                  applications[applicationCount].faculty,
                  applications[applicationCount].specialization) == 8)
    {
        applicationCount++;
        if (applicationCount >= MAX_APPLICATIONS) break;
    }

    fclose(file);
}

void saveApplicationToFile(Application *app) {
    FILE *file = fopen(FILE_PATH, "a");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }

    fprintf(file, "%s;%s;%d;%s;%.2f;%s;%s;%s\n",
            app->name,
            app->surname,
            app->age,
            app->highSchool,
            app->baccalaureateAverage,
            app->university,
            app->faculty,
            app->specialization);

    fflush(file); // Ensure it's written
    fclose(file);
}

void apply() {
    if (applicationCount >= MAX_APPLICATIONS) {
        printColored(COLOR_RED, "Application limit reached!\n");
        return;
    }

    Application newApp;
    getValidString("Enter name", newApp.name, sizeof(newApp.name));
    getValidString("Enter surname", newApp.surname, sizeof(newApp.surname));
    newApp.age = getValidInt("Enter age", 1, 100);
    getValidString("Enter high school", newApp.highSchool, sizeof(newApp.highSchool));
    newApp.baccalaureateAverage = getValidFloat("Enter baccalaureate average", 0.0f, 10.0f);
    getValidString("Select university", newApp.university, sizeof(newApp.university));
    getValidString("Select faculty", newApp.faculty, sizeof(newApp.faculty));
    getValidString("Select specialization", newApp.specialization, sizeof(newApp.specialization));

    applications[applicationCount++] = newApp;
    saveApplicationToFile(&newApp);
    printColored(COLOR_GREEN, "\nApplication submitted successfully and saved to file!\n");
}

void viewApplications() {
    if (applicationCount == 0) {
        printColored(COLOR_YELLOW, "No applications submitted yet.\n");
        return;
    }

    for (int i = 0; i < applicationCount; i++) {
        printf("\n--- Application %s%d%s ---\n", COLOR_BLUE, i + 1, COLOR_RESET);
        printf("Name: %s%s%s %s%s%s\n",
               COLOR_GREEN, applications[i].name, COLOR_RESET,
               COLOR_GREEN, applications[i].surname, COLOR_RESET);
        printf("Age: %s%d%s\n", COLOR_BLUE, applications[i].age, COLOR_RESET);
        printf("High School: %s%s%s\n", COLOR_YELLOW, applications[i].highSchool, COLOR_RESET);
        printf("Baccalaureate Average: %s%.2f%s\n",
               applications[i].baccalaureateAverage >= 5 ? COLOR_GREEN : COLOR_RED,
               applications[i].baccalaureateAverage,
               COLOR_RESET);
        printf("University: %s%s%s\n", COLOR_GREEN, applications[i].university, COLOR_RESET);
        printf("Faculty: %s%s%s\n", COLOR_YELLOW, applications[i].faculty, COLOR_RESET);
        printf("Specialization: %s%s%s\n", COLOR_BLUE, applications[i].specialization, COLOR_RESET);
    }
}

void viewStatistics() {
    if (applicationCount == 0) {
        printColored(COLOR_YELLOW, "No applications submitted yet.\n");
        return;
    }

    char university[100];
    getValidString("Enter university name to view statistics", university, sizeof(university));

    int count = 0;
    for (int i = 0; i < applicationCount; i++) {
        if (strcasecmp(applications[i].university, university) == 0) {
            count++;
        }
    }

    printf("Total applications for %s%s%s: %s%d%s\n",
           COLOR_GREEN, university, COLOR_RESET,
           COLOR_BLUE, count, COLOR_RESET);
}

void displayMenu() {
    printf("\n%sUniversity Admission System%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s1.%s Apply\n", COLOR_GREEN, COLOR_RESET);
    printf("%s2.%s View Applications\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s3.%s View Statistics\n", COLOR_BLUE, COLOR_RESET);
    printf("%s4.%s Exit\n", COLOR_RED, COLOR_RESET);
    printf("Enter your choice (1-4): ");
}

int main() {
    loadApplicationsFromFile();
    int choice;

    do {
        clearConsole();
        displayMenu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }
        getchar(); // Consume newline

        clearConsole();
        switch (choice) {
            case 1: apply(); break;
            case 2: viewApplications(); break;
            case 3: viewStatistics(); break;
            case 4: printColored(COLOR_BLUE, "Exiting...\n"); break;
            default: printColored(COLOR_RED, "Invalid choice! Please try again.\n");
        }

        if (choice != 4) {
            printf("\n%sPress ENTER to continue...%s", COLOR_YELLOW, COLOR_RESET);
            getchar();
        }
    } while (choice != 4);

    return 0;
}

