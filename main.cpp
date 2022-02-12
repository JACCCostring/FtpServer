#include <QCoreApplication>
#include "ftpserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //declaring obj
    ftpServer ftp;
    //starting server on port 2021
    ftp.startFtpServer(2021);
    //rest of the code
    return a.exec();
}
