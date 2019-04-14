#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

struct proff{
    int numOfClasses;
    int coursesAvaliable;
    char firstName[100];
    char lastName[100];
    char trained[100][100];
};

struct TA{
    int numOfClasses;
    int coursesAvaliable;
    char firstName[100];
    char lastName[100];
    char trained[100][100];
};

struct student{
    int id;
    int numOfClasses;
    char firstName[100];
    char lastName[100];
    char classes[100][100];
};

struct course{
    int numOfStudents;
    int numOfLabs;
    char name[100];
};

struct courseArr{
    struct course arr[50];
    int arrLen;
};
struct profArr{
    struct proff arr[50];
    int arrLen;
};
struct TAArr{
    struct proff arr[50];
    int arrLen;
};

int trainedP(struct proff p, struct course c){
    for (size_t i = 0; i < p.numOfClasses; i++) {
        int flag = strcmp(p.trained[i], c.name);
        if (flag == 0) {
            return 1;
        }
    }
    return 0;
}

void deleteP(struct proff p, struct profArr* pArr){
    for (size_t i = 0; i < pArr->arrLen; i++) {
        int flag1 = strcmp(p.firstName, pArr->arr[i].firstName);
        int flag2 = strcmp(p.lastName, pArr->arr[i].lastName);
        if (flag1 == 0 && flag2 == 0) {
            for (size_t j = i; j < pArr->arrLen - 1; j++) {
                pArr->arr[j] = pArr->arr[j+1];
            }
            pArr->arrLen--;
        }
    }
}

void deleteC(struct course c, struct courseArr* cArr){
    for (size_t i = 0; i < cArr->arrLen; i++) {
        int flag = strcmp(c.name, cArr->arr[i].name);
        if (flag == 0) {
            for (size_t j = i; j < cArr->arrLen - 1; j++) {
                cArr->arr[j] = cArr->arr[j+1];
            }
            cArr->arrLen--;
        }
    }
}

// void assignP(struct profArr p, struct courseArr c, int bp, int* globBP){
//
// }

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

        int i = 0;
        while (flag != 0) {
            char tempStr[50];
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
            courses[i].numOfLabs = (int)strtol(tempStr, NULL, 10);
            j++;

            k = 0;
            while (temp[j] != '\n') {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            courses[i].numOfStudents = (int)strtol(tempStr, NULL, 10);

            fgets(temp, 500, input);
            flag = strcmp(temp, "P\n");
            i++;
            numOfCourses++;
        }
        // printf("%s:\n\nNumber Of Courses: %d\n", name, numOfCourses);
        // for (size_t i = 0; i < numOfCourses; i++) {
        //     printf("%s requieres %d labs and can accept %d students\n", courses[i].name, courses[i].numOfLabs, courses[i].numOfStudents);
        // }

        int numOfProffs = 0;
        i = 0;
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
        // printf("\nNumber Of Proffs: %d, \n", numOfProffs);
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
        // printf("\nNumber Of TAs: %d, \n", numOfTAs);
        // for (size_t i = 0; i < numOfTAs; i++) {
        //     for (size_t j = 0; j < TAs[i].numOfClasses; j++) {
        //         printf("%s %s is trained for %s\n", TAs[i].firstName, TAs[i].lastName, TAs[i].trained[j]);
        //     }
        // }

        int numOfStudents = 0;
        fgets(temp, 500, input);
        i = 0;
        int j;
        int k;
        while (1) {
            char tempStr[50];
            k = 0;
            j = 0;
            while (temp[j] != 32) {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            strcpy(students[i].firstName, tempStr);
            k = 0;
            j++;

            while (temp[j] != 32) {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            strcpy(students[i].lastName, tempStr);
            k = 0;
            j++;

            while (temp[j] != 32) {
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            students[i].id = (int)strtol(tempStr, NULL, 10);
            k = 0;
            j++;

            students[i].numOfClasses = 0;
            while (temp[j] != '\n' && temp[j] != '\0') {
                if (temp[j] == 32) {
                    tempStr[k] = '\0';
                    strcpy(students[i].classes[students[i].numOfClasses], tempStr);
                    students[i].numOfClasses++;
                    k = 0;
                    j++;
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

            if (temp[j] == '\0') {
                break;
            }

            fgets(temp, 500, input);
        }
        // printf("\nNumber Of Sudents: %d, \n", numOfStudents);
        // for (size_t i = 0; i < numOfStudents; i++) {
        //     for (size_t j = 0; j < students[i].numOfClasses; j++) {
        //         printf("%s %s with id %d is attending %s\n", students[i].firstName, students[i].lastName, students[i].id, students[i].classes[j]);
        //     }
        // }
        // printf("\n");

        int badPoints = INT_MAX;

        struct profArr list;
        list.arrLen = 0;
        for (size_t i = 0; i < numOfProffs; i++) {
            list.arr[i] = proffs[i];
            list.arrLen++;
        }

        struct courseArr list1;
        list1.arrLen = 0;
        for (size_t i = 0; i < numOfCourses; i++) {
            list1.arr[i] = courses[i];
            list1.arrLen++;
        }

    }
    return 0;
}
