#include <QRegExp>
#include <QStringList>

#include "sysutils.h"

SysUtils::SysUtils(QObject *parent) :
    QObject(parent)
{
}

QString SysUtils::formatHexString(const QString &hex)
{
    QString hexStr(hex);

    QRegExp rx("([0-9A-Fa-f]{1,2})");
    QStringList list;
    int pos = 0;

    while ((pos = rx.indexIn(hexStr, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }
    return list.join(" ");
}

QString SysUtils::getHexString(QByteArray &data)
{
    QString result = "";

    if (data.isEmpty())
        return result;
    QByteArray hexData = data.toHex();
    result = SysUtils::formatHexString(QString(hexData));
    return result;
}

QString SysUtils::toLocalStr(const QString &src)
{
    return QString::fromLocal8Bit(src.toAscii());
}
