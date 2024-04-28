#ifndef SAVECFG_H
#define SAVECFG_H
#include "QString"
#include "qobject.h"
#include "qobjectdefs.h"
#include "pathlist.h"

#define NAME_NUM        4

typedef struct
{
    QString name[NAME_NUM];
    bool dateChk;
    QString toPath[TO_PATH_MAX_NUM];
    bool isPath[TO_PATH_MAX_NUM];

    int  verIndex;
    bool isVer;
}outputCfg_t;

typedef struct
{
    QString appPath;
    QString appAddr;
    QString bootPath;
    QString bootAddr;
    QString timeForm;
    QString space[NAME_NUM-1];
    bool    chkDoApp;
    bool    chkDoBoot;
    bool    chkDoAB;
}inputCfg_t;

class savecfg :public QObject
{
    Q_OBJECT
public:
    savecfg(QObject *parent = 0);
    outputCfg_t appOutputCfg;
    outputCfg_t bootOutputCfg;
    outputCfg_t abOutputCfg;

    inputCfg_t  inputCfg;
signals:
    void nameRefreashed(void);

public:
    void load();
    void save();

};

#endif // SAVECFG_H
