#ifndef BLUETOOTH_DEVICE_H
#define BLUETOOTH_DEVICE_H

#include <QObject>

#include <QBluetoothServiceDiscoveryAgent>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QBluetoothServiceInfo>

#include <QList>

class BluetoothDevice : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothDevice(QObject *parent = nullptr);
    ~BluetoothDevice();

    QVariant getDevices();
    QVariant getServices();
    QVariant getCharacteristicses();
    QString getUpdate();

    bool state();
    bool hasControllerError() const;

    bool isRandomAddress() const;
    void setRandomAddress(bool newValue);

public slots:
    void startDeviceDiscovery();
    void scanServices(const QString &address);

    void connectToService(const QString &serviceUuid);
    void disconnectFromDevice();

private slots:
    void addDevice(const QBluetoothDeviceInfo& deviceInfo);
    void deviceScanFinished();
    void deviceScanError(QBluetoothDeviceDiscoveryAgent::Error error);

    void addLowEnergyService(const QBluetoothUuid &uuid);
    void deviceConnected();
    void errorReceived(QLowEnergyController::Error error);
    void serviceScanDone();
    void deviceDisconnected();

    void serviceDetailsDiscovered(QLowEnergyService::ServiceState newState);

private:
    void setUpdate(const QString &message);

    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QLowEnergyController *controller;
//    DeviceInfo currentDevice;

    QList<QObject*> devices;
    QList<QObject*> services;
    QList<QObject*> characteristicses;

    QString previousAddress;
    QString message;

    bool connected;
    bool deviceScanState;
    bool randomAddress;

signals:

public slots:
};

#endif // BLUETOOTH_DEVICE_H
