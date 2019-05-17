#include "KeysFileModel/keysfilemodel.h"

KeysFileModel::KeysFileModel(QList<DataKeysFileClass> ls) :
    m_listKeys(ls)
{

}


int KeysFileModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_listKeys.size();
}

int KeysFileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 3;
}

QVariant KeysFileModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()) return QVariant();
    DataKeysFileClass dat = m_listKeys[index.row()];
    switch (role) {
    case Qt::DisplayRole:
        switch (index.column()) {
        case 0: return dat.posnumber();
        case 1: return dat.datExpire();
        case 2: return dat.keyData();
        }
        break;
    default:
        break;
    }
    return QVariant();
}

QVariant KeysFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if( role != Qt::DisplayRole) { return QVariant();}

    if( orientation == Qt::Vertical ) { return section; }

    switch (section) {
    case 0: return "№ ЭККР";
    case 1: return "Срок действия";
    case 2: return "Ключ";
    default:
        break;
    }
    return QVariant();
}
