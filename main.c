#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

int main(void){
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
        char* name;
        char trained[1000][100];
        int coursesAvaliable;
    };

    struct TA{
        char* name;
        char trained[1000][100];
        int coursesAvaliable;
    };

    struct course{
        char* name;
        int numOfStudents;
        int numOfLabs;
    };
    for (size_t iterator = 1; iterator <= numOfFiles; iterator++) {
        char name[8] = "input";
        char* tempIter;
        itoa(iterator, tempIter, 10);
        strcat(name, tempIter);
        strcat(name, ".txt");
        FILE *input;
        input = fopen(name, "r");

        struct course courses[50];

        char temp[50];
        fgets(temp, 500, input);
        int flag = strcmp(temp, "P\n");
        int i = 0;
        while (flag != 0) {
            char tempStr[50];
            int j = 0;
            while (temp[j] != 32) {
                tempStr[j] = temp[j];
                printf("Q\n");
                j++;
            }
            // strcpy(courses[i].name, tempStr);
            // printf("%s\n", tempStr);
            fgets(temp, 500, input);
            flag = strcmp(temp, "P\n");
        }
    }
    return 0;
}
