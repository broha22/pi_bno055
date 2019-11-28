/*
 * @Author: Brogan Miner
 * @Date:   Wednesday November 27th 2019
 * @Last Modified By:  Brogan Miner
 * @Last Modified Time:  Wednesday November 27th 2019
 * @Copyright:  (c) Your Company 2019
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include "lib/BNO055_driver/bno055.h"
#include "lib/BNO055_driver/bno055_support.h"

int main(int argc, char* argv[]) {

  I2C_routine(); //initialize bno055 i2c file descriptor and fn pointers

  bno055_init(&bno055); //init sensor via driver function
  bno055_set_power_mode(BNO055_POWER_MODE_NORMAL); //set normal power mode

  //Configure units
  bno055_set_operation_mode(BNO055_OPERATION_MODE_CONFIG);
  bno055_set_accel_unit(BNO055_ACCEL_UNIT_MSQ); //acceleromter, m/s^2
  bno055_set_gyro_unit(BNO055_GYRO_UNIT_DPS); //gyro, degrees per second
  bno055_set_euler_unit(BNO055_EULER_UNIT_DEG); //euler, degrees

  bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF); //set mode for reading raw and calculated types (absolute orientation)

  //init variables to store sensor data
  struct bno055_accel_t accel;
  struct bno055_gyro_t gyro;
  struct bno055_mag_t mag;
  struct bno055_euler_t euler;
  struct timeval tv;

  //open file descriptor
  FILE *csv;
  csv = fopen("sensor_data.csv", "w+");
  //write csv table headers
  fprintf(csv, "Time,Gyro X,Gyro Y,Gyro Z,Magnetometer X,Magnetometer Y,Magnetometer Z,Accelerometer X,Accelerometer Y,Accelerometer Z,Euler H,Euler R,Euler P\n");

  //get timestamp for start in microseconds
  gettimeofday(&tv, NULL);
  unsigned long long startUS = (unsigned long long)(tv.tv_sec) * 1000000 + (unsigned long long)(tv.tv_usec);
  unsigned long long startCopy = startUS;
  for (int i = 0; i < 36000; i++) { //loop for one hour expecting a runtime of 100 ms for the loop

    //Read gyro, mag, accel and euler
    bno055_read_gyro_xyz(&gyro);
    bno055_read_mag_xyz(&mag);
    bno055_read_accel_xyz(&accel);
    bno055_read_euler_hrp(&euler);

    //write sensor data
    fprintf(csv, "%llu,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", (startUS - startCopy) / 1000, gyro.x, gyro.y, gyro.z, mag.x, mag.y, mag.z, accel.x, accel.y, accel.z, euler.h, euler.r, euler.p);

    //get end timestamp
    gettimeofday(&tv, NULL);
    unsigned long long endUS = (unsigned long long)(tv.tv_sec) * 1000000 + (unsigned long long)(tv.tv_usec);
    while (endUS < startUS + 100000) { //busy loop recomputing endtime until 100ms have passed from start
      gettimeofday(&tv, NULL);
      endUS = (unsigned long long)(tv.tv_sec) * 1000000 + (unsigned long long)(tv.tv_usec);
    }

    startUS = endUS; //set the start for the next iteration
  }
  fclose(csv);
  return 0;
}