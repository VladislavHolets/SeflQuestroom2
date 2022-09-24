#ifndef __SOFTSPI_H
#define __SOFTSPI_H

//#define uint  unsigned int

class SOFTSPI
{
  public:
	void begin(unsigned char csnPin, unsigned char sckPin, unsigned char mosiPin, unsigned char misoPin);
	void writeByte(unsigned char dat);
	unsigned char readByte(void);
	unsigned char SPI_RW(unsigned char Byte);
	unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value);
	unsigned char SPI_Read(unsigned char reg);
	unsigned char readToBuf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
	unsigned char writeFromBuf(unsigned char reg, unsigned char *pBuf, unsigned char bytes);
  private:
	unsigned char _csnPin;
	unsigned char _sckPin;
	unsigned char _mosiPin;
	unsigned char _misoPin;
};

#endif
