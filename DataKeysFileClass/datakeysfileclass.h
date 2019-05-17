#ifndef DATAKEYSFILECLASS_H
#define DATAKEYSFILECLASS_H

#include <QString>
#include <QDate>
#include <QByteArray>
#

class DataKeysFileClass
{
public:
    DataKeysFileClass();
    int colParams() const;
    void setColParams(int colParams);

    QString posnumber() const;
    void setPosnumber(const QString &posnumber);

    QDate datExpire() const;
    void setDatExpire(const QDate &datExpire);

    QByteArray keyData() const;
    void setKeyData(const QByteArray &keyData);

    private:

    QString m_posnumber;
    QDate m_datExpire;
    QByteArray m_keyData;
};

#endif // DATAKEYSFILECLASS_H
