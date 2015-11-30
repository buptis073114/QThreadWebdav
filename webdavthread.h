#ifndef WEBDAVTHREAD_H
#define WEBDAVTHREAD_H

#include <QObject>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QFile>
#include <QTextStream>
#include <string>
#include <QIcon>
#include <QMessageBox>
#include <QDebug>
#include <QSslCertificate>
#include <QtCore>



class webdavthread : public QObject
{
    Q_OBJECT
public:
    explicit webdavthread(QObject *parent = 0);
    void DoSetup(QThread &cThread);


private:
    //void DoSetup(QThread &cThread);
    typedef QMap<QString, QStringList > PropNames;
    QNetworkReply *m_reply;
    QNetworkReply *m_authenticator_lastReply;



public slots:
    void webdav();
    void provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator);
    void sendReplyFinished(QNetworkReply *re);
    void sslErrors(QNetworkReply *reply,const QList<QSslError> &errors);

    void slotReadyRead();
    void slotError(QNetworkReply::NetworkError);
};

#endif // WEBDAVTHREAD_H
