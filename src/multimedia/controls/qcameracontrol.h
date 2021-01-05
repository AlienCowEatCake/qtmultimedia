/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QCAMERACONTROL_H
#define QCAMERACONTROL_H

#include <QtMultimedia/qmediaservice.h>
#include <QtMultimedia/qmediasource.h>

#include <QtMultimedia/qcamera.h>

QT_BEGIN_NAMESPACE

// Required for QDoc workaround
class QString;

class Q_MULTIMEDIA_EXPORT QCameraControl : public QObject
{
    Q_OBJECT

public:
    enum PropertyChangeType {
        CaptureMode = 1,
        ImageEncodingSettings = 2,
        VideoEncodingSettings = 3,
        Viewfinder = 4,
        ViewfinderSettings = 5
    };

    virtual QCamera::State state() const = 0;
    virtual void setState(QCamera::State state) = 0;

    virtual QCamera::Status status() const = 0;

    virtual QCamera::CaptureModes captureMode() const = 0;
    virtual void setCaptureMode(QCamera::CaptureModes) = 0;
    virtual bool isCaptureModeSupported(QCamera::CaptureModes mode) const = 0;

    virtual bool canChangeProperty(PropertyChangeType changeType, QCamera::Status status) const = 0;

    virtual QCamera::LockTypes supportedLocks() const = 0;

    virtual QCamera::LockStatus lockStatus(QCamera::LockType lock) const = 0;

    virtual void searchAndLock(QCamera::LockTypes locks) = 0;
    virtual void unlock(QCamera::LockTypes locks) = 0;

    virtual QList<QCameraViewfinderSettings> supportedViewfinderSettings() const = 0;

    virtual QCameraViewfinderSettings viewfinderSettings() const = 0;
    virtual void setViewfinderSettings(const QCameraViewfinderSettings &settings) = 0;

Q_SIGNALS:
    void stateChanged(QCamera::State);
    void statusChanged(QCamera::Status);
    void error(int error, const QString &errorString);
    void captureModeChanged(QCamera::CaptureModes mode);
    void lockStatusChanged(QCamera::LockType type, QCamera::LockStatus status, QCamera::LockChangeReason reason);

protected:
    explicit QCameraControl(QObject *parent = nullptr);
};

#define QCameraControl_iid "org.qt-project.qt.cameracontrol/5.0"
Q_MEDIA_DECLARE_CONTROL(QCameraControl, QCameraControl_iid)

QT_END_NAMESPACE


#endif  // QCAMERACONTROL_H

