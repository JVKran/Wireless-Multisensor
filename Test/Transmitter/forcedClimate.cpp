#include "forcedClimate.hpp"

int32_t read32 () {
  uint8_t msb, lsb, xlsb;
  msb = Wire.read(); lsb = Wire.read(); xlsb = Wire.read();
  return (uint32_t)msb<<12 | (uint32_t)lsb<<4 | (xlsb>>4 & 0x0F);
}

ForcedClimate::ForcedClimate(TwoWire & bus, const uint8_t address = 0x77):
	bus(bus),
	address(address)
{}

float ForcedClimate::getTemperature(){
	bus.beginTransmission(address);
	bus.write(0xFA);
	bus.endTransmission();
	bus.requestFrom(address, (uint32_t)3);
	int32_t adc = read32();
	// Compensate
	int32_t var1, var2, t; 
	var1 = ((((adc>>3) - ((int32_t)((uint16_t)T[1])<<1))) * ((int32_t)T[2])) >> 11;
	var2 = ((((adc>>4) - ((int32_t)((uint16_t)T[1]))) * ((adc>>4) - ((int32_t)((uint16_t)T[1])))) >> 12);
	var2 = (var2 * ((int32_t)T[3])) >> 14;
	BME280t_fine = var1 + var2;
	return (BME280t_fine*5+128)>>8;
}

float ForcedClimate::getPressure(){
	bus.beginTransmission(address);
	bus.write(0xF7);
	bus.endTransmission();
	bus.requestFrom(address, (uint32_t)3);
	int32_t adc = read32();
	// Compensate
	int32_t var1, var2;
	uint32_t p;
	var1 = (((int32_t)BME280t_fine)>>1) - (int32_t)64000;
	var2 = (((var1>>2) * (var1>>2)) >> 11 ) * ((int32_t)P[6]);
	var2 = var2 + ((var1*((int32_t)P[5]))<<1);
	var2 = (var2>>2) + (((int32_t)P[4])<<16);
	var1 = (((P[3] * (((var1>>2) * (var1>>2)) >> 13 )) >> 3) + ((((int32_t)P[2]) * var1)>>1))>>18;
	var1 = ((((32768+var1))*((int32_t)((uint16_t)P[1])))>>15);
	if (var1 == 0) return 0;
	p = (((uint32_t)(((int32_t)1048576) - adc) - (var2>>12)))*3125;
	if (p < 0x80000000) p = (p << 1) / ((uint32_t)var1);
	else p = (p / (uint32_t)var1) * 2;
	var1 = (((int32_t)P[9]) * ((int32_t)(((p>>3) * (p>>3))>>13)))>>12;
	var2 = (((int32_t)(p>>2)) * ((int32_t)P[8]))>>13;
	p = (uint32_t)((int32_t)p + ((var1 + var2 + P[7]) >> 4));
	return p;
}

float ForcedClimate::getHumidity(){
	bus.beginTransmission(address);
	bus.write(0xFD);
	bus.endTransmission();
	bus.requestFrom(address, (uint32_t)2);
	uint8_t hi = bus.read(); uint8_t lo = bus.read();
	int32_t adc = (uint16_t)(hi<<8 | lo);
	// Compensate
	int32_t var1; 
	var1 = (BME280t_fine - ((int32_t)76800));
	var1 = (((((adc << 14) - (((int32_t)H[4]) << 20) - (((int32_t)H[5]) * var1)) +
	((int32_t)16384)) >> 15) * (((((((var1 * ((int32_t)H[6])) >> 10) * (((var1 *
	((int32_t)H[3])) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) *
	((int32_t)H[2]) + 8192) >> 14));
	var1 = (var1 - (((((var1 >> 15) * (var1 >> 15)) >> 7) * ((int32_t)H[1])) >> 4));
	var1 = (var1 < 0 ? 0 : var1);
	var1 = (var1 > 419430400 ? 419430400 : var1);
	return (uint32_t)((var1>>12)*25)>>8;
}
