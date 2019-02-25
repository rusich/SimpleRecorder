import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: app
    visible: true
    width: 480
    height: 800
    title: qsTr("Hello World")

    property bool recording: false
    onClosing: {
        if(recording) {
            close.accepted = false;
        }
        else return;
    }

    RoundButton {
        Keys.onPressed: {
            if (event.key === Qt.Key_Back) {
                console.log("BACK!!!");
                event.accepted = true;
            }
        }
        id: settingsBtn
        text: ""
        width: parent.width/10
        height: width
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.rightMargin: 10
        Material.background: "grey"

        onClicked: {
            settingsDialog.visible = true;
            console.log(recorder.recordLenght);
        }

        Image {
            id: setImg
            fillMode: Image.PreserveAspectFit
            height: parent.height*0.5
            width: height
            source:  "qrc:///images/settings.png"
            anchors.centerIn: parent
        }
    }
    RoundButton {
        id: recordBtn
        text: ""
        width: parent.width/3
        height: width
        anchors.centerIn: parent
        Material.background: recording? "#ff0000": "#4CAF50"

        onClicked: {
            recording = !recording;
        }

        Image {
            id: micImg
            fillMode: Image.PreserveAspectFit
            source:  recording? "qrc:///images/stop.png" : "qrc:///images/microphone.png"
            anchors.centerIn: parent
            height:  parent.height * 0.8
        }

        SequentialAnimation
        {
            running: recording
            loops: Animation.Infinite

            ColorAnimation
            {
                target: recordBtn
                properties: "Material.background"
                to: "#ff0000"
                duration:  1
            }
            ColorAnimation
            {
                target: recordBtn
                properties: "Material.background"
                to: "#e56767"
                duration:  1000
            }
            ColorAnimation
            {
                target: recordBtn
                properties: "Material.background"
                to: "#ff0000"
                duration: 1000
            }
        }
    }

    Popup {
        id: settingsDialog
        visible: false
        modal: true
        dim: true
        focus: true
        anchors.centerIn: parent
        width: app.width * 0.9
        height: 300


        Label {
            id: dlgCaption
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 20
            font.bold: true
            font.pointSize: 18
            text: qsTr("Настройки")
        }

        Label {
            id: description
            anchors.left: dlgCaption.left
            anchors.top: dlgCaption.bottom
            anchors.topMargin: 10
            width: parent.width * 0.92
            font.pointSize: 14
            wrapMode:  Text.Wrap
            text: qsTr("Длительность записи для каждого файла (в минутах): ")
        }

        ComboBox
        {
            Component.onCompleted: {
                for(var i=0; i<rlModel.count; i++) {
                    if(rlModel.get(i).value === recorder.recordLenght){
                        rlCb.currentIndex = i;
                        break;
                    }
                }
            }

            id: rlCb
            textRole: "key"
            anchors.left: description.left
            anchors.top: description.bottom
            anchors.topMargin: 10
            width: parent.width * 0.92
            model: ListModel
            {
                id: rlModel
                ListElement { key: "1 мин."; value: 1 }
                ListElement { key: "5 мин."; value: 5 }
                ListElement { key: "10 мин."; value: 10 }
            }
        }

        RowLayout {
            id: bootomBtns
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            height: parent / 4
            Item {
                Layout.fillWidth: true
            }
            Button {
                flat: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                //                    Layout.preferredWidth: parent.width/2
                text: "Отмена"
                onClicked: settingsDialog.close()
                //                width: parent.width * 0.4
            }
            Button {
                flat: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignRight
                //                    Layout.fillWidth: true
                text: "Сохранить"
                //                    Material.background: "#4CAF50"
                //                width: parent.width * 0.4
                onClicked: {
                    recorder.recordLenght = rlModel.get(rlCb.currentIndex).value;
                    settingsDialog.close();
                }
            }
        }
    }
}
