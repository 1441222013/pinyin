#include "softKeyBoard.h"

softKeyBoard::softKeyBoard(QWidget *parent,QString pos):QFrame(parent),thisPos(pos)
{	
	setObjectName("KBBK");
	loadStyleSheet("keyBoard.qss");
	setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint|Qt::X11BypassWindowManagerHint);
	activateWindow();
	editFocus = false;
	caps = false;
	// QSqlDatabase DbConn;
    // DbConn = QSqlDatabase::addDatabase("QSQLITE", "py");
    // DbConn.setDatabaseName(qApp->applicationDirPath() + "/py.db");
    // DbConn.open();
	qApp->instance();

	setWindowFlags(Qt::FramelessWindowHint);

	numModel = false;
	model = "ENG";			//英语模式
	
	pWidth = parent->width();
	pHeight = parent->height();
	keyWidth = ((pWidth-(6*12)) / 11);
	keyHeight = keyWidth;
	if(keyHeight > 40)
		keyHeight = 40;
	
	kbHeight = keyHeight*5+5*6;
	kbWidth = pWidth;

	int hPos = pHeight - kbHeight;

	setGeometry(0,hPos,kbWidth,kbHeight);
	//setStyleSheet("border:1px solid red");

	abcGroup = new QButtonGroup(this);
	zxcGroup = new QButtonGroup(this);
	qweGroup = new QButtonGroup(this);

	qDebug()<<keyHeight<<keyWidth;
	qApp->installEventFilter(this);
	connect(qApp, SIGNAL(focusChanged(QWidget *, QWidget *)),this, SLOT(focusChanged(QWidget *, QWidget *)));
	connect(qApp, SIGNAL(focusObjectChanged(QObject * )),this, SLOT(focusObjectChanged(QObject * )));
	qweList << "q" << "w" << "e" << "r" <<"t" << "y" << "u" << "i" << "o" << "p";
	asdList << "a" << "s" << "d" << "f" << "g" << "h" << "j" << "k" << "l";
	zxcList << "z" << "x" << "c" << "v" << "b" << "n" << "m" << "," << ".";
	numList << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "0" ;
	markList1 << "@" << "#" << "$" << "/" << "(" << ")" << "\"" << "\"" << "\\" ;
	markList2 << "%" << "-" << "_" << "~" << "?" << "!" << ":" << ";" << "*" ;
	initFace();

}

void softKeyBoard::focusObjectChanged(QObject * focusObject)
{
	if(focusObject == 0)
		return;
	if(focusObject->inherits("QLineEdit"))
		qDebug()<<"focusObjectChanged"<<focusObject;
	else
		hide();
}


softKeyBoard::~softKeyBoard()
{

}

void softKeyBoard::initFace()
{
	topFrm = new pyFrm(keyHeight,this);
	topFrm->setGeometry(0,0,kbWidth,keyHeight);
	connect(topFrm,SIGNAL(send(QString)),this,SLOT(send(QString)));
	//topFrm->setStyleSheet("border:1px solid black");

	for(int i = 0;i<10;++i)
	{
		QPushButton *btn = new QPushButton(qweList.at(i),this);
		btn->setGeometry(6+(keyWidth+6)*i,keyHeight+6,keyWidth,keyHeight);
		btn->setFocusPolicy(Qt::NoFocus);
		btn->setObjectName("BASICBTN");
		qweGroup->addButton(btn,i);
	}
	for(int i = 0;i<9;++i)
	{
		QPushButton *btn = new QPushButton(asdList.at(i),this);
		btn->setGeometry(6+(keyWidth/2)+(keyWidth+6)*i,(keyHeight+6)*2,keyWidth,keyHeight);
		btn->setFocusPolicy(Qt::NoFocus);
		btn->setObjectName("BASICBTN");
		abcGroup->addButton(btn,i);
	}
	for(int i = 0;i<9;++i)
	{
		QPushButton *btn = new QPushButton(zxcList.at(i),this);
		btn->setGeometry(6+keyWidth+6+(keyWidth+6)*i,(keyHeight+6)*3,keyWidth,keyHeight);
		btn->setFocusPolicy(Qt::NoFocus);
		btn->setObjectName("BASICBTN");
		zxcGroup->addButton(btn,i);
	}

	delBtn = new QPushButton(this);
	delBtn->setObjectName("DELBTN");
	delBtn->setGeometry(6+(keyWidth+6)*10,keyHeight+6,keyWidth,keyHeight);
	delBtn->setFocusPolicy(Qt::NoFocus);

	capBtn = new QPushButton(this);
	capBtn->setObjectName("CAPBTN");
	capBtn->setGeometry(6,(keyHeight+6)*3,keyWidth,keyHeight);
	capBtn->setFocusPolicy(Qt::NoFocus);

	capRBtn = new QPushButton(this);
	capRBtn->setObjectName("CAPBTN");
	capRBtn->setGeometry(6+keyWidth+6+(keyWidth+6)*9,(keyHeight+6)*3,kbWidth - 6 -(6+keyWidth+6+(keyWidth+6)*9),keyHeight);

	enterBtn = new QPushButton(this);		//中文的时候用
	enterBtn->setObjectName("ENTERBTN");
	enterBtn->setGeometry(6+(keyWidth/2)+(keyWidth+6)*9,(keyHeight+6)*2,kbWidth - 6 -(6+(keyWidth/2)+(keyWidth+6)*9),keyHeight);
	enterBtn->setFocusPolicy(Qt::NoFocus);

	numBtn = new QPushButton(".?123",this);
	numBtn->setObjectName("TOOLBTN");
	numBtn->setGeometry(6,(keyHeight+6)*4,keyWidth+keyWidth/2,keyHeight);
	numBtn->setFocusPolicy(Qt::NoFocus);

	pyBtn = new QPushButton("拼音",this);
	pyBtn->setObjectName("TOOLBTN");
	pyBtn->setGeometry(12+keyWidth+keyWidth/2,(keyHeight+6)*4,keyWidth+keyWidth/2,keyHeight);
	pyBtn->setFocusPolicy(Qt::NoFocus);

	spaceBtn = new QPushButton(this);
	spaceBtn->setObjectName("BASICBTN");
	spaceBtn->setGeometry(24+keyWidth*3,(keyHeight+6)*4,5*keyWidth+24,keyHeight);
	spaceBtn->setFocusPolicy(Qt::NoFocus);

	numRBtn = new QPushButton(".?123",this);
	numRBtn->setObjectName("TOOLBTN");
	numRBtn->setGeometry(keyWidth*8+54,(keyHeight+6)*4,keyWidth+keyWidth/2,keyHeight);
	numRBtn->setFocusPolicy(Qt::NoFocus);

	hideBtn = new QPushButton(this);
	hideBtn->setObjectName("HIDEBTN");
	hideBtn->setGeometry(keyWidth*9+keyWidth/2+60,(keyHeight+6)*4,keyWidth+keyWidth/2,keyHeight);
	hideBtn->setFocusPolicy(Qt::NoFocus);

	connect(qweGroup,SIGNAL(buttonClicked(int)),this,SLOT(qweClick(int)));
	connect(abcGroup,SIGNAL(buttonClicked(int)),this,SLOT(abcClick(int)));
	connect(zxcGroup,SIGNAL(buttonClicked(int)),this,SLOT(zxcClick(int)));
	connect(delBtn,SIGNAL(clicked()),this,SLOT(delClick()));
	connect(capBtn,SIGNAL(clicked()),this,SLOT(capClick()));
	connect(capRBtn,SIGNAL(clicked()),this,SLOT(capClick()));
	connect(numBtn,SIGNAL(clicked()),this,SLOT(numClick()));
	connect(pyBtn,SIGNAL(clicked()),this,SLOT(changeModel()));
	connect(numRBtn,SIGNAL(clicked()),this,SLOT(numClick()));
	connect(hideBtn,SIGNAL(clicked()),this,SLOT(hide()));
	connect(spaceBtn,SIGNAL(clicked()),this,SLOT(spaceClick()));
	connect(enterBtn,SIGNAL(clicked()),this,SLOT(enterClick()));
}


void softKeyBoard::spaceClick()
{
	if(editFocus)
	{
		if((model != "pinyin")||numModel)
			edit->insert(" ");
		else
		{
			QString str = topFrm->getFirst();
			edit->insert(str);
			topFrm->clear();
		}
	}
}




void softKeyBoard::numClick()
{
	topFrm->clear();
	if(!numModel)
	{
		toNum();
		QString str,str1;
		if(model == "pinyin")
		{
			str = "拼音";
			str1 = "拼音";
		}
		else
		{
			str = "ENG";
			str1 = "ABC";
		}
			
		numBtn->setText(str);
		numRBtn->setText(str);
		pyBtn->setText(str1);
	}
	else
	{
		if(caps)
			toCaps();
		else
			toABC();
		numBtn->setText(".?123");
		numRBtn->setText(".?123");
		QString str;
		if(model == "pinyin")
		{
			str = "ABC";
		}
		else
		{
			str = "拼音";
		}
		pyBtn->setText(str);
	}
}

void softKeyBoard::capClick()
{
	if(numModel)
		return;
	else
	{
		if(caps)
			toABC();
		else
			toCaps();
	}
}

void softKeyBoard::changeModel()
{
	topFrm->clear();
	if(!numModel)
	{
		if(model == "pinyin")
		{
			model = "ENG";
			pyBtn->setText("拼音");
		}
		else
		{
			model = "pinyin";
			pyBtn->setText("ABC");
		}
	}
	else
	{
		if(caps)
			toCaps();
		else
			toABC();

		numBtn->setText(".?123");
		numRBtn->setText(".?123");
	}
}

void softKeyBoard::abcClick(int i)
{
	if(editFocus)
	{
		if(numModel)
		{
			edit->insert(markList1.at(i));
			return;
		}
		if(model == "pinyin")
			topFrm->setPinyin(asdList.at(i));
		else
		{
			if(caps)
				edit-> insert(asdList.at(i).toUpper());
			else
				edit-> insert(asdList.at(i));
		}
	}
	//position()
}
void softKeyBoard::qweClick(int i)
{
	if(editFocus)
	{
		if(numModel)
		{
			edit->insert(numList.at(i));
			return;
		}
		if(model == "pinyin")
			topFrm->setPinyin(qweList.at(i));
		else
		{
			if(caps)
				edit-> insert(qweList.at(i).toUpper());
			else
				edit-> insert(qweList.at(i));
		}
	}
}
void softKeyBoard::zxcClick(int i)
{
	if(editFocus)
	{
		if(numModel)
		{
			edit->insert(markList2.at(i));
			return;
		}
		if(model == "pinyin")
		{
			if((i == 8)||(i == 7))
			{
				edit-> insert(zxcList.at(i));
			}
			else
				topFrm->setPinyin(zxcList.at(i));
		}
			
		else
		{
			if(caps)
				edit-> insert(zxcList.at(i).toUpper());
			else
				edit-> insert(zxcList.at(i));
		}
	}
}

void softKeyBoard::delClick()
{
	if(editFocus)
	{
		if(model == "pinyin")
		{
			if(topFrm->getPyCount() != 0)
				topFrm->delPinyin();
			else
				edit->backspace();
		}
		else
		edit->backspace();
	}
}


void softKeyBoard::focusChanged(QWidget *oldWidget, QWidget *nowWidget)
{
	if(nowWidget == 0 || this->isAncestorOf(nowWidget))
		return;
	//qDebug()<<oldWidget<<nowWidget;
	if(nowWidget->inherits("QLineEdit"))
	{
		editFocus = true;
		edit = (QLineEdit*)nowWidget;
		qDebug()<<"lineEdit";
		qDebug()<<nowWidget->objectName();
		if(edit->isEnabled())
		{
			raise();
			show();
		}
			
		if(nowWidget->property("NOTUSE").toBool())
				hide();
	}
	else
	{
		editFocus = false;
		edit = NULL;
		topFrm->clear();
		hide();
	}
}

bool softKeyBoard::eventFilter(QObject *watched, QEvent *event)
{
	if(event->type() == QEvent::MouseButtonPress)
	{	
		//qDebug()<<watched;
		if(watched->inherits("QLineEdit"))
		{
			qDebug()<<"1--";
			edit = (QLineEdit*)watched;
			qDebug()<<watched->objectName();
			if(edit->isEnabled())
			{
				raise();	
				show();
			}
			if(watched->property("NOTUSE").toBool())
				hide();
		}
		else
		{

		}
	}
	else if(event->type() == QEvent::Close)
	{
		qDebug()<<watched;
	}

}

void softKeyBoard::enterClick()
{
	if(editFocus)
	{
		if((model == "pinyin")&&!numModel)
		{
			QString str = topFrm->getFirst();
			if(str == " ")
				return;
			edit->insert(str);
			topFrm->clear();
		}
	}
}



void softKeyBoard::toNum()			//数字转换
{
	numModel = true;
	for(int i = 0;i<9;++i)
	{
		qweGroup->button(i)->setText(numList.at(i));
		abcGroup->button(i)->setText(markList1.at(i));
		zxcGroup->button(i)->setText(markList2.at(i));
	}
	qweGroup->button(9)->setText(numList.at(9));
}	

void softKeyBoard::toCaps()			//大写转换
{
	caps = true;
	numModel = false;
	for(int i = 0;i<9;++i)
	{
		qweGroup->button(i)->setText(qweList.at(i).toUpper());
		abcGroup->button(i)->setText(asdList.at(i).toUpper());
		zxcGroup->button(i)->setText(zxcList.at(i).toUpper());
	}
	qweGroup->button(9)->setText(qweList.at(9).toUpper());

}	

void softKeyBoard::toABC()			//小写转换
{
	caps = false;
	numModel = false;
	for(int i = 0;i<9;++i)
	{
		qweGroup->button(i)->setText(qweList.at(i));
		abcGroup->button(i)->setText(asdList.at(i));
		zxcGroup->button(i)->setText(zxcList.at(i));
	}
	qweGroup->button(9)->setText(qweList.at(9));

}	

void softKeyBoard::send(QString chFont)
{
	if(editFocus)
		edit-> insert(chFont);
}

void softKeyBoard::loadStyleSheet(QString QSSName)
{
	QFile styleSheet(QSSName);
	if (!styleSheet.open(QIODevice::ReadOnly))
	{
		qWarning("无法打开样式配置文件！");  
   		return;  
	}
	setStyleSheet(styleSheet.readAll());
	styleSheet.close();
}
