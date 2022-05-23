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

#include "qffmpeghwaccel_mediacodec_p.h"

#include <androidsurfacetexture_p.h>
#include <QtGui/private/qrhi_p.h>

extern "C" {
#include <libavcodec/mediacodec.h>
}

#if !defined(Q_OS_ANDROID)
#    error "Configuration error"
#endif

namespace QFFmpeg {

Q_GLOBAL_STATIC(AndroidSurfaceTexture, androidSurfaceTexture, 0);

class MediaCodecTextureSet : public TextureSet
{
public:
    MediaCodecTextureSet(qint64 textureHandle) : handle(textureHandle) { }

    qint64 textureHandle(int plane) override { return (plane == 0) ? handle : 0; }

private:
    qint64 handle;
};

void MediaCodecTextureConverter::setupDecoderSurface(AVCodecContext *avCodecContext)
{
    AVMediaCodecContext *mediacodecContext = av_mediacodec_alloc_context();
    av_mediacodec_default_init(avCodecContext, mediacodecContext, androidSurfaceTexture->surface());
}

TextureSet *MediaCodecTextureConverter::getTextures(AVFrame *frame)
{
    if (!androidSurfaceTexture->isValid())
        return {};

    if (!externalTexture) {
        androidSurfaceTexture->detachFromGLContext();
        externalTexture = std::unique_ptr<QRhiTexture>(
                rhi->newTexture(QRhiTexture::Format::RGBA8, { frame->width, frame->height }, 1,
                                QRhiTexture::ExternalOES));

        if (!externalTexture->create()) {
            qWarning() << "Failed to create the external texture!";
            return {};
        }

        quint64 textureHandle = externalTexture->nativeTexture().object;
        androidSurfaceTexture->attachToGLContext(textureHandle);
    }

    // release a MediaCodec buffer and render it to the surface
    AVMediaCodecBuffer *buffer = (AVMediaCodecBuffer *)frame->data[3];
    int result = av_mediacodec_release_buffer(buffer, 1);
    if (result < 0) {
        qWarning() << "Failed to render buffer to surface.";
        return {};
    }

    androidSurfaceTexture->updateTexImage();

    return new MediaCodecTextureSet(externalTexture->nativeTexture().object);
}
}
