/*

Copyright (c) 2011, Smart Engines Limited. All rights reserved.

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY COPYRIGHT HOLDERS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of copyright holders.

*/

#pragma once
#ifndef DSHOWDEVICE_H_INCLUDE
#define DSHOWDEVICE_H_INCLUDE

#include "stream.h"

#include <map>
#include <string>

#include <dshow.h>
#include <comdef.h>
#include <strmif.h>
#include <streams.h>

_COM_SMARTPTR_TYPEDEF(ICaptureGraphBuilder2, __uuidof(ICaptureGraphBuilder2));
_COM_SMARTPTR_TYPEDEF(IGraphBuilder, __uuidof(IFilterGraph));
_COM_SMARTPTR_TYPEDEF(IMediaControl, __uuidof(IMediaControl));
_COM_SMARTPTR_TYPEDEF(IMediaEvent, __uuidof(IMediaEvent));
_COM_SMARTPTR_TYPEDEF(IBaseFilter, __uuidof(IBaseFilter));
_COM_SMARTPTR_TYPEDEF(IPin, __uuidof(IPin));

class DShowDevice : public Device
{
public:
  static DShowDevice * Instance(const char *pDeviceName);
  virtual ~DShowDevice();

private:
  DShowDevice(const char *pDeviceName);
  DShowDevice(const DShowDevice &);
  DShowDevice &operator=(const DShowDevice &);

public:
  int Take();
  int Release();
  int PushImage(const MinImg *pImg);
  int QueryImage(const MinImg *pImg);
  int QueryImagePropsEx(MinImg *pImg, ExtImgProps *pProps);
  int GetProperty(const char *pKey, char *pValue, int size);
  int SetProperty(const char *pKey, const char *pValue);

private:
  int createGraphBuilder();
  int setupGrabberFilter();
  int setupCaptureFilter();
  int connectFilterPins();

private:
  class SampleRenderer : public CBaseRenderer
  {
  public:
    SampleRenderer(HRESULT *pHr);
    virtual ~SampleRenderer();

  public:
    HRESULT DoRenderSample(IMediaSample *pMediaSample);
    HRESULT CheckMediaType(const CMediaType *pMediaType);

  public:
    CCritSec ccLock;
    unsigned int frameSize;
    unsigned char *pFrameData;
    BITMAPINFOHEADER bitmap;
  };

private:
  HRESULT          hr; 
  int              counter;
  std::string      devName;
  SampleRenderer   grabber;
  std::string      videoPath;

  ICaptureGraphBuilder2Ptr pCaptureBuilder;
  IGraphBuilderPtr         pGraphBuilder;
  IMediaControlPtr         pMediaControl;
  IMediaEventPtr           pMediaEvent;

  IBaseFilterPtr   pSourceFilter;
  IBaseFilterPtr   pGrabberFilter;
  IPinPtr          pOutPin;
};

#endif  // DSHOWDEVICE_H_INCLUDE