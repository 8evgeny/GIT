import QtQuick 2.2
import QtQuick.Layouts 1.0
import QtMultimedia 5.8
import Qt.labs.folderlistmodel 2.1


Item {
    id: background
    width: 500
    height: 200




    MediaPlayer {
        id: player
    }

    ListView
   {
        id: listView
       anchors.fill: parent




       FolderListModel
       {
           id: foldermodel
           folder: "file:///home/user/music"
           showDirs: true
           //showDotAndDotDot: true
           //nameFilters: ["*"]
           nameFilters: ["*.mp3", "*.wav"]
           sortField :  "Name"
       }

       Component {

           id: delegate


           Row {
               id: row
               Rectangle {

                   height: columnIndex.height
                   width: columnIndex.width
                   color: "#C8E2F9"
//                   border.color: "black"
//                   border.width: 1

                   Text {
                       anchors.centerIn: parent
                       text: model.index + 1
                   }
               }

               Rectangle {
                    id: colName
                   height: columnName.height
                   width: columnName.width
//                   border.color: "black"
//                   border.width: 1
                   color: row.ListView.isCurrentItem ? "skyblue" : "#C8E2F9"
//                   states: [ State {
//                           name: "highlight"
//                       PropertyChanges {
//                           target: colName
//                           color: "skyblue"
//                       }
//                   } ]


                   Text {
                       anchors.centerIn: parent
                       text: fileName
                   }
                   MouseArea {
                                    anchors.fill: parent
                                    onClicked: {row.ListView.view.currentIndex = model.index}
                                }
//                   MouseArea {
//                                       anchors.fill: parent
//                                       onClicked: {
//                                           //ListView.view.currentIndex = model.index;
//                                           if (colName.state  === "") {
//                                                    colName.state  = "highlight"
//                                                  } else {
//                                                      colName.state = ""
//                                                  }
//                                       }
//                                   }


               }

           }

       }
       model: foldermodel
       delegate: delegate
       focus: true

   }

    Item {
        id: playLogic

        property int index: -1
        property MediaPlayer mediaPlayer: player
        property FolderListModel items: FolderListModel {
            showDirs: false
            showDotAndDotDot: true
            folder: "file:///home/user/music"
            nameFilters: ["*.mp3", "*.wav"]
            sortField :  "Name"
        }

        function init(){
            if(mediaPlayer.playbackState===1){
                mediaPlayer.pause();
            }else if(mediaPlayer.playbackState===2){
                mediaPlayer.play();
            }else{
                setIndex(0);
            }
        }

        function setIndex(i)
        {
            index = i;

            if (index < 0 || index >= items.count)
            {
                index = -1;
                mediaPlayer.source = "";
            }
            else{
                mediaPlayer.source = "file://" + items.get(index,"filePath");
                console.log(mediaPlayer.source);
                listView.currentIndex = index;
                trackTitle.text = items.get(index,"fileName");
                mediaPlayer.play();
            }
        }

        function next(){
            setIndex(index + 1);
        }

        function previous(){
            setIndex(index - 1);
        }

        Connections {
            target: playLogic.mediaPlayer

            onPaused: {
                playPause.source = "icons/play.png";
            }

            onPlaying: {
                 playPause.source = "icons/pause.png";
            }

            onStopped: {
                playPause.source = "icons/play.png";
                if (playLogic.mediaPlayer.status == MediaPlayer.EndOfMedia)
                    playLogic.next();
            }

            onError: {
                console.log(error+" error string is "+errorString);
            }

            onMediaObjectChanged: {
                if (playLogic.mediaPlayer.mediaObject)
                    playLogic.mediaPlayer.mediaObject.notifyInterval = 50;
            }
        }
    }

    Rectangle {
        id: foreground
        x: columnName.width+columnIndex.width
        y: 0
        width: 0
        height: 0

        //anchors.horizontalCenter: parent.horizontalCenter
        //anchors.verticalCenter: parent.verticalCenter

        ColumnLayout{
            id: container
            anchors.horizontalCenter: parent.horizontalAlignment
            anchors.verticalCenter: parent.verticalAlignment
            width: foreground.implicitWidth - 80
            height: foreground.implicitHeight - 60

            RowLayout {
                id: wrapper
                anchors.fill: parent

                ColumnLayout {
                    id: rightWapper
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    RowLayout {
                        id: upperWrap
                        Layout.fillWidth: true
                        Layout.preferredHeight: 100
                        Layout.leftMargin: 20
                        spacing: 25

                        Image {
                            id: prevTrack
                            source: "icons/rewind.png"
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 20
                            state: "none"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: playLogic.previous()
                                onPressed: prevTrack.state = "pressed"
                                onReleased: prevTrack.state = "none"
                            }
                            states: State {
                                name: "pressed"
                                when: mouseArea.pressed
                                PropertyChanges { target: prevTrack; scale: 0.8 }
                            }
                            transitions: Transition {
                                NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                            }
                        }

                        Rectangle{
                            width: 30
                            anchors.verticalCenter: parent.verticalCenter
                            Image {
                                id: playPause
                                source: "icons/play.png"
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.horizontalCenter: parent.horizontalCenter
                                state: "none"
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: playLogic.init();
                                    onPressed: playPause.state = "pressed"
                                    onReleased: playPause.state = "none"
                                }
                                states: State {
                                    name: "pressed"
                                    when: mouseArea.pressed
                                    PropertyChanges { target: playPause; scale: 0.8 }
                                }
                                transitions: Transition {
                                    NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                                }
                            }
                        }

                        Image {
                            id: nextTrack
                            source: "icons/forward.png"
                            anchors.verticalCenter: parent.verticalCenter
                            state: "none"

                            MouseArea {
                                anchors.fill: parent
                                onClicked: playLogic.next()
                                onPressed: nextTrack.state = "pressed"
                                onReleased: nextTrack.state = "none"
                            }
                            states: State {
                                name: "pressed"
                                when: mouseArea.pressed
                                PropertyChanges { target: nextTrack; scale: 0.8 }
                            }
                            transitions: Transition {
                                NumberAnimation { properties: "scale"; duration: 100; easing.type: Easing.InOutQuad }
                            }
                        }

                        Item {
                            Layout.fillWidth: true

                            ColumnLayout {
                                anchors.verticalCenter: parent.verticalCenter
                                Layout.fillWidth: true

                                Text {
                                    id: trackTitle
                                    text: ""
                                    color: "#eeeeee"
                                    font.pointSize: 17
                                    font.bold: true
                                    style: Text.Raised
                                    wrapMode: Text.Wrap
                                }
                            }
                        }
                    }
               }
            }
        }
    }
}
