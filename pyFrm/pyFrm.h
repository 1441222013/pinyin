#ifndef _PYFRM_H_
#define _PYFRM_H_

#include <QFrame>
#include <QButtonGroup>
#include <QPushButton>
#include <QLineEdit>
#include <QtSql>


class pyFrm:public QFrame
{
    Q_OBJECT
private:
    QLineEdit *pyEdit;
    QFrame *chFrm;
    QButtonGroup *chGroup;
    int thisHeight;
    int thisWidth;
    QStringList chList;
    QPushButton *pgDn;
    QPushButton *pgUp;
    int currentPage;
    int allPage;
    int btnWidth;
    int btnHeight;
    QString firstStr;
public:
    pyFrm(int height,QWidget *parent = 0);
    ~pyFrm();
    void setPinyin(QString py);
    void delPinyin();
    void clear();
    void clearChinese();
    void viewCh(int page);
    int getPyCount();
    QString getFirst();
private slots:
    void sendText();
    void upPage();
    void downPage();
signals:
    void send(QString);
};





#endif
