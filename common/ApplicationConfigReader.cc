/////////////////////////////////////////////////////////////////////////////////
//                                                                             //
//               Copyright (C) 2011-2017 - The DESY CMS Group                  //
//                           All rights reserved                               //
//                                                                             //
//      The CMStkModLab source code is licensed under the GNU GPL v3.0.        //
//      You have the right to modify and/or redistribute this source code      //
//      under the terms specified in the license, which may be found online    //
//      at http://www.gnu.org/licenses or at License.txt.                      //
//                                                                             //
/////////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <sstream>

#include <QMessageBox>

#include "ApplicationConfigReader.h"

ApplicationConfigReader::ApplicationConfigReader( const std::string & inputFileName )
{
  inputFileName_ = std::string( inputFileName );
}

ApplicationConfigReader::~ApplicationConfigReader()
{
}

void ApplicationConfigReader::fill(std::multimap<std::string,std::string> &keyvalueMap)
{
  std::ifstream file( inputFileName_.c_str(), std::ios::in );

  if(!file.good())
  {
    std::cerr << " [ApplicationConfigReader::openAndCheckFile] ** ERROR: failed to open file: " << inputFileName_ << "." << std::endl;

    QMessageBox::critical(0,
      tr("[ApplicationConfigReader::fill]"),
      QString("Failed to open configuration file: \"%1\". No chance!").arg(QString(inputFileName_.c_str())),
      QMessageBox::Abort
    );

    throw; // must abort
  }

  std::string Key;
  std::string Value;
  std::string buffer;

//  bool inside_str = false;

  while(std::getline(file, buffer))
  {
    if(buffer[0] == '"'){ continue; }

    while(buffer[0] == ' '){ buffer = buffer.substr(1, buffer.length()); }

    if (buffer[0] == '\0'){ continue; }
    if (buffer[0] == '#'){ continue; }

    std::istringstream iss(buffer.c_str(), std::istringstream::in);
    iss >> Key;
    while (iss >> Value) {
    	keyvalueMap.insert(std::make_pair(Key, Value));
    }
  }
  
  file.close();
}
