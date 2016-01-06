import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import org.kde.plasma.mobilecomponents 0.2 as MobileComponents
import org.mgallien.QmlExtension 1.0

MobileComponents.Page {
    property var serverModel
    property var globalPlayListModel
    property string globalBrowseFlag: 'BrowseDirectChildren'
    property string globalFilter: '*'
    property string globalSortCriteria: ''

    id: rootItem

    objectName: "ServerList"
    color: MobileComponents.Theme.viewBackgroundColor

    ColumnLayout {
        anchors.fill: parent

        MobileComponents.Heading {
            Layout.bottomMargin: MobileComponents.Units.largeSpacing
            text: "UPnP Servers"
            Layout.fillWidth: true
        }

        ScrollView {
            Layout.fillHeight: true
            Layout.fillWidth: true

            ListView {
                id: serverView
                model: serverModel
                anchors.fill: parent

                delegate: MobileComponents.ListItem {
                    height: 120
                    enabled: true

                    content: MobileComponents.Label {
                        id: nameLabel
                        anchors.fill: parent
                        text: model.name
                    }


                    onClicked: {
                        contentDirectoryItem.contentDirectoryService = serverModel.remoteServer(index).contentDirectory
                        contentDirectoryModelItem.contentDirectory = serverModel.remoteServer(index).contentDirectory
                        contentDirectoryItem.rootIndex = contentDirectoryModelItem.indexFromId('0')
                        stackView.push(contentDirectoryItem)
                    }
                }
            }
        }
    }

    MediaServerListing {
        id: contentDirectoryItem

        rootIndex: contentDirectoryModelItem.indexFromId('0')
        playListModel: globalPlayListModel
        contentModel: contentDirectoryModelItem

        visible: false
    }

    UpnpContentDirectoryModel {
        id: contentDirectoryModelItem
        browseFlag: globalBrowseFlag
        filter: globalFilter
        sortCriteria: globalSortCriteria
        contentDirectory: contentDirectoryItem.contentDirectoryService
        useLocalIcons: true
    }
}
