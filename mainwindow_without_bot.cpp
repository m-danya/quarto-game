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



#define COUT QTextStream cout(stdout);

using namespace std;

struct avl_node;
struct tree;

//vector<pair<pair<double, double>, avl_node *>> mc;

vector<int> binary_rand;
avl_node * global_avl = NULL;
tree * global_tree = NULL;
double R;
bool can_move = 0;
int STAGE = 1;
QImage img_your_turn, img_you_choose, img_rules, img_menu;
QImage img_quarto;

int turn;


double mouse_x;
double mouse_y;

// LOGIC BOOLS

bool choosing_figure = 1;


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

std::vector<pair<figure, int>> left_figures; // figure и enabled


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

    this->showFullScreen();
    pictHeight = 1080; //740
    pictWidth = 1920; //1080

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()),
            this, SLOT(slotTimerAlarm()));
    //timer->start(10);
    setDef();



    recountPixels();
    double r_size = 0.4*onePixelX;

    //ПОДГРУЗКА КАРТИНОК

    img_your_turn.load(":/images/your_turn.png");
    img_you_choose.load(":/images/you_choose.png");
    img_rules.load(":/images/rules.png");
    img_menu.load(":/images/menu.png");
    img_quarto.load(":/images/quarto.png");



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

    //cout << "mouse event!" << endl;
    //cout << event->x() << " " << event->y() << endl;

    double x = (event->x());//;/onePixelX - Ox;
    double y = (event->y());//onePixelY - Oy;
    //y = -y;
    int rectangle = what_rectange_is_it(x, y); // stage проверяется внутри.
    if (rectangle >= 1 && rectangle <= 16)
    {
            QMessageBox msg;
            msg.setText(QString::number(rectangle));
            msg.exec();
    }
    if (rectangle == 17)
    {
        QMessageBox msg;
        msg.setWindowTitle("Quarto");
        msg.setIcon(QMessageBox::Information);
        msg.setText("Q u a r t o");
        msg.exec();
    }
    if (rectangle == 18)
    {
        QMessageBox msg;
        msg.setWindowTitle("Правила игры");
        msg.setIcon(QMessageBox::Information);
        msg.setText("В игре 16 фигур, отличающихся 4 признаками. Задача игроков — собрать ряд из 4 фигур с хотя бы одним общим признаком. Это может быть ряд из светлых фигур или из тёмных, из низких или высоких, из фигур с выемкой или фигур без неё, из круглых или квадратных. При этом фигуру, которой будет ходить игрок, выбирает его соперник.");
        msg.exec();
    }
    if (rectangle == 19)
    {
        STAGE = 1;
        drawGraph();
    }

    if (rectangle == 20)
    {
        STAGE = 2;
        turn = 5;
        drawGraph();
    }
    if (rectangle == 21)
    {
        STAGE = 2;
        turn = 1;
        drawGraph();
    }

    if (rectangle == 22)
    {
        QApplication::quit();
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
   if (key == 76) key = Qt::Key_L;


   if (key == Qt::Key_L)
   {
       if (can_move) can_move = 0;
       else can_move = 1;
   }
   if (!can_move)
   {
       return;
   }

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
    drawGraph();
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
        paint.drawPixmap(0, 0, QPixmap(":/images/background.png"));
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

        paint.fillRect(QRect(0, 0, 1920, 1080), QBrush(QColor::fromRgb(193, 237, 255)));
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

//        paint.drawPixmap(target, QPixmap(":/images/enabled/1.png").scaled(
//                                                          target.size(),
//                                                          Qt::IgnoreAspectRatio,
//                                                          Qt::SmoothTransformation)
//                                         );

        //paint.drawImage(0, 0, QImage(":/images/logo.png"));
        paint.drawImage(rectangles[0].first + 11, (Oy)*onePixelY + r_size * 1.85, img_quarto);
        paint.drawImage(rectangles[1].first + 11, (Oy)*onePixelY + r_size * 1.85, img_rules);
        paint.drawImage(rectangles[2].first + 11, (Oy)*onePixelY + r_size * 1.85, img_menu);

        // курсор
        //paint.drawEllipse(mouse_x - 10/2, mouse_y - 10/2, 10, 10);


        // ЛОГИКА
        paint.drawImage(rectangles[0].first, 0, img_you_choose);

//        for (int i = 0; i < 16; ++i)
//        {
//            if (board[i / 4][i % 4].haveFig == 0) continue;

//            QImage todraw = images_of_heroes[i].scaled(
//                        target.size(),
//                        Qt::IgnoreAspectRatio,
//                        Qt::SmoothTransformation);

//            paint.drawImage(QRect(rectangles[i].first, rectangles[i].second, r_size, r_size), todraw);
//            //paint.drawImage(QRect(rectangles[i-1].first, rectangles[i-1].second, r_size, r_size), images_of_heroes[i]);
//            //paint.drawImage(target, images_of_heroes[i]);
//        }

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

        if (rect_is_selected != -1)
        {
            for (auto h = left_figures.begin(); h != left_figures.end(); ++h)
            {
                int numba = h->first.P1 * 8 + h->first.P2 * 4 + h->first.P3 * 2 + h->first.P4;
                if (numba == rect_is_selected)
                {
                    h->second = 1;
                }
                else h->second = 0;
            }
        }

        for (auto h : left_figures)
        {
            int numba = h.first.P1 * 8 + h.first.P2 * 4 + h.first.P3 * 2 + h.first.P4;
            //QString path;
            QPixmap todraw;
            if (h.second) // if enabled
            {
                todraw = images_of_heroes[numba];//.scaled(
                                      //  target.size(),
                                        //Qt::IgnoreAspectRatio,
                                     //   Qt::SmoothTransformation);
            }
            else
            {
                todraw = images_of_heroes_disabled[numba];//.scaled(
                                                    //    target.size(),
                                                    //    Qt::IgnoreAspectRatio,
                                                      //  Qt::SmoothTransformation);
            }
            paint.drawPixmap(QRect(rectangles[numba].first + r_size * 4.2 + 0*onePixelX,
                                  rectangles[numba].second, r_size, r_size), todraw);

        }

    //paint.drawText(rectangles[0].first + r_size * 4.2 + 0*onePixelX, rectangles[0].second, "aaaaaa");


    }


    paint.end();

    ui->outputGraph->setPixmap(graph);
    return;
}







