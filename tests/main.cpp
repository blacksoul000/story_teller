//#include "app/main_window.hpp"

#include <QGuiApplication>
#include <iostream>

#include "domain/deti_online_handler.hpp"
#include "domain/web_skazki_handler.hpp"
#include "domain/ozornik_handler.hpp"

#include <QFile>
#include <QDebug>

bool test_0()
{
    QFile f(QCoreApplication::applicationDirPath() + "/../../tests/resources/serezik.html");
    if (!f.open(QIODevice::ReadOnly)) 
    {
        qDebug() << "Failed to open file " << f.fileName();
        return false;
    }

    domain::DetiOnlineHandler handler;
    QString buffer = f.readAll();
    if (handler.parseTittle(buffer) != "Ёжик Серёжик") return false;
    return true;
}

bool test_1()
{
    QFile f(QCoreApplication::applicationDirPath() + "/../../tests/resources/barankin.html");
    if (!f.open(QIODevice::ReadOnly)) 
    {
        qDebug() << "Failed to open file " << f.fileName();
        return false;
    }

    domain::DetiOnlineHandler handler;
    QString buffer = f.readAll();
    if (handler.parseTittle(buffer) != "Баранкин, будь человеком") return false;
    return true;
}

bool test_2()
{
    QFile f(QCoreApplication::applicationDirPath() + "/../../tests/resources/malenkiy_muk.html");
    if (!f.open(QIODevice::ReadOnly)) 
    {
        qDebug() << "Failed to open file " << f.fileName();
        return false;
    }

    domain::DetiOnlineHandler handler;
    QString buffer = f.readAll();
    if (handler.parseTittle(buffer) != "Маленький Мук") return false;
    return true;
}

int main(int argc, char** argv)
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName("StoryTeller");

    std::cout << test_0() << std::endl;
    std::cout << test_1() << std::endl;
    std::cout << test_2() << std::endl;

    return app.exec();
}
