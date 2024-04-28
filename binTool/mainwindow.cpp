﻿#include "mainwindow.h"
#include "qglobal.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QFileDialog"
#include "QDateTime"
#include "QByteArray"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cfg = new savecfg();

    timefTimer = new QTimer();
    timefTimer->setInterval(1000);
    connect(timefTimer, &QTimer::timeout, this, [this](){
        QDateTime dt = QDateTime::currentDateTime();
        ui->cTime->setText(dt.toString(cfg->inputCfg.timeForm));
    });
    timefTimer->start();

    QDateTime dt = QDateTime::currentDateTime();
    ui->cTime->setText(dt.toString(cfg->inputCfg.timeForm));

    QHBoxLayout* l;

    l= new QHBoxLayout(ui->lw_bootToPath);
    bootPath = new pathList(ui->lw_bootToPath);
    l->addWidget(bootPath);
    l= new QHBoxLayout(ui->lw_appToPath);
    appPath = new pathList(ui->lw_appToPath);
    l->addWidget(appPath);
    l= new QHBoxLayout(ui->lw_abToPath);
    abPath = new pathList(ui->lw_abToPath);
    l->addWidget(abPath);
    bootPath->setObjectName("bootPath");
    appPath->setObjectName("appPath");
    abPath->setObjectName("abPath");

    uiRefreashWithCfg();

    connect(this,SIGNAL(logOutPutRequest(QString)),this,SLOT(logOutPutRespond(QString)));
    connect(cfg,SIGNAL(nameRefreashed()),this,SLOT(nameConstract()));



    foreach(pathList* path, this->findChildren<pathList*>())
    {
        connect(path, SIGNAL(pathRefreash(QString)), this, SLOT(pathSelAndSave(QString)));
    }

    foreach(QLineEdit* le, this->findChildren<QLineEdit*>())
    {
        QString objName = le->objectName();
        if((objName.rightRef(5).startsWith("Name")||objName.startsWith("le_space"))
        &&objName.rightRef(1).toInt() <= NAME_NUM
        &&objName.rightRef(1).toInt() > 0)
        {
            connect(le, &QLineEdit::editingFinished, this, &MainWindow::nameChanged);
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_Clear_clicked()
{
    ui->logOut->clear();
}

void MainWindow::uiRefreashWithCfg()
{
    ui->le_bootPath->setText(cfg->inputCfg.bootPath);
    ui->le_bootAddr->setText(cfg->inputCfg.bootAddr);
    ui->le_appPath->setText(cfg->inputCfg.appPath);
    ui->le_appAddr->setText(cfg->inputCfg.appAddr);
    ui->le_cTimeForm->setText(cfg->inputCfg.timeForm);
    ui->le_space1->setText(cfg->inputCfg.space[0]);
    ui->le_space2->setText(cfg->inputCfg.space[1]);
    ui->le_space3->setText(cfg->inputCfg.space[2]);
    fillSpace();

    ui->chk_doBoot->setChecked(cfg->inputCfg.chkDoBoot);
    ui->chk_doApp->setChecked(cfg->inputCfg.chkDoApp);
    ui->chk_doAB->setChecked(cfg->inputCfg.chkDoAB);

    foreach(QLineEdit* le, this->findChildren<QLineEdit*>())
    {
        QString objName = le->objectName();
        if(objName.rightRef(5).startsWith("Name")
                &&objName.rightRef(1).toInt() <= NAME_NUM
                &&objName.rightRef(1).toInt() > 0)
        {
            int idx = objName.rightRef(1).toInt() - 1;
            if(objName.startsWith("le_boot"))
            {
                le->setText(cfg->bootOutputCfg.name[idx]);
            }
            else if(objName.startsWith("le_app"))
            {
                le->setText(cfg->appOutputCfg.name[idx]);
            }
            else if(objName.startsWith("le_ab"))
            {
                le->setText(cfg->abOutputCfg.name[idx]);
            }
        }
    }

    ui->chk_bootNameTime->setChecked(cfg->bootOutputCfg.dateChk);
    ui->chk_appNameTime->setChecked(cfg->appOutputCfg.dateChk);
    ui->chk_abNameTime->setChecked(cfg->abOutputCfg.dateChk);
    nameConstract();

    ui->chkBootVer->setChecked(cfg->bootOutputCfg.isVer);
    ui->chkAppVer->setChecked(cfg->appOutputCfg.isVer);
    ui->chkABVer->setChecked(cfg->abOutputCfg.isVer);

    ui->bootVerIdx->setValue(cfg->bootOutputCfg.verIndex);
    ui->appVerIdx->setValue(cfg->appOutputCfg.verIndex);
    ui->abVerIdx->setValue(cfg->abOutputCfg.verIndex);


    for(int i = 0; i < TO_PATH_MAX_NUM; i++)
    {

        if(!cfg->bootOutputCfg.toPath[i].isEmpty())
        {
            bootPath->addItem(new QListWidgetItem(cfg->bootOutputCfg.toPath[i]));
        }
        if(!cfg->appOutputCfg.toPath[i].isEmpty())
        {
            appPath->addItem(new QListWidgetItem(cfg->appOutputCfg.toPath[i]));
        }
        if(!cfg->abOutputCfg.toPath[i].isEmpty())
        {
            abPath->addItem(new QListWidgetItem(cfg->abOutputCfg.toPath[i]));
        }
    }
}


void MainWindow::logOutPutRespond(QString log)
{
    ui->logOut->append("--> "+log);
    ui->logOut->append("");
}




void MainWindow::nameChanged()
{
    QLineEdit* le = (QLineEdit*)sender();
    QString objName = le->objectName();

    int nameIdx = objName.rightRef(1).toInt()-1;

    if(objName.startsWith("le_boot"))
    {
        cfg->bootOutputCfg.name[nameIdx] = le->text();
    }
    else if(objName.startsWith("le_app"))
    {
        cfg->appOutputCfg.name[nameIdx] = le->text();
    }
    else if(objName.startsWith("le_ab"))
    {
        cfg->abOutputCfg.name[nameIdx] = le->text();
    }
    else if(objName.startsWith("le_space"))
    {
        cfg->inputCfg.space[nameIdx] = le->text();
        fillSpace();
    }
    else
    {
        //doNothing
    }
    cfg->save();

}

void MainWindow::nameConstract()
{
    QString bootName = "";
    QString appName = "";
    QString abName = "";
    //文件名组合
    for(int i = 0; i<NAME_NUM; i++)
    {
        if(cfg->bootOutputCfg.name[i]!="")
        {
            if(i<NAME_NUM-1&&cfg->bootOutputCfg.name[i+1]!="")
            {
                bootName += cfg->bootOutputCfg.name[i] + cfg->inputCfg.space[i];
            }
            else
            {
                bootName += cfg->bootOutputCfg.name[i];
            }

        }

        if(cfg->appOutputCfg.name[i]!="")
        {
            if(i<NAME_NUM-1&&cfg->appOutputCfg.name[i+1]!="")
            {
                appName += cfg->appOutputCfg.name[i] + cfg->inputCfg.space[i];
            }
            else
            {
                appName += cfg->appOutputCfg.name[i];
            }

        }

        if(cfg->abOutputCfg.name[i]!="")
        {
            if(i<NAME_NUM-1&&cfg->abOutputCfg.name[i+1]!="")
            {
                abName += cfg->abOutputCfg.name[i] + cfg->inputCfg.space[i];
            }
            else
            {
                abName += cfg->abOutputCfg.name[i];
            }

        }
    }

    QString ct = ui->cTime->text();
    if(!ct.isEmpty())
    {
        if(cfg->bootOutputCfg.dateChk)
        {
            bootName += ct ;
        }

        if(cfg->appOutputCfg.dateChk)
        {
            appName += ct ;
        }

        if(cfg->abOutputCfg.dateChk)
        {
            abName += ct ;
        }
    }

    bootName += ".bin";
    appName += ".bin";
    abName += ".bin";

    if(bootName != ui->le_bootName->text())
    {
        ui->le_bootName->setText(bootName);
        LOG_OUT("输出boot.bin名更改："+bootName);
    }

    if(appName != ui->le_appName->text())
    {
        ui->le_appName->setText(appName);
        LOG_OUT("输出app.bin名更改："+appName);
    }

    if(abName != ui->le_abName->text())
    {
        ui->le_abName->setText(abName);
        LOG_OUT("输出APP+BOOT.bin名更改："+abName);
    }

}

void MainWindow::on_le_cTimeForm_editingFinished()
{
    cfg->inputCfg.timeForm = ui->le_cTimeForm->text();
    cfg->save();
    nameChanged();
}

void MainWindow::pathSelAndSave(QString infor)
{
    pathList* path = (pathList*)sender();
    QString objName = path->objectName();
    if(objName.startsWith("boot"))
    {
        infor = "boot.bin 输出-"+infor;
        for(int i = 0; i < TO_PATH_MAX_NUM; i++)
        {
            cfg->bootOutputCfg.toPath[i]= "";
            if(i<bootPath->count()&&(bootPath->item(i)!=nullptr))
            {
                cfg->bootOutputCfg.toPath[i] = bootPath->item(i)->text();
            }
        }
    }
    else if(objName.startsWith("app"))
    {
        infor = "app.bin 输出-"+infor;
        for(int i = 0; i < TO_PATH_MAX_NUM; i++)
        {
            cfg->appOutputCfg.toPath[i]= "";
            if(i<appPath->count()&&(appPath->item(i)!=nullptr))
            {
                cfg->appOutputCfg.toPath[i] = appPath->item(i)->text();
            }
        }
    }
    else if(objName.startsWith("ab"))
    {
        infor = "app+boot.bin 输出-"+infor;
        for(int i = 0; i < TO_PATH_MAX_NUM; i++)
        {
            cfg->abOutputCfg.toPath[i]= "";
            if(i<abPath->count()&&(abPath->item(i)!=nullptr))
            {
                cfg->abOutputCfg.toPath[i] = abPath->item(i)->text();
            }
        }
    }

    cfg->save();
    LOG_OUT(infor);

}



void MainWindow::on_btn_bootPathSel_clicked()
{
    QString runPath = QCoreApplication::applicationDirPath();//获取项目的根路径
    QString binFileDirTemp = QFileDialog::getOpenFileName(this,QString("selecte bin file"),runPath,"Text Files(*.bin)",nullptr,QFileDialog::DontResolveSymlinks);
    if(!binFileDirTemp.isEmpty())
    {
        ui->le_bootPath->setText(binFileDirTemp);
        cfg->inputCfg.bootPath = binFileDirTemp;
    }
    cfg->save();
    LOG_OUT(QString("修改bin文件路径：  ")+binFileDirTemp);

}


void MainWindow::on_btn_appPathSel_clicked()
{
    QString runPath = QCoreApplication::applicationDirPath();//获取项目的根路径
    QString binFileDirTemp = QFileDialog::getOpenFileName(this,QString("selecte bin file"),runPath,"Text Files(*.bin)",nullptr,QFileDialog::DontResolveSymlinks);
    if(!binFileDirTemp.isEmpty())
    {
        ui->le_appPath->setText(binFileDirTemp);
        cfg->inputCfg.appPath = binFileDirTemp;
    }
    cfg->save();
    LOG_OUT(QString("修改bin文件路径：  ")+binFileDirTemp);
}


void MainWindow::on_chk_bootNameTime_stateChanged(int arg1)
{
    cfg->bootOutputCfg.dateChk = ui->chk_bootNameTime->isChecked();
    cfg->save();
}


void MainWindow::on_chk_appNameTime_stateChanged(int arg1)
{
    cfg->appOutputCfg.dateChk = ui->chk_appNameTime->isChecked();
    cfg->save();
}


void MainWindow::on_chk_abNameTime_stateChanged(int arg1)
{
    cfg->abOutputCfg.dateChk = ui->chk_abNameTime->isChecked();
    cfg->save();
}



bool MainWindow::reNameFileAndSend(QFile &f, QString name, QString path)
{
    if(!f.exists())
    {
        LOG_OUT("文件 不存在");
        return false;
    }

    QString dstPath = path + "/" +name;
    QString srcPath = f.fileName();
//    qDebug()<<dstPath;
//    qDebug()<<srcPath;

    QFile nf(dstPath);

    //先清除目标文件
    if(nf.exists())
    {
        QFile::remove(dstPath);
    }

    QFile::copy(srcPath,dstPath);
    if(!nf.exists())
    {
        LOG_OUT("文件 ：\n"+ srcPath + " 搬移失败 目标：\n" + path);
        return false;
    }

    return true;

}

void MainWindow::on_btn_boot_clicked()
{
    QFile file(ui->le_bootPath->text());

    if(ui->chkBootVer->isChecked())
    {
        foreach(QLineEdit* le, this->findChildren<QLineEdit*>())
        {
            QString objName = le->objectName();
            if(objName.startsWith("le_bootName")
             &&objName.rightRef(1).toInt() == ui->bootVerIdx->value())
            {
                readVerAndWrite(file,le);
                break;
            }
        }
    }

    QString name = ui->le_bootName->text();
    if(name.isEmpty())
    {
        LOG_OUT("BOOT新文件名为空");
        return;
    }

    for(int i=0; i<bootPath->count(); i++)
    {
        if((bootPath->item(i)!=nullptr)
        &&(!bootPath->item(i)->text().isEmpty())
        &&(!bootPath->item(i)->text().startsWith(NOT_USE)))
        {
            QString path = bootPath->item(i)->text();
            if(!reNameFileAndSend(file,name,path))
            {
                return;
            }
        }
    }
    LOG_OUT("BOOT文件: "+ name +" ,全路径搬移成功");
}


void MainWindow::on_btn_app_clicked()
{
    QFile file(ui->le_appPath->text());


    if(ui->chkAppVer->isChecked())
    {
        foreach(QLineEdit* le, this->findChildren<QLineEdit*>())
        {
            QString objName = le->objectName();
            if(objName.startsWith("le_appName")
             &&objName.rightRef(1).toInt() == ui->appVerIdx->value())
            {
                readVerAndWrite(file,le);
            }
        }
    }

    QString name = ui->le_appName->text();
    if(name.isEmpty())
    {
        LOG_OUT("APP新文件名为空");
        return;
    }

    for(int i=0; i<appPath->count(); i++)
    {
        if((appPath->item(i)!=nullptr)
        &&(!appPath->item(i)->text().isEmpty())
        &&(!appPath->item(i)->text().startsWith(NOT_USE)))
        {
            QString path = appPath->item(i)->text();
            if(!reNameFileAndSend(file,name,path))
            {
                return;
            }
        }
    }
    LOG_OUT("APP文件: "+ name +" ,全路径搬移成功");
}

bool MainWindow::binFileMerge(QFile &f)
{
    QString bPath = ui->le_bootPath->text();
    QString aPath = ui->le_appPath->text();
    QByteArray bAry;
    QByteArray aAry;

    QFile bF(bPath);
    QFile aF(aPath);

    QString bOffset = ui->le_bootAddr->text().remove(" ").remove("0x");
    QString aOffset = ui->le_appAddr->text().remove(" ").remove("0x");

    bool *ok=nullptr;

    long long int  bAddr = bOffset.toLongLong(ok,16);
    if(bAddr < 0x08000000
     ||bAddr > 0x08010000)
    {
        LOG_OUT("拼接检查，boot偏移地址" + QString::number(bAddr,16) + "不合规则");
        return false;
    }

    long long int aAddr = aOffset.toLongLong(ok,16);
    if(aAddr < 0x08000000
     ||aAddr > 0x08010000)
    {
        LOG_OUT("拼接检查，app偏移地址" + QString::number(aAddr,16) + "不合规则");
        return false;
    }

    LOG_OUT("boot偏移:"  + QString::number(bAddr,16) + "  app偏移:"  + QString::number(aAddr,16));

    if(!bF.exists())
    {
        LOG_OUT("拼接过程，未找到boot文件");
        return false;
    }
    if(!aF.exists())
    {
        LOG_OUT("拼接过程，未找到app文件");
        return false;
    }

    if(!bF.open(QIODevice::ReadOnly))
    {
        LOG_OUT("拼接过程，打开boot文件失败");
        return false;
    }
    if(!aF.open(QIODevice::ReadOnly))
    {
        LOG_OUT("拼接过程，打开app文件失败");
        return false;
    }

    bAry.clear();
    aAry.clear();

    bAry = bF.readAll();
    aAry = aF.readAll();

    bF.close();
    aF.close();

    if(bAry.isEmpty()||bAry.size()==0||bAry.size()>(aAddr-bAddr))
    {
        LOG_OUT("拼接过程，boot文件读取失败，或boot文件为空,或boot文件大小超过偏移量许可");
        return false;
    }
    if(aAry.isEmpty()||aAry.size()==0)
    {
        LOG_OUT("拼接过程，app文件读取失败，或者app文件为空");
        return false;
    }

    QByteArray baAry;
    baAry.clear();
    baAry.append(bAry);

    for(int i = bAry.size(); i<(aAddr-bAddr); i++ )
    {
        baAry.insert(i,char(0xFF));
    }

    baAry.append(aAry);

    if(!f.open(QIODevice::WriteOnly))
    {
        return false;
    }
    f.write(baAry);
    f.close();

    if(f.size() != aAddr - bAddr + aAry.size())
    {
        LOG_OUT("拼接过程，文件内容写入失败，文件大小: " + QString::number(f.size()) + "\n 预期文件大小：" + QString::number(aAddr - bAddr + aAry.size()));
        return false;
    }

    LOG_OUT("拼接完成，文件大小: " + QString::number(f.size()));
    return true;

}


void MainWindow::on_btn_ab_clicked()
{
    QFile appFile(ui->le_appPath->text());
    if(ui->chkABVer->isChecked())
    {
        foreach(QLineEdit* le, this->findChildren<QLineEdit*>())
        {
            QString objName = le->objectName();
            if(objName.startsWith("le_abName")
             &&objName.rightRef(1).toInt() == ui->abVerIdx->value())
            {
                readVerAndWrite(appFile,le);
            }
        }
    }

    QString name = ui->le_abName->text();
    QString runPath = QCoreApplication::applicationDirPath();//获取项目的根路径

    QString srcPath = runPath + name;
    QFile file(srcPath);

    if(file.exists())
    {
        QFile::remove(srcPath);
    }

    if(!file.open(QIODevice::WriteOnly))
    {
        LOG_OUT("拼接文件创建失败");
        return;
    }
    file.close();

    if(name.isEmpty())
    {
        LOG_OUT("拼接文件 文件名为空");
        return;
    }

    if(!binFileMerge(file))
    {
        return;
    }

    for(int i=0; i<abPath->count(); i++)
    {
        if((abPath->item(i)!=nullptr)
        &&(!abPath->item(i)->text().isEmpty())
        &&(!abPath->item(i)->text().startsWith(NOT_USE)))
        {
            QString path = abPath->item(i)->text();
            if(!reNameFileAndSend(file,name,path))
            {
                return;
            }
        }
    }
    LOG_OUT("APP文件: "+ name +" ,全路径搬移成功");

}


void MainWindow::on_le_bootAddr_editingFinished()
{
    cfg->inputCfg.bootAddr = ui->le_bootAddr->text();
    cfg->save();
}


void MainWindow::on_le_appAddr_editingFinished()
{
    cfg->inputCfg.appAddr = ui->le_appAddr->text();
    cfg->save();
}


void MainWindow::on_bootVerIdx_valueChanged(int arg1)
{
    cfg->bootOutputCfg.verIndex = ui->bootVerIdx->value();
    cfg->save();
}


void MainWindow::on_appVerIdx_valueChanged(int arg1)
{
    cfg->appOutputCfg.verIndex = ui->appVerIdx->value();
    cfg->save();
}

void MainWindow::on_abVerIdx_valueChanged(int arg1)
{
    cfg->abOutputCfg.verIndex = ui->abVerIdx->value();
    cfg->save();
}

void MainWindow::on_chkBootVer_stateChanged(int arg1)
{
    cfg->bootOutputCfg.isVer = ui->chkBootVer->isChecked();
    cfg->save();
}


void MainWindow::on_chkAppVer_stateChanged(int arg1)
{
    cfg->appOutputCfg.isVer = ui->chkAppVer->isChecked();
    cfg->save();
}

void MainWindow::on_chkABVer_stateChanged(int arg1)
{
    cfg->abOutputCfg.isVer = ui->chkABVer->isChecked();
    cfg->save();
}




bool MainWindow::readVerAndWrite(QFile &f, QLineEdit *l)
{
    if(!f.open(QIODevice::ReadOnly))
    {
        LOG_OUT("文件打开失败，无法读取版本号");
        return false;
    }

    QString ver = "";

    QByteArray ary = f.readAll();

    ver += QString::number(ary.at(ary.size()-4-1));
    ver += ".";
    ver += QString::number(ary.at(ary.size()-4-2));
    ver += ".";
    ver += QString::number(ary.at(ary.size()-4-3));
    ver += ".";
    ver += QString::number(ary.at(ary.size()-4-4));
    LOG_OUT("文件版本号：" + ver);

    l->setText(ver);
    emit l->editingFinished();
    return true;
}



void MainWindow::fillSpace(void)
{
    ui->lb_space1_1->setText(ui->le_space1->text());
    ui->lb_space1_2->setText(ui->le_space2->text());
    ui->lb_space1_3->setText(ui->le_space3->text());

    ui->lb_space2_1->setText(ui->le_space1->text());
    ui->lb_space2_2->setText(ui->le_space2->text());
    ui->lb_space2_3->setText(ui->le_space3->text());

    ui->lb_space3_1->setText(ui->le_space1->text());
    ui->lb_space3_2->setText(ui->le_space2->text());
    ui->lb_space3_3->setText(ui->le_space3->text());

}






void MainWindow::on_btn_do_clicked()
{
    if(ui->chk_doBoot->isChecked())
    {
        LOG_OUT("BOOT执行中...");
        on_btn_boot_clicked();
    }

    if(ui->chk_doApp->isChecked())
    {
        LOG_OUT("APP执行中...");
        on_btn_app_clicked();
    }

    if(ui->chk_doAB->isChecked())
    {
        LOG_OUT("拼接执行中...");
        on_btn_ab_clicked();
    }
}


void MainWindow::on_chk_doBoot_stateChanged(int arg1)
{
    cfg->inputCfg.chkDoBoot = ui->chk_doBoot->isChecked();
    cfg->save();
}


void MainWindow::on_chk_doApp_stateChanged(int arg1)
{
    cfg->inputCfg.chkDoApp = ui->chk_doApp->isChecked();
    cfg->save();
}


void MainWindow::on_chk_doAB_stateChanged(int arg1)
{
    cfg->inputCfg.chkDoAB = ui->chk_doAB->isChecked();
    cfg->save();
}
