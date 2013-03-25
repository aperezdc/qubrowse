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
    QApplication application(argc, argv);
    qDebug("QApplication created");

    application.setApplicationName("qubrowse");
    application.setApplicationVersion(VERSION);
    application.setOrganizationName("igalia");
    application.setOrganizationDomain("org");
    qDebug("QApplication configured");

    uBrowse browser;
    qDebug("uBrowse created");

    if (argc > 2) {
        unsigned long timeout = (argc > 3) ? strtoul(argv[3], NULL, 0) : 30;
        GCpuProfiler *profile = new GCpuProfiler(argv[2], timeout * 1000, &application);
        qDebug("Profiling to '%s' for %lus", argv[2], timeout);

        // Start the profiler (if libprofile.so has been LD_PRELOADed) once the
        // web page has completed loading, to avoid sampling the network code.
        profile->connect(browser.webView(),
                         SIGNAL(loadFinished(bool)),
                         SLOT(start()),
                         Qt::UniqueConnection);

        // Exit the application when the profiler has gathered data
        application.connect(profile,
                            SIGNAL(timeout()),
                            SLOT(quit()),
                            Qt::UniqueConnection);

        AutoScroller *scroller = new AutoScroller(browser.webView(), &application);
        scroller->setInterval(50);
        scroller->setDelta(5);
        scroller->connect(browser.webView(),
                          SIGNAL(loadFinished(bool)),
                          SLOT(setEnabled(bool)),
                          Qt::UniqueConnection);
    }

    qDebug("uBrowse shown");
    browser.load(QUrl::fromUserInput((argc > 1) ? argv[1] : "http://www.igalia.com"));
    browser.showMaximized();

    qDebug("Entering event loop");

    return application.exec();
}

