import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import org.mgallien.QmlExtension 1.0
import QtMultimedia 5.4

Item {
    property string mediaServerDeviceUDN
    property var pagesModel
    property var mediaServerDevice
    property StackView parentStackView
    property MediaPlayList playListModel

    property string globalBrowseFlag: 'BrowseDirectChildren'
    property string globalFilter: '*'
    property string globalSortCriteria: ''

    id: contentDirectoryRoot

    UpnpContentDirectoryModel {
        id: contentDirectoryModel
        browseFlag: globalBrowseFlag
        filter: globalFilter
        sortCriteria: globalSortCriteria
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Button {
            id: backButton

            height: Screen.pixelDensity * 8.
            Layout.preferredHeight: height
            Layout.minimumHeight: height
            Layout.maximumHeight: height
            Layout.fillWidth: true

            onClicked: if (listingView.depth > 1) {
                           listingView.pop()
                       } else {
                           parentStackView.pop()
                       }
            text: 'Back'
        }

        StackView {
            id: listingView

            Layout.fillHeight: true
            Layout.fillWidth: true

            initialItem: MediaServerListing {
                contentDirectoryService: contentDirectoryModel.contentDirectory
                rootId: '0'
                stackView: listingView
                contentModel: contentDirectoryModel
                playListModel: contentDirectoryRoot.playListModel
            }

            // Implements back key navigation
            focus: true
            Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                                 stackView.pop();
                                 event.accepted = true;
                             }
        }
    }

    function init() {
        mediaServerDevice = pagesModel.service(mediaServerDeviceUDN)
        contentDirectoryModel.contentDirectory = contentDirectoryRoot.mediaServerDevice
    }
}
