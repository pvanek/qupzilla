/* ============================================================
* QupZilla - WebKit based browser
* Copyright (C) 2010-2014  David Rosca <nowrep@gmail.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
* ============================================================ */
#ifndef BOOKMARKS_H
#define BOOKMARKS_H

#define _bookmarksToolbar Bookmarks::toTranslatedFolder("bookmarksToolbar")
#define _bookmarksMenu Bookmarks::toTranslatedFolder("bookmarksMenu")
#define _bookmarksUnsorted Bookmarks::toTranslatedFolder("unsorted")

#include <QObject>
#include <QUrl>
#include <QImage>
#include <QVariant>

#include "qz_namespace.h"

class QIcon;

class WebView;

class QT_QUPZILLA_EXPORT Bookmarks : public QObject
{
    Q_OBJECT
public:
    explicit Bookmarks(QObject* parent = 0);

    struct Bookmark {
        int id;
        QString title;
        QString folder;
        QUrl url;
        QImage image;
        bool inSubfolder;

        Bookmark() : id(-1) , inSubfolder(false) { }

        bool operator==(const Bookmark &other) const {
            return (this->title == other.title &&
                    this->folder == other.folder &&
                    this->url == other.url &&
                    this->inSubfolder == other.inSubfolder);
        }
    };

    void loadSettings();

    bool isShowingMostVisited() const;
    void setShowingMostVisited(bool state);

    bool isShowingOnlyIconsInToolbar() const;
    void setShowingOnlyIconsInToolbar(bool state);

    bool isFolder(const QString &name);

    QString lastFolder() const;
    void setLastFolder(const QString &folder);

    bool isBookmarked(const QUrl &url);

    Bookmark getBookmark(int id);
    Bookmark getBookmark(const QUrl &url);
    QVector<Bookmark> getFolderBookmarks(const QString &name);

    bool saveBookmark(const QUrl &url, const QString &title, const QIcon &icon, const QString &folder = QLatin1String("unsorted"));
    bool saveBookmark(WebView* view, QString folder = QString());

    void removeBookmark(int id);
    void removeBookmark(const QList<int> list);

    bool editBookmark(int id, const QString &title = QString(), const QUrl &url = QUrl(), const QString &folder = QString());
    bool changeIcon(int id, const QIcon &icon);

    bool createFolder(const QString &name);
    void removeFolder(const QString &name);
    bool renameFolder(const QString &before, const QString &after);

    bool createSubfolder(const QString &name);
    bool isSubfolder(const QString &name);

    void exportToHtml(const QString &fileName);

    static QString toTranslatedFolder(const QString &name);
    static QString fromTranslatedFolder(const QString &name);

signals:
    void bookmarkAdded(const Bookmarks::Bookmark &bookmark);
    void bookmarkDeleted(const Bookmarks::Bookmark &bookmark);
    void bookmarkEdited(const Bookmarks::Bookmark &before, const Bookmarks::Bookmark &after);

    void bookmarkParentChanged(const QString &name, const QByteArray &imageData, int id,
                               const QUrl &url, const QString &oldParent, const QString &newParent);

    void folderAdded(const QString &title);
    void folderDeleted(const QString &title);

    void subfolderAdded(const QString &title);
    void folderRenamed(const QString &before, const QString &after);

    void folderParentChanged(const QString &name, bool isSubfolder);

public slots:
    void bookmarkDropedLink(const QUrl &url, const QString &title, const QVariant &imageVariant,
                            const QString &folder = QLatin1String("unsorted"), bool* ok = 0);
    void changeBookmarkParent(int id, const QString &newParent, const QString &oldParent, bool* ok = 0);
    void changeFolderParent(const QString &name, bool isSubfolder, bool* ok = 0);

private:
    bool m_showMostVisited;
    bool m_showOnlyIconsInToolbar;
    QString m_lastFolder;
};

typedef Bookmarks::Bookmark Bookmark;

// Hint to QVector to use std::realloc on item moving
Q_DECLARE_TYPEINFO(Bookmark, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(Bookmark)

#endif // BOOKMARKS_H
