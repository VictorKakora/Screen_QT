#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QBuffer>
#include <QScreen>
#include <QCryptographicHash>

int count_pre = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = new DataBase();
    db->connectToDataBase();

    this->setupModel(TABLE,
                     QStringList() << trUtf8("id")
                                   << trUtf8("Имя изображения")
                                   << trUtf8("Hash")
                                   << trUtf8("Procent")
                                   << trUtf8("изображение")
                     );
    this->createUI();
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(screen_fun()));
    PrePixmap = new QPixmap();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    model = new QSqlTableModel(this);
    model->setTable(tableName);
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::createUI()
{
    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(4, true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(slotCurrentPic(QModelIndex)));
    model->select();
}

void MainWindow::on_screenButton_clicked()
{
    screen_fun();
    timer->start(60000);
}

void MainWindow::screen_fun(){
    QScreen *screen = QApplication::primaryScreen();
    QPixmap inPixmap = screen->grabWindow( 0 );
    QByteArray inByteArray;
    QBuffer inBuffer( &inByteArray );
    inBuffer.open( QIODevice::WriteOnly );
    inPixmap.save( &inBuffer, "PNG" );
    QByteArray hash_sum;
    QByteArray pre_hash_sum;

    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(inByteArray);
    hash_sum = hash.result().toHex();
    pre_hash_sum = hash_sum;

    QImage pre_img, img;

    pre_img = PrePixmap->toImage();
    img = inPixmap.toImage();

    int procent;
    if(count_pre == 0){
        procent = 0;
        count_pre = count_pre + 1;
    }
    else{
        procent = MainWindow::compare(pre_img, img);
    }

    PrePixmap->loadFromData(inByteArray);
    db->insertIntoTable(QDateTime::currentDateTime().toString("dd.MM.yyyy_hh:mm:ss.png"), inByteArray, hash_sum, procent);

    model->select();
}

int MainWindow::compare(QImage &pre_img, QImage &img){
    double count = 0;
    int w=pre_img.width();
    int h=pre_img.height();
    double s = h*w;
    for(int i=0;i<h;i++){
        for(int j=0;j<w;j++){
            if(pre_img.pixel(j, i) != img.pixel(j, i)){
                count = count + 1;
            }
        }
     }
    double dev = (count/s)*100;
    int procent = 100 - dev;
    return procent;
}

void MainWindow::slotCurrentPic(QModelIndex index)
{
    QPixmap outPixmap = QPixmap();
    outPixmap.loadFromData(model->data(model->index(index.row(), 4)).toByteArray());
    ui->picLabel->setPixmap(outPixmap.scaled(400,300));
}

void MainWindow::on_btn_stop_clicked()
{
    timer->stop();
}
