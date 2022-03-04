#ifndef DIROPERATION_H
#define DIROPERATION_H

#include <QObject>
#include <QDir> //for working with Directory

class DirOperation
{
public:
    //list all files in current path... with defualt parameter
    QStringList listDirectory(QString path = QDir::currentPath() + "/Download");
    //getters
    QString getCurrentPath();
    QString getRequestedFileName();
    bool getRoute(const QString &, const QString &);
    //static getter download method
    QByteArray downloadFileToClient(const QString &);
    void saveFile(const QByteArray &, const QString &);
    //method to verify file does not exist in directory before saving to diskt
    bool existFile(QString, const QString &);
    //single instance access method
    static DirOperation *getInstance();

private:
     static DirOperation *_instance; //for single instance access
     DirOperation(); //default constructor to avoid obj instance
     QStringList listDir;
     QString auxPath;
     QString requestedFileName;
};

#endif // DIROPERATION_H
