/*
 * ubrowse.h
 * Copyright (C) 2013 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef __ubrowse_h__
#define __ubrowse_h__

#include <QMainWindow>
#include <QProgressBar>
#include <QLineEdit>
#include <QWebView>
#include <QObject>

class QString;
class QUrl;


class uBrowse : public QMainWindow
{
    Q_OBJECT

    private:
        QWebView     _webview;
        QString      _title;
        QLineEdit    _urlEdit;
        QProgressBar _progress;

        void _updateWindowTitle();

    private Q_SLOTS:
        void _loadStartedEvent();
        void _loadFinishedEvent(bool);
        void _titleChangedEvent(const QString&);
        void _urlChangedEvent(const QUrl&);
        void _urlEditReturnPressed();

    public:
        uBrowse();

    public Q_SLOTS:
        void load(const QUrl&);
};

#endif /* !__ubrowse_h__ */

