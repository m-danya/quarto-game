#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QLabel>



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    //sQLabel *mouseTracker;


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void drawGraph(); // функция, отвечающая за отрисовку графика
    void recountPixels(); // в зависимости от точности (об этом позже) считаем количество пикселей на один условный сантиметр
    void getData(); // получаем значения границ графика и точности
    double getRect(const std::function<double (const double&)> &f, const double left, const double right);
    std::vector<std::pair<double, double>> rectangles;
    std::vector<QPixmap> images_of_heroes;
    std::vector<QPixmap> images_of_heroes_disabled;
    QTimer * timer;


private slots:

    void setDef();
    void slotTimerAlarm();


public:
    Ui::MainWindow *ui; // форма
    double leftX,rightX; // границы по х
    double leftY,rightY; // границы по у
    int pictWidth,pictHeight; // ширина и высота картинки
    double step; // шаг (точность)
    double onePixelX,onePixelY; // количество пикселей на шаг
    double Ox,Oy; // координаты центра

    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    //virtual void slotTimerAlarm();



    int what_rectange_is_it(double x, double y);

    struct hero;

//protected:
   //virtual void keyPressEvent(QKeyEvent *event);
   //virtual void mouseMoveEvent(QMouseEvent *event);

};



#endif // MAINWINDOW_H
