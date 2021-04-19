/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
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

#include "qmockdevicemanager_p.h"
#include "private/qcamerainfo_p.h"

QT_BEGIN_NAMESPACE

QMockDeviceManager::QMockDeviceManager()
    : QPlatformMediaDeviceManager()
{
    QCameraInfoPrivate *info = new QCameraInfoPrivate;
    info->description = QString::fromUtf8("defaultCamera");
    info->id = "default";
    info->isDefault = true;
    m_cameraDevices.append(info->create());
    info = new QCameraInfoPrivate;
    info->description = QString::fromUtf8("frontCamera");
    info->id = "front";
    info->isDefault = false;
    info->position = QCameraInfo::FrontFace;
    m_cameraDevices.append(info->create());
    info = new QCameraInfoPrivate;
    info->description = QString::fromUtf8("backCamera");
    info->id = "back";
    info->isDefault = false;
    info->position = QCameraInfo::BackFace;
    m_cameraDevices.append(info->create());

}

QMockDeviceManager::~QMockDeviceManager() = default;

QList<QAudioDeviceInfo> QMockDeviceManager::audioInputs() const
{
    return m_inputDevices;
}

QList<QAudioDeviceInfo> QMockDeviceManager::audioOutputs() const
{
    return m_outputDevices;
}

QList<QCameraInfo> QMockDeviceManager::videoInputs() const
{
    return m_cameraDevices;
}

QAbstractAudioInput *QMockDeviceManager::createAudioInputDevice(const QAudioDeviceInfo &info)
{
    Q_UNUSED(info);
    return nullptr;// ###
}

QAbstractAudioOutput *QMockDeviceManager::createAudioOutputDevice(const QAudioDeviceInfo &info)
{
    Q_UNUSED(info);
    return nullptr; //###
}


QT_END_NAMESPACE
