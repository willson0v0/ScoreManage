#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INIT_CAPACITY 16
#define MAX_NAME_LENGTH 30
struct record
{
    int* score;
    int number;
    char* name;
};

struct recList
{
    struct record** data;
    int capacity;
    int listSize;
};

struct record* recConstructor(int number,int* score, char* name);
struct recList* listConstructor();

int numSorter(struct record* a, struct record* b);
int scoreDesendSorter(struct record* a, struct record* b);
int scoreIncreaseSorter(struct record* a, struct record* b);
int nameIncreaseSorter(struct record* a, struct record* b);

void nullCheck(void* ptr, char* errMsg);
void pushBackRec(struct recList* toAppend, struct record* toAdd);
void getInput(struct recList* toAppend);
void printTable(struct recList* toPrint);
void numSort(struct recList* toSort);
void scoreDescendingSort(struct recList* toSort);
void scoreAscendSort(struct recList* toSort);
void qSort(struct recList* toSort, int (*sorter)(struct record* a, struct record* b), int start, int stop);
void simpleStatistic(struct recList* toCalc);
void statisticAnalysis(struct recList* table);
void listDeconstructor(struct recList* toDestroy);
void nameAscendSort(struct recList* toSort);
void searchByNum(struct recList* toSearch);
void searchByName(struct recList* toSearch);


int subjectCount;
char** subjectName;


/*
 * Create an instance of record by given stu No. and score
 * Parameter:
 *      number:     given student No.
 *      score:      given score
 * return:
 *      Pointer of the new instance.
 */
struct record* recConstructor( int number,int* score, char* name)
{
    struct record* rec = (struct record*)malloc(sizeof(struct record));
    nullCheck(rec, "stuConstructor");//check if malloc failed.
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
struct recList* listConstructor()
{
    struct recList* list = (struct recList*)malloc(sizeof(struct recList));
    nullCheck(list, "listConstructor");//check if malloc failed.
    list->capacity = INIT_CAPACITY;//initial capacity of the array
    list->listSize = 0;
    list->data = (struct record**)malloc(sizeof(struct record*)*list->capacity);
    nullCheck(list->data,"listConstructor");//check if malloc failed.

    return list;
}

/*
 *
 */
void recDeconstructor(struct record* toDestroy)
{
    free(toDestroy->name);
    free(toDestroy);
}

/*
 *
 */
void listDeconstructor(struct recList* toDestroy)
{
    for(int i =0; i<toDestroy->listSize; i++)
    {
        recDeconstructor(toDestroy->data[i]);
    }
    free(toDestroy->data);
    free(toDestroy);
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
        printf("NULL PTR @ %s\n", errMsg);
        system("pause");
        exit(1);
    }
}

/*
 * Add an instance of record to an existing list
 * Parameter:
 *      toAppend:   Given list.
 *      toAdd:      The record to add.
 */
void pushBackRec(struct recList* toAppend, struct record* toAdd)
{
    toAppend->data[toAppend->listSize++] = toAdd;
    if(toAppend->listSize==toAppend->capacity)//if is about to overflow
    {
        toAppend->capacity*=2;//double the capacity to reduce the realloc call count
        struct record** ptr = (struct record**)realloc(toAppend->data, sizeof(struct record*) * toAppend->capacity);
        nullCheck(ptr,"pushBackStu");
        toAppend->data = ptr;
    }
}

/*
 * Add records to an existing list via keyboard input
 * Parameter:
 *      toAppend:   Given list.
 */
void getInput(struct recList* toAppend)
{
    printf("please input Student No. and score. Input 0 to End.\n");
    int stuNum;
    while(1)
    {
        int* score = (int*)malloc(sizeof(int)*subjectCount);
        char* name = (char*)malloc(sizeof(char)*MAX_NAME_LENGTH);
        printf("Please input Student number: ");
        while(scanf("%d", &stuNum)!=1)
        {
            printf("Invalid input. Please try again.\n");
            fflush(stdin);
        }

        if(!stuNum)
        {
            break;//end input
        }

        getchar();
        printf("Please input Student's name: ");
        gets(name);

        for(int i =0;i<subjectCount;i++)
        {
            printf("Please input Student's score of %s: ", subjectName[i]);
            while(scanf("%d", &score[i])!=1)
            {
                printf("Invalid input. Please try again.\n");
                fflush(stdin);
            }
            printf("%d", score[i]);
        }
        pushBackRec(toAppend, recConstructor(stuNum, score, name));
        printf("Record added: number = %d & name = %s\n", stuNum, name);
    }
    //system("cls");//clear the output
    printf("Input Competed.\n");
    printTable(toAppend);
}

/*
 * Print an given list like this:
 *      +---------------+---------------+-----+
 *      |           Name|    Student No.|Score|
 *      +---------------+---------------+-----+
 *      |XXXX           |           XXXX|   XX|
 *      +---------------+---------------+-----+
 *      |XXXX           |           XXXX|   XX|
 *      +---------------+---------------+-----+
 *      |XXXX           |           XXXX|   XX|
 *      +---------------+---------------+-----+
 *      |XXXX           |           XXXX|   XX|
 *      +---------------+---------------+-----+
 * Parameter:
 *      toPrint:   Given list.
 */
void printTable(struct recList* toPrint)
{
    printf("+---------------+---------------+");
    for(int i =0;i<subjectCount;i++)
    {
        printf("----------+");
    }
    printf("\n");
    printf("| %14s|%15s|", "Name","Student No.");
    for(int i =0;i<subjectCount;i++)
    {
        printf("%10s|", subjectName[i]);
    }
    printf("\n");
    printf("+---------------+---------------+");
    for(int i =0;i<subjectCount;i++)
    {
        printf("----------+");
    }
    printf("\n");
    for(int i =0; i<toPrint->listSize; i++)
    {
        //printf("| %-14s|%15d|%5d|\n+---------------+---------------+-----+\n",toPrint->data[i]->name, toPrint->data[i]->number, toPrint->data[i]->score);//column
        printf("| %-14s|%15d|",toPrint->data[i]->name, toPrint->data[i]->number);//column
        for(int j=0;j<subjectCount;j++)
        {
            printf("%10d|",toPrint->data[i]->score[j]);
        }
        printf("\n");
        printf("+---------------+---------------+");
        for(int j =0;j<subjectCount;j++)
        {
            printf("----------+");
        }
        printf("\n");
    }
    simpleStatistic(toPrint);
}

/*
 * Sort an given list by student number(ascending).
 * Parameter:
 *      toSort:   Given list.
 */
void numSort(struct recList* toSort)
{
    qSort(toSort, numSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 * Sort an given list by student score(descending).
 * Parameter:
 *      toSort:   Given list.
 */
void scoreDescendingSort(struct recList* toSort)
{
    qSort(toSort, scoreDesendSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 * Sort an given list by student score(ascending).
 * Parameter:
 *      toSort:   Given list.
 */
void scoreAscendSort(struct recList* toSort)
{
    qSort(toSort, scoreIncreaseSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 *
 */
void nameAscendSort(struct recList* toSort)
{
    qSort(toSort, nameIncreaseSorter, 0, toSort->listSize);
    printTable(toSort);
}

/*
 * Different sorting rules.
 */
int numSorter          (struct record* a, struct record* b){return a->number < b->number;}
int scoreDesendSorter  (struct record* a, struct record* b){return  a->score > b->score ;}
int scoreIncreaseSorter(struct record* a, struct record* b){return  a->score < b->score ;}
int nameIncreaseSorter(struct record* a, struct record* b){return strcmp(a->name, b->name)<0;}

/*
 * Sort a part of the given list via quickSort by given sorting rule.
 * Parameter:
 *      toSort:     The list we want to sort.
 *      sorter:     Sorting rule.
 *      start:      The area you want to sort starts here.
 *      stop:       The area you want to sort stops here.
 */
void qSort(struct recList* toSort, int (*sorter)(struct record* a, struct record* b), int start, int stop)
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
            struct record* temp = toSort->data[i];
            toSort->data[i] = toSort->data[div];
            toSort->data[div] = temp;
            div++;
        }
    }
    struct record* temp = toSort->data[div];
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
void simpleStatistic(struct recList* toCalc)
{
    int sum[subjectCount];
    for(int i=0;i<subjectCount;i++)
    {
        sum[i]=0;
    }
    for(int i = 0; i<toCalc->listSize; i++)
    {
        for(int j=0;j<subjectCount;j++)
        {
            sum[j]+=toCalc->data[i]->score[j];
            //printf("%d\n", toCalc->data[i]->score[j]);
        }
    }
    for(int i=0;i<subjectCount;i++)
    {
        printf("%s: avg:%.2f, sum:%d\n", subjectName[i], sum[i]/(double)toCalc->listSize, sum[i]);
    }
}

/*
 * Search the given list of records for score of given index
 * Parameter:
 *      table:      given list
 */
void searchByNum(struct recList* table)
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
                printf("Record Found: %s, N0.%d, score:\n",table->data[i]->name, table->data[i]->number);
                for(int j=0;j<subjectCount;j++)
                {
                    printf("%s: %d\n", subjectName[i], table->data[i]->score[i]);
                }
                flag = 0;
            }
        }
        if(flag)
        {
            printf("Record not found!\n");
        }
    }
}

void searchByName(struct recList* table)
{
    int flag=1;
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
                printf("Record Found: %s, N0.%d, score:\n",table->data[i]->name, table->data[i]->number);
                for(int j=0;j<subjectCount;j++)
                {
                    printf("%s: %d\n", subjectName[i], table->data[i]->score[i]);
                }
                flag = 0;
            }
        }
        if(flag)
        {
            printf("Record not found!\n");
        }
    }
}

/*
 * Print statistic analysis of the list
 */
void statisticAnalysis(struct recList* table)
{
    int **count = (int**)malloc(5*sizeof(int*));
    for(int i=0;i<5;i++)
    {
        count[i] = (int*)malloc(sizeof(int)*subjectCount);
        for(int j=0;j<subjectCount;j++)
        {
            count[i][j]=0;
        }
    }
    for(int i =0; i<table->listSize; i++)
    {
        for(int j=0;j<subjectCount;j++)
        {
            int curScore = table->data[i]->score[j];
            count[curScore<=50? 0 : (curScore-50)/10][j]++;
        }
    }
    printf("Analysis:\n");
    for(int i=0;i<subjectCount;i++)
    {
        printf("%s", subjectName[i]);
        for(int j =4; j>=0; j--)
        {
            printf("\t%c: %d students,\t%.2f%%\n", 'E'-j, count[j][i], count[j][i]/(double)table->listSize*100);
        }
    }
}

int main()
{
    printf("Number: 180110618\nSubject: 5 - Program 1\n\n");
    void (*func[])(struct recList* a) =
    {
        getInput,
        simpleStatistic,
        scoreDescendingSort,
        scoreAscendSort,
        numSort,
        nameAscendSort,
        searchByNum,
        searchByName,
        statisticAnalysis,
        printTable
    };
    int inputFlag =0;
    int input;
    printf("Please input subject count");
    struct recList* myList = listConstructor();
    while(scanf("%d",&subjectCount)!=1)
    {
        printf("Invalid input.\n");
        fflush(stdin);
    }
    getchar();
    printf("Please input subject names\n");
    subjectName = (char**)malloc(sizeof(char*)*subjectCount);
    for(int i=0;i<subjectCount;i++)
    {
        printf("%d: ",i);
        subjectName[i] = (char*)malloc(sizeof(char)*MAX_NAME_LENGTH);
        gets(subjectName[i]);
    }
    while(1)
    {
        printf("\n"
               "1. Input record\n"
               "2. Calculate total and average score of course\n"
               "3. Sort in descending order by score\n"
               "4. Sort in ascending order by score\n"
               "5. Sort in ascending order by number\n"
               "6. Sort in ascending order by name\n"
               "7. Search by number\n"
               "8. Search by name\n"
               "9. Statistic analysis\n"
               "10. List record\n"
               "Q. quit.\n");
        if(scanf("%d", &input)!=1||input>10)
        {
            break;
        }
        input--;
        if(input==0)
        {
            inputFlag = 1;
        }
        else if(inputFlag==0)
        {
            printf("please init the table first!\n");
            continue;
        }
        (*func[input])(myList);
    }

    listDeconstructor(myList);
    return 0;
}
