/*
控制台可运行版本，
2020.7.25
*/

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

#define UP  'w'
#define DOMN  's'
#define LEFT  'a'
#define RIGHT  'd'

typedef struct Snake
{
    int x;
    int y;
    struct Snake* next;
} snake;

struct Food
{
    int x;
    int y;
} food;

snake* head;
int flag = 0;
char key = 1;
char prekey = 1;
char name[20];
int score;
int speed;
int level;

void welcome();               // 开始界面
void finish();                // 结束界面
void createGraph();            // 围墙打印
void goToxy(int x, int y);    // 光标跳转，横为X 0,1,2..
void goToPrint(int x, int y); // 跳转打印
void gotoDelete(int x, int y);// 跳转删除
void createFood();             // 食物产生
int clickControl();           // 获取键盘信号
int judge();                  // 游戏结束判断
void movingBody();            // 蛇的移动
void eating();                // 蛇吃到东西后的操作（伸长）
void changeBody(int x, int y);
int isOpposite();
void speedJudge();

int main()
{
    welcome();
    createGraph();
    createFood();
    clickControl();
    finish();
    return 0;
}

void welcome()
{
    goToxy(5, 5);
    printf("/***************************************************************/");
    goToxy(27, 7);
    //printf("Welcome to this snake game!\n");
    printf("欢迎来到贪吃蛇游戏！\n");
    goToxy(19, 9);
    printf("使用英文小写输入，wsad控制上下左右");
    goToxy(30, 11);
    printf("好，现在开始");
    goToxy(5, 17);
    printf("/**************************************************************/");
    goToxy(17, 13);
    printf("请输入你的名字:");
    scanf_s("%s", name,sizeof(name));
    goToxy(17, 15);
    printf("请输入你想挑战的难度等级（1~5）：");
    scanf_s("%d", &level);
    while (level < 1 || level>5)
    {
        goToxy(49, 15);
        printf("输入错误，请重新输入");
        Sleep(2000);
        goToxy(49, 15);
        printf("                     ");
        goToxy(49, 15);
        scanf_s("%d", &level);
    }
    system("cls");
}

void goToxy(int x, int y)
{
    COORD pos;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOutput, pos);
    // 隐藏光标
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = FALSE;
    cursor.dwSize = sizeof(cursor);
    SetConsoleCursorInfo(hOutput, &cursor);
    SetConsoleTextAttribute(hOutput,
        FOREGROUND_GREEN |      // 前景色_蓝色
        FOREGROUND_INTENSITY);
}
void goToPrint(int x, int y)
{
    goToxy(x, y);
    printf("■");
}
void createGraph()
{
    for (int i = 0; i < 56; i += 2)
    {
        goToPrint(i, 0);
        goToPrint(i, 27);
    }
    for (int j = 0; j < 28; j++)
    {
        goToPrint(0, j);
        goToPrint(54, j);
    }
    goToxy(67, 10);
    printf("name:%s", name);
    goToxy(67, 14);
    printf("score:%d", score);

    head = (snake*)malloc(sizeof(snake));
    snake* p = (snake*)malloc(sizeof(snake));
    head->x = 26;
    head->y = 13;
    p->x = 26;
    p->y = 12;
    head->next = p;
    p->next = NULL;
}
void finish()
{
    system("cls");
    goToxy(5, 5);
    printf("/******************************************************/");
    goToxy(18, 7);
    printf("Sorry,the game is over!\n");
    goToxy(18, 9);
    printf("Your score is %d", score);
    goToxy(18, 11);
    printf("see you next time");
    goToxy(5, 13);
    printf("/******************************************************/");
    goToxy(18, 15);
    system("pause");
    snake* p = head, * q=NULL;
    while (p->next != NULL)
    {
        q = p->next;
        free(p);
        p = q;
    }
}

void gotoDelete(int x, int y)
{
    goToxy(x, y);
    printf("  ");
}

void createFood()
{
    while (!flag)
    {
        flag = 1;
        srand((int)time(NULL));
        food.x = (rand() % 26 + 1) * 2;
        food.y = rand() % 26 + 1;
        snake* temp = head;
        //判断食物是否与蛇的身体重复
        while (1)
        {
            if (temp->next == NULL)
                break;
            if (temp->x == food.x && temp->y == food.y)
            {
                flag = 0;
                break;
            }
            temp = temp->next;
        }
        if (flag == 1)
        {
            goToxy(food.x, food.y);
            printf("▲");
        }
        
    }
}

int clickControl()
{
    while (1)
    {
        if (judge() == 0) break;
        if (_kbhit())
        {
            key = _getch();
        }
        while (key == 'p')
        {
            key = _getch();
        }
        movingBody();
        eating();
    }
    return 0;
}
int judge()
{
    if (head->x >= 54 || head->y >= 27 || head->x <= 0 || head->y <= 0)
    {
        return 0;
    }
    if (isOpposite())
    {
        snake* temp = head->next;
        while (1)
        {
            if (temp == NULL)break;
            if (temp->x == head->x && temp->y == head->y)
            {
                return 0;
            }
            temp = temp->next;
        }
    }
    return 1;
}


void movingBody()            // 蛇的移动
{
    int x = head->x;
    int y = head->y;
    snake* temp = head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    gotoDelete(temp->x, temp->y);
    if (!isOpposite())
    {
        key = prekey;
        //reverseList();
    }
    switch (key)
    {
    case UP:
        y--;
        break;
    case DOMN:
        y++;
        break;
    case LEFT:
        x -= 2;
        break;
    case RIGHT:
        x += 2;
        break;
    default:
        break;
    }
    prekey = key;
    if (x != head->x || y != head->y)
    {
        changeBody(x, y);
    }
    snake* p = head;
    goToPrint(p->x, p->y);
    speedJudge();
    goToxy(67, 12);
    printf("level:%d", level);
    goToxy(77, 12);
    printf("          ");
    goToxy(77, 12);
    printf("speed:%d", 1000/speed);
    Sleep(speed);
}
void eating()              // 蛇吃到东西后的操作（伸长）
{
    if (head->x == food.x && head->y == food.y)
    {
        flag = 0;
        createFood();
        snake* newnode = (snake*)malloc(sizeof(snake));
        snake* temp = head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newnode;
        //这个尾节点马上就会被删除，所以不用管坐标是什么
        newnode->next = NULL;
        score += 10;
        goToxy(67, 14);
        printf("score:%d", score);
    }
}
void changeBody(int x, int y)
{
    snake* newhead = (snake*)malloc(sizeof(snake));
    snake* temp = head;
    //删除尾节点
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
    newhead->x = x;
    newhead->y = y;
    newhead->next = head;
    head = newhead;
}

int isOpposite() 
{
    if (prekey == UP && key == DOMN) return 0;
    if (prekey == DOMN && key == UP) return 0;
    if (prekey == LEFT && key == RIGHT) return 0;
    if (prekey == RIGHT && key == LEFT) return 0;
    return 1;
}

void speedJudge()
{
    speed = (350 - level * 50) - (score / 10) * 5;
    if (speed <= 40)
    {
        speed = 40;
        return;
    }      
}