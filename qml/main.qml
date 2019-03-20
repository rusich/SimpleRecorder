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
    title: qsTr("SimpleRecorder")

    onClosing: {
        if(recorder.isRecording) {
            close.accepted = false;
        }
        else return;
    }

    Connections  {
        target: recorder
        onMuted: {
            mutedIndicator.text = "MUTED: " + muted;
            mutedIndicator.visible = true;
        }
    }

    Item {
        id: container
        anchors.fill: parent

        Text {
            id: mutedIndicator
            visible: false
            font.pointSize: 60
            color: "red"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
        }

        RoundButton {
            //        focus: true
            //        Keys.onPressed: {
            //            if (event.key === Qt.Key_Back) {
            //                event.accepted = true;
            //            }
            //        }
            id: recordBtn
            text: ""
            width: parent.width/3
            height: width
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 40
            Material.background: recorder.isRecording? "#ff0000": "#4CAF50"

            onClicked: {
                if(!recorder.isRecording) {
                    recorder.startRecord();
                }
                else {
                    recorder.stopRecordRotation();
                }
            }

            Image {
                id: micImg
                fillMode: Image.PreserveAspectFit
                source:  recorder.isRecording? "qrc:///images/stop.png" : "qrc:///images/microphone.png"
                anchors.centerIn: parent
                height:  parent.height * 0.8
            }

            SequentialAnimation
            {
                running: recorder.isRecording
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


        Label {
            id: lblFileName
            text: "Записывается в файл:\n"+ recorder.fileName
            visible: recorder.isRecording
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: lblDuration.top
            anchors.bottomMargin: 20
            font.pointSize: parent.width*0.07
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter

        }

        Label {
            id: lblDuration
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.leftMargin: font.pointSize
            font.weight: Font.DemiBold
            font.pointSize: parent.width*0.2
            horizontalAlignment: Text.AlignHCenter
            color: "#78d1fe"
            text: recorder.durationString

        }

        // settings Button
        RoundButton {
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
                for(var i=0; i<rlModel.count; i++) {
                    if(rlModel.get(i).value === recorder.recordLenght){
                        rlCb.currentIndex = i;
                        break;
                    }
                }
                recordNameTe.text = recorder.recordName;
                teSenderName.text = mailer.senderName;
                teSenderEmail.text = mailer.login;
                tePassword.text = mailer.password;
                teRecipient.text = mailer.recipientEmail;

                settingsDialog.visible = true;
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

        // Settings Dialog
        Popup {
            id: settingsDialog
            visible: false
            modal: true
            dim: true
            focus: true
            anchors.centerIn: parent
            width: app.width * 0.9
            height: 600


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
                id: lenghtDescription
                anchors.left: dlgCaption.left
                anchors.top: dlgCaption.bottom
                anchors.topMargin: 10
                width: parent.width * 0.92
                font.pointSize: 18
                wrapMode:  Text.Wrap
                text: qsTr("Длительность записи для каждого файла (в минутах): ")
            }

            ComboBox
            {
                id: rlCb
                textRole: "key"
                anchors.left: lenghtDescription.left
                anchors.top: lenghtDescription.bottom
                anchors.topMargin: 0
                width: parent.width * 0.92
                model: ListModel
                {
                    id: rlModel
                    ListElement { key: "1 мин."; value: 1 }
                    ListElement { key: "5 мин."; value: 5 }
                    ListElement { key: "10 мин."; value: 10 }
                }
            }
            Label {
                id: nameDescription
                anchors.left: rlCb.left
                anchors.top: rlCb.bottom
                anchors.topMargin: 10
                width: parent.width * 0.92
                font.pointSize: 18
                wrapMode:  Text.Wrap
                text: qsTr("Название записи: ")
            }

            TextField {
                id: recordNameTe
                anchors.left: nameDescription.left
                anchors.top: nameDescription.bottom
                anchors.topMargin: 0
                width: parent.width * 0.92
                font.pointSize: 18
            }

            //sender name
            Label {
                id: lblSenderName
                anchors.left: recordNameTe.left
                anchors.top: recordNameTe.bottom
                anchors.topMargin: 10
                width: parent.width * 0.92
                font.pointSize: 18
                wrapMode:  Text.Wrap
                text: qsTr("Имя отправителя: ")
            }

            TextField {
                id: teSenderName
                anchors.left: lblSenderName.left
                anchors.top: lblSenderName.bottom
                anchors.topMargin: 0
                width: parent.width * 0.92
                font.pointSize: 18
            }

            // sender email
            Label {
                id: lblSenderEmail
                anchors.left: teSenderName.left
                anchors.top: teSenderName.bottom
                anchors.topMargin: 10
                width: parent.width * 0.92
                font.pointSize: 18
                wrapMode:  Text.Wrap
                text: qsTr("Email (mail.ru): ")
            }

            TextField {
                id: teSenderEmail
                anchors.left: lblSenderEmail.left
                anchors.top: lblSenderEmail.bottom
                anchors.topMargin: 0
                width: parent.width * 0.92
                font.pointSize: 18
            }

            // password
            Label {
                id: lblPassword
                anchors.left: teSenderEmail.left
                anchors.top: teSenderEmail.bottom
                anchors.topMargin: 10
                width: parent.width * 0.92
                font.pointSize: 18
                wrapMode:  Text.Wrap
                text: qsTr("Пароль: ")
            }

            TextField {
                id: tePassword
                anchors.left: lblPassword.left
                anchors.top: lblPassword.bottom
                anchors.topMargin: 0
                width: parent.width * 0.92
                echoMode: TextInput.Password
                font.pointSize: 18
            }

            // recipient
            Label {
                id: lblRecipient
                anchors.left: tePassword.left
                anchors.top: tePassword.bottom
                anchors.topMargin: 10
                width: parent.width * 0.92
                font.pointSize: 18
                wrapMode:  Text.Wrap
                text: qsTr("Получатель: ")
            }

            TextField {
                id: teRecipient
                anchors.left: lblRecipient.left
                anchors.top: lblRecipient.bottom
                anchors.topMargin: 0
                width: parent.width * 0.92
                font.pointSize: 18
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
                    text: "Отмена"
                    onClicked: settingsDialog.close()
                }
                Button {
                    flat: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignRight
                    text: "Сохранить"
                    onClicked: {
                        recorder.recordLenght = rlModel.get(rlCb.currentIndex).value;
                        recorder.recordName = recordNameTe.text;
                        mailer.senderName = teSenderName.text;
                        mailer.login = teSenderEmail.text;
                        mailer.password = tePassword.text;
                        mailer.recipientEmail = teRecipient.text;
                        settingsDialog.close();
                    }
                }
            } // bottomBtns
        }
    } // container
}
