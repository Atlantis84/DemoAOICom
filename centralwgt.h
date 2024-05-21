#ifndef CENTRALWGT_H
#define CENTRALWGT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include "demoaoithread.h"
class CentralWgt : public QWidget
{
    Q_OBJECT
public:
    static CentralWgt* get_instance()
    {
        if(m_pInstance == nullptr)
            m_pInstance = new CentralWgt();
        return m_pInstance;
    }

protected:
    void showEvent(QShowEvent *event);
    void timerEvent(QTimerEvent *event);
private:
    explicit CentralWgt(QWidget *parent = nullptr);
    static CentralWgt* m_pInstance;

    QLineEdit* m_pLineEditExInfoA;
    QLineEdit* m_pLineEditCurrentInfoA;
    QLineEdit* m_pLineEditExWidthA;
    QLineEdit* m_pLineEditCurrentWidthA;

    QLineEdit* m_pLineEditExInfoB;
    QLineEdit* m_pLineEditCurrentInfoB;
    QLineEdit* m_pLineEditExWidthB;
    QLineEdit* m_pLineEditCurrentWidthB;

    QTextEdit* m_pTextEditLogInfo;

    int m_pTimerID;
    int m_pTimerMesID;

    QString m_pExOrderIdA;
    QString m_pExOrderWidthA;
    QString m_pExOrderIdB;
    QString m_pExOrderWidthB;

    DemoAOIThread* m_pReadThread;

signals:

public slots:
    void slot_rev_logs(const QString info);

    void slot_btn_read_click();
    void slot_btn_write_click();
    void slot_btn_start_monitor();
    void slot_btn_stop_monitor();

    void slot_rev_from_plc(const QByteArray& data);
    void slot_rev_mes_info_to_ui(const QJsonObject& obj);
};

#endif // CENTRALWGT_H
