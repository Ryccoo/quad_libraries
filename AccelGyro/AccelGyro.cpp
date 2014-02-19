#include "Energia.h"
#include "AccelGyro.h"
#include "../Wire/Wire.h"

AccelGyro::AccelGyro() {}
uint8_t AccelGyro::init() {
  Wire.setModule(0);
  Wire.begin();
  uint8_t c;
  uint8_t error;
  Serial1.println("Reading who I am bit");
  error = MPU6050_read(MPU6050_WHO_AM_I, &c, 1);
  Serial1.print("Success. I am : ");
  Serial1.println(c, HEX);
  // Clear the 'sleep' bit to start the sensor.
  MPU6050_write_reg (MPU6050_PWR_MGMT_1, 0);
  Serial1.println("Sensor started!");
  return error;
}
// --------------------------------------------------------
// MPU6050_read
//
// This is a common function to read multiple bytes
// from an I2C device.
//
// It uses the boolean parameter for Wire.endTransMission()
// to be able to hold or release the I2C-bus.
// This is implemented in Arduino 1.0.1.
//
// Only this function is used to read.
// There is no function for a single byte.
//
int AccelGyro::MPU6050_read(int start, uint8_t *buffer, int size)
{
  int i, n, error;
  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);
  if (n != 1)
    return (-10);

  n = Wire.endTransmission(false);    // hold the I2C-bus
  if (n != 0)
    return (n);

  // Third parameter is true: relase I2C-bus after data is read.
  Wire.requestFrom(MPU6050_I2C_ADDRESS, size, true);
  i = 0;
  while(Wire.available() && i<size)
  {
    buffer[i++]=Wire.read();
  }
  if ( i != size)
    return (-11);

  return (0);  // return : no error
}
// --------------------------------------------------------
// MPU6050_write
//
// This is a common function to write multiple bytes to an I2C device.
//
// If only a single register is written,
// use the function MPU_6050_write_reg().
//
// Parameters:
//   start : Start address, use a define for the register
//   pData : A pointer to the data to write.
//   size  : The number of bytes to write.
//
// If only a single register is written, a pointer
// to the data has to be used, and the size is
// a single byte:
//   int data = 0;        // the data to write
//   MPU6050_write (MPU6050_PWR_MGMT_1, &c, 1);
//
int AccelGyro::MPU6050_write(int start, const uint8_t *pData, int size)
{
  int n, error;

  Wire.beginTransmission(MPU6050_I2C_ADDRESS);
  n = Wire.write(start);        // write the start address
  if (n != 1)
    return (-20);

  n = Wire.write(pData, size);  // write data bytes
  if (n != size)
    return (-21);

  error = Wire.endTransmission(true); // release the I2C-bus
  if (error != 0)
    return (error);

  return (0);         // return : no error
}
// --------------------------------------------------------
// MPU6050_write_reg
//
// An extra function to write a single register.
// It is just a wrapper around the MPU_6050_write()
// function, and it is only a convenient function
// to make it easier to write a single register.
//
int AccelGyro::MPU6050_write_reg(int reg, uint8_t data)
{
  int error;

  error = MPU6050_write(reg, &data, 1);

  return (error);
}
int AccelGyro::read_gyro_accel_vals(uint8_t* accel_t_gyro_ptr) {
  // Read the raw values.
  // Read 14 bytes at once,
  // containing acceleration, temperature and gyro.
  // With the default settings of the MPU-6050,
  // there is no filter enabled, and the values
  // are not very stable.  Returns the error value
  // Serial1.print("Size of accel_t_gyto : ");
  // Serial1.println(sizeof(accel_t_gyro));
  // Serial1.print("Pointer for accel_t_gyro : ");
  // Serial1.println((int) &accel_t_gyro, HEX);
  // Serial1.print("x_accel_h reg value: ");
  // Serial1.println(accel_t_gyro.reg.x_accel_h, DEC);

  int error = MPU6050_read (MPU6050_ACCEL_XOUT_H, (uint8_t *) &accel_t_gyro, sizeof(accel_t_gyro));
  // Serial1.print("Error reading: ");
  // Serial1.println(error, DEC);
  // Serial1.print("x_accel_h reg value: ");
  // Serial1.println(accel_t_gyro.reg.x_accel_h, DEC);
  // Serial1.print("x_accel_l reg value: ");
  // Serial1.println(accel_t_gyro.reg.x_accel_l, DEC);

  // Swap all high and low bytes.
  // After this, the registers values are swapped,
  // so the structure name like x_accel_l does no
  // longer contain the lower byte.
  swap_endians((uint8_t*) &accel_t_gyro.reg.x_accel_h, (uint8_t*) &accel_t_gyro.reg.x_accel_l);
  swap_endians((uint8_t*) &accel_t_gyro.reg.y_accel_h, (uint8_t*) &accel_t_gyro.reg.y_accel_l);
  swap_endians((uint8_t*) &accel_t_gyro.reg.z_accel_h, (uint8_t*) &accel_t_gyro.reg.z_accel_l);

  swap_endians((uint8_t*) &accel_t_gyro.reg.t_h, (uint8_t*) &accel_t_gyro.reg.t_l);

  swap_endians((uint8_t*) &accel_t_gyro.reg.x_gyro_h, (uint8_t*) &accel_t_gyro.reg.x_gyro_l);
  swap_endians((uint8_t*) &accel_t_gyro.reg.y_gyro_h, (uint8_t*) &accel_t_gyro.reg.y_gyro_l);
  swap_endians((uint8_t*) &accel_t_gyro.reg.z_gyro_h, (uint8_t*) &accel_t_gyro.reg.z_gyro_l);
  // Serial1.println("After swap");
  // Serial1.print("x_accel_h reg value: ");
  // Serial1.println(accel_t_gyro.reg.x_accel_h, DEC);
  // Serial1.print("x_accel_l reg value: ");
  // Serial1.println(accel_t_gyro.reg.x_accel_l, DEC);
  // Serial1.println("Read successful");
  return error;
}
// The sensor should be motionless on a horizontal surface
//  while calibration is happening
void AccelGyro::calibrate_sensors() {
  int                   num_readings = 50;
  float                 x_accel = 0;
  float                 y_accel = 0;
  float                 z_accel = 0;
  float                 x_gyro = 0;
  float                 y_gyro = 0;
  float                 z_gyro = 0;

  //Serial.println("Starting Calibration");

  // Discard the first set of values read from the IMU
  read_gyro_accel_vals((uint8_t *) &accel_t_gyro);

  // Read and average the raw values from the IMU
  for (int i = 0; i < num_readings; i++) {
    read_gyro_accel_vals((uint8_t *) &accel_t_gyro);
    x_accel += accel_t_gyro.value.x_accel;
    y_accel += accel_t_gyro.value.y_accel;
    z_accel += accel_t_gyro.value.z_accel;
    x_gyro += accel_t_gyro.value.x_gyro;
    y_gyro += accel_t_gyro.value.y_gyro;
    z_gyro += accel_t_gyro.value.z_gyro;
    delay(100);
  }
  x_accel /= num_readings;
  y_accel /= num_readings;
  z_accel /= num_readings;
  x_gyro /= num_readings;
  y_gyro /= num_readings;
  z_gyro /= num_readings;

  // Store the raw calibration values globally
  accel_t_gyro_global.base_x_accel = x_accel;
  accel_t_gyro_global.base_y_accel = y_accel;
  accel_t_gyro_global.base_z_accel = z_accel;
  accel_t_gyro_global.base_x_gyro = x_gyro;
  accel_t_gyro_global.base_y_gyro = y_gyro;
  accel_t_gyro_global.base_z_gyro = z_gyro;

  //  Serial.println("Finishing Calibration");
}

void AccelGyro::set_last_read_angle_data(unsigned long time, float x, float y, float z, float x_gyro, float y_gyro, float z_gyro) {
  accel_t_gyro_global.last_read_time = time;
  accel_t_gyro_global.last_x_angle = x;
  accel_t_gyro_global.last_y_angle = y;
  accel_t_gyro_global.last_z_angle = z;
  accel_t_gyro_global.last_gyro_x_angle = x_gyro;
  accel_t_gyro_global.last_gyro_y_angle = y_gyro;
  accel_t_gyro_global.last_gyro_z_angle = z_gyro;
}

void AccelGyro::calculate() {
  // Gyro data receiver
    int error;
    double dT;

    error = read_gyro_accel_vals((uint8_t*) &accel_t_gyro);
    // Get the time of reading for rotation computations
    unsigned long t_now = millis();
    // Convert gyro values to degrees/sec
    float FS_SEL = 131;
    float gyro_x = (accel_t_gyro.value.x_gyro - accel_t_gyro_global.base_x_gyro)/FS_SEL;
    float gyro_y = (accel_t_gyro.value.y_gyro - accel_t_gyro_global.base_y_gyro)/FS_SEL;
    float gyro_z = (accel_t_gyro.value.z_gyro - accel_t_gyro_global.base_z_gyro)/FS_SEL;
    // Get raw acceleration values
    //float G_CONVERT = 16384;
    float accel_x = accel_t_gyro.value.x_accel;
    float accel_y = accel_t_gyro.value.y_accel;
    float accel_z = accel_t_gyro.value.z_accel;
    // Get angle values from accelerometer
    float RADIANS_TO_DEGREES = 180/3.14159;
    float accel_angle_y = atan(-1*accel_x/sqrt(pow(accel_y,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
    float accel_angle_x = atan(accel_y/sqrt(pow(accel_x,2) + pow(accel_z,2)))*RADIANS_TO_DEGREES;
    float accel_angle_z = 0;
    // Compute the (filtered) gyro angles
    float dt =(t_now - get_last_time())/1000.0;
    float gyro_angle_x = gyro_x*dt + get_last_x_angle();
    float gyro_angle_y = gyro_y*dt + get_last_y_angle();
    float gyro_angle_z = gyro_z*dt + get_last_z_angle();
    // Compute the drifting gyro angles
    float unfiltered_gyro_angle_x = gyro_x*dt + get_last_gyro_x_angle();
    float unfiltered_gyro_angle_y = gyro_y*dt + get_last_gyro_y_angle();
    float unfiltered_gyro_angle_z = gyro_z*dt + get_last_gyro_z_angle();
    // Apply the complementary filter to figure out the change in angle - choice of alpha is
    // estimated now.  Alpha depends on the sampling rate...
    float alpha = 0.96;
    float angle_x = alpha*gyro_angle_x + (1.0 - alpha)*accel_angle_x;
    float angle_y = alpha*gyro_angle_y + (1.0 - alpha)*accel_angle_y;
    float angle_z = gyro_angle_z;  //Accelerometer doesn't give z-angle
    // Update the saved data with the latest values
    accel_t_gyro_global.x_angle = angle_x;
    accel_t_gyro_global.y_angle = angle_y;
    accel_t_gyro_global.z_angle = angle_z;
    accel_t_gyro_global.x_last_five[accel_t_gyro_global.position] = angle_x;
    accel_t_gyro_global.position = (accel_t_gyro_global.position + 1) % 5;
    set_last_read_angle_data(t_now, angle_x, angle_y, angle_z, unfiltered_gyro_angle_x, unfiltered_gyro_angle_y, unfiltered_gyro_angle_z);
}

float AccelGyro::get_x() {
  int sum = 0;
  for(int i=0; i<5; i++) {
    sum += accel_t_gyro_global.x_last_five[i];
  }
  return sum / 5.0;
}

void AccelGyro::swap_endians(uint8_t* x, uint8_t* y) {
  uint8_t swap;
  swap = *x;
  *x = *y;
  *y = swap;
}

unsigned long AccelGyro::get_last_time() {
  return accel_t_gyro_global.last_read_time;
}
float AccelGyro::get_last_x_angle() {
  return accel_t_gyro_global.last_x_angle;
}
float AccelGyro::get_last_y_angle() {
  return accel_t_gyro_global.last_y_angle;
}
float AccelGyro::get_last_z_angle() {
  return accel_t_gyro_global.last_z_angle;
}
float AccelGyro::get_last_gyro_x_angle() {
  return accel_t_gyro_global.last_gyro_x_angle;
}
float AccelGyro::get_last_gyro_y_angle() {
  return accel_t_gyro_global.last_gyro_y_angle;
}
float AccelGyro::get_last_gyro_z_angle() {
  return accel_t_gyro_global.last_gyro_z_angle;
}