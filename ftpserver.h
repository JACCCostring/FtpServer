#ifndef FTPSERVER_H
#define FTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class ftpServer : public QObject
{
    Q_OBJECT
public:
    explicit ftpServer(QObject *parent = nullptr);

    //start server method
    void startFtpServer(qint16);

private:
    void managedDataClient(QTcpSocket *, const QByteArray &, qint16);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    QTcpServer *socketServer;
};

#endif // FTPSERVER_H
