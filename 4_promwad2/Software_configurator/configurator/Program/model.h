#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QSqlQueryModel>

class Model : public QSqlQueryModel
{
    Q_OBJECT
public:

    enum Roles {
        NumberRole = Qt::UserRole + 1,
        NameRole,
        TypeRole,
        ButtonsRole,
        GroupNumberRole,
        IpRole,
        MacRole
    };

    explicit Model(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;

signals:

public slots:
};

#endif // MODEL_H
