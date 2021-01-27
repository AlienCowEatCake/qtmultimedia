/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:COMM$
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** $QT_END_LICENSE$
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
**
****************************************************************************/

#include "qmediarecordercontrol.h"

QT_BEGIN_NAMESPACE


/*!
    \class QMediaRecorderControl
    \obsolete
    \inmodule QtMultimedia


    \ingroup multimedia_control

    \brief The QMediaRecorderControl class provides access to the recording
    functionality of a QMediaService.

    Generally you will use the QMediaRecorder class in application code - this
    class is mostly used when implementing a new QMediaService or if there is
    access to specific low level functionality not otherwise present in QMediaRecorder.

    If a QMediaService can record media it will implement QMediaRecorderControl.
    This control provides a means to set the \l {outputLocation()}{output location},
    and record, pause and stop recording via the \l setState() method.  It also
    provides feedback on the \l {duration()}{duration} of the recording.

    The interface name of QMediaRecorderControl is \c org.qt-project.qt.mediarecordercontrol/5.0 as
    defined in QMediaRecorderControl_iid.

    \sa QMediaService::requestControl(), QMediaRecorder

*/

/*!
    \macro QMediaRecorderControl_iid

    \c org.qt-project.qt.mediarecordercontrol/5.0

    Defines the interface name of the QMediaRecorderControl class.

    \relates QMediaRecorderControl
*/

/*!
    Constructs a media recorder control with the given \a parent.
*/

QMediaRecorderControl::QMediaRecorderControl(QObject* parent)
    : QMediaControl(parent)
{
}

/*!
    Destroys a media recorder control.
*/

QMediaRecorderControl::~QMediaRecorderControl()
{
}

/*!
    \fn QUrl QMediaRecorderControl::outputLocation() const

    Returns the current output location being used.
*/

/*!
    \fn bool QMediaRecorderControl::setOutputLocation(const QUrl &location)

    Sets the output \a location and returns if this operation is successful.
    If file at the output location already exists, it should be overwritten.

    The \a location can be relative or empty;
    in this case the service should use the system specific place and file naming scheme.

    After recording has started, the backend should report the actual file location
    with actualLocationChanged() signal.
*/

/*!
    \fn QMediaRecorder::State QMediaRecorderControl::state() const

    Return the current recording state.
*/

/*!
    \fn QMediaRecorder::Status QMediaRecorderControl::status() const

    Return the current recording status.
*/

/*!
    \fn qint64 QMediaRecorderControl::duration() const

    Return the current duration in milliseconds.
*/

/*!
    \fn void QMediaRecorderControl::setState(QMediaRecorder::State state)

    Set the media recorder \a state.
*/

/*!
    \fn void QMediaRecorderControl::applySettings()

    Commits the encoder settings and performs pre-initialization to reduce delays when recording
    is started.
*/

/*!
    \fn bool QMediaRecorderControl::isMuted() const

    Returns true if the recorder is muted, and false if it is not.
*/

/*!
    \fn void QMediaRecorderControl::setMuted(bool muted)

    Sets the \a muted state of a media recorder.
*/

/*!
    \fn qreal QMediaRecorderControl::volume() const

    Returns the audio volume of a media recorder control.
*/

/*!
    \fn void QMediaRecorderControl::setVolume(qreal volume)

    Sets the audio \a volume of a media recorder control.

    The volume is scaled linearly, ranging from \c 0 (silence) to \c 100 (full volume).
*/

/*!
    \fn void QMediaRecorderControl::stateChanged(QMediaRecorder::State state)

    Signals that the \a state of a media recorder has changed.
*/

/*!
    \fn void QMediaRecorderControl::statusChanged(QMediaRecorder::Status status)

    Signals that the \a status of a media recorder has changed.
*/


/*!
    \fn void QMediaRecorderControl::durationChanged(qint64 duration)

    Signals that the \a duration of the recorded media has changed.

    This only emitted when there is a discontinuous change in the duration such as being reset to 0.
*/

/*!
    \fn void QMediaRecorderControl::mutedChanged(bool muted)

    Signals that the \a muted state of a media recorder has changed.
*/

/*!
    \fn void QMediaRecorderControl::volumeChanged(qreal gain)

    Signals that the audio \a gain value has changed.
*/

/*!
    \fn void QMediaRecorderControl::actualLocationChanged(const QUrl &location)

    Signals that the actual media \a location has changed.
    This signal should be emitted at start of recording.
*/

/*!
    \fn void QMediaRecorderControl::error(int error, const QString &errorString)

    Signals that an \a error has occurred.  The \a errorString describes the error.
*/

QT_END_NAMESPACE

#include "moc_qmediarecordercontrol.cpp"
