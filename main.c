#include <stdio.h>

#include <dirent.h>

#include <string.h>

#include <limits.h>

#include <stdlib.h>

struct proff {
    int numOfClasses;
    int coursesAvailable;
    int isTeachingUntrained;
    char firstName[25];
    char lastName[25];
    char trained[30][20];
};

struct TA {
    int numOfClasses;
    int coursesAvailable;
    char firstName[25];
    char lastName[25];
    char trained[30][20];
};

struct student {
    int id;
    int numOfClasses;
    char firstName[25];
    char lastName[25];
    char classes[30][20];
};

struct course {
    int isAvailable;
    int isRunned;
    struct proff assignedP;
    struct TA assignedTA[5];
    int numOfStudents;
    int numOfLabs;
    int labsAvailable;
    char name[50];
};

struct courseArr {
    struct course arr[20];
    int arrLen;
};
struct profArr {
    struct proff arr[20];
    int arrLen;
};
struct TAArr {
    struct TA arr[20];
    int arrLen;
};

struct result {
    struct courseArr c;
    struct profArr p;
    struct TAArr ta;
};

int trainedP(struct proff p, struct course c) {
    for (size_t i = 0; i < p.numOfClasses; i++) {
        int flag = strcmp(p.trained[i], c.name);
        if (flag == 0) {
            return 1;
        }
    }
    return 0;
}

int trainedTA(struct TA ta, struct course c) {
    for (size_t i = 0; i < ta.numOfClasses; i++) {
        int flag = strcmp(ta.trained[i], c.name);
        if (flag == 0) {
            return 1;
        }
    }
    return 0;
}

void assignTA(struct TAArr ta, struct courseArr c, int bp, int* gbp, struct result* res, struct profArr p){
    int flag1 = 1;
    int flag2 = 1;
    int flag3 = 1;
    for(int i = 0; i < ta.arrLen; i++){
       // printf("%d\n", ta.arrLen);
        if (ta.arr[i].coursesAvailable != 0){
            //printf("%s - %d\n", ta.arr[i].firstName, ta.arr[i].coursesAvailable);
            flag1 = 0;
            break;
        }
    }
    //printf("THATS IT\n");

    for(int i = 0; i < c.arrLen; i++){
        if (c.arr[i].labsAvailable != 0){
            flag2 = 0;
            break;
        }
    }
    for(int i = 0; i < c.arrLen; i++){
        if(c.arr[i].labsAvailable != 0){
            //printf("Course is %s\n", c.arr[i].name);
            for(int j = 0; j < ta.arrLen; j++){
                //printf("TA has %d courses available\n", ta.arr[j].coursesAvailable);
                if(ta.arr[j].coursesAvailable != 0){
                    //printf("%s %s is checked with %s\n", ta.arr[j].firstName, ta.arr[j].lastName, c.arr[i].name);
                    if(trainedTA(ta.arr[j], c.arr[i]) == 1){
                        //printf("%s %s still can be assigned to %s\n", ta.arr[j].firstName, ta.arr[j].lastName, c.arr[i].name);
                        flag3 = 0;
                        break;
                    }
                }
            }
        }
    }
    if(flag3 == 1){
        for(int i = 0; i < ta.arrLen; i++){
            //printf(" STILL CAN HAVE %d\n", ta.arr[i].coursesAvailable);
        }

    }
    if (flag1 == 1 || flag2 == 1 || flag3 == 1) {
        for(int j = 0; j < c.arrLen; j++){
            if(c.arr[j].labsAvailable != 0){
                if(c.arr[j].isRunned = 1){
                    for(int k = 0; k < p.arrLen; k++){
                        if(strcmp(p.arr[k].firstName, c.arr[j].assignedP.firstName) == 0 && strcmp(p.arr[k].lastName, c.arr[j].assignedP.lastName) == 0){
                            bp += 5;
                            //printf("%s %s is teaching %s but course cannot be runned\n", c.arr[j].assignedP.firstName, c.arr[j].assignedP.lastName, c.arr[j].name);
                            if(p.arr[k].isTeachingUntrained == 1){
                                p.arr[k].coursesAvailable += 2;
                            }else{
                                p.arr[k].coursesAvailable += 1;
                            }
                        }
                    }
                    for(int k = 0; k < ta.arrLen; k++){
                        int tempDel = 0;
                        for(int m = 0; m < c.arr[j].numOfLabs - c.arr[j].labsAvailable; m++){
                                if(strcmp(ta.arr[k].firstName, c.arr[j].assignedTA[m].firstName) == 0 && strcmp(ta.arr[k].lastName, c.arr[j].assignedTA[m].lastName) == 0){
                                    tempDel++;
                                }
                            }
                        ta.arr[k].coursesAvailable += tempDel;
                    }
                    c.arr[j].isRunned = 0;
                    bp += 20;
                }
            }
        }
        for(int j = 0; j < ta.arrLen; j++){
            //printf("TA STILL HAS %d\n", ta.arr[j].coursesAvailable);
            bp += ta.arr[j].coursesAvailable*2;
        }

        //printf("WERE OVER %d\n", bp);
        if (bp < *gbp) {
            //printf("UPDATED\n");
            *gbp = bp;
            res->p = p;
            res->ta = ta;
            res->c = c;
        }
        return;
    }

    for(int i = 0; i < ta.arrLen; i++){
        if(ta.arr[i].coursesAvailable == 0){
            continue;
        }else{
            ta.arr[i].coursesAvailable--;
            //printf("%s %s\n", ta.arr[i].firstName, ta.arr[i].lastName);
        }

        int cur = 0;
        int curFlag = 0;
        for(int j = 0; j < c.arrLen; j++){
            if(c.arr[j].labsAvailable > 0 && c.arr[j].isRunned == 1 && trainedTA(ta.arr[i], c.arr[j]) == 1){
                cur = j;
                curFlag = 1;
                c.arr[j].labsAvailable --;
                break;
            }
        }
         if(curFlag == 0){
            continue;
        }

        // if (trainedTA(ta.arr[i], c.arr[cur]) == 0) {
        //     c.arr[cur].labsAvailable++;
        //     ta.arr[i].coursesAvailable++;
        //     continue;
        // }
        c.arr[cur].assignedTA[c.arr[cur].numOfLabs - c.arr[cur].labsAvailable-1] = ta.arr[i];
        // printf("%s %s which has %d labs is assigned to %s\n", ta.arr[i].firstName, ta.arr[i].lastName, ta.arr[i].coursesAvailable, c.arr[cur].name);
        // printf("%d\n", c.arr[cur].labsAvailable);

        assignTA(ta, c, bp, gbp, res, p);

        ta.arr[i].coursesAvailable++;
        c.arr[cur].labsAvailable++;
    }
}

void assignP(struct profArr p, struct courseArr c, int bp, int* gbp, struct result* res, struct TAArr ta){
    int flag1 = 1;
    int flag2 = 1;
    for(int i = 0; i < p.arrLen; i++){
        if (p.arr[i].coursesAvailable != 0){
            flag1 = 0;
            break;
        }
    }
    for(int i = 0; i < c.arrLen; i++){
        if (c.arr[i].isAvailable != 0){
            flag2 = 0;
            break;
        }
    }
    if (flag1 == 1 || flag2 == 1) {
        for(int i = 0; i < c.arrLen; i++){
            if(c.arr[i].isRunned == 0){
                bp += 20;
            }
        }
        for(int j = 0; j < p.arrLen; j++){
            if(p.arr[j].coursesAvailable == 1){
                bp += 5;
            }else if(p.arr[j].coursesAvailable == 2){
                bp += 10;
            }else if(p.arr[j].coursesAvailable == 0 && p.arr[j].isTeachingUntrained == 1){
                bp += 5;
            }
         }
        for(int i = 0; i < c.arrLen; i++){
            if (c.arr[i].isRunned == 1){
            }
        }
        // for(int i = 0; i < c.arrLen; i++){
        //     if(c.arr[i].isRunned == 1){
        //         printf("%s is runned by %s %s\n", c.arr[i].name, c.arr[i].assignedP.firstName, c.arr[i].assignedP.lastName);
        //     }else{
        //         printf("%s is not runned\n", c.arr[i].name);
        //     }
        // }
        // printf("BEFORE GOING TO TA WE HAVE %d BP\n", bp);

        // for(int i = 0; i  < ta.arrLen; i++){
        //     printf("%s %s - %d\n", ta.arr[i].firstName, ta.arr[i].lastName, ta.arr[i].coursesAvailable);
        // }
        assignTA(ta, c, bp, gbp, res,  p);
        return;
    }

    for(int i = 0; i < p.arrLen + 1; i++){
        if(i == p.arrLen){
            int cur;
        for(int j = 0; j < c.arrLen; j++){

            if(c.arr[j].isAvailable == 1){
                cur = j;
                c.arr[j].isAvailable = 0;
                c.arr[j].isRunned = 0;
                break;
            }
        }
        assignP(p, c, bp, gbp, res, ta);
        c.arr[cur].isAvailable = 1;

        }else{
            if(p.arr[i].coursesAvailable == 0){
            continue;
        }else{
            p.arr[i].coursesAvailable--;
        }

        int cur;
        for(int j = 0; j < c.arrLen; j++){
            if(c.arr[j].isAvailable == 1){
                cur = j;
                c.arr[j].isAvailable = 0;
                c.arr[j].isRunned = 1;
                break;
            }
        }

        int untrainedflag = 0;
        if (trainedP(p.arr[i], c.arr[cur]) == 0) {
            if(p.arr[i].coursesAvailable == 1){
                p.arr[i].coursesAvailable--;
                //bp += 5;
                untrainedflag = 1;
                p.arr[i].isTeachingUntrained = 1;
            }else{
                c.arr[cur].isAvailable = 1;
                c.arr[cur].isRunned = 0;
                p.arr[i].coursesAvailable++;
                continue;
            }
        }

        c.arr[cur].assignedP = p.arr[i];


        assignP(p, c, bp, gbp, res, ta);

        if(untrainedflag == 1){
            p.arr[i].coursesAvailable += 2;
        } else{
            p.arr[i].coursesAvailable++;
        }
        c.arr[cur].isAvailable = 1;
        c.arr[cur].isRunned = 0;
        }
    }
}

int main() {
    struct dirent * de;
    DIR* dr = opendir(".");
    if (dr == NULL) {
        printf("Could not open current directory\n");
        return 0;
    }
    int numOfFiles = 0;
    while ((de = readdir(dr)) != NULL) {
        char * str = de -> d_name;
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
        FILE * input;
        if ((input = fopen(name, "r")) == NULL) {
            return 0;
        }

        struct student students[50];
        struct TA TAs[50];
        struct course courses[50];
        struct proff proffs[50];
        char temp[500];

        int numOfCourses = 0;
        while (1) {
            char buffer[256];
            if (fgets(buffer, 256, input) == NULL) {
                printf("Unable to read line\n");
                return -1;
            }

            char courseName[64];
            int labsNeeded;
            int studentsAllowed;

            int read = sscanf(buffer, "%s %d %d", & courseName, & labsNeeded, & studentsAllowed);



            // If read less than 3 items, probably encountered a 'P'
            if (read != 3 && strcmp(courseName, "P") == 0) {
                break;
            }

            courses[numOfCourses].numOfLabs = labsNeeded;
            strcpy(courses[numOfCourses].name, courseName);
            courses[numOfCourses].numOfStudents = studentsAllowed;


            numOfCourses++;
        }
        // printf("%s:\n\nNumber Of Courses: %d\n", name, numOfCourses);
        // for (size_t i = 0; i < numOfCourses; i++) {
        //     printf("%s requieres %d labs and can accept %d students\n", courses[i].name, courses[i].numOfLabs, courses[i].numOfStudents);
        // }

        int numOfProffs = 0;
        while (1) {
            char * origBuffer = malloc(sizeof(char) * 1024);
            char * buffer = origBuffer;

            if (fgets(buffer, 1024, input) == NULL) {
                printf("Unable to read line\n");
                return -1;
            }

            char tBuf;
            int wordsRead = sscanf(buffer, "%c", & tBuf);
            // See if we read T symbol
            if (wordsRead == 1 && tBuf == 'T') {
                break;
            }

            int bytesRead = 0;
            wordsRead = sscanf(buffer, "%s %s%n", &
                    (proffs[numOfProffs].firstName), &
                                       (proffs[numOfProffs].lastName), &
                                       bytesRead
            );

            if (wordsRead != 2) {
                printf("Unable to read prof. name");
                return -1;
            }

            char courseBuffer[100];

            while (1) {
                buffer += bytesRead;
                wordsRead = sscanf(buffer, "%100s%n", & courseBuffer, & bytesRead);
                if (wordsRead != 1) {
                    goto cleanup;
                }
                strcpy(proffs[numOfProffs].trained[proffs[numOfProffs].numOfClasses], courseBuffer);
                proffs[numOfProffs].numOfClasses += 1;
            }

            cleanup:
            free(origBuffer);
            numOfProffs++;
        }

        // printf("\nNumber Of Proffs: %d, \n", numOfProffs);
        // for (size_t i = 0; i < numOfProffs; i++) {
        //     for (size_t j = 0; j < proffs[i].numOfClasses; j++) {
        //         printf("%s %s is trained for %s\n", proffs[i].firstName, proffs[i].lastName, proffs[i].trained[j]);
        //     }
        // }


        int numOfTAs = 0;
        while (1) {
            char * origBuffer = malloc(sizeof(char) * 1024);
            char * buffer = origBuffer;

            int totalBytesRead = 0;

            if (fgets(buffer, 1024, input) == NULL) {
                printf("Unable to read line\n");
                return -1;
            }

            char sBuf;
            int wordsRead = sscanf(buffer, "%c", & sBuf);
            // See if we read S symbol
            if (wordsRead == 1 && sBuf == 'S') {
                break;
            }

            int bytesRead = 0;
            wordsRead = sscanf(buffer, "%s %s%n", &
                    (TAs[numOfTAs].firstName), &
                                       (TAs[numOfTAs].lastName), &
                                       bytesRead
            );

            if (wordsRead != 2) {
                printf("Unable to read TA name");
                return -1;
            }

            char courseBuffer[100];

            while (1) {
                buffer += bytesRead;
                wordsRead = sscanf(buffer, "%100s%n", & courseBuffer, & bytesRead);
                if (wordsRead != 1) {
                    goto ta_cleanup;
                }
                strcpy(TAs[numOfTAs].trained[TAs[numOfTAs].numOfClasses], courseBuffer);
                TAs[numOfTAs].numOfClasses += 1;
            }
            ta_cleanup:
            free(origBuffer);
            numOfTAs++;
        }

        // printf("\nNumber Of TAs: %d, \n", numOfTAs);
        // for (size_t i = 0; i < numOfTAs; i++) {
        //     for (size_t j = 0; j < TAs[i].numOfClasses; j++) {
        //         printf("%s %s is trained for %s\n", TAs[i].firstName, TAs[i].lastName, TAs[i].trained[j]);
        //     }
        // }


        int i = 0;
        fgets(temp, 500, input);
        int flag = strcmp(temp, "S\n");

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
            students[i].id = (int) strtol(tempStr, NULL, 10);
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
        printf("\n");

        int badPointsG = INT_MAX;
        int badPoints = 0;

        struct profArr list;
        list.arrLen = 0;
        for (size_t i = 0; i < numOfProffs; i++) {
            list.arr[i] = proffs[i];
            list.arr[i].coursesAvailable = 2;
            list.arr[i].isTeachingUntrained = 0;
            list.arrLen++;
        }

        struct courseArr list1;
        list1.arrLen = 0;
        for (size_t i = 0; i < numOfCourses; i++) {
            list1.arr[i] = courses[i];
            list1.arr[i].isAvailable = 1;
            list1.arr[i].isRunned = 0;
            list1.arr[i].labsAvailable = list1.arr[i].numOfLabs;
            list1.arrLen++;
        }
        printf("----------------------------------------------------------------NUM OF TAs - %d\n", numOfTAs);
        struct TAArr list2;
        list2.arrLen = 0;
        for (size_t i = 0; i < numOfTAs; i++) {
            list2.arr[i] = TAs[i];
            list2.arr[i].coursesAvailable = 4;
            list2.arrLen++;
        }

        struct result res;

        //assignTA(list2, list1, badPoints, &badPointsG, &res);
        assignP(list, list1, badPoints, &badPointsG, &res, list2);

        int ggbp;

        for(int i = 0; i < res.c.arrLen; i++){
          if(res.c.arr[i].isRunned == 0){
            ggbp += 20;
          }
        }
        for(int i = 0; i < res.p.arrLen; i++){
            printf("%s %s - %d\n", res.p.arr[i].firstName, res.p.arr[i].lastName, res.p.arr[i].coursesAvailable);
          if(res.p.arr[i].coursesAvailable == 2){
            ggbp += 10;
          }
          if(res.p.arr[i].coursesAvailable == 1){
            ggbp += 5;
          }
        }
        for(int i = 0; i < res.ta.arrLen; i++){
            printf("%d\n", res.ta.arr[i].coursesAvailable);
            ggbp += res.ta.arr[i].coursesAvailable*2;
        }

        for(int i = 0; i < res.c.arrLen; i++){
            if(res.c.arr[i].isRunned == 1){
                printf("%s %s is teaching %s\n", res.c.arr[i].assignedP.firstName, res.c.arr[i].assignedP.lastName, res.c.arr[i].name);
                if(res.c.arr[i].labsAvailable == 0){
                    for (int j = 0; j < res.c.arr[i].numOfLabs; j++){
                        printf("%s %s\n", res.c.arr[i].assignedTA[j].firstName, res.c.arr[i].assignedTA[j].lastName);
                    }
                }
            }
        }
        printf("Explicit point - %d, counted later - %d.\n", badPointsG, ggbp);
    }
    return 0;
}
