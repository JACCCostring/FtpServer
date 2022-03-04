#include "ftpserver.h"

ftpServer::ftpServer(QObject *parent) : QObject(parent)
{
    //initializing obj
    socketServer = new QTcpServer(this);
    //connecting for new connections
    connect(socketServer, &QTcpServer::newConnection, this, &ftpServer::onNewConnection);
}

ftpServer::~ftpServer()
{
    if(! socketServer) socketServer->deleteLater();
    if(! dSingleton) free(dSingleton); //freeing singleton instance
}

void ftpServer::startFtpServer(qint16 port)
{
    if(socketServer->isListening()){
        qDebug()<<"Server already listening!";
        return;
    }
    //then if server is not listening then
    socketServer->listen(QHostAddress::Any, port);
    qDebug()<<"Server is listening ...";
}

void ftpServer::managedDataClient(QTcpSocket *newSocket, const QByteArray &cmdShake, qint16 port)
{
    //retrieving client address
    QString clientAddress = newSocket->peerAddress().toString();
    //saving client file to download folder in server
    QTcpSocket *socketPost = new QTcpSocket(this);
    socketPost->connectToHost(clientAddress, port);
    //establishing handshake with client
    socketPost->write(cmdShake);
    //if file exist then is true other wise false
    bool fileExist = dSingleton->existFile(QDir::currentPath()+"/Download/", dSingleton->getRequestedFileName());
    //connecting signal when new data available
    connect(socketPost, &QTcpSocket::readyRead, this, [=](){
        //when new data available
        //if file does not exist in directory then save it
        if(! fileExist){
        //retrieve data in QByteArray because later socket will be delete it
            QByteArray _data = socketPost->readAll();
            //save data to file
        dSingleton->saveFile(_data, dSingleton->getRequestedFileName());
        }
        //if file does exist then notify client
        else { socketPost->write("200:exist");
            //disconnecting from client to avoid loop sending bytes
            qDebug()<<"file already exist !";
            socketPost->disconnectFromHost();
        }
    });
    //sending new uploaded files to client
    qDebug()<<"listing new files";
    //obtaining list of files in directory
    QStringList responseList = dSingleton->listDirectory();
    //looping throug all files
    foreach(const QString &response, responseList){
        newSocket->write("\n");
        newSocket->write(response.toUtf8()); //sending response to client
    }
    //clear responseList
    responseList.clear();
}

void ftpServer::onNewConnection()
{
    QTcpSocket *newSocket = socketServer->nextPendingConnection();
    qDebug()<<"new connection: "<<newSocket->peerAddress().toString();

    //connecting for when new socket get new data
    connect(newSocket, &QTcpSocket::readyRead, this, &ftpServer::onReadyRead);
    //connecting signal for when new socket is disconnected
    connect(newSocket, &QTcpSocket::disconnected, this, [=](){ //lambda
        qDebug()<<newSocket->peerAddress().toString()<<" is disconnected!";
        newSocket->deleteLater(); //deleting new socket each time is disconnected
    });
}

void ftpServer::onReadyRead()
{
    //getting reference from QObject caller
    QTcpSocket *newSocket = qobject_cast<QTcpSocket *>( sender() );
    //creating independent socket for POST
    //QTcpSocket *socketPost = socketServer->nextPendingConnection();
    //writing standard HTTP session Protocol to socket
    newSocket->write("HTTP/1.1 200 ok\r\n\r\n");
    //process received data
    QByteArray dataFile = newSocket->readAll();
    QString request = dataFile;
    //qDebug()<<"content size: "<<request.size();
    //if route exist then
    if(dSingleton->getRoute(request, "dir")){
        qDebug()<<"listing files";
        //obtaining list of files in directory
        QStringList responseList = dSingleton->listDirectory();
        //looping throug all files
        foreach(const QString &response, responseList){
            newSocket->write("\n");
            newSocket->write(response.toUtf8()); //sending response to client
        }
        //clear responseList
        responseList.clear();
    } //end of 1st route
    else if(dSingleton->getRoute(request, ".")){ //download file.extansion to client
        //download file
        qDebug()<<"downloading files ...";
        //retrieving binary data
        QByteArray file = dSingleton->downloadFileToClient(dSingleton->getRequestedFileName());
        //sending bynary data to socket
        newSocket->write(file);
    }
    //another route
    else if(dSingleton->getRoute(request, "upload")){ //upload files to server
        qDebug()<<"uploading files";
        //preparing handshake and receiving data from client
         managedDataClient(newSocket, "200:file", 2022);
    }
    //else
    else{
       qDebug()<<"Operation not allowed!";
    }
    //if(! newSocket->bytesAvailable())
    //disconnecting socket from server
    newSocket->disconnectFromHost();
}
