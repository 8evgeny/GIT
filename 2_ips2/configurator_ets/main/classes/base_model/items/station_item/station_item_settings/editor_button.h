#pragma once
#include <QLineEdit>
#include <QCompleter>
#include <QToolButton>
#include <QHBoxLayout>
#include "include/typedefs.h"
#include "dialogs/stringlist/string_list.h"
#include "ui_station_item_settings.h"
#include "include/settings/last_settings.h"
#include "include/validators/contains_validator.h"
#include "messages/model_tracks/tracks_model.h"
#include "messages/model_tracks/tracks_filter_model.h"
#include "classes/app.h"
#include "classes/json.h"
#include "include/log.h"

IPS_UI(StationsItemSettings)

class EditorButton : public QWidget {
    Q_OBJECT

 public:
    using class_name = EditorButton;
    using inherited  = QWidget;
    using ui_t       = Ui::StationItemSettings;

 public:
    explicit EditorButton(ui_t* ui, QWidget* parent = nullptr)
        : inherited(parent)
        , m_ui(ui)
        , m_dlg(new StringListDialog)
        , m_model(new TracksModel())
        , m_fmodel(new TracksFilterModel())
    {                    
        m_pretones = getTracksNumbers();        
        m_fmodel->setSourceModel(m_model);
        m_completer = new QCompleter(m_pretones, this);
        m_completer->setModel(m_fmodel);
        instanceWidgets();
        instanceConnections();
    }
    ~EditorButton() {
        delete m_line;
        delete m_button;
        delete m_layout;
        delete m_dlg;    
        delete m_model;
        delete m_fmodel;
        delete m_completer;
    }
    void instanceWidgets() {
        m_line = new QLineEdit(this);
        m_line->setValidator(new ContainsValidator(m_pretones));
        m_button = new QToolButton(this);
        m_layout = new QHBoxLayout(this);
        m_layout->setMargin(0);
        m_layout->setSpacing(0);
        m_layout->addWidget(m_line);
        m_layout->addWidget(m_button);
        m_button->setText(QStringLiteral("..."));
        m_line->setCompleter(m_completer);
        m_line->completer()->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
        m_button->setFocusProxy(m_line);
        setFocusProxy(m_line);
    }
    void instanceConnections() {
        QObject::connect(m_line, &QLineEdit::editingFinished, this, [this] {
            if(m_close)
                emit editingFinished();
        });
        QObject::connect(m_line->completer(), QOverload<const QModelIndex &>::of(&QCompleter::activated), [=] {
            m_save = m_line->text();
            if(m_close) {
                emit editingFinished();
            }
        });
        QObject::connect(m_line, &QLineEdit::textChanged, this, &EditorButton::textChanged);
        QObject::connect(m_button, &QToolButton::clicked, this, [this]() {
            m_close = false;            
            m_dlg = new StringListDialog(iFunction::tr("Pretone Id"), m_pretones, m_line->text());
            if(m_dlg->exec() == StringListDialog::Accepted) {
                 setText(m_dlg->current());
                 m_save = m_dlg->current();
            }
            m_close = true;
        });
        QObject::connect(m_ui->m_key_pretone_play, &QLineEdit::textChanged, this, [this] (QString const & text) {
            m_fmodel->setFilter(text);
            m_line->setText(text);
        });
    }
    QStringList getTracksNumbers() noexcept {
        auto&& file = settings::allProjectsFile();
        auto const& json = json::load(file.first()).object();
        if (!json.isEmpty()) {
            auto const& tracks = json.value(IPS_STR("Messages"));
            auto arr = tracks.toArray();
            if (!arr.isEmpty())
                m_model->deserialize(arr);
        }
        QStringList list;
        for (auto i=1; i <= m_model->rowCount(); ++i)
            list.append(QString::number(i));

        return list;
    }    
    void setText(QString const& text) noexcept {
            m_line->clear();
            m_line->setText(text);
            m_save = text;
    }

 signals:
    void editingFinished();
    void textChanged(QString const& text);

 private:    
    ui_t* m_ui = nullptr;
    QString m_save;
    QLineEdit* m_line;
    QToolButton* m_button;
    QHBoxLayout* m_layout;
    QStringList m_pretones = {};
    StringListDialog* m_dlg = nullptr;
    bool m_close = true;
    TracksModel* m_model = nullptr;
    TracksFilterModel* m_fmodel = nullptr;
    QCompleter* m_completer = nullptr;
};
