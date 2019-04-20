#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

 // Struct of a prof. which keeps his/her name, surname, num of classes he/she
 // is trained for, num of classes he/she still can teach(not greater then 2),
 // list of courses he/she is trained for and the flag which shows if the prof
 // teaches a course he/she is not trained for.
struct proff {
    int numOfClasses;
    int coursesAvailable;
    int isTeachingUntrained;
    char firstName[25];
    char lastName[25];
    char trained[30][20];
};

// Struct of a TA which keeps his/her name, surname, num of classes he/she
// is trained for, num of classes he/she still can teach(not greater then 4) and
// list of courses he/she is trained for
struct TA {
    int numOfClasses;
    int coursesAvailable;
    char firstName[25];
    char lastName[25];
    char trained[30][20];
};

// Auxiliary struct of a subject used in struct of a student
// Stores course's name and flag which shows if this subject is studied by a student
struct subject {
    char name[20];
    int isStudied;
};

// Struct of student which keeps his/her name, id, number of classes he/she wants to attend and
// list of structs of these subjects
struct student {
    char id[6];
    int numOfClasses;
    char firstName[25];
    char lastName[25];
    struct subject subjects[15];
};

// Struct of a course which containes it's name, assigned prof, list of assigned TAs
// number of required labs and students, counter for labs which are not taken by any
// of TAs, flag which shows if this course is runned and flag which shows if this course
// can be assigned to a prof (I need two flags to have opportunity to not run a course on purpose,
// e.g. it is not runned, but no prof can take it).
struct course {
    int isAvailable;
    int isRunned;
    struct proff assignedP;
    struct TA assignedTA[10];
    int numOfStudents;
    int numOfLabs;
    int labsAvailable;
    char name[50];
};

// Structs of arrays. I keep arrays in this way because it is convenient to keep
// array's length near the array itself.
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

struct studArr {
    struct student arr[20];
    int arrLen;
};

// STruct result which is overwritten during program and which is supposed to
// keep information about best combination of profs and TAs.
struct result {
    struct courseArr c;
    struct profArr p;
    struct TAArr ta;
};

// Function which cheks if a prof is trained for a course
int trainedP(struct proff p, struct course c) {
    for (size_t i = 0; i < p.numOfClasses; i++) {
        int flag = strcmp(p.trained[i], c.name);
        if (flag == 0) {
            return 1;
        }
    }
    return 0;
}

// Same but for TA
int trainedTA(struct TA ta, struct course c) {
    for (size_t i = 0; i < ta.numOfClasses; i++) {
        int flag = strcmp(ta.trained[i], c.name);
        if (flag == 0) {
            return 1;
        }
    }
    return 0;
}

// Same but for students
int isWanted(struct student s, struct course c) {
    for (size_t i = 0; i < s.numOfClasses; i++) {
        int flag = strcmp(s.subjects[i].name, c.name);
        if (flag == 0) {
            return 1;
        }
    }
    return 0;
}

// Second most important function in program which chooses the best combination of TA
// Function is recursive
void assignTA(struct TAArr ta, struct courseArr c, int bp, int * gbp, struct result * res, struct profArr p) {
    //Flags for base cases
    int flag1 = 1;
    int flag2 = 1;
    int flag3 = 1;
    // Base case 1 - no more free TAs
    for (int i = 0; i < ta.arrLen; i++) {
        if (ta.arr[i].coursesAvailable != 0) {
            flag1 = 0;
            break;
        }
    }
    // Base case 2 - no more available courses
    for (int i = 0; i < c.arrLen; i++) {
        if (c.arr[i].labsAvailable != 0) {
            flag2 = 0;
            break;
        }
    }
    // Base case 3 - there are available corses and free TA, but TA are not trained
    // for any of the courses
    for (int i = 0; i < c.arrLen; i++) {
        if (c.arr[i].labsAvailable != 0) {
            for (int j = 0; j < ta.arrLen; j++) {
                if (ta.arr[j].coursesAvailable != 0) {
                    if (trainedTA(ta.arr[j], c.arr[i]) == 1) {
                        flag3 = 0;
                        break;
                    }
                }
            }
        }
    }
    if (flag1 == 1 || flag2 == 1 || flag3 == 1) {
        // Terminating stage of a call of the function
        // Following block of code checks if there are any courses whuch were ran at the
        // stage of the distribution of profs, but cannot be ran because of TAs and updates data
        // respectively.
        for (int j = 0; j < c.arrLen; j++) {
            if (c.arr[j].labsAvailable != 0) {
                if (c.arr[j].isRunned == 1) {
                    for (int k = 0; k < p.arrLen; k++) {
                        if (strcmp(p.arr[k].firstName, c.arr[j].assignedP.firstName) == 0 && strcmp(p.arr[k].lastName, c.arr[j].assignedP.lastName) == 0) {
                            bp += 5; //Add BP, update data in prof's struct
                            if (p.arr[k].isTeachingUntrained == 1) {
                                p.arr[k].coursesAvailable += 2;
                            } else {
                                p.arr[k].coursesAvailable += 1;
                            }
                        }
                    }
                    for (int k = 0; k < ta.arrLen; k++) {
                        int tempDel = 0; // Update data in TAs' structs
                        for (int m = 0; m < c.arr[j].numOfLabs - c.arr[j].labsAvailable; m++) {
                            if (strcmp(ta.arr[k].firstName, c.arr[j].assignedTA[m].firstName) == 0 && strcmp(ta.arr[k].lastName, c.arr[j].assignedTA[m].lastName) == 0) {
                                tempDel++;
                            }
                        }
                        ta.arr[k].coursesAvailable += tempDel;
                    }
                    c.arr[j].isRunned = 0; // Make course not ran
                    bp += 20; // Add BP
                }
            }
        }
        for (int j = 0; j < ta.arrLen; j++) {
            bp += ta.arr[j].coursesAvailable * 2; // Add 2 BP for each lacking course
        }

        // If current BP are less then the global ones, then update BP and struct of results
        if (bp < * gbp) {
            * gbp = bp;
            res - > p = p;
            res - > ta = ta;
            res - > c = c;
        }
        return;
    }
    //*****END OF BASE CASE*****

    // Now the function goes through all TAs and treis to assign them to a course
    for (int i = 0; i < ta.arrLen; i++) {
        if (ta.arr[i].coursesAvailable == 0) { // If TA has already taken 4 labs, then skip him/her
            continue;
        } else {
            ta.arr[i].coursesAvailable--; // Else - decrease the number of labs he/she can take
        }

        // Noe find an appropriate course
        int cur = 0; // Index of course
        int curFlag = 0; // Flag which shows whether we've chosen a course or not
        for (int j = 0; j < c.arrLen; j++) {
            if (c.arr[j].labsAvailable > 0 && c.arr[j].isRunned == 1 && trainedTA(ta.arr[i], c.arr[j]) == 1) { // Course must need more labs, be ran by a prof and suitable for current TA
                cur = j;
                curFlag = 1;
                c.arr[j].labsAvailable--;
                break;
            }
        }
        if (curFlag == 0) {
            ta.arr[i].coursesAvailable++; // If we haven't chosen any proper course, take next TA
            continue;
        }

        c.arr[cur].assignedTA[c.arr[cur].numOfLabs - c.arr[cur].labsAvailable - 1] = ta.arr[i]; //Assign current TA to the course

        assignTA(ta, c, bp, gbp, res, p); // Recursive call

        // Update data and try another connbination
        ta.arr[i].coursesAvailable++;
        c.arr[cur].labsAvailable++;
    }
}


// The most important function in the program
// Works in the same manner as th one for TA with minor differences
void assignP(struct profArr p, struct courseArr c, int bp, int * gbp, struct result * res, struct TAArr ta) {
    int flag1 = 1;
    int flag2 = 1;
    // Base case 1 - no more free profs
    for (int i = 0; i < p.arrLen; i++) {
        if (p.arr[i].coursesAvailable != 0) {
            flag1 = 0;
            break;
        }
    }
    // Base case 2 - no more free courses
    for (int i = 0; i < c.arrLen; i++) {
        if (c.arr[i].isAvailable != 0) {
            flag2 = 0;
            break;
        }
    }
    if (flag1 == 1 || flag2 == 1) {
        // Assign BP respectively
        for (int i = 0; i < c.arrLen; i++) {
            if (c.arr[i].isRunned == 0) {
                bp += 20;
            }
        }
        for (int j = 0; j < p.arrLen; j++) {
            if (p.arr[j].coursesAvailable == 1) {
                bp += 5;
            } else if (p.arr[j].coursesAvailable == 2) {
                bp += 10;
            } else if (p.arr[j].coursesAvailable == 0 && p.arr[j].isTeachingUntrained == 1) {
                bp += 5;
            }
        }

        // Run distribution for TAs
        assignTA(ta, c, bp, gbp, res, p);
        return;
    }
    //*****END OF BASE CASE*****

    // The body of the function
    // In this funcion I go through all profs and one more time.
    // It is needed to modulate the situation when the curret course is not ran by anybody
    // We need such situation for the correct distribution of BP on the stage of distributing TAs
    for (int i = 0; i < p.arrLen + 1; i++) {
        if (i == p.arrLen) { // If this is the last iteration, do not assign this course to anybody, but make it unavailable for assignment too
            int cur;
            for (int j = 0; j < c.arrLen; j++) {
                if (c.arr[j].isAvailable == 1) {
                    cur = j;
                    c.arr[j].isAvailable = 0;
                    c.arr[j].isRunned = 0;
                    break;
                }
            }
            assignP(p, c, bp, gbp, res, ta); // Recursive call
            c.arr[cur].isAvailable = 1; // Make it available again

        } else { // If it is not the last iteration
            if (p.arr[i].coursesAvailable == 0) { // If prof cannot take any course, skip him/her
                continue;
            } else {
                p.arr[i].coursesAvailable--;
            }

            int cur; // Choose the availiable course
            for (int j = 0; j < c.arrLen; j++) {
                if (c.arr[j].isAvailable == 1) {
                    cur = j;
                    c.arr[j].isAvailable = 0;
                    c.arr[j].isRunned = 1;
                    break;
                }
            }

            //Check if the prof is not trained for current course and assign flags the right way
            int untrainedflag = 0;
            if (trainedP(p.arr[i], c.arr[cur]) == 0) {
                if (p.arr[i].coursesAvailable == 1) {
                    p.arr[i].coursesAvailable--;
                    untrainedflag = 1;
                    p.arr[i].isTeachingUntrained = 1;
                } else { // If prof is untrained, but already has a course, skip him/her
                    c.arr[cur].isAvailable = 1;
                    c.arr[cur].isRunned = 0;
                    p.arr[i].coursesAvailable++;
                    continue;
                }
            }

            // Assign this prof for this course
            c.arr[cur].assignedP = p.arr[i];

            assignP(p, c, bp, gbp, res, ta); // Recursive call

            // Update data
            if (untrainedflag == 1) {
                p.arr[i].coursesAvailable += 2;
            } else {
                p.arr[i].coursesAvailable++;
            }
            c.arr[cur].isAvailable = 1;
            c.arr[cur].isRunned = 0;
        }
    }
}

// Main body of the programm where the data is parsed, proper functions are ran and BP distributed
int main() {
    // Here the current dirrectory is scanned, number of files starting with "input" are counted
    struct dirent * de;
    DIR * dr = opendir(".");
    FILE * output;
    output = fopen("AlexanderKrivonosovEmail.txt", "w");
    fprintf(output, "a.krivonosov@innopolis.university");
    if (dr == NULL) {
        printf("Could not open current directory\n");
        return 0;
    }
    // Here I find input file with maximum index and male it the last
    int numOfFiles = 0;
    while ((de = readdir(dr)) != NULL) {
        char * str = de -> d_name;
        if (str[0] == 'i' && str[1] == 'n' && str[2] == 'p' && str[3] == 'u' && str[4] == 't') {
            if (numOfFiles < (int) strtol( & str[5], NULL, 10) && (int) strtol( & str[5], NULL, 10) <= 50) {
                numOfFiles = (int) strtol( & str[5], NULL, 10);
            }
        }
    }
    closedir(dr);

    //**********START OF THE PARSING**********

    for (int iterator = 1; iterator <= numOfFiles; iterator++) {
        // Go through files including the nonexisting ones
        // Block of code with initialazing needed variables and arrays
        char name[20] = "input";
        char outName[50] = "AlexanderKrivonosovOutput";
        char tempIter[numOfFiles + 1];
        sprintf(tempIter, "%d", iterator);
        strcat(name, tempIter);
        strcat(name, ".txt");
        strcat(outName, tempIter);
        strcat(outName, ".txt");
        FILE * input;
        output = fopen(outName, "w");
        if ((input = fopen(name, "r")) == NULL) {
            fprintf(output, "Invalid input.");
            fclose(output);
            continue;
        }

        struct student students[50];
        struct TA TAs[50];
        struct course courses[50];
        struct proff proffs[50];
        char temp[500]; // Array for input lines

        // Parsing of courses
        // Get string, split it in three strings and assign respectively
        int numOfCourses = 0;
        while (1) {
            char buffer[256];
            if (fgets(buffer, 256, input) == NULL) {
                fprintf(output, "Invalid input.");
                goto next;
            }

            char courseName[64];
            int labsNeeded;
            int studentsAllowed;

            int read = sscanf(buffer, "%s %d %d", & courseName, & labsNeeded, & studentsAllowed);

            if (read != 3 && strcmp(courseName, "P") == 0) {
                break;
            }else {
                fprintf(output, "Invalid input.");
                goto next;
            }

            // Write information to proper fields of the struct
            courses[numOfCourses].numOfLabs = labsNeeded;
            strcpy(courses[numOfCourses].name, courseName);
            courses[numOfCourses].numOfStudents = studentsAllowed;
            numOfCourses++;
        }

        // Reading profs works in the same manner, but with some changes
        int numOfProffs = 0;
        while (1) {
            char * origBuffer = malloc(sizeof(char) * 1024); // The string buffer
            char * buffer = origBuffer; // Pointer to place we stopped (since we do not know the number of the subjects)

            if (fgets(buffer, 1024, input) == NULL) {
                fprintf(output, "Invalid input.");
                goto next;
            }

            char tBuf;
            int wordsRead = sscanf(buffer, "%c", & tBuf);
            if (wordsRead == 1 && tBuf == 'T') {
                break;
            }else {
                fprintf(output, "Invalid input.");
                goto next;
            }

            // Scan name and surname in this block of code
            int bytesRead = 0;
            wordsRead = sscanf(buffer, "%s %s%n", & (proffs[numOfProffs].firstName), & (proffs[numOfProffs].lastName), & bytesRead);

            if (wordsRead != 2) {
                fprintf(output, "Invalid input.");
                goto next;
            }

            char courseBuffer[100];

            // Loop for reading the subjects
            while (1) {
                buffer += bytesRead;
                wordsRead = sscanf(buffer, "%100s%n", & courseBuffer, & bytesRead);
                if (wordsRead != 1) {
                    goto cleanup;
                }
                int flag = 1;
                // Check if this course is not in the list of courses
                for (int j = 0; j < numOfCourses; j++) {
                    int check = strcmp(courses[j].name, courseBuffer);
                    if (check == 0) {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1) {
                    fprintf(output, "Invalid input.");
                    goto next;
                }
                // Write data to fields of the struct
                strcpy(proffs[numOfProffs].trained[proffs[numOfProffs].numOfClasses], courseBuffer);
                proffs[numOfProffs].numOfClasses++;
            }

            cleanup:
                free(origBuffer);
                numOfProffs++;
        }

        // Reading information about TAs works absolutely the same way
        int numOfTAs = 0;
        while (1) {
            char * origBuffer = malloc(sizeof(char) * 1024);
            char * buffer = origBuffer;

            int totalBytesRead = 0;

            if (fgets(buffer, 1024, input) == NULL) {
                fprintf(output, "Invalid input.");
                goto next;
            }

            char sBuf;
            int wordsRead = sscanf(buffer, "%c", & sBuf);
            if (wordsRead == 1 && sBuf == 'S') {
                break;
            }

            int bytesRead = 0;
            wordsRead = sscanf(buffer, "%s %s%n", & (TAs[numOfTAs].firstName), & (TAs[numOfTAs].lastName), & bytesRead);

            if (wordsRead != 2) {
                fprintf(output, "Invalid input.");
                goto next;
            }

            char courseBuffer[100];

            while (1) {
                buffer += bytesRead;
                wordsRead = sscanf(buffer, "%100s%n", & courseBuffer, & bytesRead);
                if (wordsRead != 1) {
                    goto ta_cleanup;
                }
                int flag = 1;
                for (int j = 0; j < numOfCourses; j++) {
                    int check = strcmp(courses[j].name, courseBuffer);
                    if (check == 0) {
                        flag = 0;
                        break;
                    }
                }
                if (flag == 1) {
                    fprintf(output, "Invalid input.");
                    goto next;
                }
                strcpy(TAs[numOfTAs].trained[TAs[numOfTAs].numOfClasses], courseBuffer);
                TAs[numOfTAs].numOfClasses++;
            }
            ta_cleanup:
                free(origBuffer);
            numOfTAs++;
        }

        // Reading students
        int numOfStudents = 0;
        if (fgets(temp, 500, input) == NULL) {
            fprintf(output, "Invalid input.");
        }
        int i = 0;
        int j;
        int k;
        while (1) {
            char tempStr[50];
            k = 0;
            j = 0;
            while (temp[j] != 32) { // Go until the space is met
                tempStr[k] = temp[j];
                j++;
                k++;
            }
            tempStr[k] = '\0';
            strcpy(students[i].firstName, tempStr); // Write collected information to the struct
            k = 0;
            j++;

            // Do it again
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
            strcpy(students[i].id, tempStr);
            k = 0;
            j++;

            // Go until the end of the line or of the file
            students[i].numOfClasses = 0;
            while (temp[j] != '\n' && temp[j] != '\0' && temp[j] != '\r') {
                if (temp[j] == 32) { // If the space is met, add info to the struct
                    tempStr[k] = '\0';

                    // Check if the course is in the list of courses
                    int flag = 1;
                    for (int j = 0; j < numOfCourses; j++) {
                        int check = strcmp(courses[j].name, tempStr);
                        if (check == 0) {
                            flag = 0;
                            break;
                        }
                    }
                    if (flag == 1) {
                        fprintf(output, "Invalid input.");
                        goto next;
                    }
                    snprintf(students[i].subjects[students[i].numOfClasses].name, sizeof(students[i].subjects[students[i].numOfClasses].name), "%s", tempStr);
                    students[i].numOfClasses++;
                    k = 0;
                    j++;
                }
                tempStr[k] = temp[j];
                k++;
                j++;
            }
            // End of the line or of the file
            tempStr[k] = '\0';

            // Check if the course is in the list of courses
            int flag = 1;
            for (int j = 0; j < numOfCourses; j++) {
                int check = strcmp(courses[j].name, tempStr);
                if (check == 0) {
                    flag = 0;
                    break;
                }
            }
            if (flag == 1) {
                fprintf(output, "Invalid input.");
                goto next;
            }
            snprintf(students[i].subjects[students[i].numOfClasses].name, sizeof(students[i].subjects[students[i].numOfClasses].name), "%s", tempStr);
            students[i].numOfClasses++;
            i++;
            numOfStudents++;

            if (temp[j] == '\0') {
                break;
            }

            fgets(temp, 500, input);
        }

        //**********************END OF THE PARSING****************************

        // Now fill structs with apropriate information needed for algorithm
        for (size_t i = 0; i < numOfStudents; i++) {
            for (size_t j = 0; j < students[i].numOfClasses; j++) {
                students[i].subjects[j].isStudied = 0;
            }
        }

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
        struct TAArr list2;
        list2.arrLen = 0;
        for (size_t i = 0; i < numOfTAs; i++) {
            list2.arr[i] = TAs[i];
            list2.arr[i].coursesAvailable = 4;
            list2.arrLen++;
        }

        struct result res;

        // Run the main function
        assignP(list, list1, badPoints, & badPointsG, & res, list2);

        // Distribute students
        for (int i = 0; i < numOfStudents; i++) {
            for (int j = 0; j < res.c.arrLen; j++) {
                if (isWanted(students[i], res.c.arr[j]) == 1 && res.c.arr[j].numOfStudents > 0 && res.c.arr[j].isRunned == 1) {
                    for (int k = 0; k < students[i].numOfClasses; k++) {
                        if (strcmp(students[i].subjects[k].name, res.c.arr[j].name) == 0) {
                            students[i].subjects[k].isStudied = 1;
                            res.c.arr[j].numOfStudents--;
                        }
                    }
                }
            }
        }

        // Calculate BP
        int ggbp = 0;

        for (int i = 0; i < res.c.arrLen; i++) {
            if (res.c.arr[i].isRunned == 0) {
                ggbp += 20;
            }
        }
        for (int i = 0; i < res.p.arrLen; i++) {
            if (res.p.arr[i].coursesAvailable == 2) {
                ggbp += 10;
            }
            if (res.p.arr[i].coursesAvailable == 1) {
                ggbp += 5;
            }
            if (res.p.arr[i].coursesAvailable == 0 && res.p.arr[i].isTeachingUntrained == 1) {
                ggbp += 5;
            }
        }
        for (int i = 0; i < res.ta.arrLen; i++) {
            ggbp += res.ta.arr[i].coursesAvailable * 2;
        }
        for (int i = 0; i < numOfStudents; i++) {
            for (int j = 0; j < students[i].numOfClasses; j++) {
                if (students[i].subjects[j].isStudied == 0) {
                    ggbp++;
                }
            }
        }

        // Output
        for (int i = 0; i < res.c.arrLen; i++) {
            fprintf(output, "%s\n", res.c.arr[i].name);
            if (res.c.arr[i].isRunned == 1) {
                fprintf(output, "%s %s\n", res.c.arr[i].assignedP.firstName, res.c.arr[i].assignedP.lastName);
                for (int j = 0; j < res.c.arr[i].numOfLabs; j++) {
                    fprintf(output, "%s %s\n", res.c.arr[i].assignedTA[j].firstName, res.c.arr[i].assignedTA[j].lastName);
                }
                for (int j = 0; j < numOfStudents; j++) {
                    if (isWanted(students[j], res.c.arr[i]) == 1) {
                        fprintf(output, "%s %s %s\n", students[j].firstName, students[j].lastName, students[j].id);
                    }
                }
            }
            fprintf(output, "\n");
        }

        for (int i = 0; i < res.c.arrLen; i++) {
            if (res.c.arr[i].isRunned == 0) {
                fprintf(output, "%s cannot be run.\n", res.c.arr[i].name);
            }
        }
        for (int i = 0; i < res.p.arrLen; i++) {
            if (res.p.arr[i].coursesAvailable == 2) {
                fprintf(output, "%s %s is unassigned.\n", res.p.arr[i].firstName, res.p.arr[i].lastName);
            }
            if (res.p.arr[i].coursesAvailable == 1) {
                fprintf(output, "%s %s is lacking class.\n", res.p.arr[i].firstName, res.p.arr[i].lastName);
            }
        }
        for (int i = 0; i < res.c.arrLen; i++) {
            if (res.c.arr[i].assignedP.isTeachingUntrained == 1) {
                fprintf(output, "%s %s is not trained for %s.\n", res.c.arr[i].assignedP.firstName, res.c.arr[i].assignedP.lastName, res.c.arr[i].name);
            }
        }
        for (int i = 0; i < res.ta.arrLen; i++) {
            if (res.ta.arr[i].coursesAvailable > 0) {
                fprintf(output, "%s %s is lacking %d lab(s).\n", res.ta.arr[i].firstName, res.ta.arr[i].lastName, res.ta.arr[i].coursesAvailable);
            }
        }
        for (int i = 0; i < numOfStudents; i++) {
            for (int j = 0; j < students[i].numOfClasses; j++) {
                if (students[i].subjects[j].isStudied == 0) {
                    fprintf(output, "%s %s is lacking %s\n", students[i].firstName, students[i].lastName, students[i].subjects[j].name);
                }
            }
        }
        fprintf(output, "Total score is %d.", ggbp);
next:
        fclose(output);
        fclose(input);
    }
    return 0;
}
