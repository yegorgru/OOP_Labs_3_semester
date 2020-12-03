#include "mainwindow.h"
#include "ui_mainwindow.h"

const QString part_way=":/images/";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QRect rec = QApplication::desktop()->screenGeometry();
    const int HEIGHT = rec.height();
    const int WIDTH = rec.width();

    ViewHeight = HEIGHT*0.8;
    ViewWidth = WIDTH*0.82;

    ui->setupUi(this);

    scene=new QGraphicsScene;
    scene->setSceneRect ( 0,0,ViewWidth,ViewHeight);


    ui->graphicsView->setScene(scene);

    action = false;

    items=nullptr;

    delay = 0;

    m_timer = new QTimer(this);
    connect (m_timer,&QTimer::timeout,this,&MainWindow::update_scene);

    resolution = ui->resolution->value();

    eng = nullptr;

    Red = nullptr;
    Empty = nullptr;

    ui->graphicsView->setScene(scene);
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_play_pause_clicked()
{
    if(eng!=nullptr){
        if(action){
            action=false;
            m_timer->stop();
        }
        else{
            action=true;

            m_timer->start(delay);

        }
    }
}

void MainWindow::first_paint(){
    std::vector<std::vector<bool>> F = eng->getFIELD();
    for(int i = 0;i<eng->GetRows();i++){
        for(int j = 0;j<eng->GetCols();j++){
            if(F[i][j]==1){
                items[i][j]->setPixmap(*Red);
            }
            else{
                items[i][j]->setPixmap(*Empty);
            }
        }
    }
    scene->update();
}

void MainWindow::paint(){
    std::vector<std::vector<bool>> F = eng->getFIELD();
    for(const auto& i : changed){
        if(F[i.first][i.second]==1){
            items[i.first][i.second]->setPixmap(*Red);
        }
        else{
            items[i.first][i.second]->setPixmap(*Empty);
        }
    }
    scene->update();
}

void MainWindow::update_scene(){
    eng->update(changed);
    paint();
}

void MainWindow::on_new_game_clicked()
{
    if(Red!=nullptr){
        delete Red;
    }

    if(Empty!=nullptr){
        delete Empty;
    }

    int bef_rows = -1;
    int bef_cols = -1;
    if(eng!=nullptr){
        bef_rows = eng->GetRows();
        bef_cols = eng->GetCols();
    }

    resolution = ui->resolution->value();

    if(eng!=nullptr){
        delete eng;
        eng = nullptr;
    }

    if(eng == nullptr){
        eng = new Engine(ViewWidth/resolution,ViewHeight/resolution,ui->density->value());
    }

    Red = new QPixmap(resolution,resolution);
    Red->fill(Qt::red);

    Empty = new QPixmap(resolution,resolution);
    Empty->fill(Qt::white);

    delay = (101-ui->speed->value())*20;

    scene->clear();

    if(items!=nullptr){
        for(int i=0;i<bef_rows;i++){
            delete[] items[i];
        }
        delete[] items;
    }

    items = new QGraphicsPixmapItem**[eng->GetRows()];
    for(int i=0;i<eng->GetRows();i++){
        items[i] = new QGraphicsPixmapItem*[eng->GetCols()];
    }
    int margin = resolution/2;
    for(int i=0;i<eng->GetRows();i++){
        for(int j=0;j<eng->GetCols();j++){
            items[i][j]= new QGraphicsPixmapItem;
            items[i][j]->setScale(0.75);
            items[i][j]->setFlags(QGraphicsItem::ItemIsSelectable);
            items[i][j]->setPos(margin+resolution*i,margin+resolution*j);
            scene->addItem(items[i][j]);
        }
    }

    ui->graphicsView->setScene(scene);

    first_paint();

    changed.clear();
    changed.reserve(eng->GetCols() * eng->GetRows());
    if(action == true){
        on_play_pause_clicked();
    }
}
