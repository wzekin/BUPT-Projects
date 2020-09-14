import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.1
import QtQuick.Controls.Material 2.15

Item {
    width: 1080
    height: 720

    Image {
        id: image
        x: 0
        y: 0
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectFit
        source: "qrc:/assert/login_background.png"
    }

    Rectangle {
        id: rectangle
        x: 179
        y: 117
        width: 324
        height: 181
        color: "#80ffffff"
        radius: 5
    }

    Text {
        id: text_username
        x: 194
        y: 146
        height: 40
        text: qsTr("用户名")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
    }

    TextField {
        id: textInput_username
        x: 266
        y: 146
        width: 200
        height: 40
        font.pixelSize: 20
    }

    Text {
        id: text_password
        x: 194
        y: 190
        height: 40
        text: qsTr("密码")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 20
    }

    TextField {
        id: textInput_password
        x: 266
        y: 190
        width: 200
        height: 40
        font.pixelSize: 20
    }

    Button {
        x: 203
        y: 248
        width: 89
        height: 40
        text: "登录"
        font.pointSize: 18
        onClicked: {
            if (textInput_username.text === ''){
                error.title = '登录失败'
                error.text = '请输入用户名'
                error.visible = true
                return
            }

            if (textInput_password.text === ''){
                error.title = '登录失败'
                error.text = '请输入密码'
                error.visible = true
                return
            }

            const [code, err] = client.signin(textInput_username.text, textInput_password.text)
            if(err != undefined){
                error.title = '登录失败'
                error.text = err
                error.visible = true
            }else{
                parent.parent.setSource("qrc:/main.qml")
            }
        }
    }

    Button {
        x: 345
        y: 248
        width: 92
        height: 40
        text: "注册"
        font.pointSize: 18
        onClicked: {
            if (textInput_username.text === ''){
                error.title = '注册失败'
                error.text = '请输入用户名'
                error.visible = true
                return
            }

            if (textInput_password.text === ''){
                error.title = '注册失败'
                error.text = '请输入密码'
                error.visible = true
                return
            }

            const [code, err] = client.register_(textInput_username.text, textInput_password.text)
            if (err != undefined){
                error.title = '注册失败'
                error.text = code
                error.visible = true
            }else{
                parent.parent.setSource("qrc:/main.qml")
            }

        }
    }

    MessageDialog {
        id: error
    }
}


/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
