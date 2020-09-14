import User 1.0
import Monster 1.0
import QtQuick.Dialogs 1.1
import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQml.Models 2.15

Item {
    id: main
    width: 1080
    height: 720

    Image {
        id: image
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        fillMode: Image.Stretch
        opacity: 0.8
        source: "qrc:/assert/thumb-1920-235203.png"
    }


    Component.onCompleted: {
        client.listMonster(client.user)
        client.listUser();
        if (client.user.monsters.length === 0){
            client.newMonster()
            log.visible = true
        }
        monster_list.model = client.user.monsters
        user_list.model = client.user_list
    }

    Label {
        id: label1
        x: 108
        y: 43
        width: 63
        height: 25
        text: "在线用户列表"
        font.pointSize: 14
        font.bold: true
    }

    Label {
        id: label2
        x: 408
        y: 43
        width: 87
        height: 39
        text: "小精灵列表"
        font.pointSize: 14
        font.bold: true
    }

    ScrollView {
        id: user
        width: 201
        height: 581
        background: Rectangle {
            opacity: 0.8
            color: white
        }

        anchors.verticalCenterOffset: 1
        anchors.horizontalCenterOffset: -351
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        anchors.centerIn: parent;
        ListView {
            id: user_list
            model: 0;
            delegate: ItemDelegate {
                width: parent.width
                text: (has_good ? "🏅" : "") + (has_silver ? "🥈" : "") + username + "[胜率：" + win_rate + "%]"
                onClicked:{
                    go.enabled = client.user.uuid === uuid
                    client.listMonster(client.user_list[index])
                    monster_list.model = monsters
                    var m = monsters[0]
                    lmax_hp.text = m.max_hp
                    lattack.text = m.attack
                    ldefence.text = m.defence
                    lspeed.text = m.speed
                    label.text = m.nickname + " lv" + m.level
                    go.level = m.level
                    root.my_battle_monster = m
                }
            }
        }
    }


    ScrollView {
        id: monster
        width: 201
        height: 581
        anchors.verticalCenterOffset: 1
        anchors.horizontalCenterOffset: -52
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        background: Rectangle {
            opacity: 0.8
            color: white
        }

        anchors.centerIn: parent;
        ListView {
            id: monster_list
            model: 0;
            delegate: ItemDelegate {
                text: nickname
                onClicked: {
                    width: parent.width
                    lmax_hp.text = max_hp
                    lattack.text = attack
                    ldefence.text = defence
                    lspeed.text = speed
                    label.text = nickname + " lv" + level
                    go.level = level
                    root.my_battle_monster = modelData
                }
            }
        }

    }

    Label {
        id: label
        x: 796
        y: 192
        width: 67
        height: 22
        text: qsTr("小精灵")
        horizontalAlignment: Text.AlignHCenter
        font.bold: true
        font.pointSize: 14
    }

    Label {
        id: lspeed
        x: 936
        y: 291
        width: 55
        height: 18
        font.pointSize: 12
    }

    Label {
        id: ldefence
        x: 765
        y: 291
        width: 55
        height: 18
        font.pointSize: 12
    }

    Label {
        id: lattack
        x: 936
        y: 246
        width: 55
        height: 18
        font.pointSize: 12
    }

    Label {
        id: lmax_hp
        x: 765
        y: 246
        width: 55
        height: 18
        font.pointSize: 12
    }

    Label {
        id: label6
        x: 884
        y: 291
        width: 55
        height: 18
        text: qsTr("速度：")
        font.pointSize: 12
    }

    Label {
        id: label5
        x: 704
        y: 291
        width: 55
        height: 18
        text: qsTr("防御：")
        font.pointSize: 12
    }

    Label {
        id: label4
        x: 884
        y: 246
        width: 55
        height: 18
        text: qsTr("攻击：")
        font.pointSize: 12
    }

    Label {
        id: label3
        x: 704
        y: 246
        width: 55
        height: 18
        text: qsTr("最大HP:")
        font.pointSize: 12
    }


    Button {
        id: go
        x: 802
        y: 358
        width: 69
        height: 28

        property int level: 0
        text: qsTr("出击")
        onClicked: {
            choose_dialog.visible = true
        }
    }




    Dialog {
        id: choose_dialog
        modal: true
        anchors.centerIn: parent;
        title: "请选择决斗类型"

        property int type: 0


        contentItem: RowLayout {
            id: layout
            anchors.fill: parent
            spacing: 10

            Button {
                Layout.leftMargin: 10
                Layout.minimumHeight: 100
                Layout.minimumWidth: 100
                text: "升级赛"

                onClicked: {
                    root.battle_type = 0
                    choose_dialog.visible = 0
                    client.random9(go.level)
                    grid_repead.model = client.monsters
                    monster_dialog.visible = 1
                }
            }

            Button {
                Layout.minimumHeight: 100
                Layout.minimumWidth: 100
                text: "决斗赛"

                onClicked: {
                    root.battle_type = 1
                    choose_dialog.visible = 0
                    client.random9(go.level)
                    grid_repead.model = client.monsters
                    monster_dialog.visible = 1
                }
            }
        }
    }

    Dialog {
        id: monster_dialog
        modal: true
        width: 300
        height: 350
        anchors.centerIn: parent;
        title: "请选择与之对战的小精灵"

        contentItem: GridLayout {
                    id: grid
                    anchors.fill: parent
                    columns: 3
                    rowSpacing: 10
                    columnSpacing: 10
                    anchors.topMargin: 50
                    anchors.margins: 5
                    Repeater {
                        id: grid_repead
                        model: 5
                        Button {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        text: modelData.nickname + " lv" + modelData.level
                                        onClicked: {
                                            root.battle_monster = modelData
                                            panelLoader.setSource("qrc:/battle.qml")
                                        }
                        }
                    }
            }
    }

    MessageDialog {
        id: log
        title: "自动补发"
        text: "由于您已经没有精灵了，所以系统给您补发了一只精灵"
        standardButtons: StandardButton.Ok
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.8999999761581421}
}
##^##*/
