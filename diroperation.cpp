#include "diroperation.h"
#include <QDebug>
//initializing singleton instance
DirOperation *DirOperation::_instance;

DirOperation::DirOperation() {}

QStringList DirOperation::listDirectory(QString path)
{
    //if path is ampty then defult path is current path
    //creatin obj and init.... with passed path parameter
    QDir dir(path);
    //assigning current path to aux path for future uses
    auxPath = path;
    listDir = dir.entryList(QDir::Filter::Files); //getting list of file in the current directory
    //returning list of files
    return listDir;
}

bool DirOperation::existFile(QString path, const QString &name)
{
    //if path is ampty then defult path is current path
    //creatin obj and init.... with passed path parameter
    QDir dir(path);
    //retrieving files in directory
    listDir = dir.entryList(QDir::Filter::Files); //getting list of file in the current directory
    //returning true or false if exist or not file in list
    return dir.exists(name);
}

DirOperation *DirOperation::getInstance()
{
    if(! _instance) _instance = new DirOperation;
    return _instance;
}

QString DirOperation::getCurrentPath()
{
    return auxPath;
}

QString DirOperation::getRequestedFileName()
{
    return requestedFileName;
}

bool DirOperation::getRoute(const QString &route, const QString &characters)
{
    QStringList listRoutes = route.split("/"); //split all containing /
    //retrieving the important data after GET command
     QString currentRoute = listRoutes[1]; //dir / upload / .extension
     //spliting the filename
     QStringList absFileName = listRoutes[2].split(" "); //split by space to obtain file name
     //retrieving filaname
     requestedFileName = absFileName[0]; //only 0 index wanted, 1 index is HTTP/1.1
    //if string start with 'specific characters' then
    if(currentRoute.startsWith(characters)){
        return true;        //return true meaing valid route found
    }
    else if(currentRoute.contains(characters)){
        //splitting by spaces to avoid HTTP at the end of line
        QStringList splitted = currentRoute.split(" ");
        requestedFileName = splitted[0];
        return true;
    }
    //clearing listRoutes
    listRoutes.clear();
     //if not then return false ! not found it
    return false;
}

QByteArray DirOperation::downloadFileToClient(const QString &fileName)
{
    //creating file obj  and init... with curr.. path + Download dir + file name
    QFile file(QDir::currentPath()+"/Download/"+fileName);
    //checking if file can be open for Binary read
    if(! file.open(QIODevice::ReadOnly)){
        qDebug()<<"File could'nt be opened !";
    }
        //retrieving data from file of size of file
        QByteArray dataFile = file.read(file.size()); //file.readAll() could be used here
        file.close(); //closing file after used
        //returning content file
        return dataFile;
}

void DirOperation::saveFile(const QByteArray &data, const QString &fileName)
{
    //creating file obj  and init... with curr.. path + Download dir + file name
    QFile file(QDir::currentPath()+"/Download/"+fileName);
    //checking if file can be open for Binary Write and append
    if(! file.open(QIODevice::WriteOnly | QIODevice::Append)){
        qDebug()<<"File could'nt be opened for writing !";
    }
        //checking data has bytes
       if(data.size() > 0)
        //writing data to file
       file.write(data);
       //close
       file.close();
}
