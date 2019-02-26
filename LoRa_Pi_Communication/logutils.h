/*************************************************************************
This header file provides a useful functions to set up a logger for the
programm. Copyright (C) 2019  Patrik Staudenmayer

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
****************************************************************************/

#ifndef LOGUTILS_H
#define LOGUTILS_H

#define LOGSIZE 1024 * 100 // log size in bytes
#define LOGFILES_COUNT 5

#define LOGPATH "/var/log/lora"

#include <QDate>
#include <QDebug>
#include <QObject>
#include <QString>
#include <QTime>

namespace logutils {
const QString logFolderName = LOGPATH;

bool initLogging();
void myMessageHandler(QtMsgType type, const QMessageLogContext &context,
                      const QString &msg);
} // namespace logutils

#endif // LOGUTILS_H
