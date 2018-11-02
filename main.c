#include <stdio.h>
#include <stdlib.h>
#include <header.h>

/*
 * Create an instance of record by given stu No. and score
 * Parameter:
 *      number:     given student No.
 *      score:      given score
 * return:
 *      Pointer of the new instance.
 */
struct record* stuConstructor(int number,int score)
{
    struct record* stu = (struct record*)malloc(sizeof(struct record));
    nullCheck(stu, "stuConstructor");//check if malloc failed.
    stu->score = score;
    stu->number = number;

    return stu;
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
    list->size = 0;
    list->data = (struct record**)malloc(sizeof(struct record*)*list->capacity);
    nullCheck(list->data,"listConstructor");//check if malloc failed.

    return list;
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
    toAppend->data[toAppend->size++] = toAdd;
    if(toAppend->size==toAppend->capacity)//if is about to overflow
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
    int stuNum, score;
    while(1)
    {
        while(scanf("%d", &stuNum)!=1)
        {
            printf("Invalid input. Please try again.\n");
            fflush(stdin);
        }
        if(!stuNum)
        {
            break;//end input
        }
        while(scanf("%d", &score)!=1)
        {
            printf("Invalid input. Please try again.\n");
            fflush(stdin);
        }
        pushBackRec(toAppend, stuConstructor(stuNum, score));
        printf("Record added: number = %d & score = %d\n", stuNum, score);
    }
    system("cls");//clear the output
    printf("Input Competed.\n");
    printTable(toAppend);
}

/*
 * Print an given list like this:
 *      +---------------+-----+
 *      |    Student No.|Score|
 *      +---------------+-----+
 *      |           XXXX|   XX|
 *      +---------------+-----+
 *      |           XXXX|   XX|
 *      +---------------+-----+
 *      |           XXXX|   XX|
 *      +---------------+-----+
 * Parameter:
 *      toPrint:   Given list.
 */
void printTable(struct recList* toPrint)
{
    printf("%s\n|%15s|%5s|\n%s\n", "+---------------+-----+", "Student No.", "Score", "+---------------+-----+");//Title
    for(int i =0; i<toPrint->size; i++)
    {
        printf("|%15d|%5d|\n+---------------+-----+\n", toPrint->data[i]->number, toPrint->data[i]->score);//column
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
    qSort(toSort, numSorter, 0, toSort->size);
    printTable(toSort);
}

/*
 * Sort an given list by student score(descending).
 * Parameter:
 *      toSort:   Given list.
 */
void scoreDescendingSort(struct recList* toSort)
{
    qSort(toSort, scoreDesendSorter, 0, toSort->size);
    printTable(toSort);
}

/*
 * Sort an given list by student score(ascending).
 * Parameter:
 *      toSort:   Given list.
 */
void scoreAscendSort(struct recList* toSort)
{
    qSort(toSort, scoreIncreaseSorter, 0, toSort->size);
    printTable(toSort);
}

/*
 * Different sorting rules.
 */
int numSorter          (struct record* a, struct record* b){return a->number < b->number;}
int scoreDesendSorter  (struct record* a, struct record* b){return  a->score > b->score ;}
int scoreIncreaseSorter(struct record* a, struct record* b){return  a->score < b->score ;}

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
    int sum = 0;
    for(int i = 0; i<toCalc->size; i++)
    {
        sum+=toCalc->data[i]->score;
    }
    printf("sum: %d, avg: %.2f\n",sum, sum/(double)toCalc->size);
}

/*
 * Search the given list of records for score of given index
 * Parameter:
 *      table:      given list
 */
void search(struct recList* table)
{
    int criteria=0,flag=1;
    while(1)
    {
        flag = 1;
        printf("Input student number or q to quit:\n");
        if(scanf("%d", &criteria)!=1)
        {
            printf("Quiting to main menu.\n");
            break;
        }
        for(int i=0; i<table->size; i++)
        {
            if(table->data[i]->number == criteria)
            {
                printf("Record Found: number = %d, score = %d\n", table->data[i]->number, table->data[i]->score);
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
    int count[5]= {0};
    for(int i =0; i<table->size; i++)
    {
        int curScore = table->data[i]->score;
        count[curScore<=50? 0 : (curScore-50)/10]++;
    }
    printf("Analysis:\n");
    for(int i =4; i>=0; i--)
    {
        printf("%c: %d students,\t%.2f%%\n", 'E'-i, count[i], count[i]/(double)table->size*100);
    }
}

void listDeconstructor(struct recList* toDestroy)
{
    for(int i =0; i<toDestroy->size; i++)
    {
        free(toDestroy->data[i]);
    }
    free(toDestroy->data);
    free(toDestroy);
}

int main()
{
    printf("Number: 180110618\nSubject: 5 - Program 1\n\n");
    void (*func[8])(struct recList* a) =
    {
        getInput,
        simpleStatistic,
        scoreDescendingSort,
        scoreAscendSort,
        numSort,
        search,
        statisticAnalysis,
        printTable
    };
    struct recList* myList = listConstructor();
    int flag =0;
    int input;
    while(1)
    {
        printf("\n"
               "1. Input record\n"
               "2. Calculate total and average score of course\n"
               "3. Sort in descending order by score\n"
               "4. Sort in ascending order by score\n"
               "5. Sort in ascending order by number\n"
               "6. Search by number\n"
               "7. Statistic analysis\n"
               "8. List record\n"
               "Q. quit.\n");
        if(scanf("%d", &input)!=1||input>8)
        {
            break;
        }
        input--;
        if(input==0)
        {
            flag = 1;
        }
        else if(flag==0)
        {
            printf("please init the table first!\n");
            continue;
        }
        (*func[input])(myList);
    }

    listDeconstructor(myList);
    return 0;
}
