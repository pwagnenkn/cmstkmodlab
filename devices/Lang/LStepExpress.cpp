#include <cmath>
#include <sstream>

#include "LStepExpress.h"

LStepExpress::LStepExpress( const ioport_t ioPort )
  :VLStepExpress(ioPort),
   isDeviceAvailable_(false)
{
  comHandler_ = new LStepExpressComHandler( ioPort );
  DeviceInit();
}

LStepExpress::~LStepExpress()
{
  delete comHandler_;
}

bool LStepExpress::DeviceAvailable() const
{
  return isDeviceAvailable_;
}

unsigned int LStepExpress::GetStatus() const
{
  comHandler_->SendCommand("?status"); // read status

  char buffer[1000];
  comHandler_->ReceiveString(buffer);
  StripBuffer(buffer);

  unsigned int status = std::atoi(buffer);

  return status;
}

// low level debugging methods
void LStepExpress::SendCommand(const std::string & command)
{
  comHandler_->SendCommand(command.c_str());
}

void LStepExpress::ReceiveString(std::string & buffer)
{
  char buf[1000];
  comHandler_->ReceiveString(buf);
  StripBuffer(buf);
  buffer = buf;
}

void LStepExpress::StripBuffer(char* buffer) const
{
  for (unsigned int c=0; c<sizeof(buffer);++c) {
    if(buffer[c]=='\r') {
      buffer[c] = 0;
      break;
    }
  }
}

void LStepExpress::DeviceInit()
{
  isDeviceAvailable_ = false;

  if (comHandler_->DeviceAvailable()) {
    
    isDeviceAvailable_ = true;

    char buffer[1000];
    std::string buf;

    comHandler_->SendCommand("?ver"); // read version
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    if (buf.find("PE43 1.00.01", 0)!=0) {
      isDeviceAvailable_ = false;
      return;
    }

    comHandler_->SendCommand("?iver"); // read internal version
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    buf = buffer;

    if (buf.find("E2015.02.27-3012", 0)!=0) {
      isDeviceAvailable_ = false;
      return;
    }

    comHandler_->SendCommand("?readsn"); // read serial number
    comHandler_->ReceiveString(buffer);
    StripBuffer(buffer);
    unsigned long serialNumber = std::atol(buffer);

    if (serialNumber != 40051635759) {
      isDeviceAvailable_ = false;
      return;
    }
  }
}
