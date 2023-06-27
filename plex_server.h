#ifndef PLEX_SERVER_H
#define PLEX_SERVER_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QtXml>

class plex_server : public QObject
{
    Q_OBJECT
public:
    explicit plex_server(QObject *parent = nullptr);
    QUrl plex_url();
    struct Media_Item {
        QString title;
        bool is_series = false;
        QString series;
        int year;
        int season;
        int episode;
        QString user;
        QString player;
        QString resolution;
    };
    QList<Media_Item> Media;

public slots:
    void handleDownload(QNetworkReply *reply);

private:
    QXmlStreamReader *xml;
    void parseSession(QXmlStreamReader *xml);
    QSettings appSettings;


signals:
    void updateMenu(int items);

};

#endif // PLEX_SERVER_H
