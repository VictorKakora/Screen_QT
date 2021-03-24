#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>



#define DATABASE_HOSTNAME       "ScreenDataBase"
#define DATABASE_NAME           "Screen.db"

#define TABLE                   "ScreenTable"
#define TABLE_NAME              "Name"
#define TABLE_PIC               "Pic"
#define HASH                    "Hash_sum"
#define PROCENT                 "Procent"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
     ~DataBase();
    void connectToDataBase();

private:
    QSqlDatabase    db;

private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

signals:

public slots:
    bool insertIntoTable(const QVariantList &data, const QVariantList &data1, const int &procent);
    bool insertIntoTable(const QString &name, const QByteArray &pic, const QByteArray &hash_sum, const int &procent);
};

#endif // DATABASE_H

