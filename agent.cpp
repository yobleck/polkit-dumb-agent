/*
    Copyright (C) 2020 Martin Sandsmark

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "agent.h"

#include <QApplication>
#include <QDBusMetaType>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QFileInfo>

//added by yobleck
#include <QStyleFactory>


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    //yobleck: simple argument handling
    QStringList qt_args = QCoreApplication::arguments();
    QString qt_style;
    if (argc > 1) {
        if (qt_args[1] == "h" || qt_args[1] == "-h" || qt_args[1] == "--help") { //yobleck: too lazy to make this more robust
            qInfo() << "Options:\n" << "\t-s,--style\t" << QStyleFactory::keys();
            return 1;
        }
        if (qt_args[1] == "-s" || qt_args[1] == "--style") {
            if (argc > 2) {
                qt_style = qt_args[2];
                qInfo() << "setting style:" << qt_style;
            }
        }
    }
    if (!qt_style.isNull()) {
        QApplication::setStyle(qt_style);
    }
    
    app.setQuitOnLastWindowClosed(false);

    // Details
    qRegisterMetaType<QMap<QString, QString>>("QMap<QString, QString>");
    qDBusRegisterMetaType<QMap<QString, QString>>();

    // Identity
    qRegisterMetaType<QPair<QString, QVariantMap>>("QPair<QString, QVariantMap>>");
    qDBusRegisterMetaType<QPair<QString, QVariantMap>>();

    // Identity list
    qRegisterMetaType<QList<QPair<QString, QVariantMap>>>("QList<QPair<QString, QVariantMap>>>");
    qDBusRegisterMetaType<QList<QPair<QString, QVariantMap>>>();

    if (!QDBusConnection::systemBus().isConnected()) {
        qDebug() << "failed to connect to system dbus bus";
        return 1;
    }

    QFileInfo responderInfo(LIBEXEC_DIR "polkit-dumb-agent-responder-style");
    if (!responderInfo.exists() || !responderInfo.isExecutable()) {
        qWarning() << "responder" << responderInfo.filePath() << "not installed properly, exists:" << responderInfo.exists() << "exec:" << responderInfo.isExecutable();
        return 1;
    }

    Agent agent;
    agent.responderPath = responderInfo.filePath();
    if (!QDBusConnection::systemBus().registerObject("/com/iskrembilen/polkitAuthAgent", &agent, QDBusConnection::ExportAllSlots)) {
        qWarning() << "Failed to register agent";
        return 1;
    }
    QString sessionId = qgetenv("XDG_SESSION_ID");
    bool isInt;
    sessionId.toInt(&isInt);

    if (!isInt) {
        qDebug() << "XDG_SESSION_ID " << sessionId << "is not a valid integer, defaulting to 1";
        sessionId = "1";
    }

    QDBusMessage regMessage = QDBusMessage::createMethodCall(
            "org.freedesktop.PolicyKit1",
            "/org/freedesktop/PolicyKit1/Authority",
            "org.freedesktop.PolicyKit1.Authority",
            "RegisterAuthenticationAgent"
        );
    regMessage.setArguments({
            QVariant::fromValue(QPair<QString, QVariantMap>("unix-session", {{"session-id", sessionId}})),
            "en_US.UTF-8",
            "/com/iskrembilen/polkitAuthAgent"
        });
    QDBusConnection::systemBus().send(regMessage);

    return app.exec();
}
