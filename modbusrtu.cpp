#include "modbusrtu.h"

#include <QtCore>
#include <QDebug>
//---------------------------------------------------------------------------------

QString ModbusRTU::getMeterType(){ return QString("Modbus-RTU"); }

//---------------------------------------------------------------------------------

quint16 ModbusRTU::getPluginVersion(){ return PLG_VER_1_0; }

//---------------------------------------------------------------------------------

QString ModbusRTU::getMeterAddrsAndPsswdRegExp(){ return QString("%1%2").arg("^(0|[1-9][0-9]{1}|1[0-9]{2}|2[0-4][0-9]|25[0-4])$").arg("^()$"); }

//---------------------------------------------------------------------------------

quint8 ModbusRTU::getPasswdType(){ return (quint8)1; }

//---------------------------------------------------------------------------------

QString ModbusRTU::getVersion(){ return QString("Modbus-RTU v0.0.1 24.12.2016"); }

//---------------------------------------------------------------------------------

QByteArray ModbusRTU::getDefPasswd(){ return QByteArray(); }

//---------------------------------------------------------------------------------

QString ModbusRTU::getSupportedMeterList(){ return getMeterType(); }

//---------------------------------------------------------------------------------

quint8 ModbusRTU::getMaxTariffNumber(const QString &version){  Q_UNUSED(version);  return  (quint8)0 ; }

//---------------------------------------------------------------------------------

QStringList ModbusRTU::getEnrgList4thisMeter(const quint8 &pollCode, const QString &version){ Q_UNUSED(pollCode); Q_UNUSED(version); return QStringList(); }

//---------------------------------------------------------------------------------

QPair<QVariantHash, QVariantHash> ModbusRTU::messToMeter(const QPair<QVariantHash, QVariantHash> &pairAboutMeter){ Q_UNUSED(pairAboutMeter); return QPair<QVariantHash, QVariantHash >(); }

//---------------------------------------------------------------------------------

QVariantHash ModbusRTU::decodeMeterData(const QHash<QString, QVariant> &threeHash){  Q_UNUSED(threeHash);  return QHash<QString, QVariant>(); }

//---------------------------------------------------------------------------------

QVariantHash ModbusRTU::helloMeter(const QVariantHash &hashMeterConstData){ Q_UNUSED(hashMeterConstData);  return QVariantHash(); }

//---------------------------------------------------------------------------------

QString ModbusRTU::meterTypeFromMessage(QByteArray &readArr){ Q_UNUSED(readArr);  return QString(); }

//---------------------------------------------------------------------------------

QVariantHash ModbusRTU::isItYour(const QByteArray &arr)//дані які отримав координатор з послідовного порту
{
///address
/// 0 - multicast
/// 1-255 - NI
///
    QList<quint8> mas;
    quint8 masSize;

    for(int i = 0, iMax = arr.length(); i < iMax; i++)
        mas.append((quint8)arr.at(i));

    masSize = (quint8)mas.size();
    if(masSize < 4){
        qDebug() << "masSize=" << masSize << mas << arr;
        return QVariantHash();
    }

    QByteArray messCrc("");
    messCrc.prepend((quint8)mas.takeLast());
    messCrc.prepend((quint8)mas.takeLast());

    masSize -= 2;

    QByteArray frameArr("");
    for(int i = 0; i < masSize; i++)
        frameArr.append((quint8)mas.at(i));

    QByteArray crc16ccit = QByteArray::number( crc16(mas), 16).rightJustified(4, '0');
    crc16ccit = QByteArray::fromHex(crc16ccit.right(2) + crc16ccit.left(2));

    if(crc16ccit != messCrc){
        qDebug() << "crc16ccit != messCrc " << crc16ccit.toHex() << messCrc.toHex();
        return QVariantHash();
    }

    QString modemNI = (mas.first() == (quint8)0x0) ? "" : QString::number(mas.first());
    lastModemNI = modemNI;
    modemNI.append("\r\n");

    QVariantHash hash;
    hash.insert("nodeID",  modemNI);
    hash.insert("endSymb", "");
    hash.insert("quickCRC", true);
    hash.insert("readLen", 3);

    return hash;
}

//---------------------------------------------------------------------------------

QVariantHash ModbusRTU::isItYourRead(const QByteArray &arr)//дані які отримав координатор від роутера
{
    QList<quint8> mas;
    quint8 masSize;

    for(int i = 0, iMax = arr.length(); i < iMax; i++)
        mas.append((quint8)arr.at(i));

    masSize = (quint8)mas.size();
    if(masSize < 4 || mas.first() == (quint8)0x0){
        qDebug() << "masSize=" << masSize << mas << arr;
        return QVariantHash();
    }

    QByteArray messCrc("");
    messCrc.prepend((quint8)mas.takeLast());
    messCrc.prepend((quint8)mas.takeLast());

    masSize -= 2;

    QByteArray frameArr("");
    for(int i = 0; i < masSize; i++)
        frameArr.append((quint8)mas.at(i));

    QByteArray crc16ccit = QByteArray::number( crc16(mas), 16).rightJustified(4, '0');
    crc16ccit = QByteArray::fromHex(crc16ccit.right(2) + crc16ccit.left(2));

    if(crc16ccit != messCrc){
        qDebug() << "crc16ccit != messCrc " << crc16ccit.toHex() << messCrc.toHex();
        return QVariantHash();
    }

    if(!lastModemNI.isEmpty()){
        QString modemNI = (mas.first() == (quint8)0x0) ? "" : QString::number(mas.first());

        if(lastModemNI != modemNI){
            qDebug() << "lastModemNI != modemNI " << lastModemNI << modemNI;
            return QVariantHash();
        }
    }


    lastModemNI.clear();

    QVariantHash hash;
    hash.insert("Tak", true);
    return hash;
}

//---------------------------------------------------------------------------------

QByteArray ModbusRTU::niChanged(const QByteArray &arr){ Q_UNUSED(arr);  return QByteArray(); }

//---------------------------------------------------------------------------------

QVariantHash ModbusRTU::meterSn2NI(const QString &meterSn){ Q_UNUSED(meterSn); return QVariantHash(); }

//---------------------------------------------------------------------------------

QPair<QVariantHash, QVariantHash> ModbusRTU::messParamPamPam(const QPair<QVariantHash, QVariantHash> &pairAboutMeter){  Q_UNUSED(pairAboutMeter); return QPair<QVariantHash, QVariantHash >(); }

//---------------------------------------------------------------------------------

QVariantHash ModbusRTU::decodeParamPamPam(const QHash<QString, QVariant> &threeHash){  Q_UNUSED(threeHash);  return QVariantHash(); }

//---------------------------------------------------------------------------------

quint16 ModbusRTU::crc16(QList<quint8> list)
{
    quint16 crc = 0xFFFF;

    for(int pos = 0, posMax = list.size(); pos < posMax; pos++){
        crc ^= (quint16)list.at(pos); // XOR byte into least sig. byte of crc

        for(int i = 8; i != 0; i--){    // Loop over each bit
            if((crc & 0x0001) != 0){    // If the LSB is set
                crc >>= 1;              // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }else{                      // Else LSB is not set
                crc >>= 1;              // Just shift right
            }
        }
    }
    qDebug() << crc << QString("%1").arg(crc,0,16);
    return crc;
}

//---------------------------------------------------------------------------------
