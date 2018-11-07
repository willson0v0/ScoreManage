#include <header.h>
#include <functions.c>

extern int subjectCount;
extern char** subjectName;
extern HANDLE hConsole;
extern CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
extern WORD saved_attributes;
extern void (*func[])(RecList* a);

char* funcList[] = {"1  Input record",
                    "2  Calculate total and average score of courses",
                    "3  Calculate total and average score of students",
                    "4  Sort in descending order by score",
                    "5  Sort in ascending order by score",
                    "6  Sort in ascending order by number",
                    "7  Sort in ascending order by name",
                    "8  Search by number",
                    "9  Search by name",
                    "10 Statistic analysis",
                    "11 List record",
                    "12 Write to File",
                    "Q. quit."
                   };

int main()
{
    printf("Number: 180110618\nSubject: 5 - Program 1\n\n");
    int input, screenShotMode, curList = 0,initFlag=0;
    char ssm,rf;
    RecList* myList = listConstructor();

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;

    printf("Screen shot mode?(never clear output) [y/N]");
    scanf("%c", &ssm);
    screenShotMode = (ssm=='y'||ssm=='Y');

    printf("Do you wish to read from file?[y/N]");
    fflush(stdin);
    scanf("%c", &rf);
    if(rf!='y'&&rf!='Y')
    {
        initFlag=1;
        printf("Please input subject count: ");
        safeNumInput(&subjectCount);

        getchar();
        printf("Please input subject names\n");
        subjectName = (char**)malloc(sizeof(char*)*subjectCount);
        for(int i=0; i<subjectCount; i++)
        {
            printf("%d: ",i+1);
            subjectName[i] = (char*)malloc(sizeof(char)*MAX_NAME_LENGTH);
            gets(subjectName[i]);
        }
    }
    else
    {
        readFromFile(myList);
        initFlag = 1;
    }
    while(1)
    {
        if(!screenShotMode)
        {
            pause();
            system("cls");
        }
        while(1)
        {
            system("cls");
            printf("Use arrow key to select from menu:\n");
            for(int i=0; i<=FUNC_COUNT; i++)
            {
                printf("%s", funcList[i]);
                if(i==curList)
                {
                    colorPrinter(FOREGROUND_GREEN, "\r>>");
                }
                printf("\n");
            }
            fflush(stdin);
            input = getch();
            if(input==224)
            {
                input = getch();
                if(input==80||input==77)
                {
                    curList+=(curList==FUNC_COUNT?-curList:1);
                }
                if(input==72||input==75)
                {
                    curList-=(curList==0?-FUNC_COUNT:1);
                }
            }
            else if(input==13&&curList!=FUNC_COUNT)
            {
                input = curList;
                break;
            }
            else if(input>='0'&&input<='9')
            {
                input -= '1';
                break;
            }
            else
            {
                colorPrinter(FOREGROUND_GREEN, "BYE!");
                system("pause");
                return 0;
            }
        }
        if(input==0)
        {
            initFlag = 1;
        }
        else if(initFlag==0)
        {
            printf("please init the table first!\n");
            continue;
        }
        if(!screenShotMode)
        {
            system("cls");
        }
        (*func[input])(myList);
    }
    listDeconstructor(myList);
    return 0;
}
