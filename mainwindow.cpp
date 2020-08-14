#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <QDebug>
#include <QMessageBox>
#include "QTime"
#include <stack>
#include <queue>
#include <QDockWidget>
#include <chrono>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <QApplication>
#include <QDesktopWidget>



#define COUT QTextStream cout(stdout);
#define TRN cout << "turn = " << turn << endl;

using namespace std;


//vector<pair<pair<double, double>, avl_node *>> mc;

double R;
bool can_move = 0;
int STAGE = 1;
QImage img_your_turn, img_you_choose, img_rules, img_menu;
QImage img_background, img_background_1, img_background_2, img_background_3;
QImage img_title_1;
QImage img_title_2;
QImage img_title_3;
QImage img_title_4;
QImage img_title_5;
QImage img_title_6;
QImage img_title_7;
QImage img_title_9;
QImage img_title_10;
QImage img_title_11;
QImage img_title_12;

bool fullscreen = 1;

QImage img_quarto;

int difficulty = 0;

int turn;


double mouse_x;
double mouse_y;

// LOGIC BOOLS



struct figure
{
    int P1;
    int P2;
    int P3;
    int P4;
    figure()
    {

    }
    figure(int a, int b, int c, int d)
    {
        P1 = a;
        P2 = b;
        P3 = c;
        P4 = d;
    }

    int num()
    {
        return P1 * 8 + P2 * 4 + P3 * 2 + P4;
    }

    bool operator ==(figure &b)
    {
        return (this->P1 == b.P1) && (this->P2 == b.P2) && (this->P3 == b.P3) && (this->P4 == b.P4);
        //return(this->P1 == b.P1) && (this.P2 == b.P2) && (this.P3 == b.P3) && (this.P4 == b.P4)
    }
};

struct square
{
    square()
    {
        flush();

    }
    square(figure f)
    {
        haveFig = 1;
        Figure = f;
    }
    void flush(){
        haveFig = 0;
        Figure.P1 = -1;
        Figure.P2 = -1;
        Figure.P3 = -1;
        Figure.P4 = -1;
    }

    bool haveFig;
    figure Figure;

};

struct Move
{
    bool finish = 0;
    int X; int Y;
    figure theNextFig;
};

square board[4][4];

vector <figure> been;  // использованные фигуры
figure curFigure;
int countMoves = 0;
vector<int> win_figures;
std::vector<pair<figure, int>> left_figures; // figure и enabled


Move easyComputerMove();
Move mediumComputerMove();
Move hardComputerMove();
//void drawGraph();
void makeMove(Move & move);     // делает ход

void moveBot();

bool filled();              // проверяет, заполнено ли поле
bool used(figure & a);      // проверяет использована ли фигура


int minimax(figure & FigForNextMove, int x, int y);
int searchMax(figure &tFig);
int countRows();

// подсчитывает, сколько параметров в ряду совпадает:
int countPotentialRow(figure & f1,figure & f2, figure & f3, figure & f4);

//проверяют, сколько параметров совпадают в ряду: 4, 3 или 2
bool check4EqParameter(figure & f1,figure & f2, figure & f3, figure & f4);
bool check3EqParameter(figure & f1, figure & f2, figure & f3);
bool check2EqParameter(figure & f1, figure & f2);

bool checkWin();


bool check4EqParameter(figure & f1,figure & f2, figure & f3, figure & f4){
    if (f1.P1 == f2.P1 && f2.P1 == f3.P1 && f3.P1 == f4.P1 && f1.P1!=-1)
        return true;
    if (f1.P2 == f2.P2 && f2.P2 == f3.P2 && f3.P2 == f4.P2 && f1.P2!=-1)
        return true;
    if (f1.P3 == f2.P3 && f2.P3 == f3.P3 && f3.P3 == f4.P3 && f1.P3!=-1)
        return true;
    if (f1.P4 == f2.P4 && f2.P4 == f3.P4 && f3.P4 == f4.P4 && f1.P4!=-1)
        return true;
    return false;
}

void quarto()
{
    QMessageBox msg;
    msg.setWindowTitle("Quarto");
    msg.setIcon(QMessageBox::Information);
    msg.setText("Q u a r t o");
    if (checkWin() && (turn == 1 || turn == 2 || turn == 5 || turn == 7))
    {
        cout << "win_figures = " << win_figures[0] << ", " << win_figures[1] << ", " << win_figures[2] << ", " << win_figures[3] << endl;
        if (turn == 1)
        {
            turn = 9;
            TRN
            msg.setText("Первый игрок одержал победу!");
        }
        if (turn == 2)
        {
            turn = 10;TRN
            msg.setText("Второй игрок одержал победу!");
        }
        if (turn == 5)
        {
            turn = 11;TRN
            msg.setText("Игрок одержал победу!");
        }
        if (turn == 7)
        {
            turn = 12;TRN
            msg.setText("Компьютер одержал победу!");
        }
        //drawGraph();
        msg.exec();
        //STAGE = 1;

    }
    else if (turn == 1 || turn == 2 || turn == 5 || turn == 7)
    {
        msg.setText("На поле не найдена выигрышная комбинация, перепроверьте!");
        msg.exec();
    }
    else
    {
        msg.setText("Нажимать кнопку можно только вместо выбора фигуры");
        msg.exec();
    }

    //msg.exec();
}

bool checkWin(){
    for (int i =0; i<4; i++)
    {
        if (check4EqParameter(board[i][0].Figure, board[i][1].Figure, board[i][2].Figure, board[i][3].Figure))
        {
            win_figures = {i * 4 + 0, i * 4 + 1, i * 4 + 2, i * 4 + 3};
            return true;
        }
        if (check4EqParameter(board[0][i].Figure, board[1][i].Figure, board[2][i].Figure, board[3][i].Figure))
        {
            win_figures = {0 + i, 4 + i, 8 + i, 12 + i};
            return true;
        }
    }
    if (check4EqParameter(board[0][0].Figure, board[1][1].Figure, board[2][2].Figure, board[3][3].Figure))
    {
        win_figures = {0, 5, 10, 15};
        return true;
    }
    if (check4EqParameter(board[0][3].Figure, board[1][2].Figure, board[2][1].Figure, board[3][0].Figure))
    {
        win_figures = {3, 6, 9, 12};
        return true;
    }
    return false;
}



bool check3EqParameter(figure & f1, figure & f2, figure & f3){
    if (f1.P1 == f2.P1 && f2.P1 == f3.P1  && f1.P1!=-1)
        return true;
    if (f1.P2 == f2.P2 && f2.P2 == f3.P2  && f1.P2!=-1)
        return true;
    if (f1.P3 == f2.P3 && f2.P3 == f3.P3  && f1.P3!=-1)
        return true;
    if (f1.P4 == f2.P4 && f2.P4 == f3.P4  && f1.P4!=-1)
        return true;
    return false;
}



bool check2EqParameter(figure & f1, figure & f2){
    if (f1.P1 == f2.P1 && f1.P1!=-1)
        return true;
    if (f1.P2 == f2.P2 && f1.P2!=-1)
        return true;
    if (f1.P3 == f2.P3 && f1.P3!=-1)
        return true;
    if (f1.P4 == f2.P4 && f1.P4!=-1)
        return true;
    return false;
}



int countPotentialRow(figure & f1,figure & f2, figure & f3, figure & f4){
    int count = 0;
    if ((f1.P1 == -1 && f2.P1 == -1 && f3.P1 == -1 && f4.P1 != -1) ||
        (f1.P1 == -1 && f2.P1 == -1 && f3.P1 != -1 && f4.P1 == -1) ||
        (f1.P1 == -1 && f2.P1 != -1 && f3.P1 == -1 && f4.P1 == -1) ||
        (f1.P1 != -1 && f2.P1 == -1 && f3.P1 == -1 && f4.P1 == -1))
        count = 1;
    else if (check3EqParameter(f1, f2, f3) || check3EqParameter(f4, f2, f3) || check3EqParameter(f1, f2, f4) || check3EqParameter(f1, f3, f4))
        count = 3;
    else if (check2EqParameter(f1, f2) || check2EqParameter(f1, f3) || check2EqParameter(f1, f4) || check2EqParameter(f2, f3) ||
             check2EqParameter(f2, f4) || check2EqParameter(f3, f4))
        count = 2;
    return count;
}



int countRows(){
    // хорошая ситуация - в ряду стоят три фигуры, позволяющие выиграть -10
    // в ряду нет фигур +1
    // в ряду одна фигура +2
    // плохая ситуация - в ряду 2 фигуры позволяющие выиграть + 5
    // совсем плохая ситуация - в ряду 4 фигуры позволяющие выиграть +100

    // ряд заполнен и иные ситуации - +0


    int count = 0;
    int a3 = -10, a0 = 1, a1 = 2, a2 = 5,a4 = 100;
    int t = 0;
    for (int i = 0; i<4; i++){
        if (check4EqParameter(board[i][0].Figure, board[i][1].Figure, board[i][2].Figure, board[i][3].Figure))
            count += a4;
        if (check4EqParameter(board[0][i].Figure, board[1][i].Figure, board[2][i].Figure, board[3][i].Figure))
            count += a4;

        if (board[i][0].haveFig == 0 && board[i][1].haveFig == 0 && board[i][2].haveFig == 0 && board[i][3].haveFig == 0)
            count += a0;
        if (board[0][i].haveFig == 0 && board[1][i].haveFig == 0 && board[2][i].haveFig == 0 && board[3][i].haveFig == 0)
            count += a0;

        t = countPotentialRow(board[i][0].Figure, board[i][1].Figure, board[i][2].Figure, board[i][3].Figure);

        if (t == 3)
            count += a3;
        else if (t == 2)
            count += a2;
        else if (t == 1)
            count+= a1;
        t = countPotentialRow(board[0][i].Figure, board[1][i].Figure, board[2][i].Figure, board[3][i].Figure);
        if (t == 3)
            count += a3;
        else if (t == 2)
            count += a2;
        else if (t == 1)
            count+= a1;
    }
    if (check4EqParameter(board[0][0].Figure, board[1][1].Figure, board[2][2].Figure, board[3][3].Figure))
        count+= a4;
    if (check4EqParameter(board[0][3].Figure, board[1][2].Figure, board[2][1].Figure, board[3][0].Figure))
        count += a4;

    t = countPotentialRow(board[0][0].Figure, board[1][1].Figure, board[2][2].Figure, board[3][3].Figure);
    if (t == 3)
        count += a3;
    else if (t == 2)
        count += a2;
    else if (t == 1)
        count+= a1;

    t = countPotentialRow(board[0][3].Figure, board[1][2].Figure, board[2][1].Figure, board[3][0].Figure);
    if (t == 3)
        count += a3;
    else if (t == 2)
        count += a2;
    else if (t == 1)
        count+= a1;

    return count;
}



int searchMax(figure &tFig){
    int max_ = -1;
    for (int i =0; i<4; i++)
    {
        for (int j =0; j<4; j++)
        {
            if (!board[i][j].haveFig)
            {
                board[i][j].haveFig = 1;
                board[i][j].Figure = tFig;
                int t = 0;
                t = countRows();
                board[i][j].flush();
                if (t > max_)
                {
                    max_ = t;
                }
            }
        }
    }
    return max_;
}



bool used(figure & a){
    for (int i = 0; i<been.size(); i++)
    {
        if (been[i].P1 == a.P1 && been[i].P2 == a.P2 &&
            been[i].P3 == a.P3 && been[i].P4 == a.P4)
        {
            return true;
        }
    }
    return false;
}



int minimax(figure & FigForNextMove, int x, int y){
    cout << "minmax (" << x << ", " << y << ")" << endl;
    board[x][y].Figure = curFigure;
    board[x][y].haveFig = 1;
    if (checkWin())
    {
        board[x][y].flush();
        return 300;
    }
    int score = INT_MAX;
    figure tFig;
    for (int a1 = 0; a1 < 2; a1++)
    {
        tFig.P1 = a1;
        for (int a2 = 0; a2 < 2; a2++)
        {
            tFig.P2 = a2;
            for (int a3 = 0; a3 < 2; a3++)
            {
                tFig.P3 = a3;
                for (int a4 = 0; a4 < 2; a4++)
                {
                    tFig.P4 = a4;
                    if (used(tFig)) continue;
                    int temp = searchMax(tFig);
                    if (temp < score)
                    {
                        score = temp;
                        FigForNextMove = tFig;
                    }
                }
            }
        }
    }
    board[x][y].flush();
    return score;
}



figure strToFig(string str){
    figure a;
    a.P1 = str[0] - '0';
    a.P2 = str[1] - '0';
    a.P3 = str[2] - '0';
    a.P4 = str[3] - '0';
    return a;
}



string figureToString(figure & a){
   string x = to_string(a.P1) + to_string(a.P2) + to_string(a.P3) + to_string(a.P4);
    return x;
}



void makeMove(Move & m){
    board[m.X][m.Y].Figure = curFigure;
    board[m.X][m.Y].haveFig = 1;
    //board[(curFigure.num()) / 4][(curFigure.num()) % 4].haveFig = 1;
    cout << "make move, num = " << curFigure.num() << endl;
    left_figures[curFigure.num()].second = -1;
    been.push_back(curFigure);

  //  m.finish = 0;
    if (checkWin()) m.finish = 1;
}

bool filled(){
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            if (board[i][j].haveFig == 0)
                return false;
        }
    }
    return true;
}
Move easyComputerMove(){
    Move move;
    srand(time(NULL));
    figure temp;
    vector <figure> active;
    vector <int> emptySq;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {
            if (!board[i][j].haveFig)
                emptySq.push_back(i*4+j);

        }
    }
    int t = emptySq[rand()%emptySq.size()];
    move.X = t/4;
    move.Y = t%4;
    for (int  a1 = 0; a1<2; a1++)
    {
        for (int a2 = 0; a2<2; a2++)
        {
            for (int a3 = 0; a3<2; a3++)
            {
                for (int a4 = 0; a4<2; a4++)
                {
                    temp.P1 = a1;
                    temp.P2 = a2;
                    temp.P3 = a3;
                    temp.P4 = a4;
                    if (!used(temp))
                        active.push_back(temp);
                }
            }
        }
    }
    move.theNextFig = active[rand()%active.size()];
    move.finish = checkWin();//;&&(rand()<0.75*RAND_MAX);
    return move;
}



Move mediumComputerMove(){
    int score = INT_MAX, temp = 0;
    figure ftemp;
    Move move;
    srand(time(NULL));
    if (countMoves == 0 || rand() <= RAND_MAX/2)
    {
        srand(time(NULL));

        move.X = rand()%4;
        move.Y = rand()%4;
        figure temp;
        vector <figure> active;

        vector <int> emptySq;
        for (int i = 0; i<4; i++)
        {
            for (int j = 0; j<4; j++)
            {
                if (!board[i][j].haveFig )
                    emptySq.push_back(i*4+j);

            }
        }
        int t = emptySq[rand()%emptySq.size()];
        move.X = t/4;
        move.Y = t%4;

        for (int  a1 = 0; a1<2; a1++)
        {
            for (int a2 = 0; a2<2; a2++)
            {
                for (int a3 = 0; a3<2; a3++)
                {
                    for (int a4 = 0; a4<2; a4++)
                    {
                        temp.P1 = a1;
                        temp.P2 = a2;
                        temp.P3 = a3;
                        temp.P4 = a4;
                        if (!used(temp))
                            active.push_back(temp);
                    }
                }
            }
        }
        move.theNextFig = active[rand()%active.size()];
        move.finish = checkWin();//&&(rand()<0.8*RAND_MAX);
        return move;
    }
    //outputBoard();
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {

            if (board[i][j].haveFig == 0)
            {
                temp = minimax(ftemp, i, j);
                if (temp == 300 )  // если сходит так, то игра окончена
                {
                    move.X = i;
                    move.Y = j;
                    move.theNextFig = ftemp;
                    move.finish = checkWin();//&&(rand()<0.8*RAND_MAX);
                    return move;

                }
                if (temp < score)
                {
                    score = temp;
                    move.X = i;
                    move.Y = j;
                    move.theNextFig = ftemp;
                }
            }
        }
    }
    move.finish = checkWin();//&&(rand()<0.8*RAND_MAX);
    return move;
}



Move hardComputerMove(){
    int score = INT_MAX, temp = 0;
    figure ftemp;
    Move move;
    if (countMoves == 0)
    {
        srand(time(NULL));
        vector <int> emptySq;
        for (int i = 0; i<4; i++)
        {
            for (int j = 0; j<4; j++)
            {
                if (!board[i][j].haveFig)
                    emptySq.push_back(i*4+j);

            }
        }
        int t = emptySq[rand()%emptySq.size()];
        move.X = t/4;
        move.Y = t%4;
        figure temp;
        vector <figure> active;
        for (int  a1 = 0; a1<2; a1++)
        {
            for (int a2 = 0; a2<2; a2++)
            {
                for (int a3 = 0; a3<2; a3++)
                {
                    for (int a4 = 0; a4<2; a4++)
                    {
                        temp.P1 = a1;
                        temp.P2 = a2;
                        temp.P3 = a3;
                        temp.P4 = a4;
                        if (!used(temp))
                            active.push_back(temp);
                    }
                }
            }
        }
        move.theNextFig = active[rand()%active.size()];
        move.finish = 0;
        return move;
    }
    //outputBoard();

    //bool not_empty = 0;
    cout << "not rand" << endl;
    for (int i = 0; i<4; i++)
    {
        for (int j = 0; j<4; j++)
        {

            if (board[i][j].haveFig == 0)
            {
                temp = minimax(ftemp, i, j);
                cout << "ftemp fig num = " << ftemp.num() << endl;
                cout << "temp = " << temp << endl;
                if (temp == 300 )  // если сходит так, то игра окончена
                {
                    move.X = i;
                    move.Y = j;
                    move.theNextFig = ftemp;
                    move.finish = 1;
                    return move;


                }
                if (temp < score)
                {
                    score = temp;
                    move.X = i;
                    move.Y = j;
                    move.theNextFig = ftemp;
                }
            }
        }
    }

    return move;
}



void moveBot()
{
    if (difficulty == 1)
    {
        Move m = easyComputerMove();
        makeMove(m);
        if (m.finish == 1) quarto();
        else
        {
            been.push_back(m.theNextFig);
            curFigure = m.theNextFig;
            turn = 6;TRN
        }
    }
    if (difficulty == 2)
    {
        Move m = mediumComputerMove();
        makeMove(m);
        if (m.finish == 1) quarto();
        else
        {
            been.push_back(m.theNextFig);
            curFigure = m.theNextFig;
            turn = 6;TRN
        }
    }
    if (difficulty == 3)
    {
        Move m = hardComputerMove();
        makeMove(m);
        if (m.finish == 1) quarto();
        else
        {
            been.push_back(m.theNextFig);
            curFigure = m.theNextFig;
            turn = 6;TRN
        }
    }
}








void MainWindow::setDef()
{
    //leftX = -1.5; rightX = 1.5;
    leftX = -1.9; rightX = 1.9;

    //leftY = -2.8; rightY = 0.2;
    leftY = -1.9, rightY = 1.9;
    leftX -= 5 * 0.2;
    rightX -= 5 * 0.2;

    //leftX -= 0.665;
    //rightX -= 0.665;
}


void MainWindow::slotTimerAlarm()
{
    drawGraph();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    setMouseTracking(true);

    this->setMouseTracking(1);
    //mouseTracker = new QLabel(this);
    QTextStream cout(stdout);
   // ui->upButton->setVisible(false);
    //ui->downButton->setVisible(false);
    //ui->leftButton->setVisible(false);
    //ui->rightButton->setVisible(false);

    ui->setupUi(this);
    ui->outputGraph->setMouseTracking(1);
    ui->centralWidget->setMouseTracking(1);

    QRect rec = QApplication::desktop()->screenGeometry();
    //height = rec.height();
    //width = rec.width();
    pictHeight = rec.height();//QWi; //740
    pictWidth = rec.width();//1920; //1080
    ui->outputGraph->setGeometry(0, 0, rec.width(), rec.height());
    ui->centralWidget->setGeometry(0, 0, rec.width(), rec.height());
    this->showFullScreen();

    cout << "PIX " << pictWidth << endl;
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),
            this, SLOT(slotTimerAlarm()));
    timer->start(17);
    setDef();



    recountPixels();
    double r_size = 0.4*onePixelX;

    //ПОДГРУЗКА КАРТИНОК

    //img_your_turn.load(":/images/your_turn.png");
    //img_you_choose.load(":/images/you_choose.png");
    img_rules.load(":/images/rules.png");
    img_menu.load(":/images/menu.png");
    img_quarto.load(":/images/quarto.png");
    img_title_1.load(":/images/titles/1.png");
    img_title_2.load(":/images/titles/2.png");
    img_title_3.load(":/images/titles/3.png");
    img_title_4.load(":/images/titles/4.png");
    img_title_5.load(":/images/titles/5.png");
    img_title_6.load(":/images/titles/6.png");
    img_title_7.load(":/images/titles/7.png");
    img_title_9.load(":/images/titles/9.png");
    img_title_10.load(":/images/titles/10.png");
    img_title_11.load(":/images/titles/11.png");
    img_title_12.load(":/images/titles/12.png");
    img_background.load(":/images/background.png");
    img_background_1.load(":/images/background_1.png");
    img_background_2.load(":/images/background_2.png");
    img_background_3.load(":/images/background_3.png");

//    img_background = img_background.scaled(
//                pictWidth, pictHeight,
//                Qt::IgnoreAspectRatio,
//               Qt::SmoothTransformation);
//    img_background_1 = img_background_1.scaled(
//                pictWidth, pictHeight,
//                Qt::IgnoreAspectRatio,
//               Qt::SmoothTransformation);
//    img_background_2 = img_background_2.scaled(
//                pictWidth, pictHeight,
//                Qt::IgnoreAspectRatio,
//               Qt::SmoothTransformation);
//    img_background_3 = img_background_3.scaled(
//                pictWidth, pictHeight,
//                Qt::IgnoreAspectRatio,
//               Qt::SmoothTransformation);


    images_of_heroes.resize(16);
    QRect target = QRect(0, 0, r_size, r_size);
    for (int i = 0; i < 16; ++i)
    {
        images_of_heroes[i] = QPixmap(":/images/enabled/" + QString::number(i + 1) + ".png").scaled(
                                                       target.size(),
                                                       Qt::IgnoreAspectRatio,
                                                       Qt::SmoothTransformation);

    }

    images_of_heroes_disabled.resize(16);
    for (int i = 0; i < 16; ++i)
    {
        images_of_heroes_disabled[i] = QPixmap(":/images/disabled/" + QString::number(i + 1) + ".png").scaled(
                                                       target.size(),
                                                       Qt::IgnoreAspectRatio,
                                                       Qt::SmoothTransformation);
    }

    for (int i = 0; i < 16; ++i)
    {
        int a = i;
        int p1, p2, p3, p4;
        p4 = a % 2; a /= 2;
        p3 = a % 2; a /= 2;
        p2 = a % 2; a /= 2;
        p1 = a % 2; a /= 2;
        //cout << "FIGURE #" << i << ": " << p1 << " " << p2 << " " << p3 << " " << p4 << endl;
        left_figures.push_back({figure(p1, p2, p3, p4), 0});
    }

    drawGraph();
}



MainWindow::~MainWindow()
{
    //setMouseTracking(true);

    delete ui;
}


int MainWindow::what_rectange_is_it(double x, double y)
{
    if (STAGE == 1)
    {
        if (x >= 50 && y >= 50 && x <= 480 && y <= 95)
        {
            return 20; // singleplayer
        }
        if (x >= 230 && y >= 101 && x <= 624 && y <= 155)
        {
            return 21; // multiplayer
        }
        if (x >= 356 && y >= 162 && x <= 792 && y <= 224)
        {
            return 22; // exit
        }

    }
    else if (STAGE == 2)
    {
        //x = x * onePixelX;
        //y = y * onePixelY;
        int r_size = 0.4 * onePixelX;
        COUT
        //cout << "I'm fine" << endl;
        //rectangles.resize(16);
        rectangles = {{(Ox)*onePixelX - 2*r_size,(Oy)*onePixelY - 2*r_size},
                                                   {(Ox)*onePixelX - r_size,(Oy)*onePixelY - 2*r_size},
                                                   {(Ox)*onePixelX,(Oy)*onePixelY - 2*r_size},
                                                   {(Ox)*onePixelX + r_size,(Oy)*onePixelY - 2*r_size},
                                                   {(Ox)*onePixelX - 2*r_size,(Oy)*onePixelY - r_size},
                                                   {(Ox)*onePixelX - r_size,(Oy)*onePixelY - r_size},
                                                   {(Ox)*onePixelX,(Oy)*onePixelY - r_size},
                                                   {(Ox)*onePixelX + r_size,(Oy)*onePixelY - r_size},
                                                   {(Ox)*onePixelX - 2*r_size,(Oy)*onePixelY},
                                                   {(Ox)*onePixelX - r_size,(Oy)*onePixelY},
                                                   {(Ox)*onePixelX,(Oy)*onePixelY},
                                                   {(Ox)*onePixelX + r_size,(Oy)*onePixelY},
                                                   {(Ox)*onePixelX - 2*r_size,(Oy)*onePixelY + r_size},
                                                   {(Ox)*onePixelX - r_size,(Oy)*onePixelY + r_size},
                                                   {(Ox)*onePixelX,(Oy)*onePixelY + r_size},
                                                   {(Ox)*onePixelX + r_size,(Oy)*onePixelY + r_size}};

        int now = 1;
        for (auto r : rectangles)
        {
         COUT
            if (x >= r.first && x <= r.first + r_size
                    && y >= r.second && y <= r.second + r_size)
            {
                return now;
            }
            ++now;
        }

        if (x >= 47 + 11 && x <= 224 + 11 && y >= (Oy)*onePixelY + r_size * 1.85 + 62 && y <= (Oy)*onePixelY + r_size * 1.85 + 114)
        {
            return 17; // quarto!
        }

        if (x >= rectangles[1].first + 11 && x <= rectangles[1].first + 177  + 11 && y >= (Oy)*onePixelY + r_size * 1.85 + 62 && y <= (Oy)*onePixelY + r_size * 1.85 + 114)
        {
            return 18; // rules
        }

        if (x >= rectangles[2].first + 11 && x <= rectangles[2].first + 177  + 11 && y >= (Oy)*onePixelY + r_size * 1.85 + 62 && y <= (Oy)*onePixelY + r_size * 1.85 + 114)
        {
            return 19; // gotomenu
        }

        //правые прямоугольники
        int rect_is_selected = -1;

        for (int i = 0; i < left_figures.size(); ++i)
        {
            //pair<figure, int> * now = &left_figures[i];
            pair<figure, int> h = left_figures[i];
            int numba = h.first.P1 * 8 + h.first.P2 * 4 + h.first.P3 * 2 + h.first.P4;

            if (mouse_x >= rectangles[numba].first + r_size * 4.2 + 0*onePixelX
                    && mouse_x <= rectangles[numba].first + r_size * 4.2 + 0*onePixelX
                    + r_size
                    && mouse_y >= rectangles[numba].second &&
                    mouse_y <= rectangles[numba].second + r_size)
            {
                rect_is_selected = i;
            }
        }

        if (rect_is_selected != -1) return 20 + 3 + rect_is_selected;
        //от 23 до 38 заняты.


        return 0;
    }
    return 0;
}


void MainWindow::recountPixels()
{
    onePixelX = pictWidth/(rightX-leftX);
    onePixelY = pictHeight/(rightY-leftY);
    Ox = rightX; Oy = rightY;
}


void MainWindow::mousePressEvent(QMouseEvent *event)
{
    COUT;
    int r_size = 0.4*onePixelX;

    double x = (event->x());//;/onePixelX - Ox;
    double y = (event->y());//onePixelY - Oy;
    //y = -y;
    int rectangle = what_rectange_is_it(x, y); // stage проверяется внутри.
    if (rectangle >= 1 && rectangle <= 16)
    {
            //QMessageBox msg;
            //msg.setText(QString::number(rectangle));
            //msg.exec();
            if (!(turn == 3 || turn == 4 || turn == 6)) return;
            if (board[(rectangle - 1) / 4][(rectangle - 1) % 4].haveFig == 0)
            {
                // ход человека
                countMoves++;

                board[(rectangle - 1) / 4][(rectangle - 1) % 4].haveFig = 1;
                been.push_back(curFigure);
                //filled[(rectangle - 1) / 4][(rectangle - 1) % 4] = 1;
                board[(rectangle - 1) / 4][(rectangle - 1) % 4].Figure = curFigure;
                int a = rectangle;
                int p1, p2, p3, p4;
                p4 = a % 2; a /= 2;
                p3 = a % 2; a /= 2;
                p2 = a % 2; a /= 2;
                p1 = a % 2; a /= 2;
                left_figures[curFigure.num()].second = -1;
//                for (int i = 0; i < left_figures.size(); ++i)
//                {
//                    if (left_figures[i].first.P1 == p1 && left_figures[i].first.P2 == p2 &&
//                            left_figures[i].first.P3 == p3 && left_figures[i].first.P4 == p4)

//                    {
//                        left_figures[i].second = -1;
//                        //left_figures.erase(it);
//                        break;
//                    }
//                }

                if (turn == 3)
                {
                    turn = 1;TRN
                }

                if (turn == 4)
                {
                    turn = 2;TRN
                }

                if (turn == 6)
                {
                    turn = 5;TRN
                }

                drawGraph();
            }
    }
    if (rectangle == 17)
    {
        quarto();
    }
    if (rectangle == 18)
    {
        QMessageBox msg;
        msg.setWindowTitle("Правила игры");
        msg.setIcon(QMessageBox::Information);
        msg.setText("В игре 16 фигур, отличающихся 4 признаками. Задача игроков — собрать ряд из 4 фигур (можно и по диагонали) с хотя бы одним общим признаком: <br> - животное / предмет <br> - круглая / квадратная форма<br> - черное / белое изображение<br> - оранжевый / синий цвет обводки<br> При этом фигуру, которой будет ходить игрок, выбирает его соперник. Чтобы одержать победу, нужно нажать кнопку Quarto, когда на поле есть выигрышная комбинация.");
        msg.exec();
    }
    if (rectangle == 19)
    {
        board[4][4];
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                {
                    board[i][j].flush();
                }
        been.clear();
        countMoves = 0;
        win_figures.clear();
        left_figures.clear();

        for (int i = 0; i < 16; ++i)
        {
            int a = i;
            int p1, p2, p3, p4;
            p4 = a % 2; a /= 2;
            p3 = a % 2; a /= 2;
            p2 = a % 2; a /= 2;
            p1 = a % 2; a /= 2;
            left_figures.push_back({figure(p1, p2, p3, p4), 0});
        }

        STAGE = 1;

        drawGraph();
    }

    if (rectangle == 20)
    {
//        QMessageBox askk(QMessageBox::Question, QString::fromUtf8("Выберите уровень сложности бота: ")
//                        , QString::fromUtf8("Лёгкий"),
//                        QString::fromUtf8("Средний"),
//                        QString::fromUtf8("Сложный"), 0, this);
        QMessageBox msgBox;
        //QAbstractButton reply;
        //QMessageBox::question reply;

        msgBox.setText("Выберите уровень сложности бота");
        msgBox.setIcon(QMessageBox::Question);
        QPushButton *easy = msgBox.addButton(tr("Лёгкий"), QMessageBox::ActionRole);
        QPushButton *middle = msgBox.addButton(tr("Средний"), QMessageBox::ActionRole);
        QPushButton *hard = msgBox.addButton(tr("Сложный"), QMessageBox::ActionRole);
        QPushButton *cancel = msgBox.addButton(tr("Отменить"), QMessageBox::ActionRole);


        msgBox.setDefaultButton(middle);
        int ans  = msgBox.exec();


        if (ans == 3)
        {
            cout << "go back" << endl;
            return;
        }

        difficulty = ans + 1;
        STAGE = 2;
        turn = 5;TRN
        drawGraph();
    }
    if (rectangle == 21)
    {
        STAGE = 2;
        turn = 1;TRN
        drawGraph();
    }

    if (rectangle == 22)
    {
        QApplication::quit();
    }

    if (rectangle >= 23 && rectangle <= 38 && (turn == 1 || turn == 2 || turn == 5))
    {
        int a = rectangle - 23;
        int p1, p2, p3, p4;
        p4 = a % 2; a /= 2;
        p3 = a % 2; a /= 2;
        p2 = a % 2; a /= 2;
        p1 = a % 2; a /= 2;
        string f;
        f += to_string(p1);        f += to_string(p2);
        f += to_string(p3);        f += to_string(p4);
        int beeen = 0;

        for (int i = 0; i < left_figures.size(); ++i)
        {
            if (left_figures[i].first.P1 == p1 && left_figures[i].first.P2 == p2 &&
                    left_figures[i].first.P3 == p3 && left_figures[i].first.P4 == p4)
                if (left_figures[i].second == -1)
                    beeen = 1;
        }
        //if (std::find(been.begin(), been.end(), strToFig(f)) != been.end())

        // если фигура уже использована, то она не может быть выбрана
        if (beeen)
        {
            return;
        }
        if (turn == 1)
        {
            curFigure = strToFig(f);
            been.push_back(curFigure);


            turn = 4;TRN
            drawGraph();
        }

        if (turn == 2)
        {
            curFigure = strToFig(f);
            been.push_back(curFigure);
            turn = 3;TRN
            drawGraph();
        }

        if (turn == 5)
        {
            curFigure = strToFig(f);
            been.push_back(curFigure);
            //turn = ;
            turn = 7;TRN
            moveBot();
            drawGraph();
        }

    }

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QTextStream cout(stdout);

   //cout << "event key! " << event->key() << endl;;
   int key = event->key();
   if (key == 1060) key = Qt::Key_A;
   if (key == 1042) key = Qt::Key_D;
   if (key == 1062) key = Qt::Key_W;
   if (key == 1067) key = Qt::Key_S;
   if (key == 1049) key = Qt::Key_Q;
   if (key == 1059) key = Qt::Key_E;


    if (key == 1040 || key == 70) key = Qt::Key_F;
    if (key == 76) key = Qt::Key_L;


  if (key == Qt::Key_L)
  {
      if (can_move) can_move = 0;
      else can_move = 1;
  }


   if (key == Qt::Key_F)
   {
      cout << "KEY F";
       if (fullscreen)
       {
           fullscreen = 0;
           this->showNormal();
       }
       else
       {
           fullscreen = 1;
           this->showFullScreen();
       }
   }

   if (1) return;

   if (key == Qt::Key_W)// || event->key == Qt::key_
   {
       leftY += 0.2;
       rightY += 0.2;
       //leftX -= 0.1;
       //rightX -= 0.1;
       drawGraph();

   }
   if (key == Qt::Key_S)
   {
       leftY -= 0.2;
       rightY -= 0.2;
       //leftX -= 0.1;
       //rightX -= 0.1;
       drawGraph();
   }
   if (key == Qt::Key_A)
   {
       leftX += 0.2;
       rightX += 0.2;
       drawGraph();
   }
   if (key == Qt::Key_D)
   {
       leftX -= 0.2;
       rightX -= 0.2;
       drawGraph();
   }
    if (key == Qt::Key_Q)
   {

       leftX -= -1.5;
       rightX -= 1.5;
       leftY -= -1.5;
       rightY -= 1.5;

       recountPixels();
       drawGraph();
   }
   if (key == Qt::Key_E)
   {
       leftX += -1.5;
       rightX += 1.5;
       leftY += -1.5;
       rightY += 1.5;

       recountPixels();
       drawGraph();
   }


}


void MainWindow::mouseMoveEvent(QMouseEvent *event)
{

    mouse_x = event->x();
    mouse_y = event->y();
    //drawGraph();
}



void MainWindow::drawGraph()
{
    COUT

    clearFocus();

    recountPixels();
    what_rectange_is_it(0, 0);
    QPixmap graph(pictWidth, pictHeight);
    QPainter paint;
    paint.begin(&graph);

    paint.setRenderHint(QPainter::Antialiasing);
    paint.setRenderHint(QPainter::HighQualityAntialiasing);




    paint.eraseRect(0, 0, pictWidth, pictHeight);


    paint.setPen(QPen(Qt::black,5));


    if (STAGE == 1)
    {
        //paint.fillRect(QRect(0, 0, 1920, 1080), QBrush(QColor::fromRgb(193, 237, 255)));
        int x = mouse_x;
        int y = mouse_y;

        if (x >= 50 && y >= 50 && x <= 480 && y <= 95)
        {
            paint.drawImage(0, 0, img_background_1);
            //paint.drawPixmap(0, 0, QPixmap(":/images/background_1.png"));
        }
        else if (x >= 230 && y >= 101 && x <= 624 && y <= 155)
        {
            paint.drawImage(0, 0, img_background_2);
            //paint.drawPixmap(0, 0, QPixmap(":/images/background_2.png"));
        }
        else if (x >= 356 && y >= 162 && x <= 792 && y <= 224)
        {
            paint.drawImage(0, 0, img_background_3);
           // paint.drawPixmap(0, 0, QPixmap(":/images/background_3.png"));
        }
        else
        {
            paint.drawImage(0, 0, img_background);

        }//paint.drawPixmap(0, 0, QPixmap(":/images/background.png"));
        //paint.drawEllipse(mouse_x - 10/2, mouse_y - 10/2, 10, 10);


    }
    else if (STAGE == 2)
    {

        //QRect r1(500, 500, 11, 16);

        //paint.drawRect(r1);

        R = 0.4*onePixelX;

        double r_size = 0.4*onePixelX;

        //paint.setBrush();


        paint.setPen(Qt::NoPen);

        paint.fillRect(QRect(0, 0, pictWidth, pictHeight), QBrush(QColor::fromRgb(193, 237, 255)));
        paint.fillRect(QRect((Ox)*onePixelX - 2*r_size,(Oy)*onePixelY - 2*r_size, r_size * 4, r_size * 4),
                       QBrush(QColor::fromRgb(229, 229, 229)));
                       //QBrush(QColor::fromRgb(255, 255, 255)));


        paint.setPen(QPen(Qt::black, 3));


        //paint.drawEllipse((Ox)*onePixelX - R/2,(Oy)*onePixelY - R/2, R, R);//5*onePixelX, 5*onePixelX);

        paint.drawRect((Ox)*onePixelX - r_size,(Oy)*onePixelY - r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX,(Oy)*onePixelY - r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX - r_size,(Oy)*onePixelY, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX,(Oy)*onePixelY, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        //это были 4 внутренних прямоугольника.
        //1
        paint.drawRect((Ox)*onePixelX - 2*r_size,(Oy)*onePixelY - 2*r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        //2
        paint.drawRect((Ox)*onePixelX - r_size,(Oy)*onePixelY - 2*r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        //4
        paint.drawRect((Ox)*onePixelX + r_size,(Oy)*onePixelY - 2*r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        //3
        paint.drawRect((Ox)*onePixelX,(Oy)*onePixelY - 2*r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        /// это были верхние
        paint.drawRect((Ox)*onePixelX - 2*r_size,(Oy)*onePixelY - r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX + r_size,(Oy)*onePixelY - r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);

        paint.drawRect((Ox)*onePixelX - 2*r_size,(Oy)*onePixelY, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX + r_size,(Oy)*onePixelY, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);

        //ща будут нижние

        paint.drawRect((Ox)*onePixelX - 2*r_size,(Oy)*onePixelY + r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX + r_size,(Oy)*onePixelY + r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX - r_size,(Oy)*onePixelY + r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);
        paint.drawRect((Ox)*onePixelX,(Oy)*onePixelY + r_size, r_size, r_size);//(Ox)*onePixelX - 200, (Oy)*onePixelY -200, 0.4*onePixelX, 0.4*onePixelX);

        QRect target((Ox)*onePixelX - r_size,(Oy)*onePixelY - r_size, r_size, r_size);


        if (win_figures.size() != 0 && turn > 8)
        {
            paint.setPen(QPen(QColor::fromRgb(39, 197, 106), 7));

            for (int i = 0; i < win_figures.size(); ++i)
            {
                paint.drawRect(rectangles[win_figures[i]].first, rectangles[win_figures[i]].second, r_size, r_size);
            }

            paint.setPen(QPen(Qt::black, 3));
        }


//        paint.drawPixmap(target, QPixmap(":/images/enabled/1.png").scaled(
//                                                          target.size(),
//                                                          Qt::IgnoreAspectRatio,
//                                                          Qt::SmoothTransformation)
//                                         );

        //paint.drawImage(0, 0, QImage(":/images/logo.png"));

        //quarto, rules, menu:

        paint.drawImage(rectangles[0].first + 11, (Oy)*onePixelY + r_size * 1.85, img_quarto);
        paint.drawImage(rectangles[1].first + 11, (Oy)*onePixelY + r_size * 1.85, img_rules);
        paint.drawImage(rectangles[2].first + 11, (Oy)*onePixelY + r_size * 1.85, img_menu);

        // курсор
        //paint.drawEllipse(mouse_x - 10/2, mouse_y - 10/2, 10, 10);


        // ЛОГИКА
        //paint.drawImage(rectangles[0].first, 0, img_you_choose);
        if (turn == 1) paint.drawImage(rectangles[0].first, 0, img_title_1);
        if (turn == 2) paint.drawImage(rectangles[0].first, 0, img_title_2);
        if (turn == 3) paint.drawImage(rectangles[0].first, 0, img_title_3);
        if (turn == 4) paint.drawImage(rectangles[0].first, 0, img_title_4);
        if (turn == 5) paint.drawImage(rectangles[0].first, 0, img_title_5);
        if (turn == 6) paint.drawImage(rectangles[0].first, 0, img_title_6);
        if (turn == 7) paint.drawImage(rectangles[0].first, 0, img_title_7);
        if (turn == 9) paint.drawImage(rectangles[0].first, 0, img_title_9);
        if (turn == 10) paint.drawImage(rectangles[0].first, 0, img_title_10);
        if (turn == 11) paint.drawImage(rectangles[0].first, 0, img_title_11);
        if (turn == 12) paint.drawImage(rectangles[0].first, 0, img_title_12);


        //paint.drawImage(rectangles[0].first, 0, img_you_choose);


        //paint.drawText(rectangles[0].first, 50, "turn = " + QString::number(turn));

        //отрисовка поля!
        //фигуры рисуются всегда:

        for (int i = 0; i < 16; ++i)
        {
            if (board[i / 4][i % 4].haveFig == 0) continue;
            int curfignum = board[i / 4][i % 4].Figure.num();
            paint.drawPixmap(QRect(rectangles[i].first, rectangles[i].second, r_size, r_size),
                             images_of_heroes[curfignum]);
        }
        //если игрок выбирает, куда походить, то проецируется disabled-версия на поле текущей фигуры.
        if (turn == 3 || turn == 4 || turn == 6)
        {
            int curFigNum = curFigure.P1 * 8 + curFigure.P2 * 4 + curFigure.P3 * 2 + curFigure.P4;

            for (int i = 0; i < 16; ++i)
            {
                //pair<figure, int> * now = &left_figures[i];
                //pair<figure, int> h = left_figures[i];
                int numba = i;//h.first.P1 * 8 + h.first.P2 * 4 + h.first.P3 * 2 + h.first.P4;

                if (mouse_x >= rectangles[numba].first
                        && mouse_x <= rectangles[numba].first
                        + r_size
                        && mouse_y >= rectangles[numba].second &&
                        mouse_y <= rectangles[numba].second + r_size
                        && board[numba / 4][numba % 4].haveFig == 0)
                {
                    //cout << turn << " = turn " << endl;
                   // cout << curFigNum << endl;
                    paint.drawPixmap(QRect(rectangles[i].first, rectangles[i].second, r_size, r_size),
                                     images_of_heroes_disabled[curFigNum]);

                }
            }
        }


        int rect_is_selected = -1;
        if (turn == 1 || turn == 2 || turn == 5)
        {


            for (int i = 0; i < left_figures.size(); ++i)
            {
                //pair<figure, int> * now = &left_figures[i];
                pair<figure, int> h = left_figures[i];
                int numba = h.first.P1 * 8 + h.first.P2 * 4 + h.first.P3 * 2 + h.first.P4;

                if (mouse_x >= rectangles[numba].first + r_size * 4.2 + 0*onePixelX
                        && mouse_x <= rectangles[numba].first + r_size * 4.2 + 0*onePixelX
                        + r_size
                        && mouse_y >= rectangles[numba].second &&
                        mouse_y <= rectangles[numba].second + r_size)
                {
                    rect_is_selected = i;
                }
            }


            if (rect_is_selected != -1 && left_figures[rect_is_selected].second != -1)
            {
                for (auto h = left_figures.begin(); h != left_figures.end(); ++h)
                {
                    int numba = h->first.P1 * 8 + h->first.P2 * 4 + h->first.P3 * 2 + h->first.P4;
                    if (numba == rect_is_selected)
                    {
                        h->second = 1;
                    }
                    else if (h->second != -1) h->second = 0;
                }
            }
        }
        else
        {
            for (int i = 0; i < left_figures.size(); ++i)
            {
                //left_figures[i].second = 0;
                if (figureToString(left_figures[i].first) ==
                        figureToString(curFigure))
                {
                    if (left_figures[i].second == 0) left_figures[i].second = 1;
                }
            }
        }

        //обычная отрисовка фигур уже с учетом enabled.

        for (auto h : left_figures)
        {
            int numba = h.first.P1 * 8 + h.first.P2 * 4 + h.first.P3 * 2 + h.first.P4;
            //QString path;
            QPixmap todraw;
            if (h.second == 1) // if enabled
            {
                todraw = images_of_heroes[numba];//.scaled(
                                      //  target.size(),
                                        //Qt::IgnoreAspectRatio,
                                     //   Qt::SmoothTransformation);
                paint.drawPixmap(QRect(rectangles[numba].first + r_size * 4.2 + 0*onePixelX,
                                      rectangles[numba].second, r_size, r_size), todraw);
            }
            else if (h.second == 0)
            {
                todraw = images_of_heroes_disabled[numba];//.scaled(
                                                    //    target.size(),
                                                    //    Qt::IgnoreAspectRatio,
                                                      //  Qt::SmoothTransformation);
                paint.drawPixmap(QRect(rectangles[numba].first + r_size * 4.2 + 0*onePixelX,
                                      rectangles[numba].second, r_size, r_size), todraw);
            }


        }

    //paint.drawText(rectangles[0].first + r_size * 4.2 + 0*onePixelX, rectangles[0].second, "aaaaaa");


    }


    paint.end();

    ui->outputGraph->setPixmap(graph);
    return;
}





