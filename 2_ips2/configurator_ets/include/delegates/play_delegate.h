#pragma once
#include <QMouseEvent>
#include <QApplication>
#include <QMediaPlayer>
#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include "messages/model_tracks/tracks_model.h"
#include "messages/dialogs/messages/messages_dialog.h"

class PlayDelegate: public QStyledItemDelegate {

 public:
    using class_name = PlayDelegate;
    using inherited  = QStyledItemDelegate;
    using messages_t = MessagesDialog;

 private:
    QPixmap icon(bool is) const noexcept {
        return is
            ? QPixmap(QStringLiteral(":/svg/pause_sm.svg"))
            : QPixmap(QStringLiteral(":/svg/play_sm.svg"));
    }

 public:
    explicit PlayDelegate(messages_t* dlg, QObject* parent = nullptr)
        : QStyledItemDelegate(parent)
        , m_dlg(dlg)
    {
        QObject::connect(m_dlg, &MessagesDialog::finished, [this] () {m_player.stop();});
    }
    ~PlayDelegate() = default;
    bool editorEvent(QEvent* event, QAbstractItemModel* model, QStyleOptionViewItem const&, QModelIndex const& index) {
        if (event->type() != QEvent::MouseButtonRelease) {
            auto const is = index.data(TracksModel::TrackIsPlayRole).toBool();
            if(!is) {
                auto const& path = model->data(index, TracksModel::TrackPathRole).toString();
                m_player.setMedia(QUrl::fromLocalFile(path));
                m_player.play();
            }
            else
                m_player.stop();
            model->setData(index, !is, TracksModel::TrackIsPlayRole);
        }
        return true;
    }
    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {        
        QStyledItemDelegate::paint(painter, option, index);
        auto const is = index.data(TracksModel::TrackIsPlayRole).toBool();
        QApplication::style()->drawItemPixmap(painter, option.rect, Qt::AlignCenter, icon(is));
    }

 private:
    messages_t* m_dlg = nullptr;
    QMediaPlayer m_player;
};
