#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QRect>
#include <QRectF>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPixmap>
#include <QImage>
#include <QString>
#include <QtCore>
#include <QtGui>
#include <QAbstractItemDelegate>
#include <QColor>

#include <vector>
#include <utility>

#include "engine.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update_scene();

    void first_paint();

    void paint();

private slots:
    void on_play_pause_clicked();

    void on_new_game_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *scene;

    QGraphicsPixmapItem*** items;

    bool action;

    QTimer* m_timer;

    Engine* eng;

    int resolution;

    int delay;

    int ViewHeight;
    int ViewWidth;

    QPixmap* Red;
    QPixmap* Empty;

    std::vector<std::pair<int,int>>changed;
};
