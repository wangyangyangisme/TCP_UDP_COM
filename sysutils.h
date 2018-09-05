#ifndef SYSUTILS_H
#define SYSUTILS_H

#include <QObject>

class SysUtils : public QObject
{
    Q_OBJECT
public:
    explicit SysUtils(QObject *parent = 0);

    static QString formatHexString(const QString &hex);
    static QString getHexString(QByteArray &data);
    static QString toLocalStr(const QString &src);
signals:

public slots:

};

#endif // SYSUTILS_H
