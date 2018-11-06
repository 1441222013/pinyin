#ifndef _SOFTKEYBOARD_H_
#define _SOFTKEYBOARD_H_

#include <QFrame>
#include <QDebug>
#include <QPushButton>
#include <QtSql>
#include <QLineEdit>
#include <QButtonGroup>
#include <QTextCursor>
#include <QTextBlock>
#include <QFile>

#include "pyFrm/pyFrm.h"


class softKeyBoard:public QFrame
{
	Q_OBJECT
public:
	softKeyBoard(QWidget *parent = 0,QString pos = "INSIDE");
	~softKeyBoard();
	QStringList numList;
	QStringList qweList;
	QStringList asdList;
	QStringList zxcList;
	QStringList markList1;
	QStringList markList2;
	//QStringList basicList;
	QString pressKey;
	QString thisPos;
	bool editFocus;
	QLineEdit *edit;

	void initFace();
	
private:
	QPushButton *delBtn;
	QPushButton *capBtn;	//大写
	QPushButton *capRBtn;	
	QPushButton *hideBtn;
	QPushButton *spaceBtn;
	QPushButton *enterBtn;
	QPushButton *numBtn;
	QPushButton *pyBtn;
	QPushButton *numRBtn;

	QButtonGroup *abcGroup;
	QButtonGroup *zxcGroup;
	QButtonGroup *qweGroup;

	int pWidth;
	int pHeight;

	int keyWidth;
	int keyHeight;

	int kbHeight;
	int kbWidth;

	QString model;			//拼音   英文
	bool numModel;			//数字模式
	
	pyFrm *topFrm;

	bool caps;

	void toNum();			//数字转换
	void toCaps();			//大写转换
	void toABC();			//小写转换
	void loadStyleSheet(QString QSSName);
private slots:
	void focusChanged(QWidget *oldWidget, QWidget *nowWidget);
	bool eventFilter(QObject *watched, QEvent *event);
	void abcClick(int i);
	void qweClick(int i);
	void zxcClick(int i);
	void delClick();
	void capClick();
	void enterClick();
	void numClick();
	void changeModel();
	void send(QString chFont);
	void spaceClick();
	void focusObjectChanged(QObject * focusObject);
signals:
};

#endif
