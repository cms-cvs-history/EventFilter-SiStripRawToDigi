#include "EventFilter/SiStripRawToDigi/interface/SiStripFEDEventBuffer.h"
#include "FWCore/Utilities/interface/CRC16.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include <iomanip>
#include <sstream>

void printHexValue(uint8_t value, std::ostream& os)
{
  std::ios_base::fmtflags originalFormatFlags = os.flags();
  os << std::hex << std::setfill('0') << std::setw(2);
  os << uint16_t(value);
  os.flags(originalFormatFlags);
}

void printHexWord(const uint8_t* pointer, size_t lengthInBytes, std::ostream& os)
{
  size_t i = lengthInBytes-1;
  do{
    printHexValue(pointer[i],os);
    if (i != 0) os << " ";
  } while (i-- != 0);
}

void printHex(const void* pointer, size_t lengthInBytes, std::ostream& os)
{
  const uint8_t* bytePointer = reinterpret_cast<const uint8_t*>(pointer);
  //if there is one 64 bit word or less, print it out
  if (lengthInBytes <= 8) {
    printHexWord(bytePointer,lengthInBytes,os);
  }
  //otherwise, print word numbers etc
  else {
    //header
    os << "word\tbyte\t                       \t\tbyte" << std::endl;;
    size_t words = lengthInBytes/8;
    size_t extraBytes = lengthInBytes - 8*words;
    //print full words
    for (size_t w = 0; w < words; w++) {
      const size_t startByte = w*8;
      os << w << '\t' << startByte+8 << '\t';
      printHexWord(bytePointer+startByte,8,os);
      os << "\t\t" << startByte << std::endl;
    }
    //print part word, if any
    if (extraBytes) {
      const size_t startByte = words*8;
      os << words << '\t' << startByte+8 << '\t';
      //padding
      size_t p = 8;
      while (p-- > extraBytes) {
        os << "00 ";
      }
      printHexWord(bytePointer+startByte,extraBytes,os);
      os << "\t\t" << startByte << std::endl;
    }
    os << std::endl;
  }
}


std::ostream& operator<<(std::ostream& os, const SiStripFEDEventFormat& value)
{
  switch (value) {
  case SISTRIP_EVENT_FORMAT_OLD_VME:
    os << "Old VME";
    break;
  case SISTRIP_EVENT_FORMAT_OLD_SLINK:
    os << "Old S-Link";
    break;
  case SISTRIP_EVENT_FORMAT_NEW:
    os << "New";
    break;
  case SISTRIP_EVENT_FORMAT_INVALID:
    os << "Invalid";
    break;
  default:
    os << "Unrecognized";
    break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const SiStripFEDHeaderType& value)
{
  switch (value) {
  case SISTRIP_HEADER_TYPE_FULL_DEBUG:
    os << "Full debug";
    break;
  case SISTRIP_HEADER_TYPE_APV_ERROR:
    os << "APV error";
    break;
  case SISTRIP_HEADER_TYPE_INVALID:
    os << "Invalid";
    break;
  default:
    os << "Unrecognized";
    break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const SiStripFEDReadoutMode& value)
{
  switch (value) {
  case SISTRIP_READOUT_MODE_SCOPE:
    os << "Scope mode";
    break;
  case SISTRIP_READOUT_MODE_VIRGIN_RAW:
    os << "Virgin raw";
    break;
  case SISTRIP_READOUT_MODE_PROC_RAW:
    os << "Processed raw";
    break;
  case SISTRIP_READOUT_MODE_ZERO_SUPPRESSED:
    os << "Zero suppressed";
    break;
  case SISTRIP_READOUT_MODE_ZERO_SUPPRESSED_LITE:
    os << "Zero suppressed lite";
    break;
  case SISTRIP_READOUT_MODE_INVALID:
    os << "Invalid";
    break;
  default:
    os << "Unrecognized";
    break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const SiStripFEDDataType& value)
{
  switch (value) {
  case SISTRIP_DATA_TYPE_REAL:
    os << "Real data";
    break;
  case SISTRIP_DATA_TYPE_FAKE:
    os << "Fake data";
    break;
  default:
    os << "Unrecognized";
    break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const SiStripFEDDAQEventType& value)
{
  switch (value) {
  case SISTRIP_DAQ_EVENT_TYPE_PHYSICS:
    os << "Physics trigger";
    break;
  case SISTRIP_DAQ_EVENT_TYPE_CALIBRATION:
    os << "Calibration trigger";
    break;
  case SISTRIP_DAQ_EVENT_TYPE_TEST:
    os << "Test trigger";
    break;
  case SISTRIP_DAQ_EVENT_TYPE_TECHNICAL:
    os << "Technical trigger";
    break;
  case SISTRIP_DAQ_EVENT_TYPE_SIMULATED:
    os << "Simulated event";
    break;
  case SISTRIP_DAQ_EVENT_TYPE_TRACED:
    os << "Traced event";
    break;
  case SISTRIP_DAQ_EVENT_TYPE_ERROR:
    os << "Error";
    break;
  case SISTRIP_DAQ_EVENT_TYPE_INVALID:
    os << "Unknown";
    break;
  default:
    os << "Unrecognized";
    break;
  }
  return os;
}

std::ostream& operator<<(std::ostream& os, const SiStripFEDTTSBits& value)
{
  switch (value) {
  case SISTRIP_TTS_DISCONNECTED:
    os << "Disconected";
    break;
  case SISTRIP_TTS_WARN_OVERFLOW:
    os << "Warning overflow";
    break;
  case SISTRIP_TTS_OUT_OF_SYNC:
    os << "Out of sync";
    break;
  case SISTRIP_TTS_BUSY:
    os << "Busy";
    break;
  case SISTRIP_TTS_READY:
    os << "Ready";
    break;
  case SISTRIP_TTS_ERROR:
    os << "Error";
    break;
  case SISTRIP_TTS_INVALID:
    os << "Invalid";
    break;
  default:
    os << "Unrecognized";
    break;
  }
  return os;
}


SiStripFEDEventBuffer::SiStripFEDEventBuffer(const FEDRawData& fedBuffer, bool allowBadBuffer)
  : SiStripFEDBufferBase(fedBuffer,allowBadBuffer)
{
  //reserve channel pointer cache
  channelOffsets_.reserve(SISTRIP_CHANNELS_PER_FED);
  uint8_t* payloadPointer = NULL;
  //build the correct type of FE header object
  if (headerType() != SISTRIP_HEADER_TYPE_INVALID) {
    feHeader_ = SiStripFEDFEHeader::newFEHeader(headerType(),getPointerToDataAfterTrackerSpecialHeader());
    //set the address of the start of the data for the first channel using the length from the feHeader object
    payloadPointer = getPointerToDataAfterTrackerSpecialHeader()+feHeader_->lengthInBytes();
  } else {
    feHeader_ = NULL;
    payloadPointer = getPointerToDataAfterTrackerSpecialHeader();
    if (!allowBadBuffer) {
      std::ostringstream ss;
      ss << "Header type is invalid. "
         << "Header type nibble is ";
      uint8_t headerNibble = trackerSpecialHeader().headerTypeNibble();
      printHex(&headerNibble,1,ss);
      ss << ". ";
      throw cms::Exception("SiStripFEDBuffer") << ss;
    }
  }
  lastValidChannel_ = 0;
  payloadLength_ = getPointerToByteAfterEndOfPayload()-payloadPointer;
  payload_ = SiStripFEDPayload(payloadPointer);
  //try to find channels
  try {
    findChannels();
  } catch (const cms::Exception& e) {
    //if there was a problem either rethrow the exception or just mark channel pointers NULL
    if (!allowBadBuffer) throw;
    else {
      channelOffsets_.insert(channelOffsets_.end(),size_t(SISTRIP_CHANNELS_PER_FED-lastValidChannel_),size_t(-1));
    }
  }
}

SiStripFEDEventBuffer::~SiStripFEDEventBuffer()
{
  if (feHeader_) delete feHeader_;
}

void SiStripFEDEventBuffer::findChannels()
{
  size_t offsetBeginningOfChannel = 0;
  for (size_t i = 0; i < SISTRIP_CHANNELS_PER_FED; i++) {
    //if FE unit is not enabled then skip rest of FE unit adding NULL pointers
    if (!feEnabled(i/SISTRIP_CHANNELS_PER_FEUNIT)) {
      channelOffsets_.insert(channelOffsets_.end(),size_t(SISTRIP_CHANNELS_PER_FEUNIT),size_t(-1));
      i += SISTRIP_CHANNELS_PER_FEUNIT-1;
      lastValidChannel_ += SISTRIP_CHANNELS_PER_FEUNIT;
      continue;
    }
    //if FE unit is enabled
    //check that channel length bytes fit into buffer
    if (offsetBeginningOfChannel+2 >= payloadLength_) {
      throw cms::Exception("SiStripFEDBuffer") << "Channel " << uint16_t(i) << " does not fit into buffer. "
                                               << "Channel starts at " << uint16_t(offsetBeginningOfChannel) << " in payload. "
                                               << "Payload length is " << uint16_t(payloadLength_) << ". ";
    }
    //get length and check that whole channel fits into buffer
    uint16_t channelLength = lengthFromChannelOffset(offsetBeginningOfChannel);
    if (offsetBeginningOfChannel+lengthFromChannelOffset(offsetBeginningOfChannel) > payloadLength_) {
      throw cms::Exception("SiStripFEDBuffer") << "Channel " << uint16_t(i) << " does not fit into buffer. "
                                               << "Channel starts at " << uint16_t(offsetBeginningOfChannel) << " in payload. "
                                               << "Channel length is " << uint16_t(lengthFromChannelOffset(offsetBeginningOfChannel)) << ". "
                                               << "Payload length is " << uint16_t(payloadLength_) << ". ";
    }
    //channel fits so it can be added to list
    lastValidChannel_++;
    channelOffsets_.push_back(offsetBeginningOfChannel);
    const size_t offsetEndOfChannel = offsetBeginningOfChannel+channelLength;
    //add padding if necessary and calculate offset for begining of next channel
    if (!( (i+1) % SISTRIP_CHANNELS_PER_FEUNIT )) {
      uint8_t numPaddingBytes = 8 - (offsetEndOfChannel % 8);
      if (numPaddingBytes == 8) numPaddingBytes = 0;
      offsetBeginningOfChannel = offsetEndOfChannel + numPaddingBytes;
    } else {
      offsetBeginningOfChannel = offsetEndOfChannel;
    }
  }
}

// void SiStripFEDEventBuffer::findChannelLengthsUpToAndIncludingChannel(uint8_t internalFEDChannelNum) const
// {
//   //first channel pointer is set by constructor at end of header
//   if (internalFEDChannelNum==0) return;
//   //loop from last channel which has been added to cache to required channel and get pointers
//   for (size_t i = channelPointers_.size(); i <= internalFEDChannelNum; i++) {
//     //check that FE unit is enabled and if not then skip channel (and add NULL pointer)
//     if (!feEnabled(i/SISTRIP_CHANNELS_PER_FEUNIT)) {
//       channelPointers_.push_back(NULL);
//       continue;
//     }
//     //if FE unit is enabled
//     uint8_t* pointerToPreviousChannel = channelPointers_[i-1];
//     const uint16_t previousChannelLength = lengthFromChannelPointer(pointerToPreviousChannel);
//     uint8_t* pointerToThisChannel;
//     //if this is the first channel on a new FE unit then add padding
//     if ( (internalFEDChannelNum % SISTRIP_CHANNELS_PER_FEUNIT) == 0) {
//       //first byte after previous channel
//       size_t indexFirstByteAfterPreviousChannel = (pointerToPreviousChannel-channelPointers_[0])+previousChannelLength;
//       //round to 64 bits
//       size_t indexThisChannel;
//       //if it already at the begining of a word then use this index
//       if ( (indexFirstByteAfterPreviousChannel % 8) == 0) {
//         indexThisChannel = indexFirstByteAfterPreviousChannel;
//       } else {
//         //calculate number of words (rounding down), add one and multiply by no. bytes per word
//         indexThisChannel = ((indexFirstByteAfterPreviousChannel >> 3) + 1) << 3;
//       }
//       pointerToThisChannel = channelPointers_[0] + indexThisChannel;
//     }
//     else {
//       pointerToThisChannel = pointerToPreviousChannel+previousChannelLength;
//     }
//     //check that channel fits into buffer (throw an exception if not)
//     const size_t channelOffset = pointerToThisChannel-channelPointers_[0];
//     if (channelOffset+2 > payloadLength_)
//       throw cms::Exception("SiStripFEDBuffer") << "Channel " << i << " does not fit into buffer. "
//                                                << "Channel starts at " << channelOffset << " in payload. "
//                                                << "Payload length is " << payloadLength_ << ". ";
//     if (channelOffset+lengthFromChannelPointer(pointerToThisChannel) > payloadLength_)
//       throw cms::Exception("SiStripFEDBuffer") << "Channel " << i << " does not fit into buffer. "
//                                                << "Channel starts at " << channelOffset << " in payload. "
//                                                << "Channel length is " << lengthFromChannelPointer(pointerToThisChannel) << ". "
//                                                << "Payload length is " << payloadLength_ << ". ";
//     //add pointer to cache
//     channelPointers_.push_back(pointerToThisChannel);
//   }
// }

bool SiStripFEDEventBuffer::doChecks() const
{
  //check that all channels were unpacked properly
  if (lastValidChannel_ != SISTRIP_CHANNELS_PER_FED) return false;
  //do checks from base class
  if (!SiStripFEDBufferBase::doChecks()) return false;
  return true;
}

bool SiStripFEDEventBuffer::doCorruptBufferChecks() const
{
  return ( checkCRC() &&
           checkChannelLengths() &&
           checkChannelPacketCodes() &&
           //checkClusterLengths() &&
           checkFEUnitLengths() );
           //checkFEUnitAPVAddress() &&
           //checkFEUnitAPVAddressCorrect() );
}

bool SiStripFEDEventBuffer::checkAllChannelStatusBits() const
{
  for (uint8_t iCh = 0; iCh < SISTRIP_CHANNELS_PER_FED; iCh++) {
    //if FE unit is disabled then skip all channels on it
    if (!feEnabled(iCh/SISTRIP_CHANNELS_PER_FED)) {
      iCh += SISTRIP_CHANNELS_PER_FEUNIT-1;
      continue;
    }
    //channel is bad then return false
    if (!checkStatusBits(iCh)) return false;
  }
  //if no bad channels have been found the nthey are all fine
  return true;
}

bool SiStripFEDEventBuffer::checkChannelLengths() const
{
  return (lastValidChannel_ == SISTRIP_CHANNELS_PER_FED);
}

bool SiStripFEDEventBuffer::checkChannelLengthsMatchBufferLength() const
{
  //check they fit into buffer and find lengths
  if (!checkChannelLengths()) return false;
  
  //payload length from length of data buffer
  const size_t payloadLengthInWords = payloadLength_/8;
  
  //find channel length
  //find last enabled FE unit
  uint8_t lastEnabledFeUnit = 7;
  while (!feEnabled(lastEnabledFeUnit)) lastEnabledFeUnit--;
  //last channel is last channel on last enabled FE unit
  const size_t offsetLastChannel = channelOffsets_[internalFEDChannelNum(lastEnabledFeUnit,SISTRIP_CHANNELS_PER_FEUNIT-1)];
  const size_t offsetEndOfChannelData = offsetLastChannel+lengthFromChannelOffset(offsetLastChannel);
  const size_t channelDataLength = offsetEndOfChannelData;
  //channel length in words is length in bytes rounded up to nearest word
  size_t channelDataLengthInWords = channelDataLength/8;
  if (channelDataLength % 8) channelDataLengthInWords++;
  
  //check lengths match
  if (channelDataLengthInWords == payloadLengthInWords) {
    return true;
  } else {
    return false;
  }
}

bool SiStripFEDEventBuffer::checkChannelPacketCodes() const
{
  //if mode is ZS Lite then retyurn true since check can't be done since packet code is missing
  //for other modes get the correct code
  //if the mode is not valid then return false
  uint8_t correctPacketCode = 0x00;
  switch (readoutMode()) {
  case SISTRIP_READOUT_MODE_ZERO_SUPPRESSED_LITE:
    return true;
  case SISTRIP_READOUT_MODE_SCOPE:
    correctPacketCode = SISTRIP_PACKET_CODE_SCOPE;
    break;
  case SISTRIP_READOUT_MODE_VIRGIN_RAW:
    correctPacketCode = SISTRIP_PACKET_CODE_VIRGIN_RAW;
    break;
  case SISTRIP_READOUT_MODE_PROC_RAW:
    correctPacketCode = SISTRIP_PACKET_CODE_PROC_RAW;
    break;
  case SISTRIP_READOUT_MODE_ZERO_SUPPRESSED:
    correctPacketCode = SISTRIP_PACKET_CODE_ZERO_SUPPRESSED;
    break;
  default:
    return false;
  }
  for (uint8_t iCh = 0; iCh < SISTRIP_CHANNELS_PER_FED; iCh++) {
    //if FE unit is disabled then skip all channels on it
    if (!feEnabled(iCh/SISTRIP_CHANNELS_PER_FED)) {
      iCh += SISTRIP_CHANNELS_PER_FEUNIT-1;
      continue;
    }
    //only check enabled, working channels
    if (channelGood(iCh)) {
      //if a channel is bad then return false
      if (!checkChannelPacketCode(iCh,correctPacketCode)) return false;
    }
  }
  //if no bad channels were found the they are all ok
  return true;
}

bool SiStripFEDEventBuffer::checkChannelPacketCode(const uint8_t internalFEDChannelNum, const uint8_t correctPacketCode) const
{
  const size_t channelOffset = channelOffsets_[internalFEDChannelNum];
  //if the length is somehow too small for there to be a packet code then something is wrong
  if (channelLength(internalFEDChannelNum) < 3) return false;
  const uint8_t packetCode = payload_[channelOffset+2];
  if (packetCode != correctPacketCode) return false;
  else return true;
}

bool SiStripFEDEventBuffer::checkFEUnitLengths() const
{
  //check can only be done for full debug headers
  const SiStripFEDFullDebugHeader* fdHeader = dynamic_cast<const SiStripFEDFullDebugHeader*>(feHeader_);
  if (!fdHeader) return true;
  //get golden address
  const uint8_t goldenAddress = apveAddress();
  //check all enabled FE units
  for (uint8_t iFE = 0; iFE < SISTRIP_FEUNITS_PER_FED; iFE++) {
    if (!feEnabled(iFE)) continue;
    //if address is bad then return false
    if (fdHeader->getFEUnitMajorityAddress(iFE) != goldenAddress) return false;
  }
  //if no bad addresses were found then return true
  return true;
}

std::string SiStripFEDEventBuffer::checkSummary() const
{
  std::stringstream summary;
  summary << SiStripFEDBufferBase::checkSummary();
  summary << "Check channel status bits: " << ( checkAllChannelStatusBits() ? "passed" : "FAILED" ) << std::endl;
  if (!checkAllChannelStatusBits()) {
    summary << "Channels with errors: ";
    unsigned int badChannels = 0;
    for (uint8_t iCh = 0; iCh < SISTRIP_CHANNELS_PER_FED; iCh++) {
      if (!feEnabled(iCh/SISTRIP_CHANNELS_PER_FED)) continue;
      if (!checkStatusBits(iCh)) {
        summary << uint16_t(iCh) << " ";
        badChannels++;
      }
    }
    summary << std::endl;
    summary << "Number of channels with bad status bits: " << badChannels << std::endl;
  }
  summary << "Check channel lengths match buffer length: " << ( checkChannelLengthsMatchBufferLength() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check channel packet codes: " << ( checkChannelPacketCodes() ? "passed" : "FAILED" ) << std::endl;
  //summary << "Check cluster lengths: " << ( checkClusterLengths() ? "passed" : "FAILED" ) << std::endl;
  //summary << "Check FE unit lengths: " << ( checkFEUnitLenghts() ? "passed" : "FAILED" ) << std::endl;
  //summary << "Check FE unit APV addresses match APVe: " << ( checkFEUnitAPVAddresses() ? "passed" : "FAILED" ) << std::endl;
  return summary.str();
}




SiStripFEDBufferBase::SiStripFEDBufferBase(const FEDRawData& fedBuffer, bool allowUnrecognizedFormat)
  : originalBuffer_(fedBuffer)
{
  //min buffer length. DAQ header, DAQ trailer, tracker special header. 
  static const size_t MIN_BUFFER_SIZE = 8+8+8;
  //check size is non zero and data pointer is not NULL
  if (!originalBuffer_.data()) throw cms::Exception("SiStripFEDBuffer") << "Buffer pointer is NULL. ";
  if (originalBuffer_.size() < MIN_BUFFER_SIZE) 
    throw cms::Exception("SiStripFEDBuffer") << "Buffer is too small. "
                                             << "Min size is " << MIN_BUFFER_SIZE << ". "
                                             << "Buffer size is " << originalBuffer_.size() << ". ";
  
  //construct tracker special header using second 64 bit word
  specialHeader_ = SiStripTrackerSpecialHeader(originalBuffer_.data()+8);
  
  //check the event format
  const SiStripFEDEventFormat eventFormat = specialHeader_.eventFormat();
  if (eventFormat == SISTRIP_EVENT_FORMAT_INVALID && !allowUnrecognizedFormat) {
    cms::Exception e("SiStripFEDBuffer");
    e << "Event format not recognized. "
      << "Tracker special header: " << specialHeader_;
    throw e;
  }
  //swap the buffer words so that the whole buffer is in new ordering
  if ( (eventFormat == SISTRIP_EVENT_FORMAT_OLD_VME) || (eventFormat == SISTRIP_EVENT_FORMAT_OLD_SLINK) ) {
    FEDRawData* newBuffer = new FEDRawData(originalBuffer_.size());
    const uint32_t* originalU32 = reinterpret_cast<const uint32_t*>(originalBuffer_.data());
    const size_t sizeU32 = originalBuffer_.size()/4;
    uint32_t* newU32 = reinterpret_cast<uint32_t*>(newBuffer->data());
    if (eventFormat == SISTRIP_EVENT_FORMAT_OLD_VME) {
      //swap DAQ header
      newU32[0] = originalU32[1];
      newU32[1] = originalU32[0];
      //swap DAQ trailer
      newU32[sizeU32-2] = originalU32[sizeU32-1];
      newU32[sizeU32-1] = originalU32[sizeU32-2];
      //copy payload
      memcpy(newU32+2,originalU32+2,sizeU32*4-16);
    }
    if (eventFormat == SISTRIP_EVENT_FORMAT_OLD_SLINK) {
      //copy DAQ header
      memcpy(newU32,originalU32,8);
      //copy DAQ trailer
      memcpy(newU32+sizeU32-2,originalU32+sizeU32-2,8);
      //swap the payload
      for (size_t i = 2; i < sizeU32-2; i+=2) {
        newU32[i] = originalU32[i+1];
        newU32[i+1] = originalU32[i];
      }
    }
    orderedBuffer_ = newBuffer;
  } //if ( (eventFormat == SISTRIP_EVENT_FORMAT_OLD_VME) || (eventFormat == SISTRIP_EVENT_FORMAT_OLD_SLINK) )
  else {
    orderedBuffer_ = &originalBuffer_;
  }
  
  //construct header object at begining of buffer
  daqHeader_ = SiStripFEDDAQHeader(orderedBuffer_->data());
  //construct trailer object using last 64 bit word of buffer
  daqTrailer_ = SiStripFEDDAQTrailer(orderedBuffer_->data()+orderedBuffer_->size()-8);
}

SiStripFEDBufferBase::~SiStripFEDBufferBase()
{
  //if the buffer was coppied and swapped then delete the copy
  if (orderedBuffer_ != &originalBuffer_) delete orderedBuffer_;
}

uint16_t SiStripFEDBufferBase::calcCRC() const
{
  uint16_t crc = 0xFFFF;
  const uint8_t* buffer = originalBuffer_.data();
  //DAQ header
  for (size_t i=0; i<8; i++) {
    uint8_t byte;
    if ( (specialHeader_.eventFormat() == SISTRIP_EVENT_FORMAT_NEW) || (specialHeader_.eventFormat() == SISTRIP_EVENT_FORMAT_OLD_SLINK) )
      byte = buffer[i^3];
    else 
      byte = buffer[i^7];
    crc = evf::compute_crc_8bit(crc,byte);
  }
  //payload
  for (size_t i=8; i<originalBuffer_.size()-8; i++) {
    uint8_t byte;
    if (specialHeader_.eventFormat() == SISTRIP_EVENT_FORMAT_OLD_SLINK)
      byte = buffer[i^3];
    else
      byte = buffer[i^7];
    crc = evf::compute_crc_8bit(crc,byte);
  }
  //DAQ trailer
  for (size_t i=originalBuffer_.size()-8; i<originalBuffer_.size(); i++) {
    uint8_t byte;
    //set CRC bytes to zero since these were not set when CRC was calculated
    if (i==originalBuffer_.size()-4 || i==originalBuffer_.size()-3) byte = 0x00;
    else {
      if ( (specialHeader_.eventFormat() == SISTRIP_EVENT_FORMAT_NEW) || (specialHeader_.eventFormat() == SISTRIP_EVENT_FORMAT_OLD_SLINK) )
        byte = buffer[i^3];
      else 
        byte = buffer[i^7];
    }
    crc = evf::compute_crc_8bit(crc,byte);
  }
  return crc;
}

void SiStripFEDBufferBase::print(std::ostream& os) const
{
  os << "Event format: " << eventFormat() << std::endl;
  os << "Buffer size: " << bufferSize() << " bytes" << std::endl;
  os << "Event length from DAQ trailer: " << daqEventLengthInBytes() << " bytes" << std::endl;
  os << "Source ID: " << daqSourceID() << std::endl;
  os << "Header type: " << headerType() << std::endl;
  os << "Readout mode: " << readoutMode() << std::endl;
  os << "Data type: " << dataType() << std::endl;
  os << "DAQ event type: " << daqEventType() << std::endl;
  os << "TTS state: " << daqTTSState() << std::endl;
  os << "L1 ID: " << daqLvl1ID() << std::endl;
  os << "BX ID: " << daqBXID() << std::endl;
  os << "FED status register flags: "; fedStatusRegister().printFlags(os); os << std::endl;
  os << "APVe Address: " << uint16_t(apveAddress()) << std::endl;
  os << "Enabled FE units: " << uint16_t(nFEUnitsEnabled()) << std::endl;
  os << checkSummary();
}

uint8_t SiStripFEDBufferBase::nFEUnitsEnabled() const
{
  uint8_t result = 0;
  for (uint8_t iFE = 0; iFE < SISTRIP_FEUNITS_PER_FED; iFE++) {
    if (feEnabled(iFE)) result++;
  }
  return result;
}

bool SiStripFEDBufferBase::checkSourceIDIsSiStrip() const
{
  return ( (daqSourceID() >= FEDNumbering::getSiStripFEDIds().first) &&
           (daqSourceID() <= FEDNumbering::getSiStripFEDIds().second) );
}

bool SiStripFEDBufferBase::checkMajorityAddresses() const
{
  for (uint8_t iFE = 0; iFE < SISTRIP_FEUNITS_PER_FED; iFE++) {
    if (!feEnabled(iFE)) continue;
    if (majorityAddressErrorForFEUnit(iFE)) return false;
  }
  return true;
}

bool SiStripFEDBufferBase::doChecks() const
{
  return (doTrackerSpecialHeaderChecks() && doDAQHeaderAndTrailerChecks());
}

std::string SiStripFEDBufferBase::checkSummary() const
{
  std::stringstream summary;
  summary << "Check event type valid: " << ( checkEventFormat() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check header format valid: " << ( checkHeaderType() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check readout mode valid: " << ( checkReadoutMode() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check APVe address valid: " << ( checkAPVEAddressValid() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check FE unit majority addresses: " << ( checkMajorityAddresses() ? "passed" : "FAILED" ) << std::endl;
  if (!checkMajorityAddresses()) {
    summary << "FEs with majority address error: ";
    unsigned int badFEs = 0;
    for (uint8_t iFE = 0; iFE < SISTRIP_FEUNITS_PER_FED; iFE++) {
      if (!feEnabled(iFE)) continue;
      if (majorityAddressErrorForFEUnit(iFE)) {
        summary << uint16_t(iFE) << " ";
        badFEs++;
      }
    }
    summary << std::endl;
    summary << "Number of FE Units with bad addresses: " << badFEs << std::endl;
  }
  summary << "Check for FE unit buffer overflows: " << ( checkNoFEOverflows() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check for S-Link CRC errors: " << ( checkNoSlinkCRCError() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check for S-Link transmission error: " << ( checkNoSLinkTransmissionError() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check CRC: " << ( checkCRC() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check source ID is SiStrip FED ID: " << ( checkSourceIDIsSiStrip() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check for unexpected source ID at FRL: " << ( checkNoUnexpectedSourceID() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check there are no extra headers or trailers: " << ( checkNoExtraHeadersOrTrailers() ? "passed" : "FAILED" ) << std::endl;
  summary << "Check length from trailer: " << ( checkLengthFromTrailer() ? "passed" : "FAILED" ) << std::endl;
  return summary.str();
}




void SiStripFEDStatusRegister::printFlags(std::ostream& os) const
{
  if (slinkFullFlag()) os << "SLINK_FULL ";
  if (trackerHeaderMonitorDataReadyFlag()) os << "HEADER_MONITOR_READY ";
  if (qdrMemoryFullFlag()) os << "QDR_FULL ";
  if (qdrMemoryPartialFullFlag()) os << "QDR_PARTIAL_FULL ";
  if (qdrMemoryEmptyFlag()) os << "QDR_EMPTY ";
  if (l1aFullFlag()) os << "L1A_FULL ";
  if (l1aPartialFullFlag()) os << "L1A_PARTIAL_FULL ";
  if (l1aEmptyFlag()) os << "L1A_EMPTY ";
}




SiStripTrackerSpecialHeader::SiStripTrackerSpecialHeader(const uint8_t* headerPointer)
{
  //the event format byte is one of the valid values if we assume the buffer is not swapped
  const bool validFormatByteWhenNotWordSwapped = ( (headerPointer[EVENTFORMAT^0x04] == SISTRIP_EVENT_FORMAT_CODE_NEW) ||
                                               (headerPointer[EVENTFORMAT^0x04] == SISTRIP_EVENT_FORMAT_CODE_OLD) );
  //the event format byte is the old value if we assume the buffer is swapped
  const bool validFormatByteWhenWordSwapped = (headerPointer[EVENTFORMAT] == SISTRIP_EVENT_FORMAT_CODE_OLD);
  //if the event format byte is valid if the buffer is not swapped or it is never valid
  if (validFormatByteWhenNotWordSwapped || (!validFormatByteWhenNotWordSwapped && !validFormatByteWhenWordSwapped) ) {
    memcpy(specialHeader_,headerPointer,8);
    wordSwapped_ = false;
  } else {
    memcpy(specialHeader_,headerPointer+4,4);
    memcpy(specialHeader_+4,headerPointer,4);
    wordSwapped_ = true;
  }
}

SiStripFEDEventFormat SiStripTrackerSpecialHeader::eventFormat() const
{
  if (eventFormatByte() == SISTRIP_EVENT_FORMAT_CODE_NEW) return SISTRIP_EVENT_FORMAT_NEW;
  else if (eventFormatByte() == SISTRIP_EVENT_FORMAT_CODE_OLD) {
    if (wordSwapped_) return SISTRIP_EVENT_FORMAT_OLD_VME;
    else return SISTRIP_EVENT_FORMAT_OLD_SLINK;
  }
  else return SISTRIP_EVENT_FORMAT_INVALID;
}

SiStripFEDHeaderType SiStripTrackerSpecialHeader::headerType() const
{
  if ( (headerTypeNibble() == SISTRIP_HEADER_TYPE_FULL_DEBUG) || 
       (headerTypeNibble() == SISTRIP_HEADER_TYPE_APV_ERROR) )
    return SiStripFEDHeaderType(headerTypeNibble());
  else return SISTRIP_HEADER_TYPE_INVALID;
}

SiStripFEDReadoutMode SiStripTrackerSpecialHeader::readoutMode() const
{
  const uint8_t eventTypeNibble = getTrackerEventTypeNibble();
  //if it is scope mode then return as is (it cannot be fake data)
  if (eventTypeNibble == SISTRIP_READOUT_MODE_SCOPE) return SiStripFEDReadoutMode(eventTypeNibble);
  //if not then ignore the last bit which indicates if it is real or fake
  else {
    const uint8_t mode = (eventTypeNibble & 0xE);
    switch(mode) {
    case SISTRIP_READOUT_MODE_VIRGIN_RAW:
    case SISTRIP_READOUT_MODE_PROC_RAW:
    case SISTRIP_READOUT_MODE_ZERO_SUPPRESSED:
    case SISTRIP_READOUT_MODE_ZERO_SUPPRESSED_LITE:
      return SiStripFEDReadoutMode(mode);
    default:
      return SISTRIP_READOUT_MODE_INVALID;
    }
  }   
}

SiStripFEDDataType SiStripTrackerSpecialHeader::dataType() const
{
  uint8_t eventTypeNibble = getTrackerEventTypeNibble();
  //if it is scope mode then it is always real
  if (eventTypeNibble == SISTRIP_READOUT_MODE_SCOPE) return SISTRIP_DATA_TYPE_REAL;
  //in other modes it is the lowest order bit of event type nibble
  else return SiStripFEDDataType(eventTypeNibble & 0x1);
}




SiStripFEDAPVErrorHeader::~SiStripFEDAPVErrorHeader()
{
}

size_t SiStripFEDAPVErrorHeader::lengthInBytes() const
{
  return APV_ERROR_HEADER_SIZE_IN_BYTES;
}

void SiStripFEDAPVErrorHeader::print(std::ostream& os) const
{
  printHex(header_,APV_ERROR_HEADER_SIZE_IN_BYTES,os);
}

bool SiStripFEDAPVErrorHeader::checkStatusBits(uint8_t internalFEDChannelNum, uint8_t apvNum) const
{
  //TODO: check ordering
  uint8_t byteNumber = internalFEDChannelNum * 2 / 8;
  uint8_t bitInByte = internalFEDChannelNum * 2 % 8;
  //bit high means no error
  return (!(header_[byteNumber] & (0x1<<bitInByte) ));
}

bool SiStripFEDAPVErrorHeader::checkChannelStatusBits(uint8_t internalFEDChannelNum) const
{
  return (checkStatusBits(internalFEDChannelNum,0) && checkStatusBits(internalFEDChannelNum,1));
}


SiStripFEDFullDebugHeader::~SiStripFEDFullDebugHeader()
{
}

size_t SiStripFEDFullDebugHeader::lengthInBytes() const
{
  return FULL_DEBUG_HEADER_SIZE_IN_BYTES;
}

void SiStripFEDFullDebugHeader::print(std::ostream& os) const
{
  printHex(header_,FULL_DEBUG_HEADER_SIZE_IN_BYTES,os);
}

bool SiStripFEDFullDebugHeader::checkStatusBits(uint8_t internalFEDChannelNum, uint8_t apvNum) const
{
  return ( !unlockedFromBit(internalFEDChannelNum) &&
           !outOfSyncFromBit(internalFEDChannelNum) &&
           !apvError(internalFEDChannelNum,apvNum) &&
           !apvAddressError(internalFEDChannelNum,apvNum) );
}

bool SiStripFEDFullDebugHeader::checkChannelStatusBits(uint8_t internalFEDChannelNum) const
{
  return ( !unlockedFromBit(internalFEDChannelNum) &&
           !outOfSyncFromBit(internalFEDChannelNum) &&
           !apvError(internalFEDChannelNum,0) &&
           !apvAddressError(internalFEDChannelNum,0) &&
           !apvError(internalFEDChannelNum,1) &&
           !apvAddressError(internalFEDChannelNum,1) );
}

SiStripFEDFEHeader::~SiStripFEDFEHeader()
{
}
