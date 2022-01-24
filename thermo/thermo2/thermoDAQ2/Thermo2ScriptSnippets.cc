/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2021 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include "Thermo2ScriptSnippets.h"

Thermo2ScriptSnippets::Thermo2ScriptSnippets(QWidget *parent)
  : QComboBox(parent)
{
  addItem("", "");
  addItem("comment block", "/*\n\n*/\n");

  addItem("for loop", "for (let i = 0; i < 5; i++) {\n\n}\n");
  addItem("for of loop", "let arr = [0, 1, 2];\nfor (let i of arr) {\n\n}\n");
  
  addItem("thermo - start measurement", "thermo.startMeasurement();\n");
  addItem("thermo - stop measurement", "thermo.stopMeasurement();\n");
  addItem("thermo - wait", "thermo.wait(seconds);\n");
  addItem("thermo - message", "thermo.message();\n");
  addItem("thermo - log", "thermo.log(\"\");\n");
  addItem("thermo - mattermost", "thermo.mattermost(\"\");\n");
  addItem("thermo - unix time", "utime = thermo.uTime();\n");
  addItem("thermo - execution time", "etime = thermo.uTime();\n");
  addItem("thermo - make unix time", "utime = thermo.mkUTime(year, month, day, hour, minute, second);\n");

  addItem("huber - set temperature set point", "huber.setTemperatureSetPoint(temperature);\n");
  addItem("huber - get temperature set point", "temperature = huber.getTemperatureSetPoint();\n");

  addItem("marta - set temperature set point", "marta.setTemperatureSetPoint(temperature);\n");
  addItem("marta - get temperature set point", "temperature = marta.getTemperatureSetPoint();\n");
  addItem("marta - set speed set point", "marta.setSpeedSetPoint(speed);\n");
  addItem("marta - get speed set point", "speed = marta.getSpeedSetPoint();\n");
  addItem("marta - set flow set point", "marta.setFlowSetPoint(flow);\n");
  addItem("marta - get flow set point", "flow = marta.getFlowSetPoint();\n");

  addItem("nge103b - get output state", "state = nge103b.getOutputState(channel);\n");
  addItem("nge103b - set output state", "nge103b.setOutputState(channel, state);\n");
  addItem("nge103b - get voltage", "nge103b.getVoltage(channel);\n");
  addItem("nge103b - set voltage", "nge103b.setVoltage(channel, voltage);\n");
  addItem("nge103b - get measured voltage", "nge103b.getMeasuredVoltage(channel);\n");
  addItem("nge103b - get current", "nge103b.getCurrent(channel);\n");
  addItem("nge103b - set current", "nge103b.setCurrent(channel, current);\n");
  addItem("nge103b - get measured current", "nge103b.getMeasuredCurrent(channel);\n");

  addItem("keithley - get sensor state", "state = keithley.state(sensor);\n");
  addItem("keithley - get temperature", "temperature = keithley.temperature(sensor);\n");
  addItem("keithley - get temperature as string", "temperature = keithley.temperatureAsString(sensor);\n");
  addItem("keithley - wait for stable temperatures", "keithley.waitForStableTemperature(sensors, timeout);\n");
  addItem("keithley - wait for temperature above", "keithley.waitForTemperatureAbove(sensor, temperature, timeout);\n");
  addItem("keithley - wait for temperature below", "keithley.waitForTemperatureBelow(sensor, temperature, timeout);\n");

  addItem("t2tp - get sink temperature", "temperature = t2tp.getSinkTemperature();\n");
  addItem("t2tp - set sink temperature", "t2tp.setSinkTemperature(temperature);\n");
  addItem("t2tp - get source power", "power = t2tp.getSourcePower();\n");
  addItem("t2tp - set source power", "t2tp.setSourcePower(power);\n");
  addItem("t2tp - get number of sensor in top block", "sensor = t2tp.getTopSensor(position);\n");
  addItem("t2tp - get number of sensor in bottom block", "sensor = t2tp.getBottomSensor(position);\n");
  addItem("t2tp - get temperature in top block", "temperature = t2tp.getTopTemperature(position);\n");
  addItem("t2tp - get temperature in bottom block", "temperature = t2tp.getBottomTemperature(position);\n");
}
