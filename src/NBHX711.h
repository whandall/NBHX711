#ifndef HX711_h
#define HX711_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

/*! \mainpage NBHX711
 *
 * \section intro_sec Introduction
 *
 * Library for the Avia HX711 24 bit ADC for weight scales.
 *
 * - non-blocking, instant access to averanges
 *
 * - future - callbacks for change with a configurable threshold 
 *
 * \section hist_sec History
 *
 * - 2017 first version
 *
 * \section install_sec Installation
 *
 * standard Arduino library install
 *  
 */
 
 /**
  * enumerate the three possible channel / gain combinations
  *
  */
enum ChGain {
	ChA128 = 1,
	ChB32,
	ChA64
};

//--current development 
// onRapidChange

//--future development 

/** \brief the main HX711 class
 */
class NBHX711 {
	/// data comes in on this pin
	byte dataPin;
	/// serial clock and power down control
	byte clockPin;
protected:
	/// selects gain and channel
	byte gainCode;
	/// baseline value for tare weight
	long offset;
	/// factor to convert reading to units
	float scaleFactor;
	/// size of history buffer
	byte histSize;
	/// the history buffer
	byte* histBuffer;
	/// pointer to current reading
	byte curr;
/**
 *	available data is signaled by the HX711 via LOW on dataPin
 */
	bool isReady();
/**
 *	conversion function
 *	convert 3 byte signed value to signed 32 bit long
 *	@param from [in] pointer to little endian 24 bit value
 *	@return 32 bit signed long
 */
	long cvt24(byte* from);
/**
 *	access function
 *	read 3 byte signed value to passed buffer
 *	@param storeTo [in] pointer to little endian 24 bit value
 */
	void putData(byte* storeTo);
/**
 *	utility function
 *	@param times [in] number of history elements
 *	@return index of curr - (times - 1) elements
 */
	byte backIndex(byte times);
/**
 *	utility function
 *	@param index [in] index of history element
 *	@return index of index + 1 element
 */
	byte nextIndex(byte index);
public:
/**
 *	constructor
 *  set up variables and allocate history buffer
 *	@param dout [in] data pin
 *	@param pd_sck [in] clock pin, used also to set channel and gain via additional pulses
 *	@param depth [in] depth of cyclic history buffer
 *	@param gain [in] selected gain and channel, default A-128
 */
	NBHX711(byte dout, byte pd_sck, byte depth = 12, byte gain = ChA128);
/**
 *	destructor
 *  free history buffer, release output port
 */
	~NBHX711();
/**
 *	set pinmode
 */
	void begin();
/**
 *	utility function
 *	@return depth of history buffer
 */
	byte getHistSize();
/**
 *	utility function
 *	@param gain [in] selected gain and channel, default A-128
 */
	void setGainAndChannel(byte gain = ChA128);
/**
 *	check if new value available and put it to history buffer
 *	@return true if value was read
 */
	bool update();
/**
 *	get the raw HX711 output
 *	@return most current reading
 */
	long getRaw();
/**
 *	get the arithmetic mean of the last times raw HX711 outputs
 *	@param times [in] number of history elements to process
 *	@return arithmetic mean of the last times raw HX711 outputs
 */
	long readAverage(byte times = 10);
/**
 *	compute relative average
 *	@param times [in] number of history elements to process
 *	@return readAverage(times) minus offset
 */
	long getValue(byte times = 1);
/**
 *	scale relative average to units via scaleFactor
 *	@param times [in] number of history elements to process
 *	@return getValue(times) divided by scaleFactor
 */
	float getUnits(byte times = 1);
/**
 *	make the current reading the offset
 *	@param times [in] number of history elements to process
 */
	void tare(byte times = 10);
/**
 *	set the conversion factor to map readings to gram or whatever unit is wished
 *	@param scale [in] value to divide reading by
 */
	void setScale(float scale = 1.0);
/**
 *	get the conversion factor to map readings to gram or whatever unit is wished
 *	@return value to divide reading by
 */
	float getScale();
/**
 *	set the offset
 *	@param offset [in] offset value for readings
 */
	void setOffset(long offset = 0);
/**
 *	get the offset
 *	@return offset value for readings
 */
	long getOffset();
/**
 *	set the clock pin to HIGH, which sends the HX711 into power down after 50 µs
 */
	void powerDown();
/**
 *	set the clock pin to LOW, which starts the HX711 from power, it could take 300 ms for next reading
 */
	void powerUp();
};

#endif /* HX711_h */
