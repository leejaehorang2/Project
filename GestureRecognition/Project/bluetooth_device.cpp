#include "bluetooth_device.hpp"

#include <iostream>

using namespace std;

BluetoothDevice::BluetoothDevice(QObject *parent) : QObject(parent)
{
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent;
    discoveryAgent->setLowEnergyDiscoveryTimeout(5000);

    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &BluetoothDevice::addDevice);
    connect(discoveryAgent, QOverload<QBluetoothDeviceDiscoveryAgent::Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, &BluetoothDevice::deviceScanError);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished, this, &BluetoothDevice::deviceScanFinished);

//    setUpdate("Search");
}

BluetoothDevice::~BluetoothDevice()
{
    if(discoveryAgent){
        delete discoveryAgent;
        discoveryAgent = nullptr;
    }

    if(controller){
        delete controller;
        controller = nullptr;
    }

    qDeleteAll(devices);
    qDeleteAll(services);
    qDeleteAll(characteristicses);

    devices.clear();
    services.clear();
    characteristicses.clear();
}

void BluetoothDevice::startDeviceDiscovery()
{
    qDeleteAll(devices);
    devices.clear();

    //emit devicesUpdated();

    cout << "Scanning for devices..." << endl;

    discoveryAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);

    if(discoveryAgent->isActive()){
        deviceScanState = true;
        //Q_EMIT stateChanged();
    }
}

void BluetoothDevice::addDevice(const QBluetoothDeviceInfo &deviceInfo)
{
    if(deviceInfo.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration){
        cout << "Last device added : " << deviceInfo.name().toStdString() << endl;
    }
}

void BluetoothDevice::deviceScanFinished()
{
    const QList<QBluetoothDeviceInfo> foundDevices = discoveryAgent->discoveredDevices();

    for(auto nextDevice : foundDevices){
        if(nextDevice.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration){
            //HERE!
//            devices.append(new )
        }
    }

    //emit devicesUpdated();
    deviceScanState = false;
    //emit stateChanged();

    if(devices.isEmpty()){
        cout << "No Low Energy devices found ..." << endl;
    }
    else{
        cout << "Done! Scan Again!" << endl;
    }
}

QVariant BluetoothDevice::getDevices()
{
    return QVariant::fromValue(devices);
}

QVariant BluetoothDevice::getServices()
{
    return QVariant::fromValue(services);
}

QVariant BluetoothDevice::getCharacteristicses()
{
    return QVariant::fromValue(characteristicses);
}

QString BluetoothDevice::getUpdate()
{
    return message;
}

void BluetoothDevice::scanServices(const QString &address)
{
    //HERE!
//    for(auto d : qAsConst(devices)){
//        if(auto device = qobject_cast<DeviceInfo*>(d)){
//            currentDevice.setDevice(device->getDevice());
//            break;
//        }
//    }

//    if(!currentDevice.getDevice().isValid()){
//        cout << "This is not valid device!" << endl;
//        return;
//    }

    qDeleteAll(characteristicses);
    characteristicses.clear();
    //emit characteristicsUpdated();
    qDeleteAll(services);
    services.clear();
    //emit servicesUpdated();

    cout << "Connecting Device...." << endl;

    //HERE!
//    if(controller){
//        if(previousAddress != currentDevice.getAddress()){
//           controller->disconnectFromDevice();

//           delete controller;
//           controller = nullptr;
//        }
//    }

    if(!controller){
//        controller = QLowEnergyController::createCentral(currentDevice.getDevice());
        connect(controller, &QLowEnergyController::connected, this, &BluetoothDevice::deviceConnected);
        connect(controller, QOverload<QLowEnergyController::Error>::of(&QLowEnergyController::error), this, &BluetoothDevice::errorReceived);
        connect(controller, &QLowEnergyController::disconnected, this, &BluetoothDevice::deviceDisconnected);
        connect(controller, &QLowEnergyController::serviceDiscovered, this, &BluetoothDevice::addLowEnergyService);
        connect(controller, &QLowEnergyController::discoveryFinished, this, &BluetoothDevice::serviceScanDone);
    }

    if(isRandomAddress()){
        controller->setRemoteAddressType(QLowEnergyController::RandomAddress);
    }
    else{
        controller->setRemoteAddressType(QLowEnergyController::PublicAddress);
    }

    controller->connectToDevice();

    //HERE!!
//    previousAddress = currentDevice.getAddress();
}

void BluetoothDevice::addLowEnergyService(const QBluetoothUuid &serviceUuid)
{
    QLowEnergyService *service = controller->createServiceObject(serviceUuid);
    if(!service){
        cout << "Cannto create service for uuid!" << endl;
        return;
    }

    //HERE!!
//    auto serviceInfo = new ServiceInfo(services);
//    services.append(serviceInfo);

    //emit servicesUpdated();
}

void BluetoothDevice::serviceScanDone()
{
    cout << "Service scan done!" << endl;

//    if(services.isEmpty()){
//        emit servicesUpdated();
//    }
}

void BluetoothDevice::connectToService(const QString &uuid)
{
    QLowEnergyService *service = nullptr;

    //HERE!!
//    for(auto s : qAsConst(services)){
//        auto serviceInfo = qobject_cast<ServiceInfo*>(s);
//        if(!serviceInfo){
//            continue;
//        }

//        if(serviceInfo->getUuid() == uuid){
//            service = serviceInfo->service();
//            break;
//        }
//    }
    if(!service){
        return;
    }

    qDeleteAll(characteristicses);
    characteristicses.clear();
//    emit characteristicsUpdated();

    if(service->state() == QLowEnergyService::DiscoveryRequired){
        connect(service, &QLowEnergyService::stateChanged, this, &BluetoothDevice::serviceDetailsDiscovered);
        service->discoverDetails();
        cout << " Discovering details ... " << endl;
        return;
    }

    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    //HERE!!
//    for(const QLowEnergyCharacteristic &ch : chars){
//        auto cInfo = new CharateristicInfo(ch);
//        characteristicses.append(cInfo);
//    }
//    QTimer::singleShot(0, this, &BluetoothDevice::characteristicsUpdated);
}

void BluetoothDevice::deviceConnected()
{
    cout << "Discovering services ... " << endl;
    connected = true;
    controller->discoverServices();
}

void BluetoothDevice::errorReceived(QLowEnergyController::Error error)
{
    cout << "Error : " << controller->errorString().toStdString() << endl;
}

void BluetoothDevice::disconnectFromDevice()
{
    if(controller->state() != QLowEnergyController::UnconnectedState){
        controller->disconnectFromDevice();
    }
    else{
        deviceDisconnected();
    }
}

void BluetoothDevice::deviceDisconnected()
{
    cout << "Disconnect from device" << endl;
    // emit disconnected();
}

void BluetoothDevice::serviceDetailsDiscovered(QLowEnergyService::ServiceState newState)
{
    if(newState != QLowEnergyService::ServiceDiscovered){
        if(newState != QLowEnergyService::DiscoveringServices){
            QMetaObject::invokeMethod(this,"characteristicsUpdated",Qt::QueuedConnection);
        }
        return;
    }

    auto service = qobject_cast<QLowEnergyService*>(sender());
    if(!service){
        return;
    }

    const QList<QLowEnergyCharacteristic> chars = service->characteristics();
    for(const QLowEnergyCharacteristic &ch : chars){
        //HERE!!
//        auto cInfo = new CharateristicInfo(ch);
//        characteristicses.append(cInfo);
    }
//    emit characteristicsUpdated();
}

void BluetoothDevice::deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error)
{
    if(error == QBluetoothDeviceDiscoveryAgent::PoweredOffError){
        //cout << "The Bluetooth adaptor is powered off, power it on before doing discovery." << endl;
    }
    else if(error == QBluetoothDeviceDiscoveryAgent::InputOutputError){
        //cout << "Writing or reading from the device resulted in an error." << endl;
    }
    else{
//        static QMetaEnum qme = discoveryAgent->metaObject()->enumerator(discoveryAgent->metaObject()->indexOfEnumerator("Error"));
        //cout << "Error!!!!" << endl;
    }

    deviceScanState = false;
    //emit devicesUpdated();
    //emit stateChanged();
}

bool BluetoothDevice::state()
{
    return deviceScanState;
}

bool BluetoothDevice::hasControllerError() const
{
    return (controller && controller->error() != QLowEnergyController::NoError);
}

bool BluetoothDevice::isRandomAddress() const
{
    return randomAddress;
}

void BluetoothDevice::setRandomAddress(bool newValue)
{
    randomAddress = newValue;
//    emit randomAddressChanged();
}

