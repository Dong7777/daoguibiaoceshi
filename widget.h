#ifndef WIDGET_H
#define WIDGET_H

#include <QTimer>
#include <QWidget>
#include <QSerialPort>
#include "messageprocessor.h"
#include "qlistwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void init();

private slots:

    void on_up_clicked();

    void on_down_clicked();

    void on_openPortBtn_released();

    void on_openFileBtn_released();

    void on_sendFileBtn_released();

    void on_hexDisplayChx_toggled(bool checked);

    void on_timerSendChx_toggled(bool checked);

    void on_sendStopBtn_released();

    void on_clear_clicked();

    void on_clearRecvBtn_clicked();

    void on_sendClearBtn_clicked();

    void on_sendBtn_clicked();

    void on_openPortBtn_2_released();
    void readdata();
    void autoreaddata();
    void on_openfile_clicked();
    void sendmess(int index);
    bool getMessageData(int index, Message *msg, QListWidget *datalist);
    void on_sendtestdata_clicked();

    void on_clearfile_clicked();

    void on_openfile_2_clicked();

    void on_pushButton_clicked();

    void on_openPortBtn_3_clicked();
    void shengjireaddata();

    void on_pushButton_2_clicked();
    void sendNextData();
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::Widget *ui;
    MessageProcessor *messageProcessor;

    void displayHex();
    void displayText();
    struct CompareResult {
        QByteArray originalAddress;  // 原始地址
        QByteArray extractedAddress; // 提取的地址
        QString result;              // 比对结果 ("Match" 或 "Mismatch")
    };
    // 缓冲区用于存储接收数��?
    QSerialPort shengjiserialPort;
    QSerialPort ceshiserialPort;
    QSerialPort serialPort_;
    QTimer timer_;
    int receiveCount = 0;
    QByteArray buffer; // Buffer to hold incoming data
    QByteArray comparebuffer;
    QTimer *timeoutTimer;
    QString lastDirectory;
    QList<CompareResult> comparisonResults;
    int shengjiindex = 0;
    QByteArray shengjidata;
    QByteArray address ;
    int currentIndex = 0;  // 记录当前发送的数据索引
     int zidongfasong= 0;
};
#endif // WIDGET_H
