/*
 *  Copyright 2013 Sebastian Kügler <sebas@kde.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

import QtQuick 2.0
import QtQuick.Window 2.0

import org.kde.plasma.core 0.1 as PlasmaCore
import org.kde.plasma.components 0.1 as PlasmaComponents
import org.kde.plasma.extras 0.1 as PlasmaExtras
import org.kde.qtextracomponents 0.1 as QtExtras

// DialogsPage

PlasmaComponents.Page {
    id: dialogsPage
    anchors {
        fill: parent
        margins: _s
    }
    Column {
        spacing: _s/2
        anchors.fill: parent
        PlasmaExtras.Title {
            width: parent.width
            text: "Dialogs"
        }
        Row {
            height: _h
            spacing: _s
            PlasmaComponents.Button {
                id: radio
                checkable: true
                iconSource: "dialog-ok"
                text: "QtQuick2.Window"
            }
            Window {
                id: qWindow
                visible: radio.checked
                width: childrenRect.width
                height: childrenRect.height
                color: Qt.rgba(0,0,0,0)
                DialogContent {
                    id: dContent
                    onCloseMe: {
                        qWindow.visible = false
                    }
                }
            }

            PlasmaComponents.Label {
                text: qWindow.visible ? "shown" : "hidden"
            }
        }
        Row {
            height: _h
            spacing: _s
            PlasmaComponents.Button {
                text: "PlasmaCore.Dialog"
                iconSource: "dialog-ok-apply"
                checkable: true
                //onCheckedChanged: pcDialog.visible = checked
                onCheckedChanged: pcDialog.visible = checked
            }
            PlasmaComponents.Label {
                text: pcDialog.visible ? "shown" : "hidden"
            }

            PlasmaCore.Dialog {
                id: pcDialog
                windowFlags: Qt.Popup
                //mainItem: dContent2
                color: Qt.rgba(0,0,0,0)

                mainItem: DialogContent {
                    id: dContent2
                    onCloseMe: pcDialog.visible = false
                }
            }
        }
        Row {
            height: _h
            spacing: _s
            PlasmaComponents.Button {
                text: "PlasmaComponents.Dialog"
                iconSource: "dialog-ok-apply"
                checkable: true
                onCheckedChanged: pcompDialog.visible = checked
            }
            PlasmaComponents.Label {
                text: pcompDialog.visible ? "shown" : "hidden"
            }

            PlasmaComponents.Dialog {
                id: pcompDialog
                //windowFlags: Qt.Popup
                content: dContent3
                DialogContent {
                    id: dContent3
                    onCloseMe: pcompDialog.visible = false
                }
                buttons: PlasmaComponents.ButtonRow {
                    PlasmaComponents.Button {
                        text: "Close";
                        onClicked: {
                            print("Closing...");
                            pcompDialog.close()
                        }
                    }
                    PlasmaComponents.Button {
                        text: "Accept";
                        onClicked: {
                            print("Accepting...");
                            pcompDialog.accept()
                        }
                    }
                }
            }
        }
    }
}

