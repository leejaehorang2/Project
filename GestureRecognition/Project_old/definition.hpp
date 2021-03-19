#ifndef DEFINITION_H
#define DEFINITION_H

enum _axis{
    AXIS_X_ACCELEROMETER,
    AXIS_Y_ACCELEROMETER,
    AXIS_Z_ACCELEROMETER,
    AXIS_X_GYRO,
    AXIS_Y_GYRO,
    AXIS_Z_GYRO,
    AXIS_TOTAL,
};

struct sensorData{
    float value[AXIS_TOTAL];
};

#endif // DEFINITION_H
