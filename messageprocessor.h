#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <QObject>
#include <QList>
#include <QByteArray>
#include <QFile>
#include <QSerialPort> // 确保正确包含
#include <QDebug>

struct Message
{
    QByteArray data;
    QString timestamp;
    int index;       // 消息的索引
    bool isReceived; // 添加标识位，初始为 false 表示未接收
};

class MessageProcessor : public QObject
{
    Q_OBJECT

public:
    explicit MessageProcessor(QObject *parent = nullptr);
    void loadMessagesFromFile(const QString &fileName);
    void loadAPDUFromFile(const QString &fileName);
    QString result;                  // 比对结果
    QList<Message> sentMessages;     // 存储发送的消息
    QList<Message> receivedMessages; // 存储接收的消息

private:
    void parseLine(const QString &line);
    void processBuffer(QByteArray &buffer, const QString &timestamp, bool isSend); // 处理缓冲区
    void APDULine(const QString &line);
    void processBatch(const QStringList &batchLines);
    QByteArray buffer; // Buffer to hold incoming data
    int currentIndex = 1;
    int sendindex = 0;
    int recvindex = 0;
    uint16_t messageIdCounter = 0; // 消息ID计数器
};

#endif // MESSAGEPROCESSOR_H
