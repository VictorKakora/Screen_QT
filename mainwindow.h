#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QTimer>

#include "database.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int compare(QImage &pre_img, QImage &img);

private slots:
    void on_screenButton_clicked();
    void slotCurrentPic(QModelIndex index);
    void on_btn_stop_clicked();
    void screen_fun();

private:
    Ui::MainWindow *ui;

    DataBase        *db;
    QSqlTableModel  *model;
    QTimer          *timer;
    QPixmap         *PrePixmap;

private:
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();

};

#endif // MAINWINDOW_H


