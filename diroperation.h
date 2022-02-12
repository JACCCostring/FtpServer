#ifndef DIROPERATION_H
#define DIROPERATION_H

#include <QObject>
#include <QDir> //for working with Directory

class DirOperation
{
public:
    DirOperation();
    //list all files in current path... with defualt parameter
    static QStringList listDirectory(QString path = QDir::currentPath() + "/Download");
    //getters
    static QString getCurrentPath();
    static QString getRequestedFileName();
    static bool getRoute(const QString &, const QString &);
    //static getter download method
    static QByteArray downloadFileToClient(const QString &);
    static void saveFile(const QByteArray &, const QString &);
    //method to verify file does not exist in directory before saving to diskt
    static bool existFile(QString, const QString &);

private:
    static QStringList listDir;
    static QString auxPath;
    static QString requestedFileName;
};

#endif // DIROPERATION_H
