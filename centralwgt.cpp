#include "centralwgt.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QList>
#include "gdatafactory.h"
#include <QTimerEvent>
#include "QsLog.h"
#include "aoi_zebraprinterdll.h"
#include <QMessageBox>
#include <QPushButton>
#include <QEventLoop>
#include "aoi_zebraprinterdll.h"
CentralWgt* CentralWgt::m_pInstance = nullptr;

CentralWgt::CentralWgt(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* vAll = new QVBoxLayout();

    m_pLineEditExInfoA = new QLineEdit();
    m_pLineEditCurrentInfoA = new QLineEdit();
    m_pLineEditExWidthA = new QLineEdit();
    m_pLineEditCurrentWidthA = new QLineEdit();

    m_pLineEditExInfoB = new QLineEdit();
    m_pLineEditCurrentInfoB = new QLineEdit();
    m_pLineEditExWidthB = new QLineEdit();
    m_pLineEditCurrentWidthB = new QLineEdit();

    m_pTextEditLogInfo = new QTextEdit();
    m_pTextEditLogInfo->setReadOnly(true);
    m_pTextEditLogInfo->document()->setMaximumBlockCount(1000);
    m_pTextEditLogInfo->setStyleSheet("border:1px solid rgba(0,255,0,100);color:rgb(0,0,0);background-color:rgb(0,0,0);font: 10pt 'LiSu'");

    QList<QLineEdit*> lst;
    lst<<m_pLineEditExInfoA<<m_pLineEditCurrentInfoA<<m_pLineEditExWidthA<<m_pLineEditCurrentWidthA<<
         m_pLineEditExInfoB<<m_pLineEditCurrentInfoB<<m_pLineEditExWidthB<<m_pLineEditCurrentWidthB;
    for (int i=0;i<lst.size();i++) {
        lst[i]->setStyleSheet("QLineEdit{border:1px solid rgba(0,255,0,100);font-family:LiSu;font-size:20px;"
                                          "color:rgb(0,255,0);background-color:rgba(0,0,0,0);font-weight:bold;}"
                                          "QLineEdit:hover{border:2px solid rgba(0,255,0,150);}");
//        lst[i]->setText(u8"test");
        lst[i]->setAlignment(Qt::AlignCenter);
        lst[i]->setReadOnly(true);
    }

    QList<QLabel*> lstLabel;
    QHBoxLayout* hBox1 = new QHBoxLayout();
    QGroupBox* grpBox1 = new QGroupBox(u8"A轨产品信息");
    grpBox1->setStyleSheet("QGroupBox{border:1px solid rgba(0,255,0,100);color:rgb(0,255,0);font-family:LiSu;}");
    QLabel* labelExOrder = new QLabel(u8"  前一制令单:");
    QLabel* labelCurrentOrder = new QLabel(u8"  当前制令单:");
    hBox1->addWidget(labelExOrder);
    hBox1->addWidget(m_pLineEditExInfoA);
    hBox1->addWidget(labelCurrentOrder);
    hBox1->addWidget(m_pLineEditCurrentInfoA);

    QHBoxLayout* hBox2 = new QHBoxLayout();
    QLabel* labelExWidth = new QLabel(u8"前一产品宽度:");
    QLabel* labelCurrentWidth = new QLabel(u8"当前产品宽度:");
    hBox2->addWidget(labelExWidth);
    hBox2->addWidget(m_pLineEditExWidthA);
    hBox2->addWidget(labelCurrentWidth);
    hBox2->addWidget(m_pLineEditCurrentWidthA);

    QVBoxLayout* vBox1 = new QVBoxLayout();
    vBox1->addLayout(hBox1);
    vBox1->addLayout(hBox2);
    grpBox1->setLayout(vBox1);

    QHBoxLayout* hBox4 = new QHBoxLayout();
    QGroupBox* grpBox3 = new QGroupBox(u8"B轨产品信息");
    grpBox3->setStyleSheet("QGroupBox{border:1px solid rgba(0,255,0,100);color:rgb(0,255,0);font-family:LiSu;}");
    QLabel* labelExOrderB = new QLabel(u8"  前一制令单:");
    QLabel* labelCurrentOrderB = new QLabel(u8"  当前制令单:");
    hBox4->addWidget(labelExOrderB);
    hBox4->addWidget(m_pLineEditExInfoB);
    hBox4->addWidget(labelCurrentOrderB);
    hBox4->addWidget(m_pLineEditCurrentInfoB);

    QHBoxLayout* hBox5 = new QHBoxLayout();
    QLabel* labelExWidthB = new QLabel(u8"前一产品宽度:");
    QLabel* labelCurrentWidthB = new QLabel(u8"当前产品宽度:");
    hBox5->addWidget(labelExWidthB);
    hBox5->addWidget(m_pLineEditExWidthB);
    hBox5->addWidget(labelCurrentWidthB);
    hBox5->addWidget(m_pLineEditCurrentWidthB);

    QVBoxLayout* vBox2 = new QVBoxLayout();
    vBox2->addLayout(hBox4);
    vBox2->addLayout(hBox5);
    grpBox3->setLayout(vBox2);

    QGroupBox* grpBox2 = new QGroupBox(u8"日志信息");
    grpBox2->setStyleSheet("QGroupBox{border:1px solid rgba(0,255,0,100);color:rgb(0,255,0);font-family:LiSu;}");
    QVBoxLayout* vBox3 = new QVBoxLayout();
    vBox3->addWidget(m_pTextEditLogInfo);
    QPushButton* btnRead = new QPushButton();
    btnRead->setText(u8"读测试");
    QPushButton* btnWrite = new QPushButton();
    btnWrite->setText(u8"写测试");
    connect(btnRead,SIGNAL(clicked()),this,SLOT(slot_btn_read_click()));
    connect(btnWrite,SIGNAL(clicked()),this,SLOT(slot_btn_write_click()));
//    vBox3->addWidget(btnRead);
//    vBox3->addWidget(btnWrite);
    grpBox2->setLayout(vBox3);

    lstLabel<<labelExOrder<<labelExWidth<<labelExOrderB<<labelExWidthB<<labelCurrentOrder<<
              labelCurrentWidth<<labelCurrentOrderB<<labelCurrentWidthB;
    for(int i=0;i<lstLabel.length();i++)
    {
        lstLabel[i]->setStyleSheet("color:rgb(0,255,0)");
    }

    vAll->addWidget(GDataFactory::get_head_info_wgt());
    vAll->addWidget(grpBox1);
    vAll->addWidget(grpBox3);
    vAll->addWidget(grpBox2);
    vAll->setStretch(0,2);
    vAll->setStretch(0,1);
    vAll->setStretch(0,3);
    this->setLayout(vAll);

    GDataFactory::get_plc_obj()->connectToPLC(GDataFactory::get_instance()->get_config_para("COM_PORT"),
                                              GDataFactory::get_instance()->get_config_para("COM_BARD_RATE"));

//    m_pTimerID = startTimer(1000);
    m_pTimerMesID = startTimer(5000);

    m_pReadThread = new DemoAOIThread();
    connect(m_pReadThread,SIGNAL(signal_plc_info_to_ui(const QByteArray&)),this,SLOT(slot_rev_from_plc(const QByteArray&)));
    GDataFactory::get_plc_obj()->moveToThread(m_pReadThread);
    m_pReadThread->start();
}

void CentralWgt::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    this->m_pLineEditExInfoA->setText(GDataFactory::get_instance()->get_config_para("EX_ORDER_A"));
    m_pExOrderIdA = GDataFactory::get_instance()->get_config_para("EX_ORDER_A");
    this->m_pLineEditExWidthA->setText(GDataFactory::get_instance()->get_config_para("EX_WIDTH_A"));
    m_pExOrderWidthA = GDataFactory::get_instance()->get_config_para("EX_WIDTH_A");
    this->m_pLineEditExInfoB->setText(GDataFactory::get_instance()->get_config_para("EX_ORDER_B"));
    m_pExOrderIdB = GDataFactory::get_instance()->get_config_para("EX_ORDER_B");
    this->m_pLineEditExWidthB->setText(GDataFactory::get_instance()->get_config_para("EX_WIDTH_B"));
    m_pExOrderWidthB = GDataFactory::get_instance()->get_config_para("EX_WIDTH_B");
}

void CentralWgt::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_pTimerID)
    {
        //query PLC info
        QLOG_INFO()<<GDataFactory::get_plc_obj()->readDataToPLC(PATH_READ,8);
    }

    if(event->timerId() == m_pTimerMesID)
    {
        //query Mes info
//        this->m_pLineEditExInfoA->setText(m_pExOrderIdA);
//        this->m_pLineEditExWidthA->setText(m_pExOrderWidthA);
//        this->m_pLineEditExInfoB->setText(m_pExOrderIdB);
//        this->m_pLineEditExWidthB->setText(m_pExOrderWidthB);
//        GDataFactory::get_mes_process()->exec_http_post_zd();
    }
}

void CentralWgt::slot_rev_logs(const QString info)
{
    m_pTextEditLogInfo->append(info);
}

void CentralWgt::slot_btn_read_click()
{
//    QByteArray testByteArray;

//    testByteArray = GDataFactory::get_instance()->alter_order_of_byte_array( GDataFactory::get_plc_obj()->readDataToPLC("32200",6));
//    QLOG_INFO()<<"result"<<testByteArray;

    QByteArray m_data;
    m_data.append(static_cast<char>(0x01));
    m_data.append(static_cast<char>(0x00));

    if(GDataFactory::get_plc_obj()->writeDataToPLC("32210",1,m_data))
        QLOG_INFO()<<"true";
    else {
        QLOG_WARN()<<"error";
    }
}

void CentralWgt::slot_btn_write_click()
{
    QByteArray m_data;
//    m_data.append(static_cast<char>(0x01));
//    m_data.append(static_cast<char>(0x00));

//    if(GDataFactory::get_plc_obj()->writeDataToPLC("32211",1,m_data))
//        QLOG_INFO()<<"true";
//    else {
//        QLOG_WARN()<<"error";
//    }
    m_data.append(static_cast<char>(0xBC));
    m_data.append(static_cast<char>(0x07));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));
    m_data.append(static_cast<char>(0x00));

    if(GDataFactory::get_plc_obj()->writeDataToPLC("32576",4,m_data))
        QLOG_INFO()<<"true";
    else {
        QLOG_WARN()<<"error";
    }
}

void CentralWgt::slot_btn_start_monitor()
{

}

void CentralWgt::slot_btn_stop_monitor()
{

}

void CentralWgt::slot_rev_from_plc(const QByteArray &data)
{
    QLOG_WARN()<<"rev plc data is:"<<GDataFactory::get_instance()->bytes_to_str(data);

    if(data.length() == 12)
        ;
    else {
        QLOG_WARN()<<"the length of data from PLC is not correct";
        m_pReadThread->set_start_sign(false);
        return;
    }
    QByteArray tmpData = data;

    //the first track out sign
    int firstOutSign;
    QByteArray array1;
    array1.append(tmpData[0]);
    array1.append(tmpData[1]);
    firstOutSign = array1.toHex().toInt(nullptr, 16);
    if(firstOutSign == 1)
    {
        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        GDataFactory::get_plc_obj()->writeDataToPLC("32200",1,clearData);
        QLOG_WARN()<<u8"1轨出框";
        QLOG_WARN()<<GDataFactory::get_instance()->bytes_to_str(tmpData);
        //the quantity of the first track
        int firstQuantity;
        QByteArray arrayQuantity1;
        arrayQuantity1.append(tmpData[2]);
        arrayQuantity1.append(tmpData[3]);
        firstQuantity = arrayQuantity1.toHex().toInt();
        QLOG_WARN()<<u8"1轨的数量为:"<<firstQuantity;

        AOI_ZebraPrinterDll dd;
        QString msg=QString::number(firstQuantity);
        QString str="VVVVVVVVVVVVVVVVVV";
        str.replace(0,msg.length(),msg);
        QString val;
        dd.printExcuteFunc(str.replace(0,msg.length(),msg),val);
    }

    //the second track out sign
    int secondOutSign;
    QByteArray array2;
    array2.append(tmpData[6]);
    array2.append(tmpData[7]);
    secondOutSign = array2.toHex().toInt(nullptr,16);
    if(secondOutSign == 1)
    {
        QByteArray clearData;
        clearData.append(static_cast<char>(0x00));
        clearData.append(static_cast<char>(0x00));
        GDataFactory::get_plc_obj()->writeDataToPLC("32203",1,clearData);
        QLOG_WARN()<<u8"2轨出框";
        QLOG_WARN()<<GDataFactory::get_instance()->bytes_to_str(tmpData);

        //the quantity of the second track
        int secondQuantity;
        QByteArray arrayQuantity2;
        arrayQuantity2.append(tmpData[8]);
        arrayQuantity2.append(tmpData[9]);
        secondQuantity = arrayQuantity2.toHex().toInt();
        QLOG_WARN()<<u8"2轨的数量为:"<<secondQuantity;

        AOI_ZebraPrinterDll dd;
        QString msg=QString::number(secondQuantity);
        QString str="VVVVVVVVVVVVVVVVVV";
        str.replace(0,msg.length(),msg);
        QString val;
        dd.printExcuteFunc(str.replace(0,msg.length(),msg),val);
    }


    m_pReadThread->set_start_sign(false);
}

void CentralWgt::slot_rev_mes_info_to_ui(const QJsonObject &obj)
{
    this->m_pLineEditExInfoA->setText(m_pExOrderIdA);
    this->m_pLineEditExWidthA->setText(m_pExOrderWidthA);
    this->m_pLineEditExInfoB->setText(m_pExOrderIdB);
    this->m_pLineEditExWidthB->setText(m_pExOrderWidthB);

    QString lineName = obj.value(QStringLiteral("lineName")).toString();
    QString produceCmdNumber = obj.value(QStringLiteral("produceCmdNumber")).toString();
    QString productSN = obj.value(QStringLiteral("productSN")).toString();
    QLOG_WARN()<<"the product SN from Mes is:"<<productSN;
    QString productWidth = obj.value(QStringLiteral("productWidth")).toString();
    QString spliceQuantity = obj.value(QStringLiteral("spliceQuantity")).toString();
    QString materialNumber = obj.value(QStringLiteral("materialNumber")).toString();
    QString orderNumber = obj.value(QStringLiteral("orderNumber")).toString();
    QString boardType = obj.value(QStringLiteral("boardType")).toString();
    QString trackNumber = obj.value(QStringLiteral("trackNumber")).toString();

    //1-track
    if(trackNumber == "1")
    {
        QLOG_WARN()<<u8"the width of track-1 is:"<<productWidth;
        QLOG_WARN()<<u8"the produceCmdNumber of track-1 is:"<<produceCmdNumber;
        this->m_pLineEditCurrentInfoA->setText(produceCmdNumber);
        this->m_pLineEditCurrentWidthA->setText(productWidth);
        GDataFactory::get_instance()->set_config_para_1("EX_ORDER_A",produceCmdNumber);
        GDataFactory::get_instance()->set_config_para_1("EX_WIDTH_A",productWidth);

        if(m_pExOrderIdA == produceCmdNumber)
        {
            if(m_pExOrderWidthA != productWidth)//width is different,alter width
            {
                double w = productWidth.toDouble();
                w = w*10.0;
                short wd = static_cast<short>(w);
                QByteArray m_data;
                m_data.append(static_cast<char>(wd & 0xFF));
                m_data.append(static_cast<char>((wd >> 8) & 0xFF));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));

                if(GDataFactory::get_plc_obj()->writeDataToPLC("32572",4,m_data))
                    QLOG_INFO()<<"order is same,1-track alter width SUCCESS!";
                else
                    QLOG_WARN()<<"order is same,1-track alter width FAILED!";
                QLOG_WARN()<<u8"1-track 制令单一致，宽度不一致，调宽";
            }
        }
        else//produce-cmd is different, retreat the box
        {
            if(m_pExOrderWidthA == productWidth)//width is same,retreat box
            {
                QByteArray m_data;
                m_data.append(static_cast<char>(0x01));
                m_data.append(static_cast<char>(0x00));

                if(GDataFactory::get_plc_obj()->writeDataToPLC("32210",1,m_data))
                    QLOG_INFO()<<"order is different,1-track retreat box SUCCESS!";
                else
                    QLOG_WARN()<<"order is different,1-track retreat box FAILED!";
                QLOG_WARN()<<u8"1-track 制令单不一致，宽度一致，退框";
            }
            else//width is different,alter the width
            {
                double w = productWidth.toDouble();
                w = w*10.0;
                short wd = static_cast<short>(w);
                QByteArray m_data;
                m_data.append(static_cast<char>(wd & 0xFF));
                m_data.append(static_cast<char>((wd >> 8) & 0xFF));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));

                if(GDataFactory::get_plc_obj()->writeDataToPLC("32572",4,m_data))
                    QLOG_INFO()<<"order is different,1-track alter width SUCCESS!";
                else
                    QLOG_WARN()<<"order is different,1-track alter width FAILED!";
                QLOG_WARN()<<u8"1-track 制令单不一致，宽度不一致，调宽";
            }
        }
        m_pExOrderIdA = produceCmdNumber;
        m_pExOrderWidthA = productWidth;
    }
    else//2-track
    {
        QLOG_WARN()<<u8"the width of track-2 is:"<<productWidth;
        QLOG_WARN()<<u8"the produceCmdNumber of track-2 is:"<<produceCmdNumber;
        this->m_pLineEditCurrentInfoB->setText(produceCmdNumber);
        this->m_pLineEditCurrentWidthB->setText(productWidth);
        GDataFactory::get_instance()->set_config_para_1("EX_ORDER_B",produceCmdNumber);
        GDataFactory::get_instance()->set_config_para_1("EX_WIDTH_B",productWidth);

        if(m_pExOrderIdB == produceCmdNumber)
        {
            if(m_pExOrderWidthB != productWidth)//width is different,alter width
            {
                double w = productWidth.toDouble();
                w = w*10.0;
                short wd = static_cast<short>(w);
                QByteArray m_data;
                m_data.append(static_cast<char>(wd & 0xFF));
                m_data.append(static_cast<char>((wd >> 8) & 0xFF));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));

                if(GDataFactory::get_plc_obj()->writeDataToPLC("32576",4,m_data))
                    QLOG_INFO()<<"order is same,2-track alter width SUCCESS!";
                else
                    QLOG_WARN()<<"order is same,2-track alter width FAILED!";
                QLOG_WARN()<<u8"2-track 制令单一致，宽度不一致，调宽";
            }
        }
        else
        {
            if(m_pExOrderWidthA == productWidth)//width is same,retreat box
            {
                QByteArray m_data;
                m_data.append(static_cast<char>(0x01));
                m_data.append(static_cast<char>(0x00));

                if(GDataFactory::get_plc_obj()->writeDataToPLC("32211",1,m_data))
                    QLOG_INFO()<<"order is different,2-track retreat box SUCCESS!";
                else
                    QLOG_WARN()<<"order is different,2-track retreat box FAILED!";
                QLOG_WARN()<<u8"1-track 制令单不一致，宽度一致，退框";
            }
            else//width is different,alter the width
            {
                double w = productWidth.toDouble();
                w = w*10.0;
                short wd = static_cast<short>(w);
                QByteArray m_data;
                m_data.append(static_cast<char>(wd & 0xFF));
                m_data.append(static_cast<char>((wd >> 8) & 0xFF));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));
                m_data.append(static_cast<char>(0x00));

                if(GDataFactory::get_plc_obj()->writeDataToPLC("32576",4,m_data))
                    QLOG_INFO()<<"order is different,2-track alter width SUCCESS!";
                else
                    QLOG_WARN()<<"order is different,2-track alter width FAILED!";
                QLOG_WARN()<<u8"2-track 制令单不一致，宽度不一致，调宽";
            }
        }
        m_pExOrderIdB = produceCmdNumber;
        m_pExOrderWidthB = productWidth;
    }
}
