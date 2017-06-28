#include <Arduino.h>
#include <NBHX711.h>

NBHX711::NBHX711(byte data, byte clock, byte depth, byte gain) : 
	dataPin(data),
	clockPin(clock),
	offset(0),
	scaleFactor(1.0),
	histSize(0),
	histBuffer(NULL),
	curr(0)
{
	histSize = 3 * min(max(depth, 6), 255/3);
	histBuffer = new byte[histSize];
	if (!histBuffer) {
		histSize = 0;
	} else {
		memset(histBuffer, 0, histSize);
	}
	setGainAndChannel(gain);
	powerUp();
}

NBHX711::~NBHX711() {
	delete[] histBuffer;
	pinMode(clockPin, INPUT);
}

byte NBHX711::getHistSize() {
	return histSize / 3;
}	

void NBHX711::begin() {
	pinMode(clockPin, OUTPUT);
}

bool NBHX711::isReady() {
	return digitalRead(dataPin) == LOW;
}

void NBHX711::setGainAndChannel(byte gain) {
	gainCode = gain;
}

long NBHX711::getRaw() {
	return cvt24(histBuffer + curr);
}

void NBHX711::putData(byte* storeTo) {
	// pulse the clock pin 24 times to read the data
	storeTo[2] = shiftIn(dataPin, clockPin, MSBFIRST);
	storeTo[1] = shiftIn(dataPin, clockPin, MSBFIRST);
	storeTo[0] = shiftIn(dataPin, clockPin, MSBFIRST);
	// set the channel and the gain factor for the next reading using the clock pin
	for (byte i = 0; i < gainCode; i++) {
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

long NBHX711::cvt24(byte* from) {
	uint8_t filler = 0x00;
	if (from[2] & 0x80) {
		filler--;
	}
	return (  static_cast<unsigned long>(filler) << 24
			| static_cast<unsigned long>(from[2]) << 16
			| static_cast<unsigned long>(from[1]) << 8
			| static_cast<unsigned long>(from[0]) );
}

bool NBHX711::update() {
	bool retVal = false;
	if (isReady()) {
		retVal = true;
		curr = nextIndex(curr);
		putData(histBuffer + curr);
	}
	return retVal;
}

byte NBHX711::backIndex(byte times) {
	int index = curr - (times - 1) * 3;
	if (index < 0) {
		index += histSize;
	}
	return index;
}

byte NBHX711::nextIndex(byte index) {
	int nIndex = index + 3;
	if (nIndex >= histSize) {
		nIndex = 0;
	}
	return nIndex;	
}

long NBHX711::readAverage(byte times) {
	long sum = 0;
	byte index = backIndex(times);
	for (byte i = 0; i < times; i++) {
		sum += cvt24(histBuffer + index);
		index = nextIndex(index);
	}
	return sum / times;
}

long NBHX711::getValue(byte times) {
	return readAverage(times) - offset;
}

float NBHX711::getUnits(byte times) {
	return getValue(times) / scaleFactor;
}

void NBHX711::tare(byte times) {
	setOffset(readAverage(times));
}

void NBHX711::setScale(float scale) {
	scaleFactor = scale;
}

float NBHX711::getScale() {
	return scaleFactor;
}

void NBHX711::setOffset(long inOffset) {
	offset = inOffset;
}

long NBHX711::getOffset() {
	return offset;
}

void NBHX711::powerDown() {
	digitalWrite(clockPin, LOW);
	digitalWrite(clockPin, HIGH);
}

void NBHX711::powerUp() {
	digitalWrite(clockPin, LOW);
}
