#ifndef DEFINITION_H
#define DEFINITION_H

#include <vector>
#include <stdint.h>

using namespace std;

enum _data_list{
    DATA_AXIS_X_ACCELERATION,
    DATA_AXIS_Y_ACCELERATION,
    DATA_AXIS_Z_ACCELERATION,
    DATA_PITCH,
    DATA_ROLL,
    DATA_YAW,
    DATA_TOTAL,
};

struct preprocessingDataSet{
    vector<uint8_t> accelX;
    vector<uint8_t> accelY;
    vector<uint8_t> accelZ;
    vector<uint8_t> pitch;
    vector<uint8_t> roll;
    vector<uint8_t> yaw;
};

enum _motion_image_list{
   MOTION_IMAGE_01,
   MOTION_IMAGE_02,
   MOTION_IMAGE_03,
   MOTION_IMAGE_04,
   MOTION_IMAGE_05,
   MOTION_IMAGE_06,
   MOTION_IMAGE_07,
   MOTION_IMAGE_08,
   MOTION_IMAGE_09,
   MOTION_IMAGE_10,
   MOTION_IMAGE_11,
   MOTION_IMAGE_12,
   MOTION_IMAGE_COUNT,
};


#endif // DEFINITION_H
