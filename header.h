#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#define INIT_CAPACITY 16

struct record
{
    int score;
    int number;
    char* name;
};

struct recList
{
    struct record** data;
    int capacity;
    int listSize;
};

struct record* recConstructor(int score, int number);
struct recList* listConstructor();

int numSorter(struct record* a, struct record* b);
int scoreDesendSorter(struct record* a, struct record* b);
int scoreIncreaseSorter(struct record* a, struct record* b);

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

#endif // HEADER_H_INCLUDED
