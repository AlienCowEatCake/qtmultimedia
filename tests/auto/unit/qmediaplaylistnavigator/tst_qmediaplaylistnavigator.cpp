/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

//TESTED_COMPONENT=src/multimedia

#include <QtTest/QtTest>
#include <QDebug>
#include <private/qlocalmediaplaylistprovider_p.h>
#include <private/qmediaplaylistnavigator_p.h>

QT_USE_NAMESPACE
class tst_QMediaPlaylistNavigator : public QObject
{
    Q_OBJECT
public slots:
    void init();
    void cleanup();

private slots:
    void construction();
    void setPlaylist();
    void linearPlayback();
    void loopPlayback();
    void currentItemOnce();
    void currentItemInLoop();
    void randomPlayback();

    void testItemAt();
    void testNextIndex();
    void testPreviousIndex();
    void testCurrentIndexChangedSignal();
    void testPlaybackModeChangedSignal();
    void testSurroundingItemsChangedSignal();
    void testActivatedSignal();
};

void tst_QMediaPlaylistNavigator::init()
{
    qRegisterMetaType<QMediaPlaylist::PlaybackMode>("QMediaPlaylist::PlaybackMode");
    qRegisterMetaType<QMediaContent>("QMediaContent");
}

void tst_QMediaPlaylistNavigator::cleanup()
{
}

void tst_QMediaPlaylistNavigator::construction()
{
    QLocalMediaPlaylistProvider playlist;
    QCOMPARE(playlist.mediaCount(), 0);

    QMediaPlaylistNavigator navigator(&playlist);
    QVERIFY(navigator.currentItem().isNull());
    QCOMPARE(navigator.currentIndex(), -1);
}

void tst_QMediaPlaylistNavigator::setPlaylist()
{
    QMediaPlaylistNavigator navigator(0);
    QVERIFY(navigator.playlist() != 0);
    QCOMPARE(navigator.playlist()->mediaCount(), 0);
    QCOMPARE(navigator.playlist()->media(0), QMediaContent());
    QVERIFY(navigator.playlist()->isReadOnly() );

    QLocalMediaPlaylistProvider playlist;
    QCOMPARE(playlist.mediaCount(), 0);

    navigator.setPlaylist(&playlist);
    QCOMPARE(navigator.playlist(), (QMediaPlaylistProvider*)&playlist);
    QCOMPARE(navigator.playlist()->mediaCount(), 0);
    QVERIFY(!navigator.playlist()->isReadOnly() );
}

void tst_QMediaPlaylistNavigator::linearPlayback()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);

    navigator.setPlaybackMode(QMediaPlaylist::Sequential);
    QTest::ignoreMessage(QtWarningMsg, "QMediaPlaylistNavigator: Jump outside playlist range ");
    navigator.jump(0);//it's ok to have warning here
    QVERIFY(navigator.currentItem().isNull());
    QCOMPARE(navigator.currentIndex(), -1);

    QMediaContent content1(QUrl(QLatin1String("file:///1")));
    playlist.addMedia(content1);
    navigator.jump(0);
    QVERIFY(!navigator.currentItem().isNull());

    QCOMPARE(navigator.currentIndex(), 0);
    QCOMPARE(navigator.currentItem(), content1);
    QCOMPARE(navigator.nextItem(), QMediaContent());
    QCOMPARE(navigator.nextItem(2), QMediaContent());
    QCOMPARE(navigator.previousItem(), QMediaContent());
    QCOMPARE(navigator.previousItem(2), QMediaContent());

    QMediaContent content2(QUrl(QLatin1String("file:///2")));
    playlist.addMedia(content2);
    QCOMPARE(navigator.currentIndex(), 0);
    QCOMPARE(navigator.currentItem(), content1);
    QCOMPARE(navigator.nextItem(), content2);
    QCOMPARE(navigator.nextItem(2), QMediaContent());
    QCOMPARE(navigator.previousItem(), QMediaContent());
    QCOMPARE(navigator.previousItem(2), QMediaContent());

    navigator.jump(1);
    QCOMPARE(navigator.currentIndex(), 1);
    QCOMPARE(navigator.currentItem(), content2);
    QCOMPARE(navigator.nextItem(), QMediaContent());
    QCOMPARE(navigator.nextItem(2), QMediaContent());
    QCOMPARE(navigator.previousItem(), content1);
    QCOMPARE(navigator.previousItem(2), QMediaContent());

    navigator.jump(0);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), 1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.next();//jump to the first item
    QCOMPARE(navigator.currentIndex(), 0);

    navigator.previous();
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.previous();//jump to the last item
    QCOMPARE(navigator.currentIndex(), 1);
}

void tst_QMediaPlaylistNavigator::loopPlayback()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);

    navigator.setPlaybackMode(QMediaPlaylist::Loop);
    QTest::ignoreMessage(QtWarningMsg, "QMediaPlaylistNavigator: Jump outside playlist range ");
    navigator.jump(0);
    QVERIFY(navigator.currentItem().isNull());
    QCOMPARE(navigator.currentIndex(), -1);

    QMediaContent content1(QUrl(QLatin1String("file:///1")));
    playlist.addMedia(content1);
    navigator.jump(0);
    QVERIFY(!navigator.currentItem().isNull());

    QCOMPARE(navigator.currentIndex(), 0);
    QCOMPARE(navigator.currentItem(), content1);
    QCOMPARE(navigator.nextItem(), content1);
    QCOMPARE(navigator.nextItem(2), content1);
    QCOMPARE(navigator.previousItem(), content1);
    QCOMPARE(navigator.previousItem(2), content1);

    QMediaContent content2(QUrl(QLatin1String("file:///2")));
    playlist.addMedia(content2);
    QCOMPARE(navigator.currentIndex(), 0);
    QCOMPARE(navigator.currentItem(), content1);
    QCOMPARE(navigator.nextItem(), content2);
    QCOMPARE(navigator.nextItem(2), content1); //loop over end of the list
    QCOMPARE(navigator.previousItem(), content2);
    QCOMPARE(navigator.previousItem(2), content1);

    navigator.jump(1);
    QCOMPARE(navigator.currentIndex(), 1);
    QCOMPARE(navigator.currentItem(), content2);
    QCOMPARE(navigator.nextItem(), content1);
    QCOMPARE(navigator.nextItem(2), content2);
    QCOMPARE(navigator.previousItem(), content1);
    QCOMPARE(navigator.previousItem(2), content2);

    navigator.jump(0);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), 1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), 0);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), 1);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), 0);
}

void tst_QMediaPlaylistNavigator::currentItemOnce()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);

    navigator.setPlaybackMode(QMediaPlaylist::CurrentItemOnce);

    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::CurrentItemOnce);
    QCOMPARE(navigator.currentIndex(), -1);

    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///2"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///3"))));

    QCOMPARE(navigator.currentIndex(), -1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), -1);

    navigator.jump(1);
    QCOMPARE(navigator.currentIndex(), 1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.jump(1);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), -1);
}

void tst_QMediaPlaylistNavigator::currentItemInLoop()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);

    navigator.setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);

    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::CurrentItemInLoop);
    QCOMPARE(navigator.currentIndex(), -1);

    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///2"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///3"))));

    QCOMPARE(navigator.currentIndex(), -1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.jump(1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), 1);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), 1);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), 1);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), 1);
}

void tst_QMediaPlaylistNavigator::randomPlayback()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);

    navigator.setPlaybackMode(QMediaPlaylist::Random);

    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///2"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///3"))));

    playlist.shuffle();

    QCOMPARE(navigator.currentIndex(), -1);
    navigator.next();
    int pos1 = navigator.currentIndex();
    navigator.next();
    int pos2 = navigator.currentIndex();
    navigator.next();
    int pos3 = navigator.currentIndex();

    QVERIFY(pos1 != -1);
    QVERIFY(pos2 != -1);
    QVERIFY(pos3 != -1);

    navigator.previous();
    QCOMPARE(navigator.currentIndex(), pos2);
    navigator.next();
    QCOMPARE(navigator.currentIndex(), pos3);
    navigator.next();
    int pos4 = navigator.currentIndex();
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), pos3);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), pos2);
    navigator.previous();
    QCOMPARE(navigator.currentIndex(), pos1);
    navigator.previous();
    int pos0 = navigator.currentIndex();
    QVERIFY(pos0 != -1);
    navigator.next();
    navigator.next();
    navigator.next();
    navigator.next();
    QCOMPARE(navigator.currentIndex(), pos4);

}

void tst_QMediaPlaylistNavigator::testItemAt()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);
    navigator.setPlaybackMode(QMediaPlaylist::Random);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    //Adding the media to the playlist
    QMediaContent content = QMediaContent(QUrl(QLatin1String("file:///1")));
    playlist.addMedia(content);

    //Currently it is not pointing to any index , Returns Null mediacontent
    QCOMPARE(navigator.currentIndex(), -1);
    QCOMPARE(navigator.itemAt(navigator.currentIndex()),QMediaContent());
    navigator.next();

    //Points to the added media
    int pos1 = navigator.currentIndex();
    QCOMPARE(content,navigator.itemAt(pos1));
}

void tst_QMediaPlaylistNavigator::testNextIndex()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);
    navigator.setPlaybackMode(QMediaPlaylist::Random);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    //Adding the media to the playlist
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///2"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///3"))));

    playlist.shuffle();

    //Currently it is not pointing to any index
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.next();
    int pos1 = navigator.currentIndex();
    //Pointing to the next index
    navigator.next();
    int pos2 = navigator.currentIndex();
    navigator.next();
    int pos3 = navigator.currentIndex();

    //Pointing to the previous index
    navigator.previous();
    QCOMPARE(navigator.nextIndex(1), pos3);
    navigator.previous();
    QCOMPARE(navigator.nextIndex(1), pos2);
    QCOMPARE(navigator.nextIndex(2), pos3);
    navigator.previous();
    QCOMPARE(navigator.nextIndex(1), pos1);
}

void tst_QMediaPlaylistNavigator::testPreviousIndex()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);
    navigator.setPlaybackMode(QMediaPlaylist::Random);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    //Adding the media to the playlist
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///2"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///3"))));
    playlist.shuffle();

    //Currently it is not pointing to any index
    QCOMPARE(navigator.currentIndex(), -1);

    //pointing to next index
    navigator.next();
    int pos1 = navigator.currentIndex();
    navigator.next();
    int pos2 = navigator.currentIndex();
    navigator.next();
    int pos3 = navigator.currentIndex();
    QCOMPARE(navigator.previousIndex(1), pos2);
    QCOMPARE(navigator.previousIndex(2), pos1);
    navigator.next();
    QCOMPARE(navigator.previousIndex(1), pos3);
}

void tst_QMediaPlaylistNavigator::testCurrentIndexChangedSignal()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);
    navigator.setPlaybackMode(QMediaPlaylist::Random);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    //Creating a QSignalSpy object for currentIndexChanged() signal
    QSignalSpy spy(&navigator,SIGNAL(currentIndexChanged(int)));
    QVERIFY(spy.count() == 0);

    //Adding the media to the playlist
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///2"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///3"))));

    //Currently it is not pointing to any index
    QCOMPARE(navigator.currentIndex(), -1);
    navigator.next();
    QVERIFY(spy.count() == 1);
    int pos1 = navigator.currentIndex();
    //Pointing to the next index
    navigator.next();
    QVERIFY(navigator.previousIndex(1) == pos1);
    QVERIFY(spy.count() == 2);
}

void tst_QMediaPlaylistNavigator::testPlaybackModeChangedSignal()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);
    navigator.setPlaybackMode(QMediaPlaylist::Random);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    //Creating a QSignalSpy object for currentIndexChanged() signal
    QSignalSpy spy(&navigator,SIGNAL(playbackModeChanged(QMediaPlaylist::PlaybackMode)));
    QVERIFY(spy.count() == 0);

    //Adding the media to the playlist
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));

    //set the play back mode to sequential
    navigator.setPlaybackMode(QMediaPlaylist::Sequential);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Sequential);
    QVERIFY(spy.count() == 1);

    //set the play back mode to loop
    navigator.setPlaybackMode(QMediaPlaylist::Loop);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Loop);
    QVERIFY(spy.count() == 2);
}

void tst_QMediaPlaylistNavigator::testSurroundingItemsChangedSignal()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);
    navigator.setPlaybackMode(QMediaPlaylist::Random);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    //Creating a QSignalSpy object for surroundingItemsChanged()signal
    QSignalSpy spy(&navigator,SIGNAL(surroundingItemsChanged()));
    QVERIFY(spy.count() == 0);

    //Adding the media to the playlist
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    QVERIFY(spy.count() == 1);

    //set the play back mode to sequential
    navigator.setPlaybackMode(QMediaPlaylist::Sequential);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Sequential);
    QVERIFY(spy.count() == 2);

    //Point to the next index
    navigator.next();
    QVERIFY(spy.count() == 3);
}

void tst_QMediaPlaylistNavigator::testActivatedSignal()
{
    QLocalMediaPlaylistProvider playlist;
    QMediaPlaylistNavigator navigator(&playlist);
    navigator.setPlaybackMode(QMediaPlaylist::Random);
    QCOMPARE(navigator.playbackMode(), QMediaPlaylist::Random);
    QCOMPARE(navigator.currentIndex(), -1);

    //Creating a QSignalSpy object for surroundingItemsChanged()signal
    QSignalSpy spy(&navigator,SIGNAL(activated(QMediaContent)));
    QVERIFY(spy.count() == 0);

    //Adding the media to the playlist
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///1"))));
    playlist.addMedia(QMediaContent(QUrl(QLatin1String("file:///2"))));
    playlist.shuffle();

    //Point to the next index
    navigator.next();
    QVERIFY(spy.count() == 1);

    //Jump to 0th item
    navigator.jump(0);
    QVERIFY(spy.count() == 2);

    //move to previous item
    navigator.previous();
    QVERIFY(spy.count() == 3);
}

QTEST_MAIN(tst_QMediaPlaylistNavigator)
#include "tst_qmediaplaylistnavigator.moc"
