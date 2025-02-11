
#include "widget.h"
#include "ui_widget.h"
#include <QString>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QStandardPaths>
#include <QSerialPortInfo>
#include <QThread>
#include <QtMinMax>
#include "dlt698.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    messageProcessor = new MessageProcessor(this);
    init();
}

Widget::~Widget()
{
    if (serialPort_.isOpen())
    {
        serialPort_.close();
    }
    delete ui;
}

void Widget::init()
{
    setWindowTitle("Qt串口助手");

    // 设置改变
    connect(ui->baudRateCmb, &QComboBox::currentIndexChanged, this, [=]
            {
                auto br = ui->baudRateCmb->currentData().value<QSerialPort::BaudRate>();
                if(!serialPort_.setBaudRate(br))
                {
                    QMessageBox::warning(this,"false","设置波特率失败:"+serialPort_.errorString());
                } });
    connect(ui->baudRateCmb_2, &QComboBox::currentIndexChanged, this, [=]
            {
                auto br = ui->baudRateCmb_2->currentData().value<QSerialPort::BaudRate>();
                if(!ceshiserialPort.setBaudRate(br))
                {
                    QMessageBox::warning(this,"false","设置波特率失败:"+ceshiserialPort.errorString());
                } });
    connect(ui->baudRateCmb_3, &QComboBox::currentIndexChanged, this, [=]
            {
                auto br = ui->baudRateCmb_3->currentData().value<QSerialPort::BaudRate>();
                if(!shengjiserialPort.setBaudRate(br))
                {
                    QMessageBox::warning(this,"false","设置波特率失败:"+shengjiserialPort.errorString());
                } });



    connect(ui->dataBitsCmb, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->dataBitsCmb->currentData().value<QSerialPort::DataBits>();
                if(!serialPort_.setDataBits(value))
                {
                    QMessageBox::warning(this,"false","设置数据位失败:"+serialPort_.errorString());
                } });
    connect(ui->dataBitsCmb_2, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->dataBitsCmb_2->currentData().value<QSerialPort::DataBits>();
                if(!ceshiserialPort.setDataBits(value))
                {
                    QMessageBox::warning(this,"false","设置数据位失败:"+ceshiserialPort.errorString());
                } });
    connect(ui->dataBitsCmb_3, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->dataBitsCmb_3->currentData().value<QSerialPort::DataBits>();
                if(!shengjiserialPort.setDataBits(value))
                {
                    QMessageBox::warning(this,"false","设置数据位失败:"+shengjiserialPort.errorString());
                } });



    connect(ui->stopBitsCmb, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->stopBitsCmb->currentData().value<QSerialPort::StopBits>();
                if(!serialPort_.setStopBits(value))
                {
                    QMessageBox::warning(this,"false","设置停止位失败:"+serialPort_.errorString());
                } });
    connect(ui->stopBitsCmb_2, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->stopBitsCmb_2->currentData().value<QSerialPort::StopBits>();
                if(!ceshiserialPort.setStopBits(value))
                {
                    QMessageBox::warning(this,"false","设置停止位失败:"+ceshiserialPort.errorString());
                } });
    connect(ui->stopBitsCmb_3, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->stopBitsCmb_3->currentData().value<QSerialPort::StopBits>();
                if(!shengjiserialPort.setStopBits(value))
                {
                    QMessageBox::warning(this,"false","设置停止位失败:"+shengjiserialPort.errorString());
                } });


    connect(ui->parityCmb, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->parityCmb->currentData().value<QSerialPort::Parity>();
                if(!serialPort_.setParity(value))
                {
                    QMessageBox::warning(this,"false","设置校验位失败:"+serialPort_.errorString());
                }
                qInfo()<<"sdflksjdfklsfkd"; });
    connect(ui->parityCmb_2, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->parityCmb_2->currentData().value<QSerialPort::Parity>();
                if(!ceshiserialPort.setParity(value))
                {
                    QMessageBox::warning(this,"false","设置校验位失败:"+ceshiserialPort.errorString());
                }
                qInfo()<<"sdflksjdfklsfkd"; });
    connect(ui->parityCmb_3, &QComboBox::currentIndexChanged, this, [=]
            {
                auto value = ui->parityCmb_3->currentData().value<QSerialPort::Parity>();
                if(!shengjiserialPort.setParity(value))
                {
                    QMessageBox::warning(this,"false","设置校验位失败:"+shengjiserialPort.errorString());
                }
                qInfo()<<"sdflksjdfklsfkd"; });



    // 获取所有的可用的串口
    auto portsInfo = QSerialPortInfo::availablePorts();
    for (auto &info : portsInfo)
    {
        qInfo() << info.description() << info.portName() << info.systemLocation();
        ui->protsCmb->addItem(info.portName() + ":" + info.description(), info.portName());
    }
    // 获取所有的可用的串口
    auto portsInfo2 = QSerialPortInfo::availablePorts();
    for (auto &info : portsInfo2)
    {
        qInfo() << info.description() << info.portName() << info.systemLocation();
        ui->protsCmb_2->addItem(info.portName() + ":" + info.description(), info.portName());
    }
    // 获取所有的可用的串口
    auto portsInfo3 = QSerialPortInfo::availablePorts();
    for (auto &info : portsInfo3)
    {
        qInfo() << info.description() << info.portName() << info.systemLocation();
        ui->protsCmb_3->addItem(info.portName() + ":" + info.description(), info.portName());
    }
    // 获取标准的波特率
    auto baudRates = QSerialPortInfo::standardBaudRates();
    for (auto br : baudRates)
    {
        ui->baudRateCmb->addItem(QString::number(br), br);
    }
    ui->baudRateCmb->setCurrentText("115200");
    // 获取标准的波特率
    auto baudRates2 = QSerialPortInfo::standardBaudRates();
    for (auto br : baudRates2)
    {
        ui->baudRateCmb_2->addItem(QString::number(br), br);
    }
    ui->baudRateCmb_2->setCurrentText("115200");

    // 获取标准的波特率
    auto baudRates3 = QSerialPortInfo::standardBaudRates();
    for (auto br : baudRates3)
    {
        ui->baudRateCmb_3->addItem(QString::number(br), br);
    }
    ui->baudRateCmb_3->setCurrentText("115200");
    // 设置停止位
    ui->stopBitsCmb->addItem("1", QSerialPort::OneStop);
    ui->stopBitsCmb->addItem("1.5", QSerialPort::OneAndHalfStop);
    ui->stopBitsCmb->addItem("2", QSerialPort::TwoStop);
    // 设置停止位
    ui->stopBitsCmb_2->addItem("1", QSerialPort::OneStop);
    ui->stopBitsCmb_2->addItem("1.5", QSerialPort::OneAndHalfStop);
    ui->stopBitsCmb_2->addItem("2", QSerialPort::TwoStop);
    // 设置停止位
    ui->stopBitsCmb_3->addItem("1", QSerialPort::OneStop);
    ui->stopBitsCmb_3->addItem("1.5", QSerialPort::OneAndHalfStop);
    ui->stopBitsCmb_3->addItem("2", QSerialPort::TwoStop);
    // 设置数据位
    ui->dataBitsCmb->addItem("5", QSerialPort::Data5);
    ui->dataBitsCmb->addItem("6", QSerialPort::Data6);
    ui->dataBitsCmb->addItem("7", QSerialPort::Data7);
    ui->dataBitsCmb->addItem("8", QSerialPort::Data8);
    ui->dataBitsCmb->setCurrentText("8");
    // 设置数据位
    ui->dataBitsCmb_2->addItem("5", QSerialPort::Data5);
    ui->dataBitsCmb_2->addItem("6", QSerialPort::Data6);
    ui->dataBitsCmb_2->addItem("7", QSerialPort::Data7);
    ui->dataBitsCmb_2->addItem("8", QSerialPort::Data8);
    ui->dataBitsCmb_2->setCurrentText("8");
    // 设置数据位
    ui->dataBitsCmb_3->addItem("5", QSerialPort::Data5);
    ui->dataBitsCmb_3->addItem("6", QSerialPort::Data6);
    ui->dataBitsCmb_3->addItem("7", QSerialPort::Data7);
    ui->dataBitsCmb_3->addItem("8", QSerialPort::Data8);
    ui->dataBitsCmb_3->setCurrentText("8");
    // 设置校验位
    ui->parityCmb->addItem("NoParity", QSerialPort::NoParity);
    ui->parityCmb->addItem("EvenParity", QSerialPort::EvenParity);
    ui->parityCmb->addItem("OddParity", QSerialPort::OddParity);
    ui->parityCmb->addItem("SpaceParity", QSerialPort::SpaceParity);
    ui->parityCmb->addItem("MarkParity", QSerialPort::MarkParity);
    ui->parityCmb->setCurrentIndex(1); // 偶校验在下拉框中的索引为1
    // 设置校验位
    ui->parityCmb_2->addItem("NoParity", QSerialPort::NoParity);
    ui->parityCmb_2->addItem("EvenParity", QSerialPort::EvenParity);
    ui->parityCmb_2->addItem("OddParity", QSerialPort::OddParity);
    ui->parityCmb_2->addItem("SpaceParity", QSerialPort::SpaceParity);
    ui->parityCmb_2->addItem("MarkParity", QSerialPort::MarkParity);
    ui->parityCmb_2->setCurrentIndex(1); // 偶校验在下拉框中的索引为1
    // 设置校验位
    ui->parityCmb_3->addItem("NoParity", QSerialPort::NoParity);
    ui->parityCmb_3->addItem("EvenParity", QSerialPort::EvenParity);
    ui->parityCmb_3->addItem("OddParity", QSerialPort::OddParity);
    ui->parityCmb_3->addItem("SpaceParity", QSerialPort::SpaceParity);
    ui->parityCmb_3->addItem("MarkParity", QSerialPort::MarkParity);
    ui->parityCmb_3->setCurrentIndex(1); // 偶校验在下拉框中的索引为1

    ui->hexDisplayChx->setChecked(true);
    ui->baowen_index->setText("0");
    connect(&serialPort_, &QSerialPort::readyRead, this, &Widget::readdata);
    connect(&ceshiserialPort, &QSerialPort::readyRead, this, &Widget::autoreaddata);
    connect(&shengjiserialPort, &QSerialPort::readyRead, this, &Widget::shengjireaddata);
}

void Widget::on_openPortBtn_released()
{
    // 串口是否已经打开
    if (serialPort_.isOpen())
    {
        serialPort_.close();
        ui->openPortBtn->setText("打开串口");
        if (timer_.isActive())
            timer_.stop();
        return;
    }
    // 获取串口名
    auto portName = ui->protsCmb->currentData().toString();
    // 获取波特率
    auto baudRate = ui->baudRateCmb->currentData().value<QSerialPort::BaudRate>();
    // 获取数据位
    auto dataBits = ui->dataBitsCmb->currentData().value<QSerialPort::DataBits>();
    // 获取停止位
    auto stopBits = ui->stopBitsCmb->currentData().value<QSerialPort::StopBits>();
    // 获取校验位
    auto parity = ui->parityCmb->currentData().value<QSerialPort::Parity>();

    serialPort_.setBaudRate(baudRate);
    serialPort_.setDataBits(dataBits);
    serialPort_.setStopBits(stopBits);
    serialPort_.setParity(parity);
    serialPort_.setPortName(portName);
    // 打开串口
    if (!serialPort_.open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this, "warning", portName + " open failed:" + serialPort_.errorString());
        return;
    }
    else
    {
        ui->openPortBtn->setText("关闭串口");
    }
}

void Widget::on_openFileBtn_released()
{
    auto filename = QFileDialog::getOpenFileName(this, "选择文件", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
                                                 "txt(*.txt);;all(*.*)");
    if (!filename.isEmpty())
    {
        ui->fileNameEdit->setText(filename);
    }
}

void Widget::on_sendFileBtn_released()
{
    auto filename = ui->fileNameEdit->text();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "warning", filename + " open failed:" + file.errorString());
        return;
    }
    // 最好判断一下文件的编码
    serialPort_.write(QString::fromUtf8(file.readAll()).toLocal8Bit());
}

void Widget::on_hexDisplayChx_toggled(bool checked)
{
    if (checked)
        displayHex();
    else
        displayText();
}

void Widget::displayHex()
{
    // 先把数据拿出来
    auto dataStr = ui->recvEdit->toPlainText();
    // 转成十六进制
    auto hexData = dataStr.toLocal8Bit().toHex(' ').toUpper();
    // 写回去
    ui->recvEdit->setPlainText(hexData);
}

void Widget::displayText()
{
    // 先把数据拿出来
    auto dataStr = ui->recvEdit->toPlainText();
    // 转成文本
    auto textData = QString::fromLocal8Bit(dataStr.toLocal8Bit());
    // 写回去
    ui->recvEdit->setPlainText(textData);
}

void Widget::on_timerSendChx_toggled(bool checked)
{
    if (checked)
    {
        timer_.start(ui->timerPeriodEdit->text().toUInt());
        ui->timerPeriodEdit->setEnabled(false);
    }
    else
    {
        timer_.stop();
        ui->timerPeriodEdit->setEnabled(true);
    }
}

void Widget::on_sendStopBtn_released()
{
    serialPort_.clear();
    if (timer_.isActive())
        timer_.stop();
}

void Widget::on_clear_clicked()
{
    ui->textEdit->clear();
}

void Widget::on_clearRecvBtn_clicked()
{
    ui->recvEdit->clear();
    receiveCount = 0;
}

void Widget::on_sendClearBtn_clicked()
{
    ui->sendEdit_2->clear();
}


void Widget::on_sendBtn_clicked()
{
    buffer.clear();
    QString dataStr = ui->sendEdit_2->toPlainText();
    if (ui->sendNewLineChx->isChecked())
    {
        dataStr += "\r\n";
    }

    if (dataStr.isEmpty())
    {
        QMessageBox::information(this, "提示", "发送内容为空");
        return;
    }

    QByteArray hexData = QByteArray::fromHex(dataStr.toUtf8());
    if (serialPort_.write(hexData) == -1)
    {
        QMessageBox::warning(this, "错误", "发送失败: " + serialPort_.errorString());
    }
    qDebug() << "发送数据:" << hexData.toHex().toUpper();
}

void Widget::on_openPortBtn_2_released()
{
    // 串口是否已经打开
    if (ceshiserialPort.isOpen())
    {
        ceshiserialPort.close();
        ui->openPortBtn_2->setText("打开串口");
        if (timer_.isActive())
            timer_.stop();
        return;
    }
    // 获取串口名
    auto portName = ui->protsCmb_2->currentData().toString();
    // 获取波特率
    auto baudRate = ui->baudRateCmb_2->currentData().value<QSerialPort::BaudRate>();
    // 获取数据位
    auto dataBits = ui->dataBitsCmb_2->currentData().value<QSerialPort::DataBits>();
    // 获取停止位
    auto stopBits = ui->stopBitsCmb_2->currentData().value<QSerialPort::StopBits>();
    // 获取校验位
    auto parity = ui->parityCmb_2->currentData().value<QSerialPort::Parity>();

    ceshiserialPort.setBaudRate(baudRate);
    ceshiserialPort.setDataBits(dataBits);
    ceshiserialPort.setStopBits(stopBits);
    ceshiserialPort.setParity(parity);
    ceshiserialPort.setPortName(portName);
    // 打开串口
    if (!ceshiserialPort.open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this, "warning", portName + " open failed:" + ceshiserialPort.errorString());
        return;
    }
    else
    {
        ui->openPortBtn_2->setText("关闭串口");
    }
}


void Widget::readdata()
{
    // 从串口读取所有可用的数据
    QByteArray receivedData = serialPort_.readAll();
    buffer.append(receivedData); // 将新接收的数据添加到缓冲区

    qDebug() << "接收到的数据:" << receivedData.toHex(); // 打印接收到的数据
    qDebug() << "当前缓冲区大小:" << buffer.size();      // 打印当前缓冲区大小

    // 处理缓冲区中的数据
    while (buffer.size() >= 3) // 确保缓冲区至少有3个字节
    {
        // 如果第一个字节不是0x68，移除它
        if (buffer.at(0) != 0x68)
        {
            qDebug() << "无效的起始字节:" << QString::number(static_cast<unsigned char>(buffer.at(0), 16)); // 打印无效的起始字节
            buffer.remove(0, 1);                                                                            // 移除无效的起始字节
            continue;                                                                                       // 检查下一个字节
        }

        // 从第二和第三个字节读取长度（小端）
        uint16_t length = static_cast<unsigned char>(buffer.at(1)) |
                          (static_cast<unsigned char>(buffer.at(2)) << 8);
        qDebug() << "读取的长度:" << length; // 打印读取的长度

        // 检查缓冲区中是否有足够的字节来构建完整的消息
        if (length + 2 > buffer.size())
        {
            qDebug() << "缓冲区不足以构建完整消息, 当前大小:" << buffer.size(); // 打印缓冲区大小
            break;                                                              // 数据不足，退出循环
        }

        // 验证结束标记（0x16）
        if (buffer.at(length + 1) != 0x16)
        {
            qDebug() << "无效的结束标记, 当前结束标记:" << QString::number(static_cast<unsigned char>(buffer.at(length + 1)), 16); // 打印结束标记
            buffer.remove(0, 1);                                                                                                   // 移除无效的起始字节并继续
            continue;
        }

        // 找到有效的消息，提取它
        QByteArray message = buffer.left(length + 2); // 包含起始和结束字节
        QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        ui->recvEdit->appendPlainText(timestamp + " - " + message.toHex().toUpper());
        qDebug() << "提取的有效消息:" << message.toHex(); // 打印提取的消息
        // 从缓冲区中移除已处理的字节
        buffer.remove(0, length + 2);
        break;
    }

    qDebug() << "处理后缓冲区大小:" << buffer.size(); // 打印处理后缓冲区大小
}

void Widget::autoreaddata()
{
    QByteArray message;
    // 从串口读取所有可用的数据
    QByteArray receivedData = ceshiserialPort.readAll();
    buffer.append(receivedData); // 将新接收的数据添加到缓冲区

    qDebug() << "接收到的数据:" << receivedData.toHex(); // 打印接收到的数据
    qDebug() << "当前缓冲区大小:" << buffer.size();      // 打印当前缓冲区大小

    // 处理缓冲区中的数据
    while (buffer.size() >= 3) // 确保缓冲区至少有3个字节
    {
        // 如果第一个字节不是0x68，移除它
        if (buffer.at(0) != 0x68)
        {
            qDebug() << "无效的起始字节:" << QString::number(static_cast<unsigned char>(buffer.at(0), 16)); // 打印无效的起始字节
            buffer.remove(0, 1);                                                                            // 移除无效的起始字节
            continue;                                                                                       // 检查下一个字节
        }

        // 从第二和第三个字节读取长度（小端）
        uint16_t length = static_cast<unsigned char>(buffer.at(1)) |
                          (static_cast<unsigned char>(buffer.at(2)) << 8);
        qDebug() << "读取的长度:" << length; // 打印读取的长度

        // 检查缓冲区中是否有足够的字节来构建完整的消息
        if (length + 2 > buffer.size())
        {
            qDebug() << "缓冲区不足以构建完整消息, 当前大小:" << buffer.size(); // 打印缓冲区大小
            break;                                                              // 数据不足，退出循环
        }

        // 验证结束标记（0x16）
        if (buffer.at(length + 1) != 0x16)
        {
            qDebug() << "无效的结束标记, 当前结束标记:" << QString::number(static_cast<unsigned char>(buffer.at(length + 1)), 16); // 打印结束标记
            buffer.remove(0, 1);                                                                                                   // 移除无效的起始字节并继续
            continue;
        }

        // 找到有效的消息，提取它
        message = buffer.left(length + 2);     // 包含起始和结束字节
        qDebug() << "提取的有效消息:" << message.toHex(); // 打印提取的消息
        if (message.size() > 14)
        {  // 注意，索引从 0 开始，所以第十五个字节是索引 14
            if (static_cast<unsigned char>(message.at(14)) == 0x90 && static_cast<unsigned char>(message.at(15)) == 0x00)
            {

                int length = static_cast<unsigned char>(message.at(16));
                if(message.mid(17, length) == comparebuffer.mid(17, length))
                {
                       messageProcessor->result = "success";
                }
                else
                {
                    messageProcessor->result = "bao wen bu yi zhi";
                    qWarning() << "jieshou for message:" << message.toHex();
                    qWarning() << "yuqi for message:" << comparebuffer.toHex();
                }
                qDebug() << "第十五个字节是 0x90";
            }
            else
            {
                qDebug() << "第十五个字节不是 0x90";
                if (comparebuffer == message)
                {
                    messageProcessor->result = "success";
                }
                else
                {
                    messageProcessor->result = "bao wen bu yi zhi";
                    qWarning() << "jieshou for message:" << message.toHex();
                    qWarning() << "yuqi for message:" << comparebuffer.toHex();
                }
            }
        }
        else
        {
            qDebug() << "消息长度不足 15 个字节";
        }


        ui->recvlineEdit->setText(message.toHex());
        ui->result->setText(messageProcessor->result);
        qDebug() << "处理后缓冲区大小:" << buffer.size(); // 打印处理后缓冲区大小
        break;
    }
}

void Widget::on_openfile_clicked()
{
    // auto filename = QFileDialog::getOpenFileName(this, "选择文件", QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),
    //                                              "txt(*.txt);;all(*.*)");
    if (lastDirectory.isEmpty()) {
        lastDirectory = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    // 打开文件对话框
    auto filename = QFileDialog::getOpenFileName(this, "选择文件", lastDirectory);

    if (!filename.isEmpty()) {
        // 更新上一次选择的目录
        lastDirectory = QFileInfo(filename).absolutePath();
        // 处理文件
    }
    if (!filename.isEmpty())
    {
        messageProcessor->loadMessagesFromFile(filename);
        for (const Message &message : messageProcessor->sentMessages)
        {
            // 只在发送消息不为空的情况下添加到列表中
            if (!message.data.isEmpty())
            {
                QString itemText = QString("%1:  (时间戳: %2) %3")
                                       .arg(message.index)         // 行号
                                       .arg(message.timestamp)     // 消息时间戳
                                       .arg(message.data.toHex()); // 消息内容

                ui->send_list->addItem(itemText); // 显示行号和内容
            }
        }
        for (const Message &message : messageProcessor->receivedMessages)
        {
            // 只在发送消息不为空的情况下添加到列表中
            if (!message.data.isEmpty())
            {
                QString itemText = QString("%1: (时间戳: %2) %3")
                                       .arg(message.index) // 行号
                                       .arg(message.timestamp)
                                       .arg(message.data.toHex()); // 消息内容; // 消息时间戳

                ui->recv_list->addItem(itemText); // 显示行号和内容
            }
        }
    }
    else
    {
        qDebug() << "open file error";
    }
}

bool Widget::getMessageData(int index, Message *msg, QListWidget *datalist)
{
    msg->index = -1; // 初始化索引为 -1，表示未找到有效消息

    if (index >= 0 && index < datalist->count())
    {
        // 获取指定行的 QListWidgetItem
        QListWidgetItem *item = datalist->item(index);
        if (item)
        {
            // 获取该行的文本内容
            QString itemText = item->text();
            qDebug() << "第" << index + 1 << "行的数据:" << itemText;

            // 解析文本内容
            QStringList parts = itemText.split(" "); // 按空格分割

            if (parts.size() >= 3)
            {
                // 假设格式为: "行号: (时间戳: 时间) 消息内容"
                bool ok;
                msg->index = parts[0].split(":").first().toInt(&ok);    // 获取行号
                msg->timestamp = parts[1].split(":").last().trimmed();  // 获取时间戳
                msg->data = QByteArray::fromHex(parts.last().toUtf8()); // 获取消息内容并转为 QByteArray

                if (!ok)
                {
                    qDebug() << "行号解析失败.";
                    return false; // 解析失败
                }
                return true; // 成功解析
            }
            else
            {
                qDebug() << "数据格式不正确.";
            }
        }
        else
        {
            qDebug() << "指定行没有找到有效的项.";
        }
    }
    else
    {
        qDebug() << "行索引超出范围.";
    }

    return false; // 未找到有效消息
}

void Widget::sendmess(int index)
{
    buffer.clear();
    comparebuffer.clear();
    ui->sendlineEdit->clear();
    ui->recvlineEdit->clear();
    ui->yuqilineEdit->clear();
    ui->result->clear();
    Message msg;
    getMessageData(index, &msg, ui->send_list);
    ui->sendlineEdit->setText(msg.data.toHex());
    ui->textEdit->setText(ui->send_list->item(index)->text());
    if (ceshiserialPort.write(msg.data) == -1)
    {
        QMessageBox::warning(this, "错误", "发送失败: " + ceshiserialPort.errorString());
    }
    qDebug() << "发送数据:" << msg.data.toHex().toUpper();
    getMessageData(index, &msg, ui->recv_list);
    comparebuffer = msg.data;
    ui->yuqilineEdit->setText(msg.data.toHex());
}

void Widget::on_up_clicked()
{
    int index = ui->baowen_index->text().toInt();
    if (index == 0)
    {
        QMessageBox::information(this, "提示", "索引为0");
        return;
    }
    if (index > ui->send_list->count())
    {
        QMessageBox::information(this, "提示", "索引超出发送内容大小");
        return;
    }
    index--;
    ui->baowen_index->setText(QString::number(index));
    sendmess(index);
}

void Widget::on_down_clicked()
{
    int index = ui->baowen_index->text().toInt();

    if (index+2 > ui->send_list->count())
    {
        QMessageBox::information(this, "提示", "索引超出发送内容大小");
        return;
    }
    index++;
    ui->baowen_index->setText(QString::number(index));
    sendmess(index);
}

void Widget::on_sendtestdata_clicked()
{
    int index = ui->baowen_index->text().toInt();
    if (index > ui->send_list->count())
    {
        QMessageBox::information(this, "提示", "索引超出发送内容大小");
        return;
    }
    ui->baowen_index->setText(QString::number(index));
    sendmess(index);
}

void Widget::on_clearfile_clicked()
{
    ui->send_list->clear();
    ui->recv_list->clear();

}


void Widget::on_openfile_2_clicked()
{
    if (lastDirectory.isEmpty()) {
        lastDirectory = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }

    // 打开文件对话框
    auto filename = QFileDialog::getOpenFileName(this, "选择文件", lastDirectory);

    if (!filename.isEmpty()) {
        // 更新上一次选择的目录
        lastDirectory = QFileInfo(filename).absolutePath();
        // 处理文件
    }
    if (!filename.isEmpty())
    {
        messageProcessor->loadAPDUFromFile(filename);
        for (const Message &message : messageProcessor->sentMessages)
        {
            // 只在发送消息不为空的情况下添加到列表中
            if (!message.data.isEmpty())
            {
                QString itemText = QString("%1:  (时间戳: %2) %3")
                                       .arg(message.index)         // 行号
                                       .arg(message.timestamp)     // 消息时间戳
                                       .arg(message.data.toHex()); // 消息内容

                ui->send_list->addItem(itemText); // 显示行号和内容
            }
        }
        for (const Message &message : messageProcessor->receivedMessages)
        {
            // 只在发送消息不为空的情况下添加到列表中
            if (!message.data.isEmpty())
            {
                QString itemText = QString("%1: (时间戳: %2) %3")
                                       .arg(message.index) // 行号
                                       .arg(message.timestamp)
                                       .arg(message.data.toHex()); // 消息内容; // 消息时间戳

                ui->recv_list->addItem(itemText); // 显示行号和内容
            }
        }
    }
    else
    {
        qDebug() << "open file error";
    }
}


void Widget::on_openPortBtn_3_clicked()
{
    // 串口是否已经打开
    if (shengjiserialPort.isOpen())
    {
        shengjiserialPort.close();
        ui->openPortBtn_3->setText("打开串口");
        if (timer_.isActive())
            timer_.stop();
        return;
    }
    // 获取串口名
    auto portName = ui->protsCmb_3->currentData().toString();
    // 获取波特率
    auto baudRate = ui->baudRateCmb_3->currentData().value<QSerialPort::BaudRate>();
    // 获取数据位
    auto dataBits = ui->dataBitsCmb_3->currentData().value<QSerialPort::DataBits>();
    // 获取停止位
    auto stopBits = ui->stopBitsCmb_3->currentData().value<QSerialPort::StopBits>();
    // 获取校验位
    auto parity = ui->parityCmb_3->currentData().value<QSerialPort::Parity>();

    shengjiserialPort.setBaudRate(baudRate);
    shengjiserialPort.setDataBits(dataBits);
    shengjiserialPort.setStopBits(stopBits);
    shengjiserialPort.setParity(parity);
    shengjiserialPort.setPortName(portName);
    // 打开串口
    if (!shengjiserialPort.open(QIODevice::ReadWrite))
    {
        QMessageBox::warning(this, "warning", portName + " open failed:" + shengjiserialPort.errorString());
        return;
    }
    else
    {
        ui->openPortBtn_3->setText("关闭串口");
    }
}

void Widget::on_pushButton_clicked()
{
    buffer.clear();
    QString dataStr ="6817004345AAAAAAAAAAAAA051EA0501004001020000ED0316";
    if (ui->sendNewLineChx->isChecked())
    {
        dataStr += "\r\n";
    }

    if (dataStr.isEmpty())
    {
        QMessageBox::information(this, "提示", "发送内容为空");
        return;
    }

    QByteArray hexData = QByteArray::fromHex(dataStr.toUtf8());
    qDebug() << "Data:" << hexData.toHex(' ').toUpper(); // 格式化输出为十六进制并加空格
    if (shengjiserialPort.write(hexData) == -1)
    {
        QMessageBox::warning(this, "错误", "发送失败: " + ceshiserialPort.errorString());
    }
}

void Widget::shengjireaddata()
{
    QByteArray message;
    // 从串口读取所有可用的数据
    QByteArray receivedData = shengjiserialPort.readAll();
    buffer.append(receivedData); // 将新接收的数据添加到缓冲区

    qDebug() << "接收到的数据:" << receivedData.toHex(); // 打印接收到的数据
    qDebug() << "当前缓冲区大小:" << buffer.size();      // 打印当前缓冲区大小

    // 处理缓冲区中的数据
    while (buffer.size() >= 3) // 确保缓冲区至少有3个字节
    {
        // 如果第一个字节不是0x68，移除它
        if (buffer.at(0) != 0x68)
        {
            qDebug() << "无效的起始字节:" << QString::number(static_cast<unsigned char>(buffer.at(0), 16)); // 打印无效的起始字节
            buffer.remove(0, 1);                                                                            // 移除无效的起始字节
            continue;                                                                                       // 检查下一个字节
        }

        // 从第二和第三个字节读取长度（小端）
        uint16_t length = static_cast<unsigned char>(buffer.at(1)) |
                          (static_cast<unsigned char>(buffer.at(2)) << 8);
        qDebug() << "读取的长度:" << length; // 打印读取的长度

        // 检查缓冲区中是否有足够的字节来构建完整的消息
        if (length + 2 > buffer.size())
        {
            qDebug() << "缓冲区不足以构建完整消息, 当前大小:" << buffer.size(); // 打印缓冲区大小
            break;                                                              // 数据不足，退出循环
        }

        // 验证结束标记（0x16）
        if (buffer.at(length + 1) != 0x16)
        {
            qDebug() << "无效的结束标记, 当前结束标记:" << QString::number(static_cast<unsigned char>(buffer.at(length + 1)), 16); // 打印结束标记
            buffer.remove(0, 1);                                                                                                   // 移除无效的起始字节并继续
            continue;
        }

        // 找到有效的消息，提取它
        message = buffer.left(length + 2);     // 包含起始和结束字节
        qDebug() << "提取的有效消息:" << message.toHex(); // 打印提取的消息
        if (message.size() > 14)
        {  // 注意，索引从 0 开始，所以第十五个字节是索引 14
            if (static_cast<unsigned char>(message.at(14)) == 0x85 && static_cast<unsigned char>(message.at(15)) == 0x01)
            {

                // 提取地址部分（11 22 33 44 55 66 从偏移量 21 开始，长度为 6 字节）
                for (int i = 0; i < 6; ++i) {
                    globalAddress[i] = static_cast<u8>(message[24 + i]);  // 提取并存储到 globalAddress 数组
                }
                address = message.mid(24, 6);
                // 输出结果
                qDebug() << "Address:" << address.toHex(' ').toUpper();

                ui->lineEdit->setText(address.toHex());
            }
             else if (static_cast<unsigned char>(message.at(14)) == 0x87 && static_cast<unsigned char>(message.at(15)) == 0x01)
            {

                if (static_cast<unsigned char>(message.at(21)) ==0x00)
                {
                    if(zidongfasong == 1)
                    {
                        // 可以根据接收到的数据判断是否继续发送
                        currentIndex++;  // 更新索引
                        // 继续发送下一条数据
                        sendNextData();
                        ui->listWidget->addItem(QString::number(currentIndex) +  "succes");
                    }
                    else
                    {
                        ui->recvEdit_3->setPlainText(message.toHex());
                    }

                }
                else
                {
                    if(zidongfasong == 1)
                    {
                        ui->listWidget->addItem(QString::number(currentIndex) +  "error");
                    }
                    else
                    {
                        ui->recvEdit_3->setPlainText(message.toHex());
                    }

                }

            }
            else
            {
                //comparisonResults[shengjiindex].result = "shibai";
                if(zidongfasong == 1)
                {
                     ui->listWidget->addItem(QString::number(currentIndex) +  "error");
                }
                else
                {
                    ui->recvEdit_3->setPlainText(message.toHex());
                }
            }

        }
        else
        {
            qDebug() << "消息长度不足 15 个字节";
        }
        qDebug() << "处理后缓冲区大小:" << buffer.size(); // 打印处理后缓冲区大小
        break;
    }
}


void Widget::on_pushButton_2_clicked()
{
    zidongfasong = 1;
    currentIndex = ui->lineEdit_2->text().toInt();
    sendNextData();
}

void Widget::sendNextData()
{

    // 获取消息队列大小
    int size = messageProcessor->sentMessages.size();
    buffer.clear();
    if (currentIndex >= size)
    {
        qDebug() << "所有数据已发送";
        return;  // 如果所有数据都已发送，则退出
    }

    u16 dataLen = 0;
    u8 SendBuf[2048];
    u8 APDU[2048];

    // 获取当前索引的数据长度
    u16 apduDataLen = messageProcessor->sentMessages[currentIndex].data.size();

    if (messageProcessor->sentMessages[currentIndex].data.size() <= sizeof(SendBuf))
    {
        memcpy(APDU, messageProcessor->sentMessages[currentIndex].data.data(), apduDataLen);
    }
    else
    {
        qWarning() << "数据大小超过缓冲区大小!";
    }

    // 调用请求函数发送数据
    u16 result = cjdeal_698_get_request(SendBuf, &dataLen, APDU, apduDataLen);
    QByteArray byteArray(reinterpret_cast<char*>(SendBuf), dataLen);
    qDebug() << "发送数据:" << byteArray.toHex();

    // 发送数据
    if (shengjiserialPort.write(byteArray) == -1) {
        QMessageBox::warning(this, "错误", "发送失败: " + shengjiserialPort.errorString());
    }
}


void Widget::on_pushButton_3_clicked()
{
    ui->listWidget->clear();
}


void Widget::on_pushButton_4_clicked()
{
    zidongfasong = 0;
    currentIndex = ui->lineEdit_2->text().toInt();
    sendNextData();
}

size_t convertTextToHex(const QString &input, u8 *apdu, size_t maxSize) {
    QString sanitizedInput = input.simplified(); // Remove extra spaces/newlines
    sanitizedInput.replace(" ", "");             // Remove any remaining spaces

    size_t byteCount = 0; // To track the number of valid bytes
    for (int i = 0; i < sanitizedInput.size() - 1 && byteCount < maxSize; i += 2) {
        bool ok;
        QString hexByte = sanitizedInput.mid(i, 2); // Take two characters
        apdu[byteCount++] = static_cast<u8>(hexByte.toUInt(&ok, 16)); // Convert hex to byte
        if (!ok) {
            qDebug() << "Invalid hex input at position" << i << ": " << hexByte;
            break;
        }
    }

    return byteCount; // Return the number of bytes processed
}

void Widget::on_pushButton_5_clicked()
{
    zidongfasong = 0;
    buffer.clear();
    u16 dataLen = 0;
    u8 SendBuf[2048];
    u8 APDU[2048];

    // 获取当前索引的数据长度
    // 获取 ui->sendEdit_3 中的文本内容
    QString text = ui->sendEdit_3->toPlainText();

    size_t copySize = convertTextToHex(text, APDU, sizeof(APDU));
    // if (messageProcessor->sentMessages[currentIndex].data.size() <= sizeof(SendBuf)) {
    //     memcpy(APDU, messageProcessor->sentMessages[currentIndex].data.data(), apduDataLen);
    // } else {
    //     qWarning() << "数据大小超过缓冲区大小!";
    // }

    // 调用请求函数发送数据
    u16 result = cjdeal_698_get_request(SendBuf, &dataLen, APDU, copySize);
    QByteArray sendbyteArray(reinterpret_cast<char*>(SendBuf), dataLen);
    //qDebug() << "发送数据:" << byteArray.toHex();

    // 发送数据
    if (shengjiserialPort.write(sendbyteArray) == -1) {
        QMessageBox::warning(this, "错误", "发送失败: " + shengjiserialPort.errorString());
    }
}


void Widget::on_pushButton_6_clicked()
{
    zidongfasong = 0;
}

