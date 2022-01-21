/*!
 \file appcore.h

*/

#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QTextStream>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QQueue>
#include <QStack>
#include <QtNetwork>
#include "RuLang.h"
#include <QColor>

/*!
 \brief This class contains signals and slots necessary for working with qml part of the software configurator,
        the class contains the necessary constants and methods and is the base object.

 \class AppCore appcore.h "appcore.h"
*/
class AppCore : public QObject
{
    /*!
     \brief This structure is a type for storing a list of macros.

     \typedef ListOfMacros    */
    using ListOfMacros = struct {
        qint32 nameOfPriority{}; /*! Priority Name */
        qint32 nameOfRepeat{}; /*! Repeat name */
        QString nameOfMacro; /*! Macro name */
        QString nameOfMsg; /*! Macro message */
    } ;

    /*!
     \brief This structure is a type for storing a list of keys.

    \typedef ListOfkeys    */
    using ListOfkeys =  struct {
        QString uidOfKey; /*! Unique key identifier */
        QString numberOfKey; /*! Key number */
        QString nameOfKey; /*!< Key name */
        QString functionOfKey; /*! Function for key */
        QString assignedKey; /*! Key assignment */
        QString prioriryKey; /*! Key priority */
        QString modeKey; /*! Key mode */

    };

    /*!
     \brief This structure is a type for storing a list of stations.

    \typedef ListOfStations    */
    using ListOfStations = struct {
        QString nameOfStation; /*! Station name */
        QString nameOfID; /*! ID name */
        QString ip; /*!< IP address */
        QString maskNetwork; /*!< Netmask */
        QString gateway; /*! Gateway */
        QList < ListOfkeys> listOfKeys; /*! This structure is a object for storing a list of keys. */
    } ;

    /*!
     \brief This structure is a type for storing a list of groups.

    \typedef ListOfGroups    */
    using ListOfGroups =  struct {
        QString nameOfGorup; /*! Group name */
        QString nameOfID; /*! ID name */
        QList < ListOfStations> stations; /*! This structure is a object for storing a list of groups. */
    };

    /*!
     \brief This structure is a type for storing a list of stations for status.

    \typedef ListOfStationsStatus    */
        using ListOfStationsStatus =  struct {
        QString colorStation;
        QString version;
        QString mac;
        QList < ListOfStations> stations; /*! This structure is a object for storing a list */
    };

    /*!
     \brief Codes for matching string function keys

     \enum stringCodes
    */
    enum stringCodes {
        noneConnection,
        directConnectionDuplex,
        groupCommunication,
        circularBond,
        conferenceCall,
        telephoneCommunications,
        directConnectionSimplex,
        hangUp
    };

    Q_OBJECT
public:

    explicit AppCore(QObject *parent = nullptr);

signals:
    //! Signal to transmit data to interface qml-interface

    /*!
     \brief Exit from main menu

     \fn exitFromMenu
    */
    void exitFromMenu();

    /*!
     \brief Create a status window

     \fn createWindowStatus
     \param idDevice ID Station
     \param msgDevice Current message
    */
    void createWindowStatus(QString idDevice, QString msgDevice);

    /*!
     \brief Event to close the main window

     \fn closeMainWindow
    */
    void closeMainWindow();

    /*!
     \brief There are changes in the project, a window call to save

     \fn windowChangesBeforeSave
    */
    void windowChangesBeforeSave();

    /*!
     \brief There are changes in the project, a window call to save (Menu -> Save)

     \fn windowChangesMenuSave
    */
    void windowChangesMenuSave();

    /*!
     \brief Set counter for stations after loading a file

     \fn setSounterDigitalStation
     \param idSetStaion
    */
    void setCounterDigitalStation(qint32 idSetStaion);

    /*!
     \brief Set counter for groups after loading a file

     \fn setSounterDigitalStation
     \param idSetStaion
    */
    void setCounterGroup(qint32 idSetStaion);

    /*!
     \brief This method sends the name and number of the device.

     \fn sendToQmlSubscriber
     \param str Station name
     \param number Station number
    */
    void sendToQmlSubscriber(QString str, qint32 number);

    /*!
     \brief This method sends the macro name to populate the macro list.

     \fn sendListNameOfMacro
     \param name Macro name
    */
    void sendListNameOfMacro(QString name);

    /*!
     \brief Send a list of element names to fill in with the station key.

     \fn sendListNameOfElements
     \param uidOfKeyAdd Unique key identifier
     \param numberOfKeyAdd  Key number
     \param nameOfKeyAdd Key name
     \param functionOfKeyAdd Function for key
     \param assignedKeyAdd Key assignment
    */
    void sendListNameOfElements(QString uidOfKeyAdd,
                                QString numberOfKeyAdd,
                                QString nameOfKeyAdd,
                                QString functionOfKeyAdd,
                                QString assignedKeyAdd);

    /*!
     \brief Send error message if macro does not exist.

     \fn sendErrorMacroExists
    */
    void sendErrorMacroExists();

    /*!
     \brief Error sending macro changes

     \fn sendErrorMacroExistsChanges
    */
    void sendErrorMacroExistsChanges();

    /*!
     \brief Send and fill in information about the selected macro.

     \fn sendSelectedMacro
     \param numberOfMacro Macro number
     \param nameOfMacro Macro name
     \param nameOfPriority Macro Priority
     \param nameOfMsg Macro message name
     \param nameOfRepeat Repeat
    */
    void sendSelectedMacro(qint32 numberOfMacro, QString nameOfMacro, qint32 nameOfPriority, QString nameOfMsg, qint32 nameOfRepeat);

    /*!
     \brief Send and fill in the list of existing stations for the group menu.

     \fn sendListOfStations
     \param nameOfStation Name station
     \param nameOfId Name ID
    */
    void sendListOfStations(QString nameOfStation, QString nameOfId);

    /*!
     \brief We send and fill in the list of existing stations for the group menu with which there were changes and actions.

     \fn sendUpdateListOfStations
     \param nameOfStation Name station
     \param nameOfId Name id
    */
    void sendUpdateListOfStations(QString nameOfStation, QString nameOfId);

    /*!
     \brief Send an updated list of groups for the group menu.

     \fn sendUpdateListOfGroups
     \param nameOfStation Name station
     \param nameOfId Name id
    */
    void sendUpdateListOfGroups(QString nameOfStation, QString nameOfId);

    /*!
     \brief Clear list of stations for the group menu.

     \fn sendClearListOfStations
    */
    void sendClearListOfStations();

    /*!
     \brief Send an updated list of groups, clear the list of members.

     \fn sendClearListOfParts
    */
    void sendClearListOfParts();

    /*!
     \brief Clear list of groups for the group menu.

     \fn sendClearListOfGroups
    */
    void sendClearListOfGroups();

    /*!
     \brief Submit correct data identifier index list of groups.

     \fn sendIndexCorrectDataIdentListOfGroups
     \param index Index for the list of groups
     \param dataIdent Data identifier
    */
    void sendIndexCorrectDataIdentListOfGroups(int index, QString dataIdent);

    /*!
     \brief Submit correct data identifier name for the list of groups.

     \fn sendIndexCorrectDataNameListOfGroups
     \param index Index for the list of groups
     \param dataName Name of the index
    */
    void sendIndexCorrectDataNameListOfGroups(int index, QString dataName);

    /*!
     \brief Send index error number.

     \fn sendNumberOfErrorIndex
     \param numberOfErrorIndex Error index
    */
    void sendNumberOfErrorIndex(int numberOfErrorIndex);

    /*!
     \brief Send correct index number.

     \fn sendNumberOfCorrectIndex
     \param numberOfCorrectIndex Number of the correct index.
    */
    void sendNumberOfCorrectIndex(int numberOfCorrectIndex);

    /*!
     \brief Send visible subscriber. Allow the visibility of the subscribers field.

     \fn sendVisibleSubsriber
     \param stateOnVisible State for sending about visible
    */
    void sendVisibleSubsriber(bool stateOnVisible);

    /*!
     \brief Send visible groups. Allow the visibility of the subscribers field.

     \fn sendVisibleGroups
     \param stateOnVisible State for sending about visible
    */
    void sendVisibleGroups(bool stateOnVisible);

    /*!
     \brief This method sends the name of the station.

     \fn sendNameOfStation
     \param indexOfStationFromList Index for the station from the list of stations
     \param nameOfStationList Name for the station from the list of stations
    */
    void sendNameOfStation(qint32 indexOfStationFromList, QString nameOfStationList);

    /*!
     \brief Send a list of group updates from a file.

     \fn sendUpdateListOfGroupsFromFile
     \param nameOfGroup Group name
     \param nameOfId Group ID
    */
    void sendUpdateListOfGroupsFromFile(QString nameOfGroup, QString nameOfId);

    /*!
     \brief Send a list of stations updates from a file.

     \fn sendUpdateListOfStationsFromFile
     \param nameOfStation Station name
     \param nameOfId Station ID
    */
    void sendUpdateListOfStationsFromFile(QString nameOfStation, QString nameOfId);

    /*!
     \brief Send a request to clear the station list.

     \fn sendClearAllListOfStations
    */
    void sendClearAllListOfStations();

    /*!
     \brief Send a request to clear the group list.

     \fn sendClearAllListOfGroups
    */
    void sendClearAllListOfGroups();

    /*!
     \brief Send update of station resize list.

     \fn sendUpdateListOfStationResize
    */
    void sendUpdateListOfStationResize();

    /*!
     \brief Send information about network configuration for the station.

     \fn sendInfoNetworkAboutTheStation
     \param nameOfStationMain Station name
     \param nameOfId Name ID
     \param nameIP Name IP address
     \param nameMaskNetwork Mask network
     \param nameGateway Gateway
    */
    void sendInfoNetworkAboutTheStation(QString nameOfStationMain, QString nameOfId, QString nameIP, QString nameMaskNetwork, QString nameGateway);

    /*!
     \brief Send information priority keys to the station.

     \fn sendInfoPrioriryKeysAboutTheStation
     \param modeKeyStation Key mode
     \param prioriryKeyStation Key priority
    */
    void sendInfoPrioriryKeysAboutTheStation(QString modeKeyStation, QString prioriryKeyStation);

    /*!
     \brief Send encrypted file name with extension *.enc.

     \fn sendEncFileName
     \param encFileName
    */
    void sendEncFileName(QString encFileName);

    /*!
     \brief Send MD5 for the firmware file. This method fills the text field with MD5.

     \fn sendMD5FileName
     \param MD5 String MD5
    */
    void sendMD5FileName(QString MD5);

    /*!
     \brief Send size for the firmware.

     \fn sendSizeFileName
     \param sizeFile Size firmware
    */
    void sendSizeFileName(QString sizeFile);

    /*!
     \brief Send information about COM-port/TTY-port

     \fn sendToQmlPortSerial
     \param portSerialTxt Name COM-port
    */
    void sendToQmlPortSerial(QString portSerialTxt);

    /*!
     \brief Delete the index of the selected port.

     \fn sendToQmlPortSerialDeleteIndex
     \param deleteIndex Com-port index
    */
    void sendToQmlPortSerialDeleteIndex(int deleteIndex);

    /*!
     \brief Clear the list of UART/COM-ports

     \fn sendToQmlPortSerialDeleteAll
    */
    void sendToQmlPortSerialDeleteAll();

    /*!
     \brief Current opened port.

     \fn sendToQmlPortOpenedPort
     \param openedPort Opened port name
    */
    void sendToQmlPortOpenedPort(QString openedPort);

    /*!
     \brief This method send information about paths of saved confutation files.

     \fn historyOfFiles
     \param file1 First path
     \param file2 Second path
     \param file3 Third path
    */
    void historyOfFiles(QString file1, QString file2, QString file3);

    //! assignment_list.cpp and assignment_list.qml
    //! send the current list of stations and groups for the assignment list
    /*!
     \brief Common list of stations and groups.

     \fn listOfStationsAndGroups
     \param itemForAdding This element is needed to add to the list
     \param nameStationAdding  This element is needed to add to the list
    */
    void listOfStationsAndGroups(QString itemForAdding, QString nameStationAdding);

    /*!
     \brief Send the selected name from the list assignment.

     \fn sendToQmlSubscriberAssignment
     \param strAssignmentName Assignment name
    */
    void sendToQmlSubscriberAssignment(QString strAssignmentName);

    /*!
     \brief Send subscriber assignment for RS232 Window.

     \fn sendToQmlSubscriberAssignmentRs232
     \param strAssignmentName Assignment name
    */
    void sendToQmlSubscriberAssignmentRs232(QString strAssignmentName);

    /*!
     \brief Send subscriber assignment for Firmware Window.

     \fn sendToQmlSubscriberAssignmentFirmware
     \param strAssignmentName Assignment name
    */
    void sendToQmlSubscriberAssignmentFirmware(QString strAssignmentName);

    /*!
     \brief Get a list of current station positions.

     \fn getCurrentPositionListOfStation
    */
    void getCurrentPositionListOfStation();

    /*!
     \brief Restore the current list of station positions.

     \fn restoreCurrentPositionListOfStation
    */
    void restoreCurrentPositionListOfStation();

    //!verification
    /*!
     \brief Send general error message.

     \fn sendWrongGeneric
    */
    void sendWrongGeneric();

    /*!
     \brief Send a message about successful verification.

     \fn sendOkGeneric
    */
    void sendOkGeneric();

    /*!
     \brief Send error information in ID.

     \fn sendWrongNameId
     \param nameOfStation Station ID
    */
    void sendWrongNameId(QString nameOfStation);

    /*!
     \brief Send error information in name.

     \fn sendWrongName
     \param nameOfStation Station ID
    */
    void sendWrongName(QString nameOfStation);

    /*!
     \brief Send error information in type assign.

     \fn sendWrongTypeAssign
     \param nameOfStation Type assign
    */
    void sendWrongTypeAssign(QString nameOfStation);

    /*!
     \brief Send error information in key ID.

     \fn sendWrongIdKeyNotValid
     \param nameOfStation Key ID
    */
    void sendWrongIdKeyNotValid(QString nameOfStation);

    /*!
     \brief Send error information about IP address.

     \fn sendWrongIp
     \param nameOfStation IP address
    */
    void sendWrongIp(QString nameOfStation);

    /*!
     \brief Send error information about network mask.

     \fn sendWrongMask
     \param nameOfStation Network mask
    */
    void sendWrongMask(QString nameOfStation);

    /*!
     \brief Send error information about network gateway.

     \fn sendWrongGateway
     \param nameOfStation Gateway
    */
    void sendWrongGateway(QString nameOfStation);

    /*!
     \brief Send error information in key number.

     \fn sendWrongNumberKeyNotValid
     \param nameOfStation Key number
    */
    void sendWrongNumberKeyNotValid(QString nameOfStation);

    /*!
     \brief Send error information in key function.

     \fn sendWrongFunctionKeys
     \param nameOfStation Key function
    */
    void sendWrongFunctionKeys(QString nameOfStation);

    /*!
     \brief Send error information in ID.

     \fn sendWrongCidId
     \param nameOfStation ID
    */
    void sendWrongCidId(QString nameOfStation);

    /*!
     \brief Send error information in key mode.

     \fn sendWrongMode
     \param nameOfStation Key mode
    */
    void sendWrongMode(QString nameOfStation);

    /*!
     \brief Send error information in key priority.

     \fn sendWrongPriority
     \param nameOfStation Key priority
    */
    void sendWrongPriority(QString nameOfStation);

    //!network
    /*!
     \brief

     \fn addNetworkListGroups
     \param nameOfNetworkList
    */
    void addNetworkListGroups(QString nameOfNetworkList);

    /*!
     \brief Add to the network list group

     \fn addNetworkListGroupsOn
     \param nameOfNetworkList Network card
    */
    void addNetworkListGroupsOn(QString nameOfNetworkList);

    /*!
     \brief No network card selected, network card must be selected

     \fn needSelectNetworkCard
    */
    void needSelectNetworkCard();

    /*!
     \brief Update the network list group

     \fn updateNetworkListGroups
    */
    void updateNetworkListGroups();

    //!programmer
    /*!
     \brief Fill in the fields of the programmer window.

     \fn fillInfoForProgrammerWindow
     \param fillNumber Number station
     \param fillName Name station
     \param fillIpb IP address of the station
    */
    void fillInfoForProgrammerWindow(QString fillNumber, QString fillName, QString fillIp);

     /*!
      \brief Fill in the fields of the programmer window.

      \fn fillInfoForProgrammerWindow
      \param fillNumber Number station
      \param fillName Name station
      \param fillIpb IP address of the station
      \param fillColor Color status
      \param fillMac MAC
      \param fillVersion Version firmware
    */
    void fillInfoForProgrammerWindowByJson(QString fillNumber, QString fillName, QString fillIp, QString fillColor, QString fillMac, QString fillVersion);

    /*!
     \brief This method clears the list of stations for status

     \fn clearListOfStationsStatus
     */
    void clearListOfStationsStatus();

    /*!
     \brief Send JSON for the selected box.

     \fn sendJsonUseSelectedBox
    */
    void sendJsonUseSelectedBox();

    /*!
     \brief This method clears the list of stations

     \fn clearListOfStations
    */
    void clearListOfStations();

    /*!
     \brief This method clears the list of groups

     \fn clearListOfGroups
    */
    void clearListOfGroups();

    /*!
     \brief Get verification for the station.

     \fn getStartVerificationStation
    */
    void getStartVerificationStation();

    /*!
     \brief Clear subscribers

     \fn clearVisibleSubsriber
    */
    void clearVisibleSubsriber();

    /*!
     \brief Send wrong Name in groups Id

     \fn sendWrongNameInGroupsId
    */
    void sendWrongNameInGroupsId(QString nameOfGroup);

public slots:
    //! The slot for the receiving of data from the QML-interface

    /*!
     \brief Get status from stations

     \fn readStatusFromStations
     \param nameID Name subscriber
    */
    void readStatusFromStations(const QString &nameID);

    /*!
     \brief Station configuration record

    \fn readStatusFromStations
    \param nameID Name subscriber
    */
    //void writeConfigurationToStations(const QString &nameID);

    /*!
     \brief Get the subscriber from the subscriber window.

     \fn receiveFromQmlSubscriber
     \param str Name subscriber
     \param number Number from the list
    */
    void receiveFromQmlSubscriber(QString str, qint32 number);

    /*!
     \brief Receive macro from the macro window.

     \fn receiveFromQmlMacro
     \param nameOfMacro Macro name
     \param nameOfPriority Macro priority
     \param nameOfMsg Macro massage
     \param nameOfRepeat Macro repeat
    */
    void receiveFromQmlMacro(const QString &nameOfMacro, qint32 nameOfPriority, const QString &nameOfMsg, qint32 nameOfRepeat);

    /*!
     \brief Update the selected macro from list macros.

     \fn changesOfMacro
     \param nameOfMacro Macro name
     \param nameOfPriority Macro priority
     \param nameOfMsg Macro massage
     \param nameOfRepeat Name repeat
    */
    void changesOfMacro(const QString &nameOfMacro, qint32 nameOfPriority, const QString &nameOfMsg, qint32 nameOfRepeat);

    /*!
     \brief Get list exists macros.

     \fn getListOfMacros
    */
    void getListOfMacros();

    /*!
     \brief Fill the current macro.

     \fn fillMacro
     \param name Macro name
    */
    void fillMacro(const QString &name);

    /*!
     \brief Remove the macro.

     \fn removeMacro
     \param number Number macro
    */
    void removeMacro(qint32 number);

    /*!
     \brief Add new key block for the digital station.

     \fn addNewKeyBlock
     \param nameOfStation Station name
    */
    void addNewKeyBlock(const QString &nameOfStation);

    /*!
     \brief Delete the selected key block.

     \fn deleteBlock
     \param nameOfStation Station name
     \param uidOfKey Key uid
    */
    void deleteBlock(const QString &nameOfStation, const QString &uidOfKey);

    /*!
     \brief Enable visible the rectangle for groups.

     \fn enableVisibleGroups
    */
    void enableVisibleGroups();

    /*!
     \brief Get index for the selected digital station.

     \fn sendCurrentIndexOfDigitalStation
     \param index Station index
     \param nameOfStation Station name
    */
    void sendCurrentIndexOfDigitalStation(const qint32 &index, const QString &nameOfStation);

    /*!
     \brief Save the station.

     \fn saveStation
     \param nameOfStation Station name
     \param idOfStation ID name
    */
    void saveStation(const QString &nameOfStation, const QString &idOfStation);

    /*!
     \brief Delete the station by name.

     \fn deleteStation
     \param nameOfStation Station name
    */
    void deleteStation(const QString &nameOfStation);

    /*!
     \brief Get list all stations.

     \fn getListOfStations
    */
    void getListOfStations();

    /*!
     \brief Create the group for the list of groups.

     \fn createGroup
     \param nameOfGroup Group name
     \param idOfGroup Group ID
    */
    void createGroup(const QString &nameOfGroup, const QString &idOfGroup);

    /*!
     \brief Delete the group from list of groups.

     \fn deleteGroup
     \param nameOfGroup Group name
    */
    void deleteGroup(const QString &nameOfGroup);

    /*!
     \brief Add a new station to groups.

     \fn addNewStationToGroup
     \param nameOfGroup Group name
     \param nameOfStation Station name
    */
    void addNewStationToGroup(const QString &nameOfGroup, const QString &nameOfStation);

    /*!
     \brief Remove station from group.

     \fn deleteStationFromGroup
     \param nameOfGroup Group name
     \param nameOfStation Station name
    */
    void deleteStationFromGroup(const QString &nameOfGroup, const QString &nameOfStation);

    /*!
     \brief Update viewers for list groups.

     \fn updateViewListOfGroups
     \param nameOfGroup Index of the group
    */
    void updateViewListOfGroups(const QString &nameOfGroup);

    /*!
     \brief Update viewers for list groups.

     \fn updateViewListOfGroups
     \param indexOfGroup Index of the group
    */
    void updateViewListOfGroups(int indexOfGroup);

    /*!
     \brief Update viewers for list groups.

     \fn upgradeListOfGroupsIdent
     \param indexOfGroup  Index of the group
     \param ident Identification
    */
    void upgradeListOfGroupsIdent(int indexOfGroup, const QString &ident);

    /*!
     \brief Update name for list groups.

     \fn upgradeListOfGroupsName
     \param indexOfGroup Index of the group
     \param name Name
    */
    void upgradeListOfGroupsName(int indexOfGroup, const QString &name);

    /*!
     \brief Update key number for the subscriber.

     \fn updateKeySubscriberNumberOfKey
     \param nameOfStation Station name
     \param uidOfKey UID for the key
     \param numberOfKey Key number
    */
    void updateKeySubscriberNumberOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &numberOfKey);

    /*!
     \brief Update key name for the subscriber.

     \fn updateKeySubscriberNameOfKey
     \param nameOfStation Station name
     \param uidOfKey UID for the key
     \param nameOfKey Key name
    */
    void updateKeySubscriberNameOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &nameOfKey);

    /*!
     \brief Update key function for the subscriber.

     \fn updateKeySubscriberFunctionOfKey
     \param nameOfStation Station name
     \param uidOfKey UID for the key
     \param functionOfKey Key function
    */
    void updateKeySubscriberFunctionOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &functionOfKey);

    /*!
     \brief Update key assigned for the subscriber.

     \fn updateKeySubscriberAssignedOfKey
     \param nameOfStation Station name
     \param uidOfKey UID for the key
     \param assignedOfKey Key assigned
    */
    void updateKeySubscriberAssignedOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &assignedOfKey);

    /*!
     \brief Update key priority for the subscriber.

     \fn updateKeySubscriberPriorityOfKey
     \param nameOfStation Station name
     \param uidOfKey UID for the key
     \param priorityOfKey Key priority
    */
    void updateKeySubscriberPriorityOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &priorityOfKey);

    /*!
     \brief Update key mode for the subscriber.

     \fn updateKeySubscriberModeOfKey
     \param nameOfStation Station name
     \param uidOfKey UID for the key
     \param modeOfKey Key mode
    */
    void updateKeySubscriberModeOfKey(const QString &nameOfStation, const QString &uidOfKey, const QString &modeOfKey);

    //! Update mechanism for information stations
    /*!
     \brief Get a station name for the station.

     \fn updateStationName
     \param currentNameOfStation Current station name
     \param newNameOfStation New station name
    */
    void updateStationName(const QString &currentNameOfStation, const QString &newNameOfStation);

    /*!
     \brief Get a station ID for the station.

     \fn updateStationId
     \param currentNameOfStation Station name
     \param newNameOfId Station ID
    */
    void updateStationId(const QString &currentNameOfStation, const QString &newNameOfId);

    /*!
     \brief Get a new IP address for the station.

     \fn updateStationIp
     \param currentNameOfStation Station name
     \param newIP IP address
    */
    void updateStationIp(const QString &currentNameOfStation, const QString &newIP);

    /*!
     \brief Get a new network mask for the station.

     \fn updateStationMaskNetwork
     \param currentNameOfStation Station name
     \param newMaskNetwork Network mask
    */
    void updateStationMaskNetwork(const QString &currentNameOfStation, const QString &newMaskNetwork);

    /*!
     \brief Get a new gateway for the station.

     \fn updateStationGateway
     \param currentNameOfStation Station name
     \param newGateway Gateway
    */
    void updateStationGateway(const QString &currentNameOfStation, const QString &newGateway);

    /*!
     \brief Save a list of stations and Json groups.

     \fn saveListOfStationAndGroupsQJson
    */
    void saveListOfStationAndGroupsQJson();

    /*!
     \brief Save a list of stations and Json groups.

     \fn saveListOfStationAndGroupsQJson
     \param nameSave Name file for JSON
    */
    void saveListOfStationAndGroupsQJson(const QString &nameSave);

    /*!
     \brief Load a list of stations and Json groups.

     \fn loadListOfStationAndGroupsQJson
    */
    void loadListOfStationAndGroupsQJson();

    /*!
     \brief Load a list of stations and Json groups.

     \fn loadListOfStationAndGroupsQJson
     \param nameLoad Name file for JSON
    */
    void loadListOfStationAndGroupsQJson(const QString &nameLoad);

    /*!
     \brief This method encrypts the file, packs the firmware file, and also encrypts the firmware file

     \fn AppCore::encryptionBinFile
     \param pathFile Path to unencrypted firmware file
     \param key Encryption key
     \param dateTime Firmware creation time
     \param mainNumber Firmware version
     \param subNumber Firmware subversion
    */
    void encryptionBinFile(const QUrl &pathFile,  const QString &key, const QString &dateTime, const qint16 &mainNumber, const qint16 &subNumber);

    /*!
     \brief Get speed and dev name for the port.

     \fn receiveFromQmlOpenPort
     \param dev Dev name for OS system.
     \param speed Speed port.
    */
    void receiveFromQmlOpenPort(const QString &dev, qint32 speed);

    /*!
     \brief Get information that the port is closing.

     \fn receiveFromQmlClosePort
    */
    void receiveFromQmlClosePort();

    /*!
     \brief Get information about the station.

     \fn receiveInformationAboutPort
    */
    void receiveInformationAboutPort();

    /*!
     \brief Update COM-ports list.

     \fn updateListOfUsbUarts
    */
    void updateListOfUsbUarts();

    /*!
     \brief Get JSON settings to the station.

     \fn sendConfigJsonToStation
     \param stationID Station ID.
    */
    void sendConfigJsonToStation(const QString &stationID);

    /*!
     \brief Get a position in the list of stations.

     \fn sendPositionInListOfStation
    */
    void sendPositionInListOfStation();


    //! Assignment_list.cpp and assignment_list.qml
    /*!
     \brief Get the list of stations and groups from the assignment list.

     \fn getListOfStationsAndGroups
    */
    void getListOfStationsAndGroups();

    /*!
     \brief Get list of COM/TTY-ports

     \fn getListOfStationsRs232
    */
    void getListOfStationsRs232();

    /*!
     \brief The method transmits a list of an existing station, using this list you can select the desired station to download firmware.

     \fn getListOfStationsFirmware
    */
    void getListOfStationsFirmware();

    //! Get the selected name from the list assignment
    /*!
     \brief Get current name of stations and group name.

     \fn getCurrentNameOfStationsAndGroups
     \param nameItem
    */
    void getCurrentNameOfStationsAndGroups(const QString &nameItem);

    /*!
     \brief Get current name of stations from RS232 window.

     \fn getCurrentNameOfStationsRs232
     \param nameItem
    */
    void getCurrentNameOfStationsRs232(const QString &nameItem);

    //! Get the current/selected priorities for the selected key
    /*!
     \brief Get list of subscribers for key priority.

     \fn getSubscriberListPriorityOfKeys
     \param nameOfStation Station name
     \param uidOfKey UID key
    */
    void getSubscriberListPriorityOfKeys(const QString &nameOfStation, const QString &uidOfKey);

    /*!
     \brief Update the list of files for History.

     \fn updateInfoAboutListFiles
    */
    void updateInfoAboutListFiles();

    /*!
     \brief Update all views.

     \fn updateAllViews
    */
    void updateAllViews();

    /*!
     \brief Update all views from a position.

     \fn updateAllViewsWithPostion
    */
    void updateAllViewsWithPostion();

    /*!
     \brief Select language.

     \fn setCurrentLanguage
     \param lang Language RU/EN
    */
    void setCurrentLanguage(int lang);

    /*!
     \brief Start verification for all stations.

     \fn startVerificationStations
     \return bool Status
    */
    bool startVerificationStations();


    /*!
     \brief Start verification for all groups.

     \fn startVerificationStations
     \return bool Status
    */
    bool startVerificationGroups();

    /*!
     \brief Start verification for the station.

    \fn startVerificationStation
    */
    void startVerificationStation();


    /*!
     \brief Start verification for the station

     \fn startVerificationStation
     \param name Station name
    */
    void startVerificationStation(const QString& name);

    /*!
     \brief IP address verification.

     \fn ipVerification
     \param ip IP address
     \return bool Status
    */
    bool ipVerification(const QString &ip);

    /*!
     \brief Key verification by the station name and key number

     \fn keyVerification
     \param stationName Station name
     \param keyNumber Key number
     \return bool Status
    */
    bool keyVerification(const QString &stationName, const QString &keyNumber);

    /*!
     \brief Key function verification.

     \fn functionVerification
     \param functionName Function name
     \return bool Status
    */
    bool functionVerification(const QString &functionName);

    /*!
     \brief Get network card list.

     \fn getNetworkList
    */
    void getNetworkList();

    /*!
     \brief Save current selected network card

     \fn saveCurrentNetworkCard
     \param newNetworkCard Network card
    */
    void saveCurrentNetworkCard(const QString &newNetworkCard);

    /*!
     \brief Send data to the station via IP UDP.

     \fn sendDataByUdp
     \param data Byte array is current data
     \param Ip IP address
     \return bool Status
    */
    bool sendDataByUdp(const QByteArray &data, const QString &Ip);
    
    /*!
     \brief Send data to the station via IP UDP Mu.

    \fn sendDataByUdp
     \param data Byte array is current data
     \param Ip IP address
     \return bool Status
    */
    bool sendDataByUdpMulticast(const QByteArray &data, const QString &Ip);

    /*!
     \brief Send Json configuration to the station over Ethernet.

     \fn sendConfigJsonToStationByEthernet
     \param stationID Station ID
     \param stationID Target Station ID
    */
    void sendConfigJsonToStationByEthernet(const QString &stationID, const QString &toId);

    /*!
     \brief This method gets information about current stations

     \fn getInformationForProgrammerWindow
    */
    void getInformationForProgrammerWindow();

    /*!
     \brief Json submit request

     \fn requestSendJson
    */
    void requestSendJson();

    /*!
     \brief Start the firmware download by calling this method after clicking the “Download firmware” button,
            if the status argument is 1, then the firmware is transmitted to all subscribers who are in the multicast group,
            if the status is 0, then the firmware is transmitted by the station, the number, station number is selected from the list.

     \fn startLoadFirmware
     \param stationID Station name
     \param state 0 - transfer firmware by name, 1 - transfer firmware to all
    */
    void startLoadFirmware(const QString &stationID, bool state);

    /*!
     \brief This method to convert byte array to hex string

     \fn AppCore::hexStr
     \param data Byte array
     \return std::string Hex string
    */
    std::string hexStr(const QByteArray &data);

    /*!
     \brief This method to convert the hex of a string to an array of bytes

     \fn AppCore::strHex
     \param data Hex string
     \return QByteArray Byte array
    */
    QByteArray strHex(const std::string &data);

    /*!
     \brief This method resets the current state for the project

     \fn resetCurrentStateForProject
    */
    void resetCurrentStateForProject();

    /*!
     \brief Receive Json configuration from stations over Ethernet.

     \fn sendConfigJsonToStationByEthernet
     \param stationID Station ID
    */
    bool receiveDataByUdpMulticast(const QByteArray &data, const QString &Ip);

    /*!
     \brief Get status JSON

     \fn statusChangedJson
     \param idJson ID station
     \param versionJson Version firmware
     \param macJson MAC
     \param ipJson IP
    */
    void statusChangedJson(const QString &idJson, const QString &versionJson, const QString &macJson, const QString &ipJson);

    /*!
     \brief Verify Changes Before Saving

     \fn checkChanges
     \param state State for saving
    */
    void checkChanges(bool state);

    /*!
     \brief To need close the main window

     \fn needCloseMainWindow
    */
    void needCloseMainWindow();

    /*!
     \brief This method is required to remotely reboot the MCU.

     \fn rebootMcyByJson
     \param nameId Id station name
    */
    void rebootMcuByJson(const QString& nameId);

    /*!
     \brief This method is required to write configuration the MCU.

     \fn writeConfigMcuByJson
     \param nameId Id station name
    */
    void writeConfigMcuByJson(const QString& nameId);

    /*!
     \brief This method is required to delete configuration the MCU.

     \fn deleteConfigMcuByJson
     \param nameId Id station name
    */
    void deleteConfigMcuByJson(const QString& nameId);
    /*!
     \brief Get status message

     \fn getStatusMsg
     \param msg Current message
    */
    void getStatusMsg(const QString &id, const QString& msg);

    /*!
     \brief Get current index

     \fn getCurrentIndexInList
    */
    void getCurrentIndexInList();

    /*!
     \brief Send signal to exit from the application

     \fn exitActionFromMenu
    */
    void exitActionFromMenu();

private:

    QList < ListOfMacros > listOfMacros; /*! List of existing macros */
    QList < ListOfStations> listOfStations; /*! List of existing stations */
    QList < ListOfGroups> listOfGroups; /*! List of existing groups */

    QList < ListOfStations> listOfStationsSaved; /*! List of existing stations, saved */
    QList < ListOfGroups> listOfGroupsSaved; /*! List of existing groups, saved */

    QList <ListOfStationsStatus> listOfStatus; /*! List of existing stations for status*/

    QList <QString> listUarts; /*! List of UARTs/COM-pors */
    QList <QString> listOfFiles; /*! List of files in History file (3 elements) */

    QSerialPort port; /*! Port to open */

    qint32 uartSpeed{}; /*! UART speed 115200 default speed */
    qint32 currentLanguage = 0; /*! Current selected language, 0 - English, 1 - Russian */

    QString portNameUarts; /*! Name of current UART */
    QString versionFirmware; /*! Current version of the firmware */
    QString subVersionFirmware; /*! Current subversion of the firmware  */
    QString networkCard; /*! Selected network card */
    QString uartDev; /*! Node for current UART */

    QTimer *timerListOfUsbUarts = nullptr; /*! A timer that updates the list of available ports for UART */

    QByteArray firmwareForDownload; /*! Current encrypted firmware after generation */

    QString groupAddress {"232.0.0.0"}; /*! Multicast group for the interface */

    const qint32 CONST_RESERV_AREA = 440; /*! Reserved area size  */
    const qint32 CONST_TIME_HEAD_FIRMWARE = 30; /*! Size for storing the time and date the firmware was created */

    const qint32 MIN_PRIORITY_KEY = 1; /*! Minimum key priority for a key block */
    const qint32 MAX_PRIORITY_KEY = 8; /*! The maximum key priority for a key block */

    const qint32 MIN_MOD_KEY = 1; /*! Lock keys */
    const qint32 MAX_MOD_KEY = 2; /*! Non-latching keys */

    const qint32 MIN_ID = 100; /*! Minimum station number */
    const qint32 MAX_ID = 65000; /*! The maximum station number */

    const qint32 MIN_KEY_NUMBER = 1; /*! Minimum key number for a key block */
    const qint32 MAX_KEY_NUMBER = 128; /*! The maximum key number for the key block */

    const quint16 PORT_JSON = 65215; //65001; /*! Port for processing incoming requests */

    bool stateNewProejct = true; /*! flag it is a new project */
};
#endif // APPCORE_H
