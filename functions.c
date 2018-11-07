#include<header.h>

int subjectCount;
char** subjectName;
HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD saved_attributes;

void (*func[])(RecList* a) =
{
    getInput,
    simpleStatistic,
    printRecStat,
    scoreDescendingSort,
    scoreAscendSort,
    numSort,
    nameSort,
    searchByNum,
    searchByName,
    statisticAnalysis,
    printTable,
    writeToFile,
    readFromFile
};

/*
 * Create an instance of record by given stu No. and score
 * Parameter:
 *      number:     given student No.
 *      score:      given score
 * return:
 *      Pointer of the new instance.
 */
Record* recConstructor(int number,int* score, char* name)
{
    Record* rec = (Record*)malloc(sizeof(Record));
    nullCheck(rec, "recConstructor");//check if malloc failed.
    rec->score = score;
    rec->number = number;
    rec->name = name;

    return rec;
}

/*
 * Create an empty instance of record list
 * return:
 *      Pointer of the new instance.
 */
RecList* listConstructor()
{
    RecList* list = (RecList*)malloc(sizeof(RecList));
    nullCheck(list, "listConstructor");        //check if malloc failed.
    list->capacity = INIT_CAPACITY;             //initial capacity of the array
    list->listSize = 0;
    list->data = (Record**)malloc(sizeof(Record*)*list->capacity);
    nullCheck(list->data,"listConstructor");    //check if malloc failed.

    return list;
}

/*
 * Construct a new instance of record
 * Return:
 *      A new instance of record.
 */
void recDeconstructor(Record* toDestroy)
{
    free(toDestroy->name);
    free(toDestroy->score);
    free(toDestroy);
}

/*
 * Construct a new instance of list
 * Return:
 *      A new instance of list.
 */
void listDeconstructor(RecList* toDestroy)
{
    for(int i =0; i<toDestroy->listSize; i++)
    {
        recDeconstructor(toDestroy->data[i]);
    }
    free(toDestroy->data);
    free(toDestroy);
}

void freeSubjectNames()
{
    for(int i=0;i<subjectCount;i++)
    {
        free(subjectName[i]);
    }
    free(subjectName);
    subjectCount = -1;
}

/*
 * Print messages with color
 * Parameter:
 *      color:      Pre-defined color code
 *      content:    msg to print
 */
void colorPrinter(WORD color, char* content)
{
    SetConsoleTextAttribute(hConsole, color|FOREGROUND_INTENSITY);
    printf("%s", content);
    SetConsoleTextAttribute(hConsole, saved_attributes);
}

/*
 * Check if a pointer is NULL. If yes, print error message and quit.
 * Parameter:
 *      ptr:        The pointer to be examined.
 *      errMsg:     The error massage to print.
 */
inline void nullCheck(void* ptr, char* errMsg)
{
    if(ptr==NULL)
    {
        colorPrinter(FOREGROUND_RED,"NULL PTR @ ");
        colorPrinter(FOREGROUND_RED,errMsg);
        system("pause");
        exit(1);
    }
}

/*
 * Num input with err check. Make user to input again if was invalid.
 * Parameter:
 *      in:     Where to store the input
 */
void safeNumInput(int* in)
{
    while(scanf("%d", in)!=1)
    {
        colorPrinter(FOREGROUND_RED, "Invalid input. Please try again: ");
        fflush(stdin);
    }
}

/*
 * Add an instance of record to an existing list
 * Parameter:
 *      toAppend:   Given list.
 *      toAdd:      The record to add.
 */
void pushBackRec(RecList* toAppend, Record* toAdd)
{
    toAppend->data[toAppend->listSize++] = toAdd;
    if(toAppend->listSize==toAppend->capacity)//if is about to overflow
    {
        toAppend->capacity*=2;//double the capacity to reduce the realloc call count
        Record** ptr = (Record**)realloc(toAppend->data,
                                         sizeof(Record*) * toAppend->capacity);
        nullCheck(ptr,"pushBackStu");
        toAppend->data = ptr;
    }
}

/*
 * Add records to an existing list via keyboard input
 * Parameter:
 *      toAppend:   Given list.
 */
void getInput(RecList* toAppend)
{
    printf("please input Student No. and score. Input 0 to End.\n");
    int stuNum;
    while(1)
    {
        int* score = (int*)malloc(sizeof(int)*subjectCount);
        char* name = (char*)malloc(sizeof(char)*MAX_NAME_LENGTH);
        printf("Please input Student number: ");
        safeNumInput(&stuNum);

        if(!stuNum)
        {
            break;//end input
        }

        getchar();
        printf("Please input Student's name: ");
        gets(name);

        for(int i =0; i<subjectCount; i++)
        {
            printf("Please input Student's score of %s: ", subjectName[i]);
            safeNumInput(&score[i]);
        }
        pushBackRec(toAppend, recConstructor(stuNum, score, name));
        printf("Record added: number = %d & name = %s\n", stuNum, name);
    }
    //system("cls");//clear the output
    colorPrinter(FOREGROUND_GREEN, "Input Completed.\n");
    printTable(toAppend);
}

/*
 * Print an given list like this:
 *      +---------------+---------------+-------------+---------------+---------------+---------------+
 *      |           Name|    Student No.|      Chinese|            ...|            Sum|           Avg.|
 *      +---------------+---------------+-------------+---------------+---------------+---------------+
 *      |XXXX           |           XXXX|           XX|            ...|             XX|             XX|
 *      +---------------+---------------+-------------+---------------+---------------+---------------+
 *      |XXXX           |           XXXX|           XX|            ...|             XX|             XX|
 *      +---------------+---------------+-------------+---------------+---------------+---------------+
 *      |XXXX           |           XXXX|           XX|            ...|             XX|             XX|
 *      +---------------+---------------+-------------+---------------+---------------+---------------+
 *      |XXXX           |           XXXX|           XX|            ...|             XX|             XX|
 *      +---------------+---------------+-------------+---------------+---------------+---------------+
 *      |XXXX           |           XXXX|           XX|            ...|             XX|             XX|
 *      +---------------+---------------+-------------+---------------+---------------+---------------+
 * Parameter:
 *      toPrint:   Given list.
 */
void printTable(RecList* toPrint)
{
    printf("+---------------+---------------+");
    for(int i =0; i<subjectCount+2; i++)
    {
        printf("----------+");
    }
    printf("\n");
    printf("| %14s|%15s|", "Name","Student No.");
    for(int i =0; i<subjectCount; i++)
    {
        printf("%10s|", subjectName[i]);
    }
    printf("%10s|%10s|", "Sum","Avg.");
    printf("\n");
    printf("+---------------+---------------+");
    for(int i =0; i<subjectCount+2; i++)
    {
        printf("----------+");
    }
    printf("\n");
    for(int i =0; i<toPrint->listSize; i++)
    {
        printf("| %-14s|%15d|",toPrint->data[i]->name, toPrint->data[i]->number);//column
        for(int j=0; j<subjectCount; j++)
        {
            printf("%10d|",toPrint->data[i]->score[j]);
        }
        int total = getTotalScore(toPrint->data[i]->score);
        printf("%10d|%10.2f|",total,total/(double)subjectCount);
        printf("\n");
        printf("+---------------+---------------+");
        for(int j =0; j<subjectCount+2; j++)
        {
            printf("----------+");
        }
        printf("\n");
    }
    simpleStatistic(toPrint);
}

/*
 * A shorter version of printTable(), only print the num,sum & avg.
 * Parameter:
 *      toPrint:     the table to print
 */
void printRecStat(RecList* toPrint)
{
    printf("+---------------+---------------+----------+----------+\n"
           "| %14s|%15s|%10s|%10s|\n"
           "+---------------+---------------+----------+----------+\n"
           , "Name","Student No.", "Sum","Avg.");
    for(int i =0; i<toPrint->listSize; i++)
    {
        printf("| %-14s|%15d|",toPrint->data[i]->name, toPrint->data[i]->number);//column

        int total = getTotalScore(toPrint->data[i]->score);
        printf("%10d|%10.2f|",total,total/(double)subjectCount);
        printf("\n");
        printf("+---------------+---------------+----------+----------+\n");
    }
}

/*
 * Sort an given list by student number(ascending).
 * Parameter:
 *      toSort:   Given list.
 */
void numSort(RecList* toSort)
{
    qSort(toSort, numSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 * Sort an given list by student score(descending).
 * Parameter:
 *      toSort:   Given list.
 */
void scoreDescendingSort(RecList* toSort)
{
    qSort(toSort, scoreDesendSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 * Sort an given list by student score(ascending).
 * Parameter:
 *      toSort:   Given list.
 */
void scoreAscendSort(RecList* toSort)
{
    qSort(toSort, scoreIncreaseSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 * Sort an given list by student name.
 * Parameter:
 *      toSort:   Given list.
 */
void nameSort(RecList* toSort)
{
    qSort(toSort, nameIncreaseSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 * Calculate the sum of a student.
 * Parameter:
 *      scoreArray: student's scores.
 */
int getTotalScore(int* scoreArray)
{
    int sum = 0;
    for(int i =0; i<subjectCount; i++)
    {
        sum+=scoreArray[i];
    }
    return sum;
}

/*
 * Different sorting rules.
 */
int numSorter(Record* a, Record* b){return a->number < b->number;}
int scoreDesendSorter(Record* a, Record* b){return getTotalScore(a->score)>getTotalScore(b->score);}
int scoreIncreaseSorter(Record* a, Record* b){return getTotalScore(a->score)<getTotalScore(b->score);}
int nameIncreaseSorter(Record* a, Record* b){return strcmp(a->name, b->name)<0;}

/*
 * Sort a part of the given list via quickSort by given sorting rule.
 * Parameter:
 *      toSort:     The list we want to sort.
 *      sorter:     Sorting rule.
 *      start:      The area you want to sort starts here.
 *      stop:       The area you want to sort stops here.
 */
void qSort(RecList* toSort, int (*sorter)(Record* a, Record* b), int start, int stop)
{
    if(start==stop)
    {
        return;
    }
    int div = start;
    for(int i=start; i<stop-1; i++)
    {
        if((*sorter)(toSort->data[i], toSort->data[stop-1]))
        {
            Record* temp = toSort->data[i];
            toSort->data[i] = toSort->data[div];
            toSort->data[div] = temp;
            div++;
        }
    }
    Record* temp = toSort->data[div];
    toSort->data[div] = toSort->data[stop-1];
    toSort->data[stop-1] = temp;
    qSort(toSort, sorter, start, div);
    qSort(toSort, sorter, div+1, stop);
}

/*
 * Print the sum and average of scores in given table.
 * Parameter:
 *      toCalc:     given list
 */
void simpleStatistic(RecList* toCalc)
{
    int sum[subjectCount];
    for(int i=0; i<subjectCount; i++)
    {
        sum[i]=0;
    }
    for(int i = 0; i<toCalc->listSize; i++)
    {
        for(int j=0; j<subjectCount; j++)
        {
            sum[j]+=toCalc->data[i]->score[j];
            //printf("%d\n", toCalc->data[i]->score[j]);
        }
    }
    for(int i=0; i<subjectCount; i++)
    {
        printf("%15s: avg:%.2f, sum:%d\n", subjectName[i], sum[i]/(double)toCalc->listSize, sum[i]);
    }
}

/*
 * Search the given list of records for score of given index
 * Parameter:
 *      table:      given list
 */
void searchByNum(RecList* table)
{
    int criteria=0,flag=1;
    while(1)
    {
        flag = 1;
        printf("Input student number or q to quit:\n");
        if(scanf("%d", &criteria)!=1)
        {
            printf("Quiting to main menu.\n");
            fflush(stdin);
            break;
        }
        for(int i=0; i<table->listSize; i++)
        {
            if(table->data[i]->number == criteria)
            {
                colorPrinter(FOREGROUND_GREEN, "Record Found: ");
                printf("%s, N0.%d, score:\n",table->data[i]->name, table->data[i]->number);
                for(int j=0; j<subjectCount; j++)
                {
                    printf("\t%15s: %d\n", subjectName[j], table->data[i]->score[j]);
                }
                flag = 0;
            }
        }
        if(flag)
        {
            colorPrinter(FOREGROUND_RED,"Record not found!\n");
        }
    }
}

/*
 * Search record by name.
 * Parameter:
 *      table:  the table to search
 */
void searchByName(RecList* table)
{
    int flag=1;
    fflush(stdin);
    while(1)
    {
        char* input = (char*)malloc(MAX_NAME_LENGTH*sizeof(char));
        flag = 1;
        printf("Input student's name or q to quit:\n");
        gets(input);
        if(strcmp(input,"q")==0)
        {
            printf("Quiting to main menu.\n");
            break;
        }
        for(int i=0; i<table->listSize; i++)
        {
            if(strcmp(table->data[i]->name, input)==0)
            {
                colorPrinter(FOREGROUND_GREEN, "Record Found: ");
                printf("%s, N0.%d, score:\n",table->data[i]->name, table->data[i]->number);
                for(int j=0; j<subjectCount; j++)
                {
                    printf("\t%15s: %d\n", subjectName[j], table->data[i]->score[j]);
                }
                flag = 0;
            }
        }
        if(flag)
        {
            colorPrinter(FOREGROUND_RED,"Record not found!\n");
        }
    }
}

/*
 * Print statistic analysis of the list
 * Parameter:
 *      table:  the table to analyze
 */
void statisticAnalysis(RecList* table)
{
    int **count = (int**)malloc(subjectCount*sizeof(int*));
    for(int i=0; i<subjectCount; i++)
    {
        count[i] = (int*)malloc(sizeof(int)*5);
        for(int j=0; j<5; j++)
        {
            count[i][j]=0;
        }
    }
    for(int i =0; i<table->listSize; i++)
    {
        for(int j=0; j<subjectCount; j++)
        {
            int curScore = table->data[i]->score[j];
            count[j][curScore<=50? 0 : (curScore==100? 4 : (curScore-50)/10)]++;
        }
    }
    printf("Analysis:\n");
    for(int i=0; i<subjectCount; i++)
    {
        printf("%s\n", subjectName[i]);
        for(int j =4; j>=0; j--)
        {
            printf("\t%c: %3d students,\t%6.2f%%\n", 'E'-j, count[i][j], count[i][j]/(double)table->listSize*100);
        }
    }

    for(int i=0;i<subjectCount;i++)
    {
        free(count[i]);
    }
    free(count);
}

/*
 * Save current list to a file.
 * Parameter:
 *      toSave: current list.
 */
void writeToFile(RecList* toSave)
{
    char* name = malloc(MAX_ADDR*sizeof(char));
    printf("Please input file name: ");
    fflush(stdin);
    gets(name);
    FILE* file = fopen(name, "w");
    nullCheck(file,"writeToFile");
    fprintf(file, "%d\n", subjectCount);
    for(int i=0; i<subjectCount; i++)
    {
        fprintf(file, "%s\n",subjectName[i]);
    }
    fprintf(file,"%d\n",toSave->listSize);
    for(int i=0; i<toSave->listSize; i++)
    {
        fprintf(file, "%s\n%d\n", toSave->data[i]->name, toSave->data[i]->number);
        for(int j=0; j<subjectCount; j++)
        {
            fprintf(file,"\t%d", toSave->data[i]->score[j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    free(name);

    colorPrinter(FOREGROUND_GREEN, "Completed\n");
}

/*
 * Read from a file for list.
 * Parameter:
 *      toStore:        the list to store the result
 */
void readFromFile(RecList* toStore)
{
    char* fName = malloc(MAX_ADDR*sizeof(char));
    int isAppend = subjectCount!=-1;
    int fileSubCount;
    char** fileSubName;
    FILE* file;

    system("dir /A:-D /B");
    printf("\n\nPlease input file name: ");

    fflush(stdin);
    gets(fName);
    file = fopen(fName, "r");

    while(file == NULL)
    {
        colorPrinter(FOREGROUND_RED, "Unable to open the file. Please try again.\n");
        system("dir /A:-D /B");
        printf("\nPlease input file name: ");
        fflush(stdin);
        gets(fName);
        file = fopen(fName, "r");
    }

    if(fscanf(file, "%d", &fileSubCount)!=1 || (isAppend && fileSubCount!=subjectCount)) goto ERR;
    subjectCount = fileSubCount;

    fileSubName = (char**)malloc(sizeof(char*)*subjectCount);
    for(int i =0; i<subjectCount; i++)
    {
        fileSubName[i] = malloc(MAX_NAME_LENGTH*sizeof(char));
        if(fscanf(file, "%s", fileSubName[i])!=1 || (isAppend && strcmp(fileSubName[i],subjectName[i])!=0)) goto ERR;
    }

    if(!isAppend)
    {
        subjectName = fileSubName;
    }

    int size;

    if(fscanf(file, "%d", &size)!=1) goto ERR;

    for(int i =0; i<size; i++)
    {
        int num, *scores;
        char* name = malloc(sizeof(char)*MAX_NAME_LENGTH);
        scores = malloc(sizeof(int)*subjectCount);
        fgetc(file);
        fgets(name,MAX_NAME_LENGTH-1, file);
        name[strlen(name)-1]='\0';

        if(fscanf(file, "%d", &num)!=1) goto ERR;

        for(int j=0; j<subjectCount; j++)
        {
            if(fscanf(file, "%d", &scores[j])!=1) goto ERR;
        }
        Record* cur = recConstructor(num, scores, name);
        pushBackRec(toStore, cur);
    }
    system("cls");
    printf("Number: 180110618\nSubject: 5 - Program 1\n\n");
    printTable(toStore);

    fclose(file);
    free(fName);
    return;

    ERR:
        colorPrinter(FOREGROUND_RED, "Format err!\n");
        readFromFile(toStore);
        return;
}

/*
 * Eng ver of system("pause");
 */
void pause()
{
    fflush(stdin);
    colorPrinter(FOREGROUND_GREEN,"Press any key to continue.");
    if(getch()==224)
        getch();
}
