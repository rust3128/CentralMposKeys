#ifndef KEYSFILEMODEL_H
#define KEYSFILEMODEL_H

#include "DataKeysFileClass/datakeysfileclass.h"

#include <QObject>
#include <QAbstractTableModel>
#include <QList>

class KeysFileModel : public QAbstractTableModel
{
    Q_OBJECT
    QList<DataKeysFileClass> m_listKeys;
public:
    KeysFileModel(QList<DataKeysFileClass> ls);

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // KEYSFILEMODEL_H
