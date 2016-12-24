#ifndef MODBUSRTU_H
#define MODBUSRTU_H

#include <QObject>

#include <QtPlugin>
#include <QPair>
#include <QVariantHash>

//#define FULL_AMR 1
//#define SAMPLE_AMR 1

//#define PLG_SAMPLE  0 //0 - full plagin , 1 - only itIsYour and itIsYourRead + nessasory some functions
#define PLG_BBB 1

#ifdef PLG_BBB
#include "meterplugin.h"
#else
#ifndef Q_OS_WIN
//#include "../zbyralko_emb_k/meterplugin.h"
#include "../zbyralko_emb_k/meterplugin.h"
#else
#include "../../Zbyrator/zbyrator/meterplugin.h"
#endif
#endif



class ModbusRTU : public QObject, MeterPlugin
{

    Q_OBJECT

   #if QT_VERSION >= 0x050000
       Q_PLUGIN_METADATA(IID "ua.zbyralko.hello_zb.MeterPlugin" FILE "zbyralko.json")
   #endif
       Q_INTERFACES(MeterPlugin)

   public:
       QString getMeterType() ;

       quint16 getPluginVersion() ;

       QString getMeterAddrsAndPsswdRegExp();

       quint8 getPasswdType() ;

       QString getVersion() ;

       QByteArray getDefPasswd() ;

       QString getSupportedMeterList() ;

       quint8 getMaxTariffNumber(const QString &version) ;

       QStringList getEnrgList4thisMeter(const quint8 &pollCode, const QString &version) ;


       QPair< QVariantHash, QVariantHash > messToMeter(const QPair< QVariantHash, QVariantHash > &pairAboutMeter) ;

       QVariantHash decodeMeterData(const QHash<QString, QVariant> &threeHash);

       QVariantHash helloMeter(const QVariantHash &hashMeterConstData);

       QString meterTypeFromMessage(QByteArray &readArr);

       QVariantHash isItYour(const QByteArray &arr);

       QVariantHash isItYourRead(const QByteArray &arr);

       QByteArray niChanged(const QByteArray &arr);

       QVariantHash meterSn2NI(const QString &meterSn);

       QPair<QString,QString> niChandedFixedNI();



       //parametryzatsiya
       QPair< QVariantHash, QVariantHash > messParamPamPam(const QPair< QVariantHash, QVariantHash > &pairAboutMeter);
       QVariantHash decodeParamPamPam(const QHash<QString, QVariant> &threeHash) ;


private:
       quint16 crc16(QList<quint8> list);
       QString lastModemNI;

};

#endif // MODBUSRTU_H
