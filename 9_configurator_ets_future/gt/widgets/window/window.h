#pragma once
#include <QIcon>
#include <QString>
#include <QDialog>
#include <QApplication>
#include <ps2/qt/widgets/algorithm.h>
#include <ps2/qt/settings/default_settings.h>
#include <ps2/qt/settings/default_settings_variant.h>
#include <main/app.h>
#include <main/consts.h>
#include <main/typedefs.h>

/** begin namespace gt::widget */
namespace gt::widget {

template<class T>
class Window {
 public:
    using type_t     = T;
    using class_name = Window<T>;
    using settings_t = ps2::DefaultSettings;
    using variant_t  = ps2::default_settings::Variant;
    using ilist_t    = std::initializer_list<variant_t>;
    using default_t  = ps2::default_settings::WidgetGeometryType;

 private:
    type_t*    m_window = Q_NULLPTR;
    QString    m_name;
    settings_t m_settings;

 protected:
    virtual void doSettingsRead() = 0;
    virtual void doSettingsWrite() = 0;
    virtual void doSingleShotRead(ilist_t list) = 0;
    virtual void doSingleShotWrite(ilist_t list) = 0;

 protected:
    type_t const* window() const noexcept {
        return m_window;
    }
    type_t* window() noexcept {
        return m_window;
    }
    settings_t* settings() {
        return &m_settings;
    }

 public:
    explicit Window() Q_DECL_EQ_DEFAULT;
    virtual ~Window() Q_DECL_EQ_DEFAULT;

    QString const& name() const noexcept {
        return m_name;
    }
    void setName(QString const& name) noexcept {
        m_name = name;
    }
    void setName(QString&& name) noexcept {
        m_name = qMove(name);
    }
    /**
     * \code
     *      gt::dialog::init(this, name(), title, {{this, 400}});
     *      gt::dialog::init(this, name(), m_title, {{this, 600}, {QStringLiteral("table"), ui->m_table}});
     *      gt::dialog::init(this, QStringLiteral("Notepad"), iTitle::tr("Notepad"), {{this, ps2::Fraction3{2}}});
     * \endcode
    **/
    void init(type_t* window, QString const& name, QString const& title, ilist_t list) {
        m_window = window;
        m_name   = name;
        m_window->setWindowTitle(ps2::windowTitle(title));
        m_window->setWindowIcon(ico::program());

        if(list.size())
            updateSize(list);
    }
    /**
     * \code
     *      gt::dialog::init({{this, 400}});
     *      gt::dialog::init({{this, default_t::Adjusted}});
     * \endcode
    **/
    void updateSize(ilist_t list) noexcept {
        m_settings = settings_t(list);
        doSettingsRead();
    }
    void fixMinSize(type_t* window) noexcept {
        window->adjustSize();
        window->setMinimumSize(window->size());
    }
    void settingsWrite() noexcept {
        doSettingsWrite();
    }
    void settingsRead() noexcept {
        doSettingsRead();
    }
    inline void singleShotRead(ilist_t list) noexcept {
        doSingleShotRead(list);
    }
    inline void singleShotWrite(ilist_t list) noexcept {
        doSingleShotWrite(list);
    }
};

} // end namespace gt::widget
