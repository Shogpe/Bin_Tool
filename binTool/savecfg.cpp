#include "savecfg.h"
#include "qdebug.h"
#include <QSettings>


savecfg::savecfg(QObject *parent)
{
    load();
}


void savecfg::load()
{
    QSettings* settings = new QSettings("config.ini", QSettings::IniFormat);

    //输入文件
    inputCfg.bootAddr = settings->value("inputCfgBootAddr", "0x08000000").toString();
    inputCfg.bootPath = settings->value("inputCfgBootPath", "BOOT.bin").toString();
    inputCfg.appAddr = settings->value("inputCfgAppAddr", "0x08005000").toString();
    inputCfg.appPath = settings->value("inputCfgAppPath", "APP.bin").toString();
    inputCfg.timeForm = settings->value("inputCfgTimeForm", "yyyy-MM-dd").toString();
    inputCfg.space[0] =  settings->value("inputCfgSpace0", "_").toString();
    inputCfg.space[1] =  settings->value("inputCfgSpace1", "_V").toString();
    inputCfg.space[2] =  settings->value("inputCfgSpace2", ".").toString();

    inputCfg.chkDoBoot = settings->value("inputCfgDoBoot",true).toBool();
    inputCfg.chkDoApp = settings->value("inputCfgDoApp",true).toBool();
    inputCfg.chkDoAB = settings->value("inputCfgDoAB",true).toBool();

    //文件名组合
    for(int i = 0; i<NAME_NUM; i++)
    {
        bootOutputCfg.name[i] = settings->value("bootName"+QString::number(i+1), "").toString();
        appOutputCfg.name[i] = settings->value("appName"+QString::number(i+1), "").toString();
        abOutputCfg.name[i] = settings->value("abName"+QString::number(i+1), "").toString();
    }

    //日期选项
    bootOutputCfg.dateChk = settings->value("bootDateChk", false).toBool();
    appOutputCfg.dateChk = settings->value("appDateChk", false).toBool();
    abOutputCfg.dateChk = settings->value("abDateChk", true).toBool();

    //版本选项
    bootOutputCfg.isVer =  settings->value("bootIsVer", false).toBool();
    appOutputCfg.isVer =  settings->value("appIsVer", false).toBool();
    abOutputCfg.isVer =  settings->value("abIsVer", false).toBool();
    bootOutputCfg.verIndex =  settings->value("booVerIndex", 3).toInt();
    appOutputCfg.verIndex =  settings->value("appVerIndex", 3).toInt();
    abOutputCfg.verIndex =  settings->value("abVerIndex", 3).toInt();

    //路径

    for(int i = 0; i<TO_PATH_MAX_NUM; i++)
    {
        bootOutputCfg.toPath[i] = settings->value("bootToPath"+QString::number(i+1), "").toString();
        appOutputCfg.toPath[i] = settings->value("appToPath"+QString::number(i+1), "").toString();
        abOutputCfg.toPath[i] = settings->value("abToPath"+QString::number(i+1), "").toString();
    }
    settings->deleteLater();
}


void savecfg::save()
{
    QSettings* settings = new QSettings("config.ini", QSettings::IniFormat);

    //输入文件
    settings->setValue("inputCfgBootAddr", inputCfg.bootAddr);
    settings->setValue("inputCfgBootPath", inputCfg.bootPath);
    settings->setValue("inputCfgAppAddr", inputCfg.appAddr);
    settings->setValue("inputCfgAppPath", inputCfg.appPath);
    settings->setValue("inputCfgTimeForm",inputCfg.timeForm);
    settings->setValue("inputCfgSpace0",inputCfg.space[0]);
    settings->setValue("inputCfgSpace1",inputCfg.space[1]);
    settings->setValue("inputCfgSpace2",inputCfg.space[2]);

    settings->setValue("inputCfgDoBoot",inputCfg.chkDoBoot);
    settings->setValue("inputCfgDoApp",inputCfg.chkDoApp);
    settings->setValue("inputCfgDoAB",inputCfg.chkDoAB);

    //文件名组合
    for(int i = 0; i<NAME_NUM; i++)
    {
        settings->setValue("bootName"+QString::number(i+1), bootOutputCfg.name[i]);
        settings->setValue("appName"+QString::number(i+1), appOutputCfg.name[i]);
        settings->setValue("abName"+QString::number(i+1), abOutputCfg.name[i]);
    }

    //日期选项
    settings->setValue("bootDateChk", bootOutputCfg.dateChk);
    settings->setValue("appDateChk", appOutputCfg.dateChk);
    settings->setValue("abDateChk", abOutputCfg.dateChk);

    //版本选项
    settings->setValue("bootIsVer", bootOutputCfg.isVer);
    settings->setValue("appIsVer", appOutputCfg.isVer);
    settings->setValue("abIsVer", abOutputCfg.isVer);
    settings->setValue("booVerIndex", bootOutputCfg.verIndex);
    settings->setValue("appVerIndex", appOutputCfg.verIndex);
    settings->setValue("abVerIndex", abOutputCfg.verIndex);

    //路径
    for(int i = 0; i<TO_PATH_MAX_NUM; i++)
    {
        settings->setValue("bootToPath"+QString::number(i+1), bootOutputCfg.toPath[i]);
        settings->setValue("appToPath"+QString::number(i+1), appOutputCfg.toPath[i]);
        settings->setValue("abToPath"+QString::number(i+1), abOutputCfg.toPath[i]);
    }
    settings->deleteLater();
    emit nameRefreashed();





}
