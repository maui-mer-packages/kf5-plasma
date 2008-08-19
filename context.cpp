/*
 *   Copyright 2008 by Aaron Seigo <aseigo@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "context.h"

namespace Plasma
{

class ContextPrivate
{
public:
};

Context::Context(QObject *parent)
    : QObject(parent),
      d(new ContextPrivate)
{
}

Context::~Context()
{
      delete d;
}

void Context::createContext(const QString &name)
{
}

QStringList Context::contexts() const
{
    return QStringList();
}

void Context::setCurrentContext(const QString &name)
{
}

QString Context::currentContext() const
{
    return QString();
}

} // namespace Plasma

#include "context.moc"

