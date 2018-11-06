#include "pyFrm.h"

pyFrm::pyFrm(int height,QWidget *parent):QFrame(parent),thisHeight(height)
{
    setObjectName("KBBK");
    firstStr = "";
    thisWidth = parent->width();
    pyEdit = new QLineEdit(this);
    pyEdit->setEnabled(false);
    pyEdit->setGeometry(0,0,thisWidth/10,thisHeight);
    pyEdit->setObjectName("PYEDIT");
    pyEdit->setAlignment(Qt::AlignCenter);
    //pyEdit->setStyleSheet("QLineEdit:disabled{color:black;font-size:30px;}");

    

    chFrm = new QFrame(this);
    chFrm->setGeometry(thisWidth/10,0,thisWidth/10*8,thisHeight);

    btnWidth = (thisWidth/10*8)/12;
    btnHeight = thisHeight;

    int upDownWidth = thisWidth/10/2;
    pgUp = new QPushButton("<",this);
    pgUp->setGeometry(thisWidth/10*9,0,upDownWidth,thisHeight);
    pgUp->setFocusPolicy(Qt::NoFocus);
    pgUp->setObjectName("CNBTN");
    pgUp->setFlat(true);
    connect(pgUp,SIGNAL(clicked()),this,SLOT(upPage()));


    pgDn = new QPushButton(">",this);
    pgDn->setGeometry(thisWidth/10*9+upDownWidth,0,upDownWidth,thisHeight);
    pgDn->setFocusPolicy(Qt::NoFocus);
    pgDn->setObjectName("CNBTN");
    pgDn->setFlat(true);
    connect(pgDn,SIGNAL(clicked()),this,SLOT(downPage()));

    QSqlDatabase DbConn;
    DbConn = QSqlDatabase::addDatabase("QSQLITE", "py");
    DbConn.setDatabaseName("py.db");
    qDebug()<<"sql:"<<DbConn.open();

    currentPage = 1;
    allPage = 1;
    
}

int pyFrm::getPyCount()
{
    return pyEdit->text().size();
}

pyFrm::~pyFrm()
{
}

void pyFrm::setPinyin(QString py)
{
    pyEdit->insert(py);
    QSqlQuery query(QSqlDatabase::database("py"));
    QString sql = QString("select word from pinyin where pinyin like ?");
    query.prepare(sql);
    QString str = pyEdit->text()+"%";
    qDebug()<<str;
	query.addBindValue(str);
    query.exec();
    QString result;
    while(query.next()) 
    {
        result += query.value(0).toString() + " ";
    }
    qDebug()<<result;
    chList = result.split(" ");
    viewCh(1);
}
void pyFrm::delPinyin()
{
    pyEdit->backspace();
    if(pyEdit->text().isEmpty())
    {
        clear();
        return;
    }
    QSqlQuery query(QSqlDatabase::database("py"));
    QString sql = QString("select word from pinyin where pinyin like ?");
    query.prepare(sql);
    QString str = pyEdit->text()+"%";
    qDebug()<<str;
	query.addBindValue(str);
    query.exec();
    QString result;
    while(query.next()) 
    {
        result += query.value(0).toString() + " ";
    }
    qDebug()<<result;
    chList = result.split(" ");
    viewCh(1);
}

void pyFrm::clear()
{
    firstStr = "";
    pyEdit->setText("");
    clearChinese();
    chList.clear();
}

void pyFrm::clearChinese()
{
    if(chFrm != NULL)
    {
        delete chFrm;
        chFrm = NULL;
    }
    chFrm = new QFrame(this);
    chFrm->setGeometry(thisWidth/10,0,thisWidth/10*8,thisHeight);
    chFrm->show();
}

void pyFrm::viewCh(int page)
{
    currentPage = page;
    clearChinese();
    int count = 0;
    int chInt = 0;
    chInt = (page-1)*12;
    firstStr = "";
    int first = 0;
    for(int i = ((page-1)*12) ; i < (page*12) ; ++i)
    {
        if(chList.count() <= chInt)       //数据小于7个
        {
            return;
        }
        while(chList.at(chInt).size() > 2)
        {
            //++chInt;
            chList.removeAt(chInt);
            qDebug()<<chList.count();
    
            if(chList.count() <= chInt)       //数据小于7个
            {
                return;
            }
        }
            
        if(chList.count() <= chInt)       //数据小于7个
        {
            return;
        }
        QPushButton *btn = new QPushButton(chList.at(chInt),chFrm);
        if(first == 0)
            firstStr = chList.at(chInt);
        btn->setGeometry(btnWidth*count,0,btnWidth,btnHeight);
        connect(btn,SIGNAL(clicked()),this,SLOT(sendText()));
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setObjectName("CNBTN");
        btn->setFlat(true);
        btn->show();
        ++count;
        ++chInt;
        ++first;
    }
}

void pyFrm::sendText()
{
    QPushButton *btn;
    btn = (QPushButton *)sender();
    qDebug()<<btn->text();
    send(btn->text());
    clear();
}

void pyFrm::upPage()
{
    if(currentPage <= 1)
        return;
    else
        --currentPage;
    qDebug()<<currentPage<<allPage;
    viewCh(currentPage);
}
void pyFrm::downPage()
{
    allPage = chList.count()/10;
    if(chList.count() % 10 > 0)
        ++allPage;
    if(allPage <= currentPage)
        return;
    else
        ++currentPage;
    qDebug()<<currentPage<<allPage;
    viewCh(currentPage);
}

QString pyFrm::getFirst()
{
    if(firstStr == "")
    {
        if(pyEdit->text().isEmpty())
            return " ";
        else
            return pyEdit->text();
    }
    else
        return firstStr;
}

