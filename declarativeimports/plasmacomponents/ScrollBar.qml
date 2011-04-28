/*
*   Copyright (C) 2011 by Daker Fernandes Pinheiro <dakerfp@gmail.com>
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

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

import QtQuick 1.0
import org.kde.plasma.core 0.1 as PlasmaCore

// TODO: add support mouse wheel and key events
Item {
    id: scrollbar

    // Common API
    property Flickable flickableItem: null

    // Plasma API
    property int orientation: Qt.Horizontal
    property bool animated: false
    property bool inverted: false
    property bool updateValueWhileDragging: true
    property alias stepSize: range.stepSize
    property alias pressed: mouseArea.pressed

    // Convinience API
    property bool _isVertical: orientation == Qt.Vertical

    width: _isVertical ? 22 : 200
    height: _isVertical ? 200 : 22

    visible: flickableItem && handle.width < contents.width

    Item {
        id: contents

        width: _isVertical ? scrollbar.height : scrollbar.width
        height: _isVertical ? scrollbar.width : scrollbar.height
        rotation: _isVertical ? -90 : 0

        anchors.centerIn: parent

        PlasmaCore.RangeModel {
            id: range

            minimumValue: 0
            maximumValue: {
                var diff;
                if (_isVertical)
                    diff = flickableItem.contentHeight - flickableItem.height;
                else
                    diff = flickableItem.contentWidth - flickableItem.width;

                return Math.max(0, diff);
            }
            stepSize: 0.0
            inverted: _isVertical ? !scrollbar.inverted : scrollbar.inverted
            positionAtMinimum: 0 + handle.width / 2
            positionAtMaximum: contents.width - handle.width / 2
            value: _isVertical ? flickableItem.contentY : flickableItem.contentX
            onValueChanged: {
                if (flickableItem.flicking)
                    return;

                if (_isVertical)
                    flickableItem.contentY = value;
                else
                    flickableItem.contentX = value;
            }
            position: handle.x
            onPositionChanged: { handle.x = position; }

        }

        PlasmaCore.FrameSvgItem {
            id: groove

            anchors.fill: parent
            imagePath: "widgets/scrollbar"
            prefix: "background-horizontal"
        }

        PlasmaCore.FrameSvgItem {
            id: handle

            transform: Translate { x: - handle.width / 2 }
            x: fakeHandle.x
            anchors.verticalCenter: groove.verticalCenter
            width: {
                var ratio;
                if (_isVertical)
                    ratio = flickableItem.visibleArea.heightRatio;
                else
                    ratio = flickableItem.visibleArea.widthRatio;

                return ratio * parent.width;
            }
            height: parent.height - margins.top // TODO: check mergin
            imagePath: "widgets/scrollbar"
            prefix: {
                if (scrollbar.pressed)
                    return "sunken-slider";

                if (scrollbar.activeFocus || mouseArea.containsMouse)
                    return "mouseover-slider";
                else
                    return "slider";
            }

            Behavior on x {
                id: behavior
                enabled: !mouseArea.drag.active && scrollbar.animated &&
                    !flickableItem.flicking

                PropertyAnimation {
                    duration: behavior.enabled ? 150 : 0
                    easing.type: Easing.OutSine
                }
            }
        }

        Item {
            id: fakeHandle
            width: handle.width
            height: handle.height
            transform: Translate { x: - handle.width / 2 }
        }

        MouseArea {
            id: mouseArea

            anchors.fill: parent
            drag {
                target: fakeHandle
                axis: Drag.XAxis
                minimumX: range.positionAtMinimum
                maximumX: range.positionAtMaximum
            }

            onPressed: {
                // Clamp the value
                var newX = Math.max(mouse.x, drag.minimumX);
                newX = Math.min(newX, drag.maximumX);

                // Debounce the press: a press event inside the handler will not
                // change its position, the user needs to drag it.
                if (Math.abs(newX - fakeHandle.x) > handle.width / 2)
                    range.position = newX;

                scrollbar.forceActiveFocus();
            }
            onReleased: {
                // If we don't update while dragging, this is the only
                // moment that the range is updated.
                if (!scrollbar.updateValueWhileDragging)
                    range.position = fakeHandle.x;
            }
        }
    }

    // Range position normally follow fakeHandle, except when
    // 'updateValueWhileDragging' is false. In this case it will only follow
    // if the user is not pressing the handle.
    Binding {
        when: updateValueWhileDragging || !mouseArea.pressed
        target: range
        property: "position"
        value: fakeHandle.x
    }

    // During the drag, we simply ignore position set from the range, this
    // means that setting a value while dragging will not "interrupt" the
    // dragging activity.
    Binding {
        when: !mouseArea.drag.active
        target: fakeHandle
        property: "x"
        value: range.position
    }
}
