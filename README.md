# QThreadWebdav
QT实现的Webdav的HTTPS通信
HTTP太不安全了，抓包随便就可以拿到报文，但是Webdav有不能通过QT自带的authenticationRequired进行用户名和密码的验证，只好自己拼接报文头，
本项目就是介绍如何用HTTPS进行Webdav通信。
authenticationRequired就是相当于我们访问一个ftp，ftp需要用户输入用户名和密码，authenticationRequired就是让用户输入
如果url是http的话，authenticationRequired是有效的，可以直接用，但是HTTPS就不行，直接就是报错RemoteHostClosedError，所以自己实现了填写HTTPS头部
    QString concatenated = QString("app1") + ":" + QString("ssssss");
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    QString headerData = "Basic " + data;
    gRequest.setRawHeader("Authorization", headerData.toLocal8Bit());
这样就行了，抓包看到的内容全部是加密的。安全了。



![image](https://github.com/buptis073114/TestAndroidJNI/blob/master/1130108806.jpg)
![image](https://github.com/buptis073114/TestAndroidJNI/blob/master/1381875294.jpg)
![image](https://github.com/buptis073114/TestAndroidJNI/blob/master/77042545.jpg)
