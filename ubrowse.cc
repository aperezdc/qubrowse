/*
 * ubrowse.cc
 * Copyright (C) 2013 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "ubrowse.h"
#include <QToolBar>
#include <QStatusBar>
#include <QDebug>


static const struct {
    QWebSettings::WebAttribute attribute;
    bool                       value;
} s_attributes[] = {
    { QWebSettings::JavascriptEnabled,      true  },
    { QWebSettings::JavaEnabled,            false },
    { QWebSettings::PluginsEnabled,         false },
    { QWebSettings::FrameFlatteningEnabled, true  },
};


uBrowse::uBrowse():
    QMainWindow(),
    _webview(),
    _title("uBrowse"),
    _urlEdit()
{
    for (uint i = 0; i < sizeof(s_attributes) / sizeof(s_attributes[0]); i++)
        _webview.settings()->setAttribute(s_attributes[i].attribute, s_attributes[i].value);

    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setUnifiedTitleAndToolBarOnMac(true);
    setCentralWidget(&_webview);

    _urlEdit.setSizePolicy(QSizePolicy::Expanding,
                           _urlEdit.sizePolicy().verticalPolicy());
    connect(&_urlEdit,
            SIGNAL(returnPressed()),
            SLOT(_urlEditReturnPressed()),
            Qt::UniqueConnection);

    QStatusBar *statusbar(statusBar());
    statusbar->addPermanentWidget(&_progress);
    statusbar->setSizeGripEnabled(false);
    statusbar->setVisible(false);

    _progress.connect(&_webview,
                    SIGNAL(loadProgress(int)),
                    SLOT(setValue(int)),
                    Qt::UniqueConnection);

    QToolBar *toolbar(addToolBar("Navigation"));
    toolbar->addAction(_webview.pageAction(QWebPage::Back));
    toolbar->addAction(_webview.pageAction(QWebPage::Forward));
    toolbar->addWidget(&_urlEdit);
    toolbar->addAction(_webview.pageAction(QWebPage::Stop));
    toolbar->addAction(_webview.pageAction(QWebPage::Reload));

    _updateWindowTitle();

    connect(&_webview,
            SIGNAL(titleChanged(const QString&)),
            SLOT(_titleChangedEvent(const QString&)),
            Qt::UniqueConnection);

    connect(&_webview,
            SIGNAL(loadStarted()),
            SLOT(_loadStartedEvent()),
            Qt::UniqueConnection);

    connect(&_webview,
            SIGNAL(loadFinished(bool)),
            SLOT(_loadFinishedEvent(bool)),
            Qt::UniqueConnection);

    connect(&_webview,
            SIGNAL(urlChanged(const QUrl&)),
            SLOT(_urlChangedEvent(const QUrl&)),
            Qt::UniqueConnection);
}


void
uBrowse::_titleChangedEvent(const QString& title)
{
    _title = title;
    _updateWindowTitle();
}


void
uBrowse::_urlChangedEvent(const QUrl&)
{
    _urlEdit.setText(_webview.url().toString());
    _updateWindowTitle();
}


void
uBrowse::_loadStartedEvent()
{
    _title.clear();
    _updateWindowTitle();
    statusBar()->setVisible(true);
    statusBar()->showMessage("Loading...");
}


void
uBrowse::_loadFinishedEvent(bool)
{
    statusBar()->setVisible(false);
    _updateWindowTitle();
    qDebug("Loading finished");
}


void
uBrowse::_updateWindowTitle()
{
    static const QString progressFormat(" - %1%");
    QString newTitle("uBrowse");

    if (_title.isEmpty()) {
        const QUrl& url(_webview.url());
        if (!url.isEmpty()) {
            newTitle.append(" - ");
            newTitle.append(url.toString());
        }
    }
    else {
        newTitle.append(" - ");
        newTitle.append(_title);
    }
    setWindowTitle(newTitle);
}


void
uBrowse::_urlEditReturnPressed()
{
    load(QUrl::fromUserInput(_urlEdit.text()));
}


void
uBrowse::load(const QUrl& url)
{
    _webview.load(url);
    _urlChangedEvent(url);
    _webview.setFocus();
}
