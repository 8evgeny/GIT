#pragma once
#include <QTimer>
#include <QPixmap>
#include "gt/typedefs.h"
#include "ui_message_box.h"

/**
 * \code
    gt::MessBox::info(this, iMessage::tr("File '%11 has been saved sucsessfully.").arg(file));
    gt::MessBox::error(this, iMessage::tr("An error occurrated while saving the '%1' file!").arg(file));
 *
 * \endcode
**/
/** \namespace gt */
namespace gt {

class MessBox: public QDialog {
    Q_DISABLE_COPY(MessBox)

 private:
    explicit MessBox(QWidget* parent, QPixmap const& icon, QString const& text) 
        : QDialog(parent)
        , ui(new Ui::MessBox)
        , m_timer(new QTimer(parent)) {
        ui->setupUi(this);
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
        setAttribute(Qt::WA_DeleteOnClose, true);
        ui->l_text->setText(text);
        ui->l_icon->setPixmap(icon);

        if (parent)
           setGeometry(parent->geometry().right() - width(), parent->geometry().bottom() - height(), width(), height());

        QTimer::singleShot(3000, this, SLOT(close()));
        QObject::connect(m_timer, &QTimer::timeout, this, [this]() {
            if (m_timer->isActive())
                m_timer->stop();
        });
        m_timer->start(2000);
    }

 public:
    virtual ~MessBox() Q_DECL_OVERRIDE {
        delete m_timer;
        delete ui;
    }
    static void info(QWidget* parent, QString const& text) noexcept {
        auto dlg = new MessBox(parent, QPixmap(GT_STR(":/svg/status_ok.svg")), text);
        dlg->show();
    }
    static void error(QWidget* parent, QString const& text) noexcept {
        auto dlg = new MessBox(parent, QPixmap(GT_STR(":/svg/status_error.svg")), text);
        dlg->show();
    }

 private:
    Ui::MessBox* ui = nullptr;
    QTimer* m_timer = nullptr;
};
} // end namespace gt

