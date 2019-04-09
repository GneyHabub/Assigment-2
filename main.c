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
        char trained[1000][100];
        int coursesAvaliable;
    };

    struct TA{
        char firstName[100];
        char lastName[100];
        char trained[1000][100];
        int coursesAvaliable;
    };

    struct course{
        char name[100];
        int numOfStudents;
        int numOfLabs;
    };
    for (int iterator = 1; iterator <= numOfFiles; iterator++) {
        char name[8] = "input";
        char tempIter[numOfFiles + 1];
        sprintf(tempIter, "%d", iterator);
        strcat(name, tempIter);
        strcat(name, ".txt");
        FILE *input;
        input = fopen(name, "r");

        struct course courses[50];
        struct proff proffs[50];
        char temp[50];
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
            printf("%d\n", courses[i].numOfStudents);

            fgets(temp, 500, input);
            flag = strcmp(temp, "P\n");
            i++;
            numOfCourses++;
        }

        // int numOfProffs;
        // fgets(temp, 500, input);
        // flag = strcmp(temp, "T\n");
        // while (flag != 0) {
        //     char tempStr[50];
        //     int i = 0;
        //     int j = 0;
        //     int k = 0;
        //
        //     while (temp[j] != 32){
        //         tempStr[j] = temp[j];
        //         j++;
        //     }
        //     tempStr[j] = '\0';
        //     strcpy(proffs[i].firstName, tempStr);
        //
        //     j++;
        //     k = 0;
        //     while (temp[j] != 32){
        //         tempStr[k] = temp[j];
        //         j++;
        //         k++;
        //     }
        //     tempStr[k] = '\0';
        //     j++;
        //     strcpy(proffs[i].lastName, tempStr);
        //     printf("%s %s\n", proffs[i].firstName, proffs[i].lastName);
        //
        //     fgets(temp, 500, input);
        //     flag = strcmp(temp, "T\n");
        //     i++;
        //     numOfProffs++;
        // }
    }
    return 0;
}
