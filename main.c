#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

int main(){
    struct dirent *de;
    DIR *dr = opendir(".");
    if (dr == NULL){
        printf("Could not open current directory" );
        return 0;
    }
    int numOfFiles = 0;
    while ((de = readdir(dr)) != NULL){
        char* str = de->d_name;
        if (str[0] == 'i' && str[1] == 'n' && str[2] == 'p' && str[3] == 'u' && str[4] == 't') {
            numOfFiles++;
        }
    }
    if (numOfFiles > 50) {
        numOfFiles = 50;
    }
    closedir(dr);

    struct proff{
        char firstName[100];
        char lastName[100];
        char trained[100][100];
        int numOfClasses;
        int coursesAvaliable;
    };

    struct TA{
        char firstName[100];
        char lastName[100];
        char trained[100][100];
        int numOfClasses;
        int coursesAvaliable;
    };

    struct student{
        char firstName[100];
        char lastName[100];
        char classes[100][100];
        int id;
        int numOfClasses;
    };

    struct course{
        char name[100];
        int numOfStudents;
        int numOfLabs;
    };
    for (int iterator = 1; iterator <= numOfFiles; iterator++) {
        char name[20] = "input";
        char tempIter[numOfFiles + 1];
        sprintf(tempIter, "%d", iterator);
        strcat(name, tempIter);
        strcat(name, ".txt");
        FILE *input;
        input = fopen(name, "r");

        struct student students[50];
        struct TA TAs[50];
        struct course courses[50];
        struct proff proffs[50];
        char temp[500];
        fgets(temp, 500, input);
        int flag = strcmp(temp, "P\n");
        int numOfCourses = 0;

        while (flag != 0) {
            char tempStr[50];
            int i = 0;
            int j = 0;
            int k = 0;
            while (temp[j] != 32) {
                tempStr[j] = temp[j];
                j++;
            }
            tempStr[j] = '\0';
            j++;
            strcpy(courses[i].name, tempStr);

            while (temp[j] != 32) {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            courses[i].numOfLabs = atoi(tempStr);
            j++;

            k = 0;
            while (temp[j] != '\n') {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            courses[i].numOfStudents = atoi(tempStr);

            fgets(temp, 500, input);
            flag = strcmp(temp, "P\n");
            i++;
            numOfCourses++;
        }

        int numOfProffs = 0;
        int i = 0;
        fgets(temp, 500, input);
        flag = strcmp(temp, "T\n");
        while (flag != 0) {
            char tempStr[50];
            int j = 0;
            int k = 0;

            while (temp[j] != 32){
                tempStr[j] = temp[j];
                j++;
            }
            tempStr[j] = '\0';
            strcpy(proffs[i].firstName, tempStr);

            j++;
            k = 0;
            while (temp[j] != 32){
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            strcpy(proffs[i].lastName, tempStr);

            j++;
            proffs[i].numOfClasses = 0;
            k = 0;
            while (temp[j] != '\n') {
                if (temp[j] == 32) {
                    tempStr[k] = '\0';
                    strcpy(proffs[i].trained[proffs[i].numOfClasses], tempStr);
                    proffs[i].numOfClasses++;
                    k = 0;
                    j++;
                }
                tempStr[k] = temp[j];
                k++;
                j++;
            }
            tempStr[k] = '\0';
            strcpy(proffs[i].trained[proffs[i].numOfClasses], tempStr);
            proffs[i].numOfClasses++;

            fgets(temp, 500, input);
            flag = strcmp(temp, "T\n");
            i++;
            numOfProffs++;
        }
        // for (size_t i = 0; i < numOfProffs; i++) {
        //     for (size_t j = 0; j < proffs[i].numOfClasses; j++) {
        //         printf("%s %s is trained for %s\n", proffs[i].firstName, proffs[i].lastName, proffs[i].trained[j]);
        //     }
        // }

        int numOfTAs = 0;
        i = 0;
        fgets(temp, 500, input);
        flag = strcmp(temp, "S\n");
        while (flag != 0) {
            char tempStr[50];
            int j = 0;
            int k = 0;

            while (temp[j] != 32){
                tempStr[j] = temp[j];
                j++;
            }
            tempStr[j] = '\0';
            strcpy(TAs[i].firstName, tempStr);

            j++;
            k = 0;
            while (temp[j] != 32){
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            strcpy(TAs[i].lastName, tempStr);

            j++;
            TAs[i].numOfClasses = 0;
            k = 0;
            while (temp[j] != '\n') {
                if (temp[j] == 32) {
                    tempStr[k] = '\0';
                    strcpy(TAs[i].trained[TAs[i].numOfClasses], tempStr);
                    TAs[i].numOfClasses++;
                    k = 0;
                    j++;
                }
                tempStr[k] = temp[j];
                k++;
                j++;
            }
            tempStr[k] = '\0';
            strcpy(TAs[i].trained[TAs[i].numOfClasses], tempStr);
            TAs[i].numOfClasses++;

            fgets(temp, 500, input);
            flag = strcmp(temp, "S\n");
            i++;
            numOfTAs++;
        }
        // for (size_t i = 0; i < numOfTAs; i++) {
        //     for (size_t j = 0; j < TAs[i].numOfClasses; j++) {
        //         printf("%s %s is trained for %s\n", TAs[i].firstName, TAs[i].lastName, TAs[i].trained[j]);
        //     }
        // }

        int numOfStudents = 0;
        fgets(temp, 500, input);
        i = 0;
        int j = 0;
        int k = 0;
        char tempStr[50];
        while (1) {
            printf("%d: ", i);
            while (temp[j] != 32) {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            strcpy(students[i].firstName, tempStr);
            printf("%s ", students[i].firstName);
            k = 0;
            j++;

            while (temp[j] != 32) {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            strcpy(students[i].lastName, tempStr);
            printf("%s ", students[i].lastName);
            k = 0;
            j++;

            while (temp[j] != 32) {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            students[i].id = atoi(tempStr);
            printf("%d ", students[i].id);
            k = 0;
            j++;

            students[i].numOfClasses = 0;
            while (temp[j] != '\n' && temp[j] != EOF) {
                //printf("BLIA\n");
                if (temp[j] == 32) {
                    tempStr[k] = '\0';
                    strcpy(students[i].classes[students[i].numOfClasses], tempStr);
                    students[i].numOfClasses++;
                    k = 0;
                    j++;
                    printf("%s\n", tempStr);
                }
                tempStr[k] = temp[j];
                k++;
                j++;
            }
            tempStr[k] = '\0';
            strcpy(students[i].classes[students[i].numOfClasses], tempStr);
            students[i].numOfClasses++;
            i++;
            numOfStudents++;

            if (temp[j] == EOF) {
                break;
            }

            fgets(temp, 500, input);
        }
        //printf("num od std: %d, \n", numOfStudents);
        // for (size_t i = 0; i < numOfStudents; i++) {
        //     for (size_t j = 0; j < students[i].numOfClasses; j++) {
        //         printf("%s %s is giong to %s\n", students[i].firstName, students[i].lastName, students[i].classes[j]);
        //     }
        // }
    }
    return 0;
}
