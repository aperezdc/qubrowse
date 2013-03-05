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
#include <QApplication>


int
main (int argc, char **argv)
{
    QApplication application(argc, argv);
    application.setApplicationName("qubrowse");
    application.setApplicationVersion(VERSION);
    application.setOrganizationName("igalia");
    application.setOrganizationDomain("org");

    uBrowse browser;
    browser.show();
    browser.load(QUrl::fromUserInput((argc > 1) ? argv[1] : "http://www.igalia.com"));

    return application.exec();
}

