/*
 * autoscroller.cc
 * Copyright (C) 2013 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "autoscroller.h"
#include <QWebView>
#include <QWebPage>
#include <QWebFrame>
#include <QTimer>
#include <QSize>
#include <QDebug>


AutoScroller::AutoScroller(QWebView *webView, QObject *parent):
    QObject(parent),
    _webView(webView),
    _timer(),
    _delta(15)
{
    _timer.setInterval(1000);
    connect(&_timer, SIGNAL(timeout()), SLOT(_doScroll()), Qt::UniqueConnection);
}


void AutoScroller::setEnabled(bool enabled)
{
    if (enabled)
        _timer.start();
    else
        _timer.stop();
}


void AutoScroller::reverseDirection()
{
    _delta = (- _delta);
}


void AutoScroller::setDelta(int delta)
{
    if (_delta < 0)
        delta = -delta;
    _delta = delta;
}


void AutoScroller::setInterval(uint milliseconds)
{
    _timer.setInterval(milliseconds);
}


void AutoScroller::_doScroll()
{
    const QWebPage *page(_webView->page());
    if (page) {
        QWebFrame *frame(page->mainFrame());
        if (frame) {
            const QSize size(frame->contentsSize());
            QPoint pos(frame->scrollPosition());

            int lastY(pos.y());
            pos.ry() += _delta;
            frame->setScrollPosition(pos);

            if (lastY == frame->scrollPosition().y())
                reverseDirection();
        }
    }
}
