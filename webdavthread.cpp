#include "webdavthread.h"

#include <QSslSocket>
#include <QAuthenticator>
#include <QSslPreSharedKeyAuthenticator>


webdavthread::webdavthread(QObject *parent) : QObject(parent)
{

}


void webdavthread::DoSetup(QThread &cThread){
    connect(&cThread,SIGNAL(started()),this,SLOT(webdav()));
}

void webdavthread::webdav(){

    QNetworkAccessManager *gManager = new QNetworkAccessManager;
    // Read the SSL certificate
    QFile file("D:/QT_workspace/test01/certificate/ssl.crt");

    //QFile file("D:/QT_workspace/QThreadWebdav/cert/localhost.pem");
    //QFile file("D:/QT_workspace/QThreadWebdav/cert/localhost.crt");
    file.open(QIODevice::ReadOnly);
    QByteArray bytes = file.readAll();

    // Create a certificate object
    //QSslCertificate certificate(bytes);
    // Add this certificate to all SSL connections
    //QSslSocket::addDefaultCaCertificate(certificate);
    QNetworkRequest gRequest;
    //QSslConfiguration config = gRequest.sslConfiguration();
    QSslConfiguration config = QSslConfiguration::defaultConfiguration();

    //config.setCaCertificates(certificate);
    //config.setLocalCertificate(certificate);
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1_0OrLater);

    //config.setLocalCertificate(certificate);
    gRequest.setSslConfiguration(config);

    QString url_s = "https://localhost/webdav.php";
    QUrl url(url_s);

    gRequest.setUrl(url);
    gRequest.setRawHeader("Connection", "keep-alive");

    QString concatenated = QString("username") + ":" + QString("password");
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    gRequest.setRawHeader("Authorization", headerData.toLocal8Bit());

    gRequest.setRawHeader("Accept", "text\/html,application\/xhtml+xml,application\/xml;q=0.9,image\/webp,*\/*;q=0.8");

    gRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream");

    connect(gManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(sendReplyFinished(QNetworkReply*)));

    connect(gManager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    connect(gManager, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(provideAuthenication(QNetworkReply*,QAuthenticator*)));

    webdavthread::PropNames query;
    QStringList props;

    props << "getlastmodified";
    props << "getcontentlength";
    props << "resourcetype";
    query["DAV:"] = props;
    QByteArray query1;

    query1 = "<?xml version=\"1.0\" encoding=\"utf-8\" ?>";
    query1 += "<D:propfind xmlns:D=\"DAV:\" >";
    query1 += "<D:prop>";
    foreach (QString ns, query.keys())
    {
        foreach (const QString key, query[ns])
            if (ns == "DAV:")
                query1 += "<D:" + key + "/>";
            else
                query1 += "<" + key + " xmlns=\"" + ns + "\"/>";
    }
    query1 += "</D:prop>";
    query1 += "</D:propfind>";

    url.setPath("/");
    //req.setUrl(reqUrl);
    gRequest.setRawHeader("Depth", 1 == 2 ? QString("infinity").toUtf8() : QString::number(1).toUtf8());

    QBuffer* dataIO = new QBuffer;
    dataIO->setData(query1);
    dataIO->open(QIODevice::ReadOnly);
    if(dataIO != 0 && dataIO->size() !=0) {
        gRequest.setHeader(QNetworkRequest::ContentLengthHeader, dataIO->size());

        qDebug()<<"query1 is "<<query1;

        qDebug()<<"dataIO size is "<<dataIO->size();

        //gRequest.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    }
   // QString strdata("PROPFIND");
    //QByteArray PROPData = QByteArray("PROPFIND", 9);
    m_reply = gManager->sendCustomRequest(gRequest, "PROPFIND", dataIO);
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)),
             this, SLOT(slotError(QNetworkReply::NetworkError)));
}




void webdavthread::provideAuthenication(QNetworkReply* reply, QAuthenticator* authenticator){
    qDebug() <<"\n"<<"in provideAuthenication";

    if (reply == m_authenticator_lastReply) {
        reply->abort();
        //emit errorChanged("WebDAV server requires authentication. Check WebDAV share settings!");
        reply->deleteLater();
        reply=0;
    }
    m_authenticator_lastReply = reply;

    QVariantHash opts = authenticator->options();
    QVariant optVar;
    foreach(optVar, opts) {
        qDebug() << "QWebdav::authenticationRequired()  option == " << optVar.toString();
    }
    authenticator->setUser(QString("username"));
    authenticator->setPassword(QString("password"));
    return;

}


void webdavthread::sendReplyFinished(QNetworkReply *re)
{
    qDebug() <<"\n"<<"in sendReplyFinished";
    qDebug() <<re->error()<<"\n";

    if(re->error() == QNetworkReply::NoError){
        QString ans = re->readAll();
        qDebug() <<ans;
    }
    else if(re->error() == QNetworkReply::RemoteHostClosedError){//RemoteHostClosedError为远处主机关闭了连接时发出的错误信号
        qDebug() <<re->error()<<"\n";
    }
    else if(re->error() == QNetworkReply::AuthenticationRequiredError){

    }

}


void webdavthread::sslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{

    qDebug() << "sslErrors()   reply->url == " << reply->url().toString(QUrl::RemoveUserInfo);
    qDebug() << "error is " << errors[0];
    QSslCertificate sslcert = errors[0].certificate();
    reply->ignoreSslErrors();

//    if ( ( sslcert.digest(QCryptographicHash::Md5) == m_sslCertDigestMd5 ) &&
//         ( sslcert.digest(QCryptographicHash::Sha1) == m_sslCertDigestSha1) )
//    {
//        // user accepted this SSL certifcate already ==> ignore SSL errors
//        reply->ignoreSslErrors();
//    } else {
//        // user has to check the SSL certificate and has to accept manually
//        emit checkSslCertifcate(errors);
//        reply->abort();
//    }
}

void webdavthread::slotReadyRead(){
    qDebug() << "slotReadyRead";
}
void webdavthread::slotError(QNetworkReply::NetworkError error){
    qDebug() << "slotError error" << error;
}

