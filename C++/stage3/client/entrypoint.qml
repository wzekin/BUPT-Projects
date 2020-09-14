import Monster 1.0
import QtQuick 2.15
import QtQuick.Window 2.2
import QtQuick.Controls 2.15

Window {
    id: root
    visible: true
    width: 1080
    height: 720

    property int battle_type:0;
    property var battle_monster: null
    property var my_battle_monster: null


    title: qsTr("宠物小精灵")

    //    function showGeneralSettingsPage()//常规设置
    //    {
    //        panelLoader.setSource("qrc:/qml/GeneralSettings.qml")
    //    }

    //    function showLinkPage()//连接设置
    //    {
    //         panelLoader.setSource("qrc:/qml/LinkSettings.qml")
    //    }
    function showLoginPage()
    {
        panelLoader.setSource("qrc:/login.qml")
    }

    Component.onCompleted: showLoginPage()

    Loader {
        id: panelLoader
        function setSource(source) {
            panelLoader.source = source
        }

        function setSourceComponent(sourceComponent) {
            panelLoader.sourceComponent = sourceComponent
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
