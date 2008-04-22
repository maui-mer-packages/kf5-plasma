/*
 *   Copyright 2007 by Aaron Seigo <aseigo@kde.org>
 *   Copyright 2008 by Marco Martin <notmart@gmail.com>
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

#include "desktoptoolbox_p.h"

#include <QGraphicsSceneHoverEvent>
#include <QPainter>
#include <QRadialGradient>

#include <plasma/theme.h>
#include <KColorScheme>

#include <KDebug>

#include <plasma/applet.h>


namespace Plasma
{

class EmptyGraphicsItem : public QGraphicsItem
{
    public:
        EmptyGraphicsItem(QGraphicsItem *parent)
            : QGraphicsItem(parent)
        {
            setAcceptsHoverEvents(true);
        }

        QRectF boundingRect() const
        {
            return QRectF(QPointF(0, 0), m_rect.size());
        }

        QRectF rect() const
        {
            return m_rect;
        }

        void setRect(const QRectF &rect)
        {
            //kDebug() << "setting rect to" << rect;
            prepareGeometryChange();
            m_rect = rect;
            setPos(rect.topLeft());
        }

        void paint(QPainter * p, const QStyleOptionGraphicsItem*, QWidget*)
        {
            Q_UNUSED(p)
            //p->setPen(Qt::red);
            //p->drawRect(boundingRect());
        }

    private:
        QRectF m_rect;
};

// used with QGrahphicsItem::setData
static const int ToolName = 7001;

class DesktopToolbox::Private
{
public:
    Private()
      : icon("plasma"),
        toolBacker(0),
        animId(0),
        animFrame(0)
    {}

    KIcon icon;
    EmptyGraphicsItem *toolBacker;
    QTime stopwatch;
    Plasma::Phase::AnimId animId;
    qreal animFrame;
};

DesktopToolbox::DesktopToolbox(QGraphicsItem *parent)
    : Toolbox(parent),
      d(new Private)
{
    connect(Plasma::Phase::self(), SIGNAL(movementComplete(QGraphicsItem*)), this, SLOT(toolMoved(QGraphicsItem*)));

    setZValue(10000000);
    setFlag(ItemClipsToShape, true);
    setFlag(ItemClipsChildrenToShape, false);
    setFlag(ItemIgnoresTransformations, true);
}

QRectF DesktopToolbox::boundingRect() const
{
    return QRectF(0, 0, -size()*2, size()*2);
}

void DesktopToolbox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->translate(boundingRect().topLeft());

    QColor color1 = KColorScheme(QPalette::Active, KColorScheme::Window,
                               Plasma::Theme::defaultTheme()->colorScheme()).background().color();
    color1.setAlpha(64);

    QColor color2 = KColorScheme(QPalette::Active, KColorScheme::Window,
                               Plasma::Theme::defaultTheme()->colorScheme()).foreground().color();
    color2.setAlpha(64);

    QPainterPath p = shape();
    QRadialGradient gradient(boundingRect().topLeft(), size() + d->animFrame);
    gradient.setFocalPoint(boundingRect().topLeft());
    gradient.setColorAt(0, color1);
    gradient.setColorAt(.87, color1);
    gradient.setColorAt(.97, color2);
    color2.setAlpha(0);
    gradient.setColorAt(1, color2);
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setBrush(gradient);
    painter->drawPath(p);
    painter->restore();

    const qreal progress = d->animFrame / size();

    if (progress <= 0.9) {
        d->icon.paint(painter, QRect(QPoint((int)boundingRect().left() - iconSize().width() + 2, 2), iconSize()), Qt::AlignCenter, QIcon::Disabled, QIcon::Off);
    }

    if (progress > 0.1) {
        painter->save();
        painter->setOpacity(progress);
        d->icon.paint(painter, QRect(QPoint((int)boundingRect().left() - iconSize().width() + 2, 2), iconSize()));
        painter->restore();
    }

    painter->restore();
}

QPainterPath DesktopToolbox::shape() const
{
    QPainterPath path;
    int toolSize = size() + (int)d->animFrame;
    path.arcTo(QRectF(boundingRect().left() - toolSize, boundingRect().top() - toolSize, toolSize*2, toolSize*2), 180, 90);

    return path;
}

void DesktopToolbox::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if (showing() || d->stopwatch.elapsed() < 100) {
        QGraphicsItem::hoverEnterEvent(event);
        return;
    }

    QPainterPath path;
    int toolSize = size() + (int)d->animFrame - 15;
    path.moveTo(size()*2, 0);
    path.arcTo(QRectF(size() * 2 - toolSize, -toolSize, toolSize*2, toolSize*2), 180, 90);
    path.lineTo(size()*2, 0);

    if (path.contains(event->pos())) {
        QGraphicsItem::hoverEnterEvent(event);
        return;
    }

    showToolbox();
    QGraphicsItem::hoverEnterEvent(event);
}

void DesktopToolbox::showToolbox()
{
    if (showing()) {
        return;
    }

    int maxwidth = 0;
    foreach (QGraphicsItem* tool, QGraphicsItem::children()) {
        if (!tool->isEnabled()) {
            continue;
        }
        maxwidth = qMax(static_cast<int>(tool->boundingRect().width()), maxwidth);
    }

    // put tools 5px from icon edge
    const int iconWidth = 32;
    int x = (int)boundingRect().left() - maxwidth - iconWidth - 5;
    int y = (int)boundingRect().top() + 5;
    Plasma::Phase* phase = Plasma::Phase::self();
    foreach (QGraphicsItem* tool, QGraphicsItem::children()) {
        if (tool == d->toolBacker) {
            continue;
        }

        if (!tool->isEnabled()) {
            if (tool->isVisible()) {
                const int height = static_cast<int>(tool->boundingRect().height());
                phase->moveItem(tool, Plasma::Phase::SlideOut, QPoint(size() * 2, -height));
            }
            continue;
        }

        //kDebug() << "let's show and move" << tool << tool->boundingRect();
        tool->show();
        phase->moveItem(tool, Plasma::Phase::SlideIn, QPoint(x, y));
        //x += 0;
        y += static_cast<int>(tool->boundingRect().height()) + 5;
    }

    if (!d->toolBacker) {
        d->toolBacker = new EmptyGraphicsItem(this);
    }
    d->toolBacker->setRect(QRectF(QPointF(x, 0), QSizeF(maxwidth, y - 10)));
    d->toolBacker->show();

    if (d->animId) {
        phase->stopCustomAnimation(d->animId);
    }

    setShowing(true);
    // TODO: 10 and 200 shouldn't be hardcoded here. There needs to be a way to
    // match whatever the time is that moveItem() takes. Same in hoverLeaveEvent().
    d->animId = phase->customAnimation(10, 240, Plasma::Phase::EaseInCurve, this, "animate");
    d->stopwatch.restart();
}

void DesktopToolbox::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    //kDebug() << event->pos() << event->scenePos() << d->toolBacker->rect().contains(event->scenePos().toPoint());
    if ((d->toolBacker && d->toolBacker->rect().contains(event->scenePos().toPoint())) ||
        d->stopwatch.elapsed() < 100) {
        QGraphicsItem::hoverLeaveEvent(event);
        return;
    }
    hideToolbox();
    QGraphicsItem::hoverLeaveEvent(event);
}

void DesktopToolbox::hideToolbox()
{
    if (!showing()) {
        return;
    }

    int x = size() * 2;
    int y = 0;
    Plasma::Phase* phase = Plasma::Phase::self();
    foreach (QGraphicsItem* tool, QGraphicsItem::children()) {
        if (tool == d->toolBacker) {
            continue;
        }

        const int height = static_cast<int>(tool->boundingRect().height());
        phase->moveItem(tool, Plasma::Phase::SlideOut, QPoint(x, y-height));
    }

    if (d->animId) {
        phase->stopCustomAnimation(d->animId);
    }

    setShowing(false);
    d->animId = phase->customAnimation(10, 240, Plasma::Phase::EaseOutCurve, this, "animate");

    if (d->toolBacker) {
        d->toolBacker->hide();
    }

    d->stopwatch.restart();
}

void DesktopToolbox::animate(qreal progress)
{
    if (showing()) {
        d->animFrame = size() * progress;
    } else {
        d->animFrame = size() * (1.0 - progress);
    }

    //kDebug() << "animating at" << progress << "for" << d->animFrame;

    if (progress >= 1) {
        d->animId = 0;
    }

    update();
}

void DesktopToolbox::toolMoved(QGraphicsItem *item)
{
    //kDebug() << "geometry is now " << static_cast<Plasma::Widget*>(item)->geometry();
    if (!showing() &&
        QGraphicsItem::children().indexOf(static_cast<Plasma::Applet*>(item)) != -1) {
        item->hide();
    }
}

} // plasma namespace

#include "desktoptoolbox_p.moc"

