#include "messageprocessor.h"
#include <QSerialPort>
#include <QApplication>  // 添加这一行

const uint8_t START_BYTE = 0x68;
const uint8_t END_BYTE = 0x16;

MessageProcessor::MessageProcessor(QObject *parent) : QObject(parent) {}

void MessageProcessor::loadMessagesFromFile(const QString &fileName)
{
    sentMessages.clear();
    receivedMessages.clear();
    currentIndex = 0;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot open file:" << fileName;
        return;
    }

    while (!file.atEnd())
    {
        QString line = file.readLine().trimmed();
        parseLine(line);
    }
    file.close();

    // 检查未匹配的发送消息
    for (int i = 0; i < sentMessages.size(); ++i)
    {
        if (!sentMessages[i].isReceived) // 如果发送消息没有被接收
        {
            if (i < receivedMessages.size())
            {
                receivedMessages[i] = {"ERROR", sentMessages[i].timestamp, static_cast<int>(i), true}; // 标记为错误
            }
            else
            {
                receivedMessages.append({"ERROR", sentMessages[i].timestamp, static_cast<int>(i), true}); // 添加错误消息
            }
        }
    }
}

void MessageProcessor::loadAPDUFromFile(const QString &fileName)
{
    sentMessages.clear();
    receivedMessages.clear();
    currentIndex = 0;
    sendindex = 0;
    recvindex = 0;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning() << "Cannot open file:" << fileName;
        return;
    }

    while (!file.atEnd())
    {
        QString line = file.readLine().trimmed();
        APDULine(line);
    }
    file.close();
    // sentMessages.clear();
    // receivedMessages.clear();
    // currentIndex = 0;

    // QFile file(fileName);
    // if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    // {
    //     qWarning() << "Cannot open file:" << fileName;
    //     return;
    // }

    // int batchSize = 1000;  // 每批次处理1000行
    // QStringList batchLines;

    // while (!file.atEnd())
    // {
    //     QString line = file.readLine().trimmed();
    //     batchLines.append(line);

    //     // 当批次满了，就处理这批数据
    //     if (batchLines.size() >= batchSize)
    //     {
    //         processBatch(batchLines);
    //         batchLines.clear(); // 清空当前批次
    //         QCoreApplication::processEvents();  // 允许处理UI更新
    //     }
    // }

    // // 处理剩余的行
    // if (!batchLines.isEmpty())
    // {
    //     processBatch(batchLines);
    // }

    // file.close();
}

// 处理每一批次的APDU数据
void MessageProcessor::processBatch(const QStringList &batchLines)
{
    for (const QString &line : batchLines)
    {
        APDULine(line);  // 处理每一行数据
    }
}

void MessageProcessor::parseLine(const QString &line)
{
    QString timestamp = line.section(' ', 0, 2); // 提取时间戳
    QByteArray buffer;
    buffer.append(QByteArray::fromHex(line.toUtf8())); // 将新接收的数据添加到缓冲区

    // 检查发送报文
    if (line.contains("发送->"))
    {
        // 提取并存储发送的消息
        processBuffer(buffer, timestamp, true);
    }
    // 检查接收报文
    else if (line.contains("接收<-"))
    {
        // 提取并存储接收的消息
        processBuffer(buffer, timestamp, false);
    }

    qDebug() << "处理后缓冲区大小:" << buffer.size(); // 打印处理后缓冲区大小
}

void MessageProcessor::APDULine(const QString &line)
{
    // 提取时间戳
    QString timestamp = line.section(' ', 0, 2);

    // 提取数据部分，从 "cTaskData =" 或 "cOutData =" 后开始
    QString hexString;

    if (line.contains("cTaskData =")) {
        hexString = line.section("cTaskData =", 1, -1).trimmed();
    } else if (line.contains("cOutData =")) {
        hexString = line.section("cOutData =", 1, -1).trimmed();
    }

    // 检查十六进制字符串的有效性
    bool isValidHex = true;
    for (QChar ch : hexString) {
        if (!ch.isDigit() && !(ch >= 'A' && ch <= 'F') && !(ch >= 'a' && ch <= 'f')) {
            isValidHex = false;
            break;
        }
    }

    if (!isValidHex || hexString.isEmpty()) {
        //qWarning() << "Invalid hex string detected: " << hexString;
        return; // 跳过无效数据
    }

    QByteArray buffer = QByteArray::fromHex(hexString.toUtf8());

    // 判断是发送数据还是接收数据
    if (line.contains("cTaskData = 0701")) {
        // 处理发送的消息
        Message msg;
        msg.index = sendindex;
        msg.timestamp = timestamp;
        msg.data = buffer;
        msg.isReceived = false;
        sentMessages.append(msg);  // 使用构造的 Message 对象
        sendindex++;
       // qWarning() << "cTaskData hex string : " << hexString;
    } else if (line.contains("cOutData = 8701")) {
        // 处理接收的消息
        Message msg;
        msg.index = recvindex;
        msg.timestamp = timestamp;
        msg.data = buffer;
        msg.isReceived = true;
        receivedMessages.append(msg);  // 使用构造的 Message 对象
        recvindex++;
       // qWarning() << "cOutData hex string : " << hexString;
    } else if (line.contains("cOperateMode =") || line.contains("cESAMID =")) {
        // 处理其他数据（如 cOperateMode, cESAMID 等）
        //qDebug() << "Found additional data: " << line;
    }
}




void MessageProcessor::processBuffer(QByteArray &buffer, const QString &timestamp, bool isSend)
{
    while (buffer.size() >= 3) // 确保缓冲区至少有三个字节
    {
        // 查找消息起始字节
        int startIndex = buffer.indexOf(START_BYTE);
        if (startIndex == -1) // 如果没有找到起始字节
        {
            buffer.clear(); // 清空缓冲区，结束处理
            return;
        }

        buffer.remove(0, startIndex); // 移除未处理的字节

        // 检查第一个字节是否是起始字节
        if (buffer.at(0) != START_BYTE)
        {
            buffer.remove(0, 1); // 移除无效字节并继续循环
            continue;
        }

        // 解析消息长度
        uint16_t length = static_cast<unsigned char>(buffer.at(1)) |
                          (static_cast<unsigned char>(buffer.at(2)) << 8);

        // 检查消息是否完整
        if (length + 2 > buffer.size() || buffer.at(length + 1) != END_BYTE)
        {
            buffer.remove(0, 1); // 移除无效字节并继续循环
            continue;
        }

        QByteArray message = buffer.left(length + 2); // 提取完整消息
        buffer.remove(0, length + 2); // 移除已处理的部分

        // 处理发送和接收消息
        if (isSend)
        {
            // 处理发送消息
            sentMessages.append({message, timestamp, currentIndex, false}); // 将消息存入发送列表
            currentIndex++; // 更新当前索引
            // 确保接收消息列表的大小与当前索引一致
            while (receivedMessages.size() < currentIndex)
            {
                receivedMessages.append({"", "", receivedMessages.size(), true}); // 添加空消息
            }
        }
        else
        {
            // 处理接收消息
            if (currentIndex > 0 && currentIndex - 1 < sentMessages.size())
            {
                auto &sentMessage = sentMessages[currentIndex - 1]; // 获取对应的发送消息
                if (!sentMessage.isReceived)
                {
                    sentMessage.isReceived = true; // 标记为已接收
                    receivedMessages[currentIndex - 1] = {message, timestamp, sentMessage.index, true}; // 存储接收的消息
                }
                else
                {
                    sentMessages.append ( {"ERROR", timestamp, sentMessage.index, true}); // 如果已接收，标记为错误
                    currentIndex++; // 更新当前索引
                }
            }
            else
            {
                // 如果没有对应的发送消息
                if (currentIndex - 1 < sentMessages.size())
                {
                    sentMessages[currentIndex - 1] = {"ERROR", timestamp, currentIndex - 1, false}; // 在发送列表中标记为错误
                }
                receivedMessages[currentIndex - 1] = {message, timestamp, currentIndex - 1, true}; // 存储接收的消息
            }
        }
    }
}
