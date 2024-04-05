// Copyright (C) 2024 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef QSCOPEDENVIRONMENTVARIABLE_H
#define QSCOPEDENVIRONMENTVARIABLE_H

#include <QtCore/qbytearrayview.h>
#include <QtCore/qtenvironmentvariables.h>

struct QScopedEnvironmentVariable
{
    QScopedEnvironmentVariable(const QScopedEnvironmentVariable &) = delete;
    QScopedEnvironmentVariable(QScopedEnvironmentVariable &&) = delete;
    QScopedEnvironmentVariable &operator=(const QScopedEnvironmentVariable &) = delete;
    QScopedEnvironmentVariable &operator=(QScopedEnvironmentVariable &&) = delete;

    QScopedEnvironmentVariable(const char *envvar, QByteArrayView name) : envvar{ envvar }
    {
        Q_ASSERT(envvar);
        qputenv(envvar, name);
    };

    ~QScopedEnvironmentVariable() { qunsetenv(envvar); }

private:
    const char *envvar;
};

#endif // QSCOPEDENVIRONMENTVARIABLE_H
