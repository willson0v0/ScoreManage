#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INIT_CAPACITY 16
#define MAX_NAME_LENGTH 30
#define FUNC_COUNT 13
#define MAX_ADDR 100

typedef struct
{
    int* score;
    int number;
    char* name;
} Record;

typedef struct
{
    Record** data;
    int capacity;
    int listSize;
} RecList;

Record* recConstructor(int number,int* score, char* name);
RecList* listConstructor();

int numSorter(Record* a, Record* b);
int scoreDesendSorter(Record* a, Record* b);
int scoreIncreaseSorter(Record* a, Record* b);
int nameIncreaseSorter(Record* a, Record* b);
int getTotalScore(int* scoreArray);

void saveNumInput(int* in);
void nullCheck(void* ptr, char* errMsg);
void pushBackRec(RecList* toAppend, Record* toAdd);
void qSort(RecList* toSort, int (*sorter)(Record* a, Record* b), int start, int stop);
void listDeconstructor(RecList* toDestroy);
void colorPrinter(WORD color, char* content);
void pause();

void getInput(RecList* toAppend);
void printTable(RecList* toPrint);
void printRecStat(RecList* toPrint);
void numSort(RecList* toSort);
void scoreDescendingSort(RecList* toSort);
void scoreAscendSort(RecList* toSort);
void simpleStatistic(RecList* toCalc);
void statisticAnalysis(RecList* table);
void nameAscendSort(RecList* toSort);
void searchByNum(RecList* toSearch);
void searchByName(RecList* toSearch);
void writeToFile(RecList* toSave);
void readFromFile(RecList* toStore);

#endif // HEADER_H_INCLUDED
