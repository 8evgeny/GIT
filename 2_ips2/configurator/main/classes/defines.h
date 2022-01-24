#pragma once

#define IPS_LANGUGE_EVENT(ui, parent)                     \
    if (event->type() == QEvent::LanguageChange) {        \
        if (ui)                                           \
            ui->retranslateUi(parent);                    \
    }                                                     \

#define IPS_JSON_EXTRACT_OBJECT(data, val, name)          \
    auto const& val = data.value(name);                   \
    if(val.isUndefined() || !val.isObject())              \
        return false;

#define IPS_JSON_EXTRACT_ARRAY(data, val, name)           \
    auto const& val = data.value(name);                   \
    if(val.isUndefined() || !val.isArray())               \
        return false;

#define IPS_JSON_TO_SIMPLE_STR(obj, name, val)            \
    if (!json::toString(obj, name, &val))                 \
        return false;                                     \

#define IPS_JSON_TO_SIMPLE_UINT(obj, name, val)           \
    if (!json::toUint(obj, name, &val))                   \
        return false;                                     \

#define IPS_JSON_TO_STR(obj, name, val)                   \
    QString val;                                          \
    if (!json::toString(obj, name, &val))                 \
        return false;                                     \

#define IPS_JSON_TO_INT(obj, name, val)                   \
    int val = 0;                                          \
    if (!json::toInt(obj, name, &val))                    \
        return false;                                     \

#define IPS_JSON_TO_UINT(obj, name, val)                  \
    uint val = 0;                                         \
    if (!json::toUint(obj, name, &val))                   \
        return false;                                     \

#define IPS_INSTANCESIGNALS(table, tabletype, add, rem)                                      \
QObject::connect(add, &QToolButton::clicked, this, [this]() {                                \
    tabletype.insertRows(tabletype.rowCount({}), 1);                                         \
    rem->setEnabled(true);                                                                   \
    mh::simple::view::selectRow(table, tabletype.items().size()-1);                          \
});                                                                                          \
QObject::connect(table, &QTableView::clicked, this, [this](const QModelIndex& index) {       \
    mh::simple::view::selectRow(table, index.row());                                         \
});                                                                                          \
QObject::connect(rem, &QToolButton::clicked, [this] () {                                     \
    auto const& indx = mh::selected(table);                                                  \
    if (indx.isValid()) {                                                                    \
        tabletype.removeRows(indx.row(), 1);                                                 \
        auto const row = indx.row() == 0 ? 0 : indx.row() - 1;                               \
        mh::simple::view::selectRow(table, row);                                             \
        rem->setEnabled(tabletype.rowCount());                                               \
    }                                                                                        \
});                                                                                          \

