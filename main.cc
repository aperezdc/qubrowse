/*
 * main.cc
 * Copyright (C) 2013 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef VERSION_MAJOR
#define VERSION_MAJOR 0
#endif // !VERSION_MAJOR

#ifndef VERSION_MINOR
#define VERSION_MINOR 0
#endif // !VERSION_MINOR

#define __STRINGIFY(x) #x

#define VERSION (__STRINGIFY (VERSION_MAJOR) "." __STRINGIFY (VERSION_MINOR))


#include "ubrowse.h"
#include "autoscroller.h"
#include "gcpuprofiler.h"
#include <QApplication>
#include <QDebug>


int
main (int argc, char **argv)
{
    QApplication *application = new QApplication(argc, argv);
    qDebug("QApplication created");

    application->setApplicationName("qubrowse");
    application->setApplicationVersion(VERSION);
    application->setOrganizationName("igalia");
    application->setOrganizationDomain("org");
    qDebug("QApplication configured");

    GCpuProfiler profile("qubrowse.prof");
    qDebug("GCpuProfiler created");

    uBrowse *browser = new uBrowse();
    qDebug("uBrowse created");

    profile.connect(browser->webView(),
                    SIGNAL(loadFinished(bool)),
                    SLOT(start()),
                    Qt::UniqueConnection);

    AutoScroller *scroller = new AutoScroller(browser->webView());
    scroller->setInterval(50);
    scroller->setDelta(5);
    scroller->connect(browser->webView(),
                      SIGNAL(loadFinished(bool)),
                      SLOT(setEnabled(bool)),
                      Qt::UniqueConnection);
    qDebug("Scroller created");

    browser->show();
    qDebug("uBrowse shown");
    browser->load(QUrl::fromUserInput((argc > 1) ? argv[1] : "http://www.igalia.com"));

    qDebug("Entering event loop");
    int result = application->exec();

    profile.flush();
    return result;
}

