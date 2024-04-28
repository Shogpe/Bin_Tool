#include "pathlist.h"
#include "mainwindow.h"
#include "qcoreapplication.h"
#include "qdebug.h"
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QFileDialog>

#pragma execution_character_set("utf-8")
pathList::pathList(QWidget *parent)
{
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this, &QListWidget::customContextMenuRequested,this, &pathList::showListWidgetMenuSlot);

    addPath = new QAction("新增路径",this);
    delPath = new QAction("移除路径",this);

    pathEn = new QAction("启用该路径",this);
    pathDisEn = new QAction("暂时不使用该路径",this);
    menu = new QMenu();

    menu->addAction(addPath);
    menu->addAction(delPath);
    menu->addSeparator();
    menu->addAction(pathEn);
    menu->addAction(pathDisEn);

    connect(addPath, &QAction::triggered, this, &pathList::add);
    connect(delPath, &QAction::triggered, this, &pathList::del);
    connect(pathEn, &QAction::triggered, this, &pathList::enable);
    connect(pathDisEn, &QAction::triggered, this, &pathList::disable);
}


void pathList::showListWidgetMenuSlot(QPoint pos)
{
    menu->exec(QCursor::pos());
}



void pathList::add(bool checked = false)
{
    QString infor;

    QString runPath = QCoreApplication::applicationDirPath();//获取项目的根路径
    QString binFileDirTemp = QFileDialog::getExistingDirectory();
    if(this->count()<=TO_PATH_MAX_NUM)
    {
        this->addItem(binFileDirTemp);
        infor = "新增路径：" + binFileDirTemp;
        emit pathRefreash(infor);
    }


}

void pathList::del(bool checked = false)
{
    QString infor;

    if(this->currentItem() != nullptr)
    {
        infor = "移除路径：" + this->currentItem()->text();
        this->removeItemWidget(this->currentItem());
        delete this->currentItem();

        emit pathRefreash(infor);
    }


}

void pathList::enable(bool checked = false)
{
    QString infor;

    if(this->currentItem() != nullptr)
    {
        if(this->currentItem()->text().startsWith(QString(NOT_USE)))
        {
            this->currentItem()->setText(this->currentItem()->text().remove(NOT_USE));
            infor = "启用路径：" + this->currentItem()->text();

            emit pathRefreash(infor);
        }

    }


}

void pathList::disable(bool checked = false)
{
    QString infor;

    if(this->currentItem() != nullptr)
    {
        if(!this->currentItem()->text().startsWith(QString(NOT_USE)))
        {
            infor = "禁用路径：" + this->currentItem()->text();
            this->currentItem()->setText(NOT_USE + this->currentItem()->text());

            emit pathRefreash(infor);
        }

    }


}

