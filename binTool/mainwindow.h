#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "savecfg.h"
#include <QMainWindow>
#include "QTimer"
#include "pathlist.h"
#include "QFile"

#define LOG_OUT(s)   {emit logOutPutRequest(s);}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void uiRefreashWithCfg();
private:
    savecfg* cfg;
    void extracted(QString &objName);
    void nameChanged();

    QTimer* timefTimer;
    pathList* bootPath;
    pathList* appPath;
    pathList* abPath;

signals:
    void refreashCfg(void);
    void logOutPutRequest(QString log);

private slots:

    void nameConstract();

    void pathSelAndSave(QString infor);

    void logOutPutRespond(QString log);


    void on_btn_bootPathSel_clicked();

    void on_btn_appPathSel_clicked();

    void on_chk_bootNameTime_stateChanged(int arg1);

    void on_chk_appNameTime_stateChanged(int arg1);

    void on_chk_abNameTime_stateChanged(int arg1);

    void on_btn_boot_clicked();

    void on_btn_app_clicked();

    void on_btn_ab_clicked();

    void on_le_bootAddr_editingFinished();

    void on_le_appAddr_editingFinished();

    void on_bootVerIdx_valueChanged(int arg1);

    void on_appVerIdx_valueChanged(int arg1);

    void on_chkBootVer_stateChanged(int arg1);

    void on_chkAppVer_stateChanged(int arg1);

    void on_btn_Clear_clicked();

    void on_chkABVer_stateChanged(int arg1);

    void on_abVerIdx_valueChanged(int arg1);

    void on_le_cTimeForm_editingFinished();


    void on_btn_do_clicked();

    void on_chk_doBoot_stateChanged(int arg1);

    void on_chk_doApp_stateChanged(int arg1);

    void on_chk_doAB_stateChanged(int arg1);

private:
    bool reNameFileAndSend(QFile &f, QString name, QString path);
    bool binFileMerge(QFile &f);
    bool readVerAndWrite(QFile &f, QLineEdit *l);
    void fillSpace(void);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
