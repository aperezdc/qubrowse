/*
 * autoscroller.h
 * Copyright (C) 2013 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef __autoscroller_h__
#define __autoscroller_h__

#include <QObject>
#include <QTimer>
#include <QPoint>

class QWebView;


class AutoScroller : public QObject
{
    Q_OBJECT

public:
    explicit AutoScroller(QWebView *webView, QObject *parent = 0);

    void reverseDirection();
    void setInterval(uint milliseconds);
    void setDelta(int delta);

public slots:
    void setEnabled(bool enabled);

private slots:
    void _doScroll();

private:
    QWebView       *_webView;
    QTimer          _timer;
    int             _delta;

    Q_DISABLE_COPY(AutoScroller);
};


#endif /* !__autoscroller_h__ */

