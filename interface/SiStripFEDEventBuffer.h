#ifndef EventFilter_SiStripRawToDigi_SiStripFEDBuffer_H
#define EventFilter_SiStripRawToDigi_SiStripFEDBuffer_H

#include "boost/cstdint.hpp"
#include <iostream>
#include <string>
#include "DataFormats/FEDRawData/interface/FEDRawData.h"

//setters
//channel object?
//BE status register

//
// Constants
//

static const uint8_t SISTRIP_INVALID=0xFF;

static const uint8_t SISTRIP_APVS_PER_CHANNEL=2;
static const uint8_t SISTRIP_FEUNITS_PER_FED=8;
static const uint8_t SISTRIP_CHANNELS_PER_FEUNIT=12;
static const uint8_t SISTRIP_CHANNELS_PER_FED=SISTRIP_FEUNITS_PER_FED*SISTRIP_CHANNELS_PER_FEUNIT;
static const uint8_t SISTRIP_APVS_PER_FED=SISTRIP_CHANNELS_PER_FED*SISTRIP_APVS_PER_CHANNEL;
static const uint8_t SISTRIP_APV_MAX_ADDRESS=192;

enum SiStripFEDEventFormat { SISTRIP_EVENT_FORMAT_INVALID=SISTRIP_INVALID,\
                             SISTRIP_EVENT_FORMAT_OLD_VME,
                             SISTRIP_EVENT_FORMAT_OLD_SLINK,
                             SISTRIP_EVENT_FORMAT_NEW };
//these are the values which appear in the buffer.
static const uint8_t SISTRIP_EVENT_FORMAT_CODE_OLD = 0xED;
static const uint8_t SISTRIP_EVENT_FORMAT_CODE_NEW = 0xC5;

//enum values are values which appear in buffer. DO NOT CHANGE!
enum SiStripFEDHeaderType { SISTRIP_HEADER_TYPE_INVALID=SISTRIP_INVALID,
                            SISTRIP_HEADER_TYPE_FULL_DEBUG=1,
                            SISTRIP_HEADER_TYPE_APV_ERROR=2 };

//enum values are values which appear in buffer. DO NOT CHANGE!
enum SiStripFEDReadoutMode { SISTRIP_READOUT_MODE_INVALID=SISTRIP_INVALID,
                             SISTRIP_READOUT_MODE_SCOPE=0x1,
                             SISTRIP_READOUT_MODE_VIRGIN_RAW=0x2,
                             SISTRIP_READOUT_MODE_PROC_RAW=0x6,
                             SISTRIP_READOUT_MODE_ZERO_SUPPRESSED=0xA,
                             SISTRIP_READOUT_MODE_ZERO_SUPPRESSED_LITE=0xC };

static const uint8_t SISTRIP_PACKET_CODE_SCOPE = 0xE5;
static const uint8_t SISTRIP_PACKET_CODE_VIRGIN_RAW = 0xE6;
static const uint8_t SISTRIP_PACKET_CODE_PROC_RAW = 0xF2;
static const uint8_t SISTRIP_PACKET_CODE_ZERO_SUPPRESSED = 0xEA;

//enum values are values which appear in buffer. DO NOT CHANGE!
enum SiStripFEDDataType { SISTRIP_DATA_TYPE_REAL=0,
                          SISTRIP_DATA_TYPE_FAKE=1 };

//enum values are values which appear in buffer. DO NOT CHANGE!
//see http://cmsdoc.cern.ch/cms/TRIDAS/horizontal/RUWG/DAQ_IF_guide/DAQ_IF_guide.html
enum SiStripFEDDAQEventType { SISTRIP_DAQ_EVENT_TYPE_PHYSICS=0x1,
                              SISTRIP_DAQ_EVENT_TYPE_CALIBRATION=0x2,
                              SISTRIP_DAQ_EVENT_TYPE_TEST=0x3,
                              SISTRIP_DAQ_EVENT_TYPE_TECHNICAL=0x4,
                              SISTRIP_DAQ_EVENT_TYPE_SIMULATED=0x5,
                              SISTRIP_DAQ_EVENT_TYPE_TRACED=0x6,
                              SISTRIP_DAQ_EVENT_TYPE_ERROR=0xF,
                              SISTRIP_DAQ_EVENT_TYPE_INVALID=SISTRIP_INVALID };

//enum values are values which appear in buffer. DO NOT CHANGE!
//TODO: check the disconnected state
//see http://cmsdoc.cern.ch/cms/TRIDAS/horizontal/RUWG/DAQ_IF_guide/DAQ_IF_guide.html
enum SiStripFEDTTSBits { SISTRIP_TTS_DISCONNECTED=0x0,
                         SISTRIP_TTS_WARN_OVERFLOW=0x1,
                         SISTRIP_TTS_OUT_OF_SYNC=0x2,
                         SISTRIP_TTS_BUSY=0x4,
                         SISTRIP_TTS_READY=0x8,
                         SISTRIP_TTS_ERROR=0x12,
                         SISTRIP_TTS_INVALID=SISTRIP_INVALID };

//
// Global function declarations
//

//used by these classes
inline uint8_t internalFEDChannelNum(uint8_t internalFEUnitNum, uint8_t internalChannelNum);
void printHex(const void* pointer, size_t length, std::ostream& os);
//to make enums printable
std::ostream& operator<<(std::ostream& os, const SiStripFEDEventFormat& value);
std::ostream& operator<<(std::ostream& os, const SiStripFEDHeaderType& value);
std::ostream& operator<<(std::ostream& os, const SiStripFEDReadoutMode& value);
std::ostream& operator<<(std::ostream& os, const SiStripFEDDataType& value);
std::ostream& operator<<(std::ostream& os, const SiStripFEDDAQEventType& value);
std::ostream& operator<<(std::ostream& os, const SiStripFEDTTSBits& value);

//
// Class definitions
//

//see http://cmsdoc.cern.ch/cms/TRIDAS/horizontal/RUWG/DAQ_IF_guide/DAQ_IF_guide.html
class SiStripFEDDAQHeader
{
public:
  inline SiStripFEDDAQHeader() { }
  inline SiStripFEDDAQHeader(const uint8_t* header);
  //0x5 in first fragment
  inline uint8_t boeNibble() const { return ( (header_[3] & 0xF0) >> 4 ); }
  inline uint8_t eventTypeNibble() const { return (header_[3] & 0x0F); }
  SiStripFEDDAQEventType eventType() const;
  inline uint32_t l1ID() const;
  inline uint16_t bxID() const;
  inline uint16_t sourceID() const;
  inline uint8_t version() const { return ( (header_[4] & 0xF0) >> 4 ); }
  //0 if current header word is last, 1 otherwise
  inline bool hBit() const { return (header_[4] & 0x8); }
  inline bool lastHeader() const { return !hBit(); }
  inline void print(std::ostream& os) const { printHex(header_,8,os); }
private:
  uint8_t header_[8];
};

//see http://cmsdoc.cern.ch/cms/TRIDAS/horizontal/RUWG/DAQ_IF_guide/DAQ_IF_guide.html
class SiStripFEDDAQTrailer
{
public:
  inline SiStripFEDDAQTrailer() { }
  inline SiStripFEDDAQTrailer(const uint8_t* trailer);
  //0xA in first fragment
  inline uint8_t eoeNibble() const { return ( (trailer_[3] & 0xF0) >> 4 ); }
  inline uint32_t eventLengthIn64BitWords() const;
  inline uint32_t eventLengthInBytes() const { return eventLengthIn64BitWords()*8; }
  inline uint16_t crc() const;
  //set to 1 if FRL detects a transmission error over S-link
  inline bool cBit() const { return (trailer_[5] & 0x80); }
  inline bool slinkTransmissionError() const { return cBit(); }
  //set to 1 if the FED ID is not the one expected by the FRL
  inline bool fBit() const { return (trailer_[5] & 0x40); }
  inline bool badFEDID() const { return fBit(); }
  inline uint8_t eventStatusNibble() const { return (trailer_[5] & 0x0F); }
  inline uint8_t ttsNibble() const { return ( (trailer_[4] & 0xF0) >> 4); }
  inline SiStripFEDTTSBits ttsBits() const;
  //0 if the current trailer is the last, 1 otherwise
  inline bool tBit() const { return (trailer_[4] & 0x08); }
  inline bool lastTrailer() const { return !tBit(); }
  //set to 1 if the S-link sender card detects a CRC error (the CRC it computes is put in the CRC field)
  inline bool rBit() const { return (trailer_[4] & 0x04); }
  inline bool slinkCRCError() const { return rBit(); }
  inline void print(std::ostream& os) const { printHex(trailer_,8,os); }
private:
  uint8_t trailer_[8];
};

class SiStripFEDStatusRegister
{
public:
  inline SiStripFEDStatusRegister(const uint16_t fedStatusRegister)
    : data_(fedStatusRegister) { }
  inline bool slinkFullFlag() const { return getBit(0); }
  inline bool trackerHeaderMonitorDataReadyFlag() const { return getBit(1); }
  inline bool qdrMemoryFullFlag() const { return getBit(2); }
  inline bool qdrMemoryPartialFullFlag() const { return getBit(3); }
  inline bool qdrMemoryEmptyFlag() const { return getBit(4); }
  inline bool l1aFullFlag() const { return getBit(5); }
  inline bool l1aPartialFullFlag() const { return getBit(6); }
  inline bool l1aEmptyFlag() const { return getBit(7); }
  inline void print(std::ostream& os) const { printHex(&data_,2,os); }
  void printFlags(std::ostream& os) const;
private:
  inline bool getBit(const uint8_t num) const { return ( (0x1<<num) & (data_) ); }
  const uint16_t data_;
};

class SiStripTrackerSpecialHeader
{
public:
  inline SiStripTrackerSpecialHeader();
  //construct with a pointer to the data. The data will be coppied and swapped if necessary. 
  SiStripTrackerSpecialHeader(const uint8_t* headerPointer);
  inline const uint8_t* getPointer() const { return specialHeader_; }
  inline uint8_t eventFormatByte() const { return specialHeader_[EVENTFORMAT^0x04]; }
  SiStripFEDEventFormat eventFormat() const;
  inline uint8_t headerTypeNibble() const { return ( (specialHeader_[BUFFERTYPE^0x04] & 0xF0) >> 4 ); }
  SiStripFEDHeaderType headerType() const;
  inline uint8_t getTrackerEventTypeNibble() const { return (specialHeader_[BUFFERTYPE^0x04] & 0x0F); }
  SiStripFEDReadoutMode readoutMode() const;
  SiStripFEDDataType dataType() const;
  inline uint8_t apveAddress() const { return specialHeader_[APVEADDRESS^0x04]; }
  inline uint8_t getAPVAddressErrorRegister() const { return specialHeader_[ADDRESSERROR^0x04]; }
  inline bool majorityAddressErrorForFEUnit(uint8_t internalFEUnitNum) const;
  inline uint8_t getFEEnableRegister() const { return specialHeader_[FEENABLE^0x04]; }
  inline bool feEnabled(uint8_t internalFEUnitNum) const;
  inline uint8_t getFEOverflowRegister() const { return specialHeader_[FEOVERFLOW^0x04]; }
  inline bool feOverflow(uint8_t internalFEUnitNum) const;
  inline uint16_t getFEDStatusRegister() const;
  inline SiStripFEDStatusRegister fedStatusRegister() const { return SiStripFEDStatusRegister(getFEDStatusRegister()); }
  inline void print(std::ostream& os) const { printHex(specialHeader_,8,os); }
private:
  enum byteIndicies { FEDSTATUS=0, FEOVERFLOW=2, FEENABLE=3, ADDRESSERROR=4, APVEADDRESS=5, BUFFERTYPE=6, EVENTFORMAT=7 };
  //copy of header, 32 bit word swapped if needed
  uint8_t specialHeader_[8];
  //was the header word swapped?
  bool wordSwapped_;
};

class SiStripFEDBufferBase
{
public:
  SiStripFEDBufferBase(const FEDRawData& fedBuffer, bool allowUnrecognizedFormat = false);
  virtual ~SiStripFEDBufferBase();
  //dump buffer to stream
  inline void dump(std::ostream& os) const { printHex(orderedBuffer_->data(),orderedBuffer_->size(),os); }
  //dump original buffer before word swapping
  inline void dumpOriginalBuffer(std::ostream& os) const { printHex(originalBuffer_.data(),originalBuffer_.size(),os); }
  void print(std::ostream& os) const;
  //calculate the CRC from the buffer
  uint16_t calcCRC() const;
  //methods to get parts of the buffer
  inline SiStripFEDDAQHeader daqHeader() const { return daqHeader_; }
  inline SiStripFEDDAQTrailer daqTrailer() const { return daqTrailer_; }
  inline size_t bufferSize() const { return originalBuffer_.size(); }
  inline SiStripTrackerSpecialHeader trackerSpecialHeader() const { return specialHeader_; }
  //methods to get info from DAQ header
  inline SiStripFEDDAQEventType daqEventType() const { return daqHeader_.eventType(); }
  inline uint32_t daqLvl1ID() const { return daqHeader_.l1ID(); }
  inline uint16_t daqBXID() const { return daqHeader_.bxID(); }
  inline uint16_t daqSourceID() const { return daqHeader_.sourceID(); }
  inline uint16_t sourceID() const { return daqSourceID(); }
  //methods to get info from DAQ trailer
  inline uint32_t daqEventLengthIn64bitWords() const { return daqTrailer_.eventLengthIn64BitWords(); }
  inline uint32_t daqEventLengthInBytes() const { return daqTrailer_.eventLengthInBytes(); }
  inline uint16_t daqCRC() const { return daqTrailer_.crc(); }
  inline SiStripFEDTTSBits daqTTSState() const { return daqTrailer_.ttsBits(); }
  //methods to get info from the tracker special header
  inline SiStripFEDEventFormat eventFormat() const { return specialHeader_.eventFormat(); }
  inline SiStripFEDHeaderType headerType() const { return specialHeader_.headerType(); }
  inline SiStripFEDReadoutMode readoutMode() const { return specialHeader_.readoutMode(); }
  inline SiStripFEDDataType dataType() const { return specialHeader_.dataType(); }
  inline uint8_t apveAddress() const { return specialHeader_.apveAddress(); }
  inline bool majorityAddressErrorForFEUnit(uint8_t internalFEUnitNum) const { return specialHeader_.majorityAddressErrorForFEUnit(internalFEUnitNum); }
  inline bool feEnabled(uint8_t internalFEUnitNum) const { return specialHeader_.feEnabled(internalFEUnitNum); }
  virtual uint8_t nFEUnitsEnabled() const;
  inline bool feOverflow(uint8_t internalFEUnitNum) const { return specialHeader_.feOverflow(internalFEUnitNum); }
  inline bool feGood(uint8_t internalFEUnitNum) const { return ( !majorityAddressErrorForFEUnit(internalFEUnitNum) && !feOverflow(internalFEUnitNum) ); }
  inline SiStripFEDStatusRegister fedStatusRegister() const { return specialHeader_.fedStatusRegister(); }
  
  //summary checks
  //check that tracker special header is valid (does not check for FE unit errors indicated in special header)
  inline bool doTrackerSpecialHeaderChecks() const;
  //check for errors in DAQ heaqder and trailer
  inline bool doDAQHeaderAndTrailerChecks() const;
  //do both
  virtual bool doChecks() const;
  //print the result of all detailed checks
  virtual std::string checkSummary() const;
  
  //detailed checks
  inline bool checkCRC() const { return ( checkNoSlinkCRCError() && (calcCRC()==daqCRC()) ); }
  //methods to check tracker special header
  inline bool checkEventFormat() const { return (eventFormat() != SISTRIP_EVENT_FORMAT_INVALID); }
  inline bool checkHeaderType() const { return (headerType() != SISTRIP_HEADER_TYPE_INVALID); }
  inline bool checkReadoutMode() const { return (readoutMode() != SISTRIP_READOUT_MODE_INVALID); }
  inline bool checkAPVEAddressValid() const { return (apveAddress() <= SISTRIP_APV_MAX_ADDRESS); }
  inline bool checkNoFEOverflows() const { return !specialHeader_.getFEOverflowRegister(); }
  //methods to check daq header and trailer
  inline bool checkNoSlinkCRCError() const { return !daqTrailer_.slinkCRCError(); }
  inline bool checkNoSLinkTransmissionError() const { return !daqTrailer_.slinkTransmissionError(); }
  bool checkSourceIDIsSiStrip() const;
  inline bool checkNoUnexpectedSourceID() const { return !daqTrailer_.badFEDID(); }
  inline bool checkNoExtraHeadersOrTrailers() const { return ( (daqHeader_.boeNibble() == 0x5) && (daqTrailer_.eoeNibble() == 0xA) ); }
  inline bool checkLengthFromTrailer() const { return (bufferSize() == daqEventLengthInBytes()); }
protected:
  inline const uint8_t* getPointerToDataAfterTrackerSpecialHeader() const
    { return orderedBuffer_->data()+16; }
  inline uint8_t* getPointerToDataAfterTrackerSpecialHeader();
  inline const uint8_t* getPointerToByteAfterEndOfPayload() const
    { return orderedBuffer_->data()+orderedBuffer_->size()-8; }
  inline uint8_t* getPointerToByteAfterEndOfPayload();
private:
  const FEDRawData& originalBuffer_;
  const FEDRawData* orderedBuffer_;
  SiStripFEDDAQHeader daqHeader_;
  SiStripFEDDAQTrailer daqTrailer_;
  SiStripTrackerSpecialHeader specialHeader_;
};

class SiStripFEDFEHeader
{
public:
  //factory function: allocates new SiStripFEDFEHeader derrivative of appropriate type
  inline static std::auto_ptr<SiStripFEDFEHeader> newFEHeader(SiStripFEDHeaderType headerType, const uint8_t* headerBuffer);
  virtual ~SiStripFEDFEHeader();
  //the length of the header
  virtual size_t lengthInBytes() const = 0;
  //check that there are no errors indicated in which ever error bits are available in the header
  //check bits for both APVs on a channel
  inline bool checkChannelStatusBits(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return checkChannelStatusBits(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  virtual bool checkChannelStatusBits(uint8_t internalFEDChannelNum) const = 0;
  //check bits for one APV
  inline bool checkStatusBits(uint8_t internalFEUnitNum, uint8_t internalChannelNum, uint8_t apvNum) const
    { return checkStatusBits(internalFEDChannelNum(internalFEUnitNum,internalChannelNum),apvNum); }
  virtual bool checkStatusBits(uint8_t internalFEDChannelNum, uint8_t apvNum) const = 0;
  virtual void print(std::ostream& os) const = 0;
};

class SiStripFEDAPVErrorHeader : public SiStripFEDFEHeader
{
public:
  inline SiStripFEDAPVErrorHeader(const uint8_t* headerBuffer);
  virtual ~SiStripFEDAPVErrorHeader();
  virtual size_t lengthInBytes() const;
  virtual bool checkChannelStatusBits(uint8_t internalFEDChannelNum) const;
  virtual bool checkStatusBits(uint8_t internalFEDChannelNum, uint8_t apvNum) const;
  virtual void print(std::ostream& os) const;
private:
  static const size_t APV_ERROR_HEADER_SIZE_IN_64BIT_WORDS = 3;
  static const size_t APV_ERROR_HEADER_SIZE_IN_BYTES = APV_ERROR_HEADER_SIZE_IN_64BIT_WORDS*8;
  uint8_t header_[APV_ERROR_HEADER_SIZE_IN_BYTES];
};

class SiStripFEDFullDebugHeader : public SiStripFEDFEHeader
{
public:
  inline SiStripFEDFullDebugHeader(const uint8_t* headerBuffer);
  virtual ~SiStripFEDFullDebugHeader();
  virtual size_t lengthInBytes() const;
  virtual bool checkChannelStatusBits(uint8_t internalFEDChannelNum) const;
  virtual bool checkStatusBits(uint8_t internalFEDChannelNum, uint8_t apvNum) const;
  virtual void print(std::ostream& os) const;
  
  inline bool fePresent(uint8_t internalFEUnitNum) const { return (getFEPayloadLength(internalFEUnitNum) != 0); }
  inline uint8_t getFEUnitMajorityAddress(uint8_t internalFEUnitNum) const { return feWord(internalFEUnitNum)[9]; }
  inline uint16_t getFEPayloadLength(uint8_t internalFEUnitNum) const { return ( feWord(internalFEUnitNum)[14] + (feWord(internalFEUnitNum)[15]<<8) ); }
  inline uint32_t getBEStatusRegister() const { return get32BitWordFrom(feWord(0)+10); }
  inline uint32_t getDAQRegister() const { return get32BitWordFrom(feWord(7)+10); }
  inline uint32_t getDAQRegister2() const { return get32BitWordFrom(feWord(6)+10); }
  
  //These methods return true if there was an error of the appropriate type (ie if the error bit is 0).
  //They return false if the error could not occur due to a more general error.
  //was channel unlocked
  inline bool unlocked(uint8_t internalFEDChannelNum) const { return unlockedFromBit(internalFEDChannelNum); }
  inline bool unlocked(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return unlocked(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  //was channel out of sync if it was unlocked
  inline bool outOfSync(uint8_t internalFEDChannelNum) const
    { return ( !unlocked(internalFEDChannelNum) && outOfSyncFromBit(internalFEDChannelNum) ); }
  inline bool outOfSync(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return outOfSync(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  //was there an internal APV error if it was in sync
  inline bool apvError(uint8_t internalFEDChannelNum, uint8_t apvNum) const
    { return ( !outOfSync(internalFEDChannelNum) && apvErrorFromBit(internalFEDChannelNum,apvNum) ); }
  inline bool apvError(uint8_t internalFEUnitNum, uint8_t internalChannelNum, uint8_t apvNum) const
    { return apvError(internalFEDChannelNum(internalFEUnitNum,internalChannelNum),apvNum); }
  //was the APV address wrong if it was in sync (does not depend on APV internal error bit)
  inline bool apvAddressError(uint8_t internalFEDChannelNum, uint8_t apvNum) const
    { return ( !outOfSync(internalFEDChannelNum) && apvAddressErrorFromBit(internalFEDChannelNum,apvNum) ); }
  inline bool apvAddressError(uint8_t internalFEUnitNum, uint8_t internalChannelNum, uint8_t apvNum) const
    { return apvAddressError(internalFEDChannelNum(internalFEUnitNum,internalChannelNum),apvNum); }
  
  //These methods return true if there was an error of the appropriate type (ie if the error bit is 0).
  //They ignore any previous errors which make the status bits meaningless and return the value of the bit anyway.
  //In general, the methods above which only return an error for the likely cause are more useful.
  inline bool unlockedFromBit(uint8_t internalFEDChannelNum) const { return !getBitVal(internalFEDChannelNum,5); }
  inline bool unlockedFromBit(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return unlockedFromBit(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  inline bool outOfSyncFromBit(uint8_t internalFEDChannelNum) const { return !getBitVal(internalFEDChannelNum,4); }
  inline bool outOfSyncFromBit(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return outOfSyncFromBit(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  inline bool apvErrorFromBit(uint8_t internalFEDChannelNum, uint8_t apvNum) const { return !getBitVal(internalFEDChannelNum,0+2*apvNum); }
  inline bool apvErrorFromBit(uint8_t internalFEUnitNum, uint8_t internalChannelNum, uint8_t apvNum) const
    { return apvErrorFromBit(internalFEDChannelNum(internalFEUnitNum,internalChannelNum),apvNum); }
  inline bool apvAddressErrorFromBit(uint8_t internalFEDChannelNum, uint8_t apvNum) const { return !getBitVal(internalFEDChannelNum,1+2*apvNum); }
  inline bool apvAddressErrorFromBit(uint8_t internalFEUnitNum, uint8_t internalChannelNum, uint8_t apvNum) const
    { return apvAddressErrorFromBit(internalFEDChannelNum(internalFEUnitNum,internalChannelNum),apvNum); }
private:
  inline bool getBitVal(uint8_t internalFEDChannelNum, uint8_t bit) const;
  static inline uint32_t get32BitWordFrom(const uint8_t* startOfWord);
  inline const uint8_t* feWord(uint8_t internalFEUnitNum) const { return header_+internalFEUnitNum*2*8; }
  static const size_t FULL_DEBUG_HEADER_SIZE_IN_64BIT_WORDS = SISTRIP_FEUNITS_PER_FED*2;
  static const size_t FULL_DEBUG_HEADER_SIZE_IN_BYTES = FULL_DEBUG_HEADER_SIZE_IN_64BIT_WORDS*8;
  uint8_t header_[FULL_DEBUG_HEADER_SIZE_IN_BYTES];
};

class SiStripFEDPayload
{
public:
  inline SiStripFEDPayload() : data_(NULL) { }
  inline SiStripFEDPayload(const uint8_t* data) : data_(data) { }
  inline uint8_t operator[](size_t offset) const { return data_[(offset & (~0x7)) + 7 - (offset & (0x7))]; }
private:
  const uint8_t* data_;
};

class SiStripFEDEventBuffer : public SiStripFEDBufferBase
{
public:
  //construct from buffer
  //if allowBadBuffer is set to true then exceptions will not be thrown if the channel lengths do not make sense or the event format is not recognized
  SiStripFEDEventBuffer(const FEDRawData& fedBuffer, bool allowBadBuffer = false);
  virtual ~SiStripFEDEventBuffer();
  inline const SiStripFEDFEHeader* feHeader() const { return feHeader_.get(); }
  //check that channel is on enabled FE Unit and has no errors
  inline bool channelGood(uint8_t internalFEDChannelNum) const;
  bool channelGood(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return channelGood(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  //these methods return the offset in the data where the channel starts. This will not be valid if the FE unit is disabled
  inline size_t getChannelOffset(uint8_t internalFEDChannelNum) const;
  inline size_t getChannelOffset(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const 
    { return getChannelOffset(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  inline uint16_t channelLength(uint8_t internalFEDChannelNum) const;
  inline uint16_t channelLength(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return channelLength(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }

  //functions to check buffer. All return true if there is no problem.
  bool checkMajorityAddresses() const;
  //minimum checks to do before using buffer
  virtual bool doChecks() const;
  
  //additional checks to check for corrupt buffers
  //check channel lengths fit inside to buffer length
  bool checkChannelLengths() const;
  //check that channel lengths add up to buffer length (this does the previous check as well)
  bool checkChannelLengthsMatchBufferLength() const;
  //check channel packet codes match readout mode
  bool checkChannelPacketCodes() const;
  //check FE unit lengths in FULL DEBUG header match the lengths of their channels
  bool checkFEUnitLengths() const;
  //check FE unit APV addresses in FULL DEBUG header are equal to the APVe address if the majority was good
  //bool checkFEUnitAPVAddresses() const;
  //do all corrupt buffer checks
  virtual bool doCorruptBufferChecks() const;
  
  //check that there are no errors in channel, APV or FEUnit status bits
  //these are done by channelGood(). Channels with bad status bits may be disabled so bad status bits do not usually indicate an error
  inline bool checkStatusBits(uint8_t internalFEDChannelNum) const { return feHeader_->checkChannelStatusBits(internalFEDChannelNum); }
  inline bool checkStatusBits(uint8_t internalFEUnitNum, uint8_t internalChannelNum) const
    { return checkStatusBits(internalFEDChannelNum(internalFEUnitNum,internalChannelNum)); }
  //same but for all channels on enabled FE units
  bool checkAllChannelStatusBits() const;
  
  //print a summary of all checks
  virtual std::string checkSummary() const;
  
  const SiStripFEDPayload& payload() const { return payload_; }
  size_t payloadLength() const { return payloadLength_; }
  
  virtual uint8_t nFEUnitsEnabled() const;
private:
  inline bool fePresent(uint8_t internalFEUnitNum) const { return fePresent_[internalFEUnitNum]; }
  uint8_t getCorrectPacketCode() const;
  inline uint16_t lengthFromChannelOffset(const size_t channelOffset) const;
  void findChannels();
  //check packet code is some value for a particular channel. channel data must be present
  bool checkChannelPacketCode(const uint8_t internalFEDChannelNum, const uint8_t correctPacketCode) const;
  std::vector<size_t> channelOffsets_;
  std::auto_ptr<const SiStripFEDFEHeader> feHeader_;
  SiStripFEDPayload payload_;
  uint16_t payloadLength_;
  uint8_t lastValidChannel_;
  bool fePresent_[SISTRIP_FEUNITS_PER_FED];
};

//
// Inline function definitions
//

uint8_t internalFEDChannelNum(uint8_t internalFEUnitNum, uint8_t internalChannelNum)
{
  return (internalFEUnitNum*SISTRIP_CHANNELS_PER_FEUNIT + internalChannelNum);
}

inline std::ostream& operator<<(std::ostream& os, const SiStripFEDDAQHeader& data) { data.print(os); return os; }
inline std::ostream& operator<<(std::ostream& os, const SiStripFEDDAQTrailer& data) { data.print(os); return os; }
inline std::ostream& operator<<(std::ostream& os, const SiStripTrackerSpecialHeader& data) { data.print(os); return os; }
inline std::ostream& operator<<(std::ostream& os, const SiStripFEDStatusRegister& data) { data.print(os); return os; }
inline std::ostream& operator<<(std::ostream& os, const SiStripFEDFEHeader& data) { data.print(os); return os; }
inline std::ostream& operator<<(std::ostream& os, const SiStripFEDBufferBase& data) { data.print(os); return os; }

//SiStripFEDEventBuffer

size_t SiStripFEDEventBuffer::getChannelOffset(uint8_t internalFEDChannelNum) const
{
  return channelOffsets_[internalFEDChannelNum];
}

uint16_t SiStripFEDEventBuffer::channelLength(uint8_t internalFEDChannelNum) const
{
  return lengthFromChannelOffset(channelOffsets_[internalFEDChannelNum]);
}

uint16_t SiStripFEDEventBuffer::lengthFromChannelOffset(const size_t channelOffset) const
{
  return ( payload_[channelOffset] | (payload_[channelOffset+1]<<8) );
}

bool SiStripFEDEventBuffer::channelGood(uint8_t internalFEDChannelNum) const
{
  return ( (internalFEDChannelNum <= lastValidChannel_) &&
           feGood(internalFEDChannelNum/SISTRIP_CHANNELS_PER_FEUNIT) &&
           fePresent(internalFEDChannelNum/SISTRIP_CHANNELS_PER_FEUNIT) &&
           checkStatusBits(internalFEDChannelNum) );
}

//SiStripFEDBufferBase

bool SiStripFEDBufferBase::doTrackerSpecialHeaderChecks() const
{
  return ( checkEventFormat() &&
           checkHeaderType() &&
           checkReadoutMode() &&
           checkAPVEAddressValid() );
}

bool SiStripFEDBufferBase::doDAQHeaderAndTrailerChecks() const
{
  return ( checkNoSLinkTransmissionError() &&
           checkSourceIDIsSiStrip() &&
           checkNoUnexpectedSourceID() &&
           checkNoExtraHeadersOrTrailers() &&
           checkLengthFromTrailer() );
}

//re-use the const method by using static and const casts to avoid code duplication
uint8_t* SiStripFEDBufferBase::getPointerToDataAfterTrackerSpecialHeader()
{
  const SiStripFEDBufferBase* constThis = static_cast<const SiStripFEDBufferBase*>(this);
  const uint8_t* constPointer = constThis->getPointerToDataAfterTrackerSpecialHeader();
  return const_cast<uint8_t*>(constPointer);
}

uint8_t* SiStripFEDBufferBase::getPointerToByteAfterEndOfPayload()
{
  const SiStripFEDBufferBase* constThis = static_cast<const SiStripFEDBufferBase*>(this);
  const uint8_t* constPointer = constThis->getPointerToByteAfterEndOfPayload();
  return const_cast<uint8_t*>(constPointer);
}

//SiStripTrackerSpecialHeader

SiStripTrackerSpecialHeader::SiStripTrackerSpecialHeader()
  : wordSwapped_(false)
{
}

bool SiStripTrackerSpecialHeader::majorityAddressErrorForFEUnit(uint8_t internalFEUnitNum) const
{
  //TODO: check this is correct order
  return ( (0x1<<internalFEUnitNum) & getAPVAddressErrorRegister() );
}

bool SiStripTrackerSpecialHeader::feEnabled(uint8_t internalFEUnitNum) const
{
  //TODO: check this is correct order
  return ( (0x1<<internalFEUnitNum) & getFEEnableRegister() );
}

bool SiStripTrackerSpecialHeader::feOverflow(uint8_t internalFEUnitNum) const
{
  //TODO: check this is correct order
  return ( (0x1<<internalFEUnitNum) & getFEOverflowRegister() );
}

uint16_t SiStripTrackerSpecialHeader::getFEDStatusRegister() const
{
  //get 16 bits
  //TODO: Is this the correct byte ordering
  uint16_t statusRegister = ( (specialHeader_[(FEDSTATUS+1)^0x04]<<8) | specialHeader_[FEDSTATUS^0x04]);
  return statusRegister;
}

//SiStripFEDFEHeader

std::auto_ptr<SiStripFEDFEHeader> SiStripFEDFEHeader::newFEHeader(SiStripFEDHeaderType headerType, const uint8_t* headerBuffer)
{
  switch (headerType) {
  case SISTRIP_HEADER_TYPE_FULL_DEBUG:
    return std::auto_ptr<SiStripFEDFEHeader>(new SiStripFEDFullDebugHeader(headerBuffer));
  case SISTRIP_HEADER_TYPE_APV_ERROR:
    return std::auto_ptr<SiStripFEDFEHeader>(new SiStripFEDAPVErrorHeader(headerBuffer));
  default:
    //TODO: throw exception
    return std::auto_ptr<SiStripFEDFEHeader>();
  }
}

SiStripFEDAPVErrorHeader::SiStripFEDAPVErrorHeader(const uint8_t* headerBuffer)
{
  memcpy(header_,headerBuffer,APV_ERROR_HEADER_SIZE_IN_BYTES);
}

SiStripFEDFullDebugHeader::SiStripFEDFullDebugHeader(const uint8_t* headerBuffer)
{
  memcpy(header_,headerBuffer,FULL_DEBUG_HEADER_SIZE_IN_BYTES);
}

bool SiStripFEDFullDebugHeader::getBitVal(uint8_t internalFEDChannelNum, uint8_t bit) const
{
  const uint8_t* pFEWord = feWord(internalFEDChannelNum / SISTRIP_CHANNELS_PER_FEUNIT);
  const uint8_t bitInFeWord = (internalFEDChannelNum % SISTRIP_CHANNELS_PER_FEUNIT) * 6 + bit;
  return ( pFEWord[bitInFeWord/8] & (0x1 << bitInFeWord%8) );
}

uint32_t SiStripFEDFullDebugHeader::get32BitWordFrom(const uint8_t* startOfWord)
{
  //TODO: check byte ordering
  return ( startOfWord[0] | (startOfWord[1]<<8) | (startOfWord[2]<<16) | (startOfWord[3]<<24) );
}

//SiStripFEDDAQHeader

SiStripFEDDAQHeader::SiStripFEDDAQHeader(const uint8_t* header)
{
  memcpy(header_,header,8);
}

SiStripFEDDAQEventType SiStripFEDDAQHeader::eventType() const
{
  switch(eventTypeNibble()) {
  case SISTRIP_DAQ_EVENT_TYPE_PHYSICS:
  case SISTRIP_DAQ_EVENT_TYPE_CALIBRATION:
  case SISTRIP_DAQ_EVENT_TYPE_TEST:
  case SISTRIP_DAQ_EVENT_TYPE_TECHNICAL:
  case SISTRIP_DAQ_EVENT_TYPE_SIMULATED:
  case SISTRIP_DAQ_EVENT_TYPE_TRACED:
  case SISTRIP_DAQ_EVENT_TYPE_ERROR:
    return SiStripFEDDAQEventType(eventTypeNibble());
  default:
    return SISTRIP_DAQ_EVENT_TYPE_INVALID;
  }
}

uint32_t SiStripFEDDAQHeader::l1ID() const
{
  //TODO: check byte ordering
  return ( header_[0] | (header_[1]<<8) | (header_[2]<<16) );
}

uint16_t SiStripFEDDAQHeader::bxID() const
{
  //TODO: check byte ordering
  return ( (header_[7]<<4) | ((header_[6]&0xF0)>>4) );
}

uint16_t SiStripFEDDAQHeader::sourceID() const
{
  //TODO: check byte ordering
  return ( ((header_[6]&0x0F)<<8) | header_[5] );
}

//SiStripFEDDAQTrailer

SiStripFEDDAQTrailer::SiStripFEDDAQTrailer(const uint8_t* trailer)
{
  memcpy(trailer_,trailer,8);
}

uint32_t SiStripFEDDAQTrailer::eventLengthIn64BitWords() const
{
  //TODO: check byte ordering
  return ( trailer_[0] | (trailer_[1]<<8) | (trailer_[2]<<16) );
}

uint16_t SiStripFEDDAQTrailer::crc() const
{
  //TODO: check byte ordering
  return ( trailer_[6] | (trailer_[7]<<8) );
}

SiStripFEDTTSBits SiStripFEDDAQTrailer::ttsBits() const
{
  switch(ttsNibble()) {
  case SISTRIP_TTS_DISCONNECTED:
  case SISTRIP_TTS_WARN_OVERFLOW:
  case SISTRIP_TTS_OUT_OF_SYNC:
  case SISTRIP_TTS_BUSY:
  case SISTRIP_TTS_READY:
  case SISTRIP_TTS_ERROR:
    return SiStripFEDTTSBits(ttsNibble());
  default:
    return SISTRIP_TTS_INVALID;
  }
}

#endif //EventFilter_SiStripRawToDigi_SiStripFEDBuffer_H
