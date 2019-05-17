#include "datakeysfileclass.h"

#define COL_PARAMS 3

DataKeysFileClass::DataKeysFileClass()
{

}

QString DataKeysFileClass::posnumber() const
{
    return m_posnumber;
}

void DataKeysFileClass::setPosnumber(const QString &posnumber)
{
    m_posnumber = posnumber;
}

QDate DataKeysFileClass::datExpire() const
{
    return m_datExpire;
}

void DataKeysFileClass::setDatExpire(const QDate &datExpire)
{
    m_datExpire = datExpire;
}

QByteArray DataKeysFileClass::keyData() const
{
    return m_keyData;
}

void DataKeysFileClass::setKeyData(const QByteArray &keyData)
{
    m_keyData = keyData;
}
