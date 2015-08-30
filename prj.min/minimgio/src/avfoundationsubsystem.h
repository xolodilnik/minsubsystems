#pragma once
#ifndef AVFOUNDATIONSUBSYSTEM_H_INCLUDED
#define AVFOUNDATIONSUBSYSTEM_H_INCLUDED

#include "subsystem.h"
#include "stream.h"

class AVFoundationSubSystem : public SubSystem
{
public:
  static SubSystem * Instance();
  int GetDeviceNameList(char *pDeviceNames, int size) const;
  int OpenStream(const char *pDeviceName, char *pURI, int size);

private:
  AVFoundationSubSystem();
  AVFoundationSubSystem(const AVFoundationSubSystem &);
  AVFoundationSubSystem &operator=(const AVFoundationSubSystem &);
};

#endif // AVFOUNDATIONSUBSYSTEM_H_INCLUDED