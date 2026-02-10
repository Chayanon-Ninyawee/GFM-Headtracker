#include "bno085_controller.h"

Bno085Controller::Bno085Controller(i2c_inst_t *i2cPort, uint sdaPin, uint sclPin, uint8_t address)
    : i2c_(i2cPort)
    , sdaPin_(sdaPin)
    , sclPin_(sclPin)
    , address_(address) {}

bool Bno085Controller::begin() {
    // Init I2C
    gpio_init(sdaPin_);
    gpio_init(sclPin_);
    gpio_set_function(sdaPin_, GPIO_FUNC_I2C);
    gpio_set_function(sclPin_, GPIO_FUNC_I2C);
    gpio_pull_up(sdaPin_);
    gpio_pull_up(sclPin_);
    i2c_init(i2c_, 400000);  // 400kHz I2C

    sleep_ms(2000);

    if (!imu_.begin(address_, i2c_)) {
        printf("BNO08x not detected. Check wiring.\n");
        return false;
    }

    printf("BNO08x connected!\n");
    return true;
}

bool Bno085Controller::enableRotation(uint interval_ms) {
    if (!imu_.enableGameRotationVector(interval_ms)) {
        printf("Failed to enable rotation vector!\n");
        return false;
    }
    return true;
}

bool Bno085Controller::enableAccelerometer(uint interval_ms) {
    if (!imu_.enableAccelerometer(interval_ms)) {
        printf("Failed to enable accelerometer!\n");
        return false;
    }
    return true;
}

bool Bno085Controller::update(ImuAccel &accel, ImuEuler &euler) {
    if (imu_.getSensorEvent()) {
        switch (imu_.getSensorEventID()) {
        case SENSOR_REPORTID_GAME_ROTATION_VECTOR:
            euler.h = imu_.getYaw() * 180.0f / M_PI;
            euler.p = imu_.getPitch() * 180.0f / M_PI;
            euler.r = imu_.getRoll() * 180.0f / M_PI;
            return true;

        case SENSOR_REPORTID_ACCELEROMETER:
            accel.x = imu_.getAccelX();
            accel.y = imu_.getAccelY();
            accel.z = imu_.getAccelZ();
            return true;

        default:
            break;
        }
    }
    return false;
}

bool Bno085Controller::tareNow(bool zAxis, sh2_TareBasis_t basis) {
    return imu_.tareNow(zAxis, basis);
}

bool Bno085Controller::saveTare() {
    return imu_.saveTare();
}

bool Bno085Controller::clearTare() {
    return imu_.clearTare();
}

bool Bno085Controller::setCalibrationConfig(uint8_t sensors) {
    return imu_.setCalibrationConfig(sensors);
}

bool Bno085Controller::saveCalibration() {
    return imu_.saveCalibration();
}

bool Bno085Controller::wasReset() {
    return imu_.wasReset();
}
