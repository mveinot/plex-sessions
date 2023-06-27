#include "settingsdialog.h"
#include "ui_settingsdialog.h"

settingsDialog::settingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::settingsDialog)
{
    QCoreApplication::setOrganizationName("mvgrafx");
    QCoreApplication::setOrganizationDomain("mvgrafx.net");
    QCoreApplication::setApplicationName("Plex Sessions");
    ui->setupUi(this);
    ui->lineEdit->setText(appSettings.value("server/host", "plexserver.local").toString());
    ui->lineEdit_2->setText(appSettings.value("server/token", "X-Plex-Token here").toString());
    ui->spinBox->setValue(appSettings.value("server/port", 32400).toInt());
    ui->checkBox->setChecked(appSettings.value("server/use_ssl", false).toBool());
}

settingsDialog::~settingsDialog()
{
    delete ui;
}

void settingsDialog::on_buttonBox_accepted()
{
    appSettings.setValue("server/host", ui->lineEdit->text());
    appSettings.setValue("server/token", ui->lineEdit_2->text());
    appSettings.setValue("server/port", ui->spinBox->value());
    appSettings.setValue("server/use_ssl", ui->checkBox->isChecked());
}

void settingsDialog::showURLTestResult(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        ui->label_4->setText("Success!");
    } else
    {
        ui->label_4->setText(reply->errorString());
    }
}

void settingsDialog::on_pushButton_clicked()
{
    ui->label_4->setText("");
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QString server = ui->lineEdit->text();
    QString token = ui->lineEdit_2->text();
    int port = ui->spinBox->value();
    bool use_ssl = ui->checkBox->isChecked();

    QUrl testURL;

    if (use_ssl)
    {
        testURL = QUrl("https://"+server+":"+QString::number(port)+"/status/sessions?X-Plex-Token="+token);
    } else {
        testURL = QUrl("http://"+server+":"+QString::number(port)+"/status/sessions?X-Plex-Token="+token);
    }

    QNetworkRequest request = QNetworkRequest(testURL);
    connect(manager, &QNetworkAccessManager::finished, this, &settingsDialog::showURLTestResult);
    manager->get(request);
}

