import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    id: appWindow
    width: 450
    height: 400
    visible: true
    title: qsTr("经济")

    RowLayout {
        anchors.fill: parent

        Text {
            text: qsTr("用户名：")
        }

        Text {
            text: qsTr("密码：")
        }

        Button {
            text: qsTr("确定")
            onClicked: model.submit()
        }
        Button {
            text: qsTr("取消")
            onClicked: model.revert()
        }
    }
}
