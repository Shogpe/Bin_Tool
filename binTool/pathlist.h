#ifndef PATHLIST_H
#define PATHLIST_H

#include "qmenu.h"


#include "qlistwidget.h"
#include "qobject.h"
#include "qobjectdefs.h"

#define NOT_USE "[未启用]"
#define TO_PATH_MAX_NUM 16
class pathList:public QListWidget
{
    Q_OBJECT
public:
    pathList(QWidget *parent = 0);
private slots:
    void showListWidgetMenuSlot(QPoint pos);


public:
    QMenu* menu = new QMenu();

    QAction* addPath;
    QAction* delPath;

    QAction* pathEn;
    QAction* pathDisEn;

public:
    void add(bool f);
    void del(bool f);
    void enable(bool f);
    void disable(bool f);

signals:
    void pathRefreash(QString infor);
};

#endif // PATHLIST_H
