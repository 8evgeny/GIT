#pragma once
#include "include/main_pch.h"
#include "main/classes/program.h"

class StatusBar final {
    using class_name = StatusBar;

private:
    QLabel* m_text = nullptr;

public:
    explicit StatusBar(QStatusBar* statusbar = nullptr) {
        instance(statusbar);
    }

private:
    void instance(QStatusBar* bar) noexcept {
        auto statusContainer = new QWidget(bar);
        auto layout = new QHBoxLayout(statusContainer);
        m_text = new QLabel(bar);
        layout->addWidget(m_text);
        bar->addPermanentWidget(statusContainer, 1);
        m_text->setStyleSheet(IPS_STR("QLabel {color: #abb2bc; font-size: 10px; margin: -5px 0 0 0; top: 10px;}"));
        statusContainer->setStyleSheet(IPS_STR("QWidget {padding: 0 30px}"));                
    }

public:
    void setText(QString const& file)  noexcept {        
        m_text->setText(file);
    }
    void clear() noexcept {
        m_text->setText({});
    }
    void setProject(QString const& file)  noexcept {
        m_text->setText(iDesc::tr("The project is %1").arg(file));
    }
};
