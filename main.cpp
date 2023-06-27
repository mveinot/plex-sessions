#include "settingsdialog.h"
#include "plex_server.h"

#include <QApplication>
#include <QIcon>
#include <QSystemTrayIcon>
#include <QMainWindow>
#include <QMenu>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QSettings>
#include <QTimer>

QMenu* iconmenu;
plex_server* plex;
QMenu *actions;
QMenu *sessions;
settingsDialog *s;
QNetworkAccessManager *manager;
QTimer *timer;

void exitApp()
{
    QApplication::quit();
}

void showSettings()
{
    s = new settingsDialog();
    s->show();
}

void drawMenu(int items)
{
    sessions->clear();

    if (items == 0)
    {
        sessions->setTitle("No sessions");
    }
    if (items == 1)
    {
        sessions->setTitle("1 session");
    }
    sessions->setTitle(QString::number(items)+" sessions");
    for (int i = 0; i < items; i++)
    {
        QAction *newact;
        if (plex->Media[i].is_series)
        {
            newact = new QAction(plex->Media[i].series + " S"+QStringLiteral("%1").arg(plex->Media[i].season, 2, 10, QLatin1Char('0'))+"E"+QStringLiteral("%1").arg(plex->Media[i].episode, 2, 10, QLatin1Char('0'))+" (" + plex->Media[i].title + ")");
        } else {
            newact = new QAction(plex->Media[i].title + " ("+QString::number(plex->Media[i].year)+")");
        }
        sessions->addAction(newact);
    }
}

void getSessions()
{
    manager->get(QNetworkRequest(plex->plex_url()));
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("mvgrafx");
    QCoreApplication::setOrganizationDomain("mvgrafx.net");
    QCoreApplication::setApplicationName("Plex Sessions");
    QSettings appSettings;
    a.setQuitOnLastWindowClosed(false);
    QPixmap icon(16,16);
    icon.load (":/Icons/Plex.icns");
    iconmenu = new QMenu();
    plex = new plex_server();
    QIcon oIcon( icon );
    a.setWindowIcon(oIcon);
    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(oIcon);
    trayIcon->setIcon(oIcon);

    trayIcon->setContextMenu(iconmenu);
    actions = iconmenu->addMenu("Actions");
    sessions = iconmenu->addMenu("Sessions");
    QAction *settings_act = new QAction("Settings...");
    QAction *quit_act = new QAction("Exit");
    actions->addAction(settings_act);
    actions->addAction(quit_act);
    trayIcon->setVisible(true);

    timer = new QTimer();

    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished, plex, &plex_server::handleDownload);
    QObject::connect(plex, &plex_server::updateMenu, drawMenu);
    QObject::connect(quit_act, &QAction::triggered, exitApp);
    QObject::connect(settings_act, &QAction::triggered, showSettings);
    QObject::connect(timer, &QTimer::timeout, getSessions);
    QString server_string = appSettings.value("server/host","not set").toString();

    if (server_string == "not set")
    {
        s = new settingsDialog();
        s->show();
    } else
    {
        manager->get(QNetworkRequest(plex->plex_url()));
        timer->start(5000);
    }
    return a.exec();
}

