#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QObject>


namespace app
{
    class MainWindow : public QObject
    {
        Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

    public slots:
        void onAppendFile(const QString& url, const QString& tittle, const QString& preview);
        void onAppendUrl(const QString& url);
        void onRemoveStory(const QString& id);

    private:
        void onPageLoaded();

    private:
        struct Impl;
        Impl* d;
    };
}  // namespace app


#endif //MAIN_WINDOW_HPP
