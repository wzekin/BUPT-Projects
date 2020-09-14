import User 1.0
import Monster 1.0
import QtQuick.Dialogs 1.1
import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item {
    id: battle
    width: 1080
    height: 720

    property real my_count_time: 0;
    property real count_time: 0;
    property bool is_failed: false

    Component.onCompleted: {
            root.my_battle_monster.reset_hp()
            root.battle_monster.reset_hp()
    }

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

    Timer {
        id:timer
        interval: 100; running: true; repeat: true
        onTriggered: {
             let other = root.battle_monster
             let my = root.my_battle_monster
             count_time += other.speed * 0.1;
             my_count_time += my.speed * 0.1;
             if (my_count_time >= 70){
                 my_count_time = 0;
                 let rand = Math.random()
                 if (rand > 0.9){
                     textArea.text +=  "\""  + other.nickname+ "\" 闪避了 \"" + my.nickname  + "\" 的攻击\n";
                     return
                 }

                 var damage = my.monster_attack(other)
                 if (rand <0.1){
                     damage += 5
                    textArea.text += "\"我的" + my.nickname  + "\" 暴击了！\n"
                 }
                 textArea.text += "\"我的" + my.nickname  + "\" 使用 \"" + my.attack_name + "\" 攻击 \"" + other.nickname + "\" 造成了"+ damage + "点伤害\n";
                 other.attacked(damage)
                 if (other.is_died){
                    timer.stop()
                    textArea.text += "\"" + other.nickname + "\" 失去战斗能力\n"
                    textArea.text += "\"" + my.nickname + "\" 获胜\n"
                    my.monster_kill(other)
                    if(my.should_level_up){
                        my.level_up()
                        textArea.text += "\"" + my.nickname + "\" 升级了！\n"
                    }
                    client.upsertMonster(my)
                    log.title = '您获胜了'
                    log.text = '您获胜了'
                    log.visible = true
                    battle.is_failed = false


                    return
                 }
             }
             if (count_time >= 70){
                 count_time = 0;
                 let rand = Math.random()
                 if (rand > 0.9){
                     textArea.text +=  "\""  + my.nickname+ "\" 闪避了 \"" + other.nickname  + "\" 的攻击\n";
                     return
                 }

                 var damage = other.monster_attack(my)
                 if (rand <0.1){
                     damage += 5
                    textArea.text += "\"" + other.nickname  + "\" 暴击了！\n"
                 }
                 my.attacked(damage)
                 textArea.text +=  "\""  + other.nickname+ "\" 使用 \"" + other.attack_name  + "\" 攻击 \"我的" + my.nickname + "\" 造成了"+ damage + "点伤害\n";
                 if (my.is_died){
                    timer.stop()
                    textArea.text += "\"" +  my.nickname + "\" 失去战斗能力\n"
                    textArea.text += "\"" + other.nickname + "\" 获胜\n"
                            log.title = '您失败了'
                            log.text = '您失败了'
                            log.visible = true
                    battle.is_failed = true
                    return
                 }
             }
        }
    }

    TextArea {
        id: textArea
        x: 103
        y: 78
        width: 826
        height: 548
        font.wordSpacing: 0.1
        font.bold: true
        font.pointSize: 12
        background: Rectangle {
            opacity: 0.8
            color: white
        }
    }



    MessageDialog {
        id: log
        standardButtons: StandardButton.Ok
        onAccepted :{
            if (root.battle_type === 0){
                panelLoader.setSource("qrc:/main.qml")
            }else{
                if (battle.is_failed){
                    client.addBattle(false)
                    raw_repeat.model = client.random3();
                    choose_dialog.visible = true;
                }else{
                    client.addBattle(true)
                    client.addMonster(root.battle_monster)
                    panelLoader.setSource("qrc:/main.qml")
                }
            }

        }
    }
        Dialog {
            id: choose_dialog
            modal: true
            anchors.centerIn: parent;
            title: "请选择放生的精灵"

            contentItem: RowLayout {
                id: layout
                anchors.fill: parent
                spacing: 10

            Repeater {
                id: raw_repeat
                model: 3
                Button {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        text: modelData.nickname + " lv" + modelData.level
                        onClicked: {
                            client.releaseMonster(modelData)
                            panelLoader.setSource("qrc:/main.qml")
                        }
                }
            }
            }
        }

}
