#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

namespace Ui {
class settingsDialog;
}

class settingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit settingsDialog(QWidget *parent = nullptr);
    ~settingsDialog();

private slots:
    void on_buttonBox_accepted();
    void showURLTestResult(QNetworkReply *reply);
    void on_pushButton_clicked();

private:
    Ui::settingsDialog *ui;
    QSettings appSettings;
};

#endif // SETTINGSDIALOG_H
