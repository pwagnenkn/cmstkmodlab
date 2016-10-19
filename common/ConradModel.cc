#include <nqlogger.h>

#include "ConradModel.h"

ConradModel::ConradModel(QObject * /* parent */)
: QObject(),
  AbstractDeviceModel(),
  switchStates_(8, OFF)
{
  setDeviceEnabled(true);
  setControlsEnabled(true);
}

/// Shuts down all the devices before closing down.
ConradModel::~ConradModel()
{
  close();
}

/**
  Will try to enable communication with the switch.
  */
void ConradModel::setDeviceEnabled(bool enabled)
{
  AbstractDeviceModel::setDeviceEnabled(enabled);
}

void ConradModel::enableSwitch( int device )
{
  setSwitchEnabled(device, true);
}

void ConradModel::disableSwitch( int device )
{
  setSwitchEnabled(device, false);
}

void ConradModel::setControlsEnabled(bool enabled)
{
  emit controlStateChanged(enabled);
}

/**
  Auto-detects Conrad card through /dev/ttyUSB* files and tests the connection.
  */
void ConradModel::initialize( void )
{
  // Set state to ConradModel::INITIALIZING so views
  // may be notified of non-responsiveness
  setDeviceState( INITIALIZING );

#ifdef USE_FAKEIO

  renewController("/dev/ttyUSBFake");

  controller_->initialize();

  std::vector<bool> status = controller_->queryStatus();

  // Announce new states
  if (status.size() == 8) {

    setAllSwitchesReady( status );
    setDeviceState( READY );
  }

#else

  // create a list with all available ttyUSB device (system) files
  QStringList filters("ttyUSB*");

  QDir devDir("/dev");
  devDir.setNameFilters(filters);
  devDir.setFilter(QDir::System);
  QStringList list = devDir.entryList();

  // Only loop over list when not empty
  if (!list.empty()) {

    // browse and try initialize() until conradController_ gets an answer
    QStringList::const_iterator it = list.begin();
    QString port = QString( "/dev/" ) + *it;

    renewController( port );

    while (it < list.end() && !controller_->initialize()) {

      // Compose full absolute location of USB device
      port = QString( "/dev/" ) + *it;
      renewController( port );

      ++it;
    }

    // check communication; if it is not at the end, switch was found
    if (it != list.end()) {

      // read and init status
      std::vector<bool> status = controller_->queryStatus();

      // Announce new states
      if (status.size() == 8) {

        setAllSwitchesReady( status );
        setDeviceState( READY );

        // FIXME Redirect to central logger
        //    if( debugLevel_ >= 1 )
        NQLog("ConradModel::initialize()", NQLog::Message)
            << "connection to conrad via: "
            << port.toStdString() << ".";

      } else {

        /*
         would be 0 if query failed (according to ConradController::queryStatus)
         This means device malfunction, so set state accordingly
         */

        setDeviceFullOff();

        NQLog("ConradModel::initialize()", NQLog::Fatal)
            << "** ERROR: received malformed state vector.";

      }
    } else {
      // if not successful; i.e. DEVICE NOT FOUND
      setDeviceFullOff();
      // TODO Log why it failed
      NQLog("ConradModel::initialize()", NQLog::Fatal)
          << "Cannot connect to Conrad. Make sure that";
      NQLog("ConradModel::initialize()", NQLog::Fatal)
          << "/dev/ttyUSB* is present and readable and no other process";
      NQLog("ConradModel::initialize()", NQLog::Fatal)
          << "is connecting to the device.";
    }
  } else {

    setDeviceFullOff();

    NQLog("ConradModel::initialize()", NQLog::Fatal)
        << "Cannot connect to Conrad. Make sure that";
    NQLog("ConradModel::initialize()", NQLog::Fatal)
        << "/dev/ttyUSB* is present and readable and no other process";
    NQLog("ConradModel::initialize()", NQLog::Fatal)
        << "is connecting to the device.";

  }
#endif
}

/// Sets all the device states to OFF
void ConradModel::setDeviceFullOff()
{
  setDeviceState( OFF );
  setAllSwitchesReady( std::vector<bool>( 8, false ) );
}

/**
  Shuts down all the connected devices and closes down the connection to the
  switch.
  */
void ConradModel::close()
{
  // TODO Controlled shutdown for busy (initializing) device.

  if (getDeviceState() == READY) {

    // Notify that no enable commands should be sent.
    setDeviceState( CLOSING );

    // Disable all connected devices
    for (int i = 0; i < 8; ++i)
      setSwitchEnabledRaw( i, false );

    // Disable switch
    destroyController();

    setDeviceState( OFF );
  }
}

/**
  Sets the current state of the switch and emits a switchStateChanged upon
  change.
  */
void ConradModel::setDeviceState(State state)
{
  if (state_ != state) {
    state_ = state;
    emit deviceStateChanged(state_);
  }
}


/**
  Enabled or disables a certain device channel on the switch.
  */
void ConradModel::setSwitchEnabled(int device,
                                   bool enabled)
{
  // TODO Upon failure, check if the switch is still reachable

  // Check if the switch communication is ready
  if ( getDeviceState() == READY )
    setSwitchEnabledRaw(device, enabled);
  else
    setSwitchState(device, OFF);

  // TODO Notify of malfunction
}

/// Returns whether the requested device is currently enabled.
const State& ConradModel::getSwitchState(int device) const
{
  return switchStates_.at( device );
}

/// Sets the current state for a device and emits a signal if necessary.
void ConradModel::setSwitchState(int device,
                                 State state)
{
  if ( switchStates_.at( device ) != state ) {
    switchStates_[device] = state;
    emit switchStateChanged( device, state );
  }
}

/**
  Small convenience method to set all device status using the vector provided by
  ConradController::queryStatus().
  */
void ConradModel::setAllSwitchesReady(const std::vector<bool>& ready)
{
  for ( unsigned int i = 0; i < ready.size(); ++i ) {
    // Set device state according to queried state
    setSwitchState( i, ready.at(i) ? READY : OFF );
  }
}

/// Sets the switch state without checking the device state
void ConradModel::setSwitchEnabledRaw(int device, bool enabled)
{
  if (enabled && getSwitchState(device) == OFF) {

    setSwitchState( device, INITIALIZING );
    // Add 1 to device because of shift required by setChannel
    bool success = controller_->setChannel( device + 1, enabled );
    setSwitchState( device, success ? READY : OFF );

  } else if ( !enabled && getSwitchState(device) == READY ) {

    setSwitchState( device, CLOSING );
    // Add 1 to device because of shift required by setChannel
    bool success = controller_->setChannel( device + 1, enabled );
    setSwitchState( device, success ? OFF : READY );

  }
}
