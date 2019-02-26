/*************************************************************************
This file provides a useful functions to set up a logger for the programm.
Copyright (C) 2019  Patrik Staudenmayer

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

#include "logutils.h"

#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDir>
#include <QFileInfoList>
#include <QMutex>
#include <QMutexLocker>
#include <iostream>

namespace logutils
{
static QString logFileName;

void initLogFileName()
{
logFileName = QString(logFolderName + "/Log_%1__%2.txt")
              .arg(QDate::currentDate().toString("yyyy_MM_dd"))
              .arg(QTime::currentTime().toString("hh_mm_ss_zzz"));
}

/**
* @brief deletes old log files, only the last ones are kept
*/
void deleteOldLogs()
{
QDir dir;
dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
dir.setSorting(QDir::Time | QDir::Reversed);
dir.setPath(logFolderName);

QFileInfoList list = dir.entryInfoList();
if (list.size() <= LOGFILES_COUNT)
{
  return; //no files to delete
} else
{
  for (int i = 0; i < (list.size() - LOGFILES_COUNT); i++)
  {
    QString path = list.at(i).absoluteFilePath();
    QFile file(path);
    file.remove();
  }
}
}

bool initLogging()
{
  // Create folder for LOGFILES if not exists
  if(!QDir(logFolderName).exists())
  {
    QDir().mkdir(logFolderName);
  }

  deleteOldLogs(); //delete old log files
  initLogFileName(); //create the logfile name

  QFile outFile(logFileName);
  if(outFile.open(QIODevice::WriteOnly | QIODevice::Append))
  {
    qInstallMessageHandler(logutils::myMessageHandler);
    return true;
  }
  else
  {
    return false;
  }
}

void myMessageHandler(QtMsgType type, const QMessageLogContext &context,
                        const QString& msg)
{
    //Making logging handler threadsafe
    static QMutex mutex;
    QMutexLocker lock(&mutex);              //Lock handler. Gets unlocked when variable is destroyed



    //check file size and if needed create new log!
    {
      QFile outFileCheck(logFileName);
      long long size = outFileCheck.size();

      if (size > LOGSIZE) //check current log size
      {
        deleteOldLogs();
        initLogFileName();
      }
    }

    QFile outFile(logFileName);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);

    switch (type) {
        case QtDebugMsg:
            ts << QDate::currentDate().toString("yyyy_MM_dd") << "  " << QTime::currentTime().toString("hh_mm_ss_zzz") << "  " << "Debug: " << msg
               << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtInfoMsg:
            ts << QDate::currentDate().toString("yyyy_MM_dd") << "  " << QTime::currentTime().toString("hh_mm_ss_zzz") << "  " << "Info: " << msg
               << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtWarningMsg:
            ts << QDate::currentDate().toString("yyyy_MM_dd") << "  " << QTime::currentTime().toString("hh_mm_ss_zzz") << "  " << "Warning: " << msg
               << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtCriticalMsg:
            ts << QDate::currentDate().toString("yyyy_MM_dd") << "  " << QTime::currentTime().toString("hh_mm_ss_zzz") << "  " << "Critical: " << msg
               << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            break;
        case QtFatalMsg:
            ts << QDate::currentDate().toString("yyyy_MM_dd") << "  " << QTime::currentTime().toString("hh_mm_ss_zzz") << "  " << "Fatal: " << msg
               << " (" << context.file << ":" << context.line << ", " << context.function << ")" << endl;
            abort();
        }
}
} //end of namespace logutils
