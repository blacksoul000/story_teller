#include "app/main_window.hpp"

//internal
#include "domain/deti_online_handler.hpp"
#include "domain/story_group.hpp"
#include "domain/story_object.hpp"
#include "domain/story_list.hpp"
#include "domain/story_database.hpp"
#include "domain/page_handler_factory.hpp"

//Qt
#include <QCoreApplication>
#include <QStandardPaths>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlEngine>
#include <QTimer>
#include <QDebug>
#include <QDir>

//Android
#ifdef ANDROID
#include <QtAndroid>
#include <QAndroidJniObject>
#endif


using app::MainWindow;
using domain::AbstractPageHandler;


namespace
{
}


struct MainWindow::Impl
{
    QQuickView* viewer = nullptr;
    AbstractPageHandler* pageHandler = nullptr;
    domain::StoryList* storyList = nullptr;
    domain::StoryDatabase* db = nullptr;

#ifdef ANDROID
    QAndroidJniObject wakeLock;
    bool wakeLocked = false;
#endif
};

MainWindow::MainWindow():
    QObject(),
    d(new Impl)
{
    qmlRegisterUncreatableType< domain::StoryObject >("vitaliy.bondar.storyteller", 1, 0, "Story", "");
    qmlRegisterUncreatableType< domain::StoryGroup >("vitaliy.bondar.storyteller", 1, 0, "StoryGroup", "");

    qmlRegisterUncreatableType< domain::StoryList >("vitaliy.bondar.storyteller", 1, 0, "StoryList", "");

    const QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    QString dbPath = path + "/story_teller.db";

    d->db = new domain::StoryDatabase(dbPath);
    if (!d->db->isOpen()) 
    {
        qApp->quit();
        return;
    }

    // d->db = new domain::StoryDatabase(QUrl("qrc:/resources/story_teller.db").toLocalFile());
    // d->db = new domain::StoryDatabase("./story_teller.db");
    d->storyList = new domain::StoryList;
    d->viewer = new QQuickView;
    d->viewer->rootContext()->setContextProperty("storyList", d->storyList);
    d->viewer->setSource(QUrl("qrc:/qml/Main.qml"));
    d->viewer->showFullScreen();
    d->viewer->requestActivate();

#ifdef ANDROID
    QAndroidJniObject activity = QtAndroid::androidActivity();

    QAndroidJniObject serviceName = QAndroidJniObject::getStaticObjectField<jstring>(
                "android/content/Context","POWER_SERVICE");
    QAndroidJniObject powerMgr = activity.callObjectMethod("getSystemService",
                                            "(Ljava/lang/String;)Ljava/lang/Object;",
                                            serviceName.object<jobject>());
    QAndroidJniObject tag = QAndroidJniObject::fromString("Robotank");
    d->wakeLock = powerMgr.callObjectMethod("newWakeLock",
                                        "(ILjava/lang/String;)Landroid/os/PowerManager$WakeLock;",
                                        6, //SCREEN_DIM_WAKE_LOCK
                                        tag.object<jstring>());
#endif

    connect(d->viewer->engine(), &QQmlEngine::quit, qApp, &QCoreApplication::quit);
    connect(d->viewer->rootObject(), SIGNAL(appendFile(const QString&, const QString&, const QString&)), 
        this, SLOT(onAppendFile(const QString&, const QString&, const QString&)));
    connect(d->viewer->rootObject(), SIGNAL(appendUrl(const QString&)), 
        this, SLOT(onAppendUrl(const QString&)));
    connect(d->viewer->rootObject(), SIGNAL(removeStory(const QString&)), 
        this, SLOT(onRemoveStory(const QString&)));

    bool ok;
    auto stories = d->db->loadAll(&ok);
    if (!ok)
    {
        qDebug() << "Failed to load stories";
        return;
    }

    for (auto group: stories)
    {
        d->storyList->addStoryGroup(group);
    }
}

MainWindow::~MainWindow()
{
    delete d->viewer;
    delete d;
}

void MainWindow::onPageLoaded()
{
    domain::StoryGroup* group = new domain::StoryGroup(d->pageHandler->tittle(), d->pageHandler->preview());

    for (const auto& media: d->pageHandler->media())
    {
        group->addStory(new domain::StoryObject(media));
    }
    if (!d->db->addStoryGroup(group)) return;
    d->storyList->addStoryGroup(group);
    delete d->pageHandler;
    d->pageHandler = nullptr;
}

void MainWindow::onAppendFile(const QString& url, const QString& tittle, const QString& preview)
{
    qDebug() << Q_FUNC_INFO << url << tittle << preview;
    if (!QFile(url).exists() || !QFile(preview).exists())
    {
        qDebug() << "Failed to add file";
        return;
    }

    QUrl storyUrl = QUrl::fromLocalFile(url);
    QUrl storyPreview = QUrl::fromLocalFile(preview);

    domain::StoryGroup* group = new domain::StoryGroup(tittle, storyPreview);
    group->addStory(new domain::StoryObject(tittle, storyUrl, 0.0));  // TODO - duration
    d->storyList->addStoryGroup(group);
}

void MainWindow::onAppendUrl(const QString& url)
{
    qDebug() << Q_FUNC_INFO << url;
    if (d->pageHandler)
    {
        qDebug() << "Busy";
        return;
    }

    QUrl storyUrl = QUrl::fromUserInput(url);
    qDebug() << Q_FUNC_INFO << storyUrl;
    d->pageHandler = domain::PageHandlerFactory::create(storyUrl, this);
    if (!d->pageHandler)
    {
        // TODO - show error
        qDebug() << "Failed to parse url";
        return;
    }
    connect(d->pageHandler, &AbstractPageHandler::finished, this, &MainWindow::onPageLoaded);
    d->pageHandler->startRequest(storyUrl);
}

void MainWindow::onRemoveStory(const QString& id)
{
    qDebug() << Q_FUNC_INFO << id;
    if (!d->db->removeStoryGroup(id))
    {
        // TODO - show error
        qDebug() << "Failed to remove story group";
        return;
    }

    d->storyList->removeStoryGroup(id);
}
