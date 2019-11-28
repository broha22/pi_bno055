#include "bno055_support.h"
s8 I2C_routine(void) {
	bno055.bus_write = BNO055_I2C_bus_write;
	bno055.bus_read = BNO055_I2C_bus_read;
	bno055.delay_msec = BNO055_delay_msek;
	bno055.dev_addr = BNO055_I2C_ADDR1;

	if ((file_i2c = open("/dev/i2c-1", O_RDWR)) < 0) {
    return -1;
  }
  if(ioctl(file_i2c, I2C_SLAVE, bno055.dev_addr) != 0) {
    return -1;
  }
  u8 reg = BNO055_CHIP_ID_ADDR;
  if(write(file_i2c, &reg, 1) != 1) {
    return -1;
  }

	return BNO055_INIT_VALUE;
}

s8 BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
  s32 BNO055_iERROR = BNO055_INIT_VALUE;
	u8 array[I2C_BUFFER_LEN];
	u8 stringpos = BNO055_INIT_VALUE;
	array[BNO055_INIT_VALUE] = reg_addr;
	for (stringpos = BNO055_INIT_VALUE; stringpos < cnt; stringpos++) {
		array[stringpos + BNO055_I2C_BUS_WRITE_ARRAY_INDEX] =
			*(reg_data + stringpos);
	}

  if (write(file_i2c, array, cnt + 1) != cnt + 1) {
		printf("Failed to write to the i2c bus.\n");
	}
  
  return (s8)BNO055_iERROR;
}

s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8 *reg_data, u8 cnt) {
  s32 BNO055_iERROR = BNO055_INIT_VALUE;
	u8 array[I2C_BUFFER_LEN] = {BNO055_INIT_VALUE};
	u8 stringpos = BNO055_INIT_VALUE;

  if (write(file_i2c, &reg_addr, 1) != 1) {
		printf("Failed to write to the i2c bus.\n");
	}
  sleep(0.15);
  if (read(file_i2c, array, cnt) != cnt) {
		printf("Failed to read from the i2c bus.\n");
	}
  for (stringpos = BNO055_INIT_VALUE; stringpos < cnt; stringpos++) {
		*(reg_data + stringpos) = array[stringpos];
  }
	return (s8)BNO055_iERROR;
}

void BNO055_delay_msek(u32 msek) {
	sleep((double)msek / 1000.0);
}
