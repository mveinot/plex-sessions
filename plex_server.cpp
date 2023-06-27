#include "plex_server.h"
#include <QSettings>

plex_server::plex_server(QObject *parent)
    : QObject{parent}
{

}

QUrl plex_server::plex_url()
{
    QString server = appSettings.value("server/host", "not set").toString();
    QString token = appSettings.value("server/token", "not set").toString();
    int port = appSettings.value("server/port", 32400).toInt();
    bool use_ssl = appSettings.value("server/use_ssl", false).toBool();

    if (use_ssl)
        return QUrl("https://"+server+":"+QString::number(port)+"/status/sessions?X-Plex-Token="+token);

    return QUrl("http://"+server+":"+QString::number(port)+"/status/sessions?X-Plex-Token="+token);
}

void plex_server::handleDownload(QNetworkReply *reply)
{
    Media.clear();
    QByteArray data = reply->readAll();
    xml = new QXmlStreamReader(data);
    while (!xml->atEnd() && !xml->hasError())
    {

        if (xml->readNext() == QXmlStreamReader::StartElement && xml->name() == QString("Video"))
        {
            parseSession(xml);
        }
    }
    emit updateMenu(Media.count());
}

void plex_server::parseSession(QXmlStreamReader *session)
{
    Media_Item item;
    if(session->tokenType() != QXmlStreamReader::StartElement && session->name() != QString("Video"))
    {
        qDebug() << "Called XML parseQuestionElement "
                 << "without a question Element in the XML stream!";
        return;
    }

    foreach (const QXmlStreamAttribute &atr, session->attributes())
    {
        //qDebug() << atr.name() << " : " << atr.value();
    }

    item.title = session->attributes().value("title").toString();
    item.year = session->attributes().value("year").toInt();
    if (session->attributes().value("type").toString() == QString("episode"))
    {
        item.is_series = true;
        item.series = session->attributes().value("grandparentTitle").toString();
        item.season = session->attributes().value("parentIndex").toInt();
        item.episode = session->attributes().value("index").toInt();
    }
    Media.append(item);
}
