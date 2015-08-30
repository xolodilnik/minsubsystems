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
#pragma warning (disable : 4995)

#include "dshowutils.h"
#include "dshowdevice.h"
#include "utils.h"

#include <cmath>
#include <cstring>
#include <cassert>
#include <algorithm>

#include <minutils/minerr.h>
#include <minutils/crossplat.h>
#include <minimgio/device.h>

static int checkImageTypes(const MinImg *pImg, const BITMAPINFOHEADER &bmp)
{
  if (pImg->format != FMT_UINT || pImg->channelDepth != 1)
    return BAD_ARGS;

  if (pImg->channels != std::max<int>(1, bmp.biBitCount / 8))
    return BAD_ARGS;
  if (pImg->height != std::abs(bmp.biHeight))
    return BAD_ARGS;
  if (pImg->width != bmp.biWidth)
    return BAD_ARGS;

  return NO_ERRORS;
}

DShowDevice::SampleRenderer::SampleRenderer(HRESULT *pHr) :
CBaseRenderer(GUID_NULL, NAME(""), NULL, pHr)
{
  memset(&bitmap, 0x00, sizeof(bitmap));
  pFrameData = NULL;
  frameSize = 0;
}

DShowDevice::SampleRenderer::~SampleRenderer()
{
  delete[] pFrameData;
}

HRESULT DShowDevice::SampleRenderer::DoRenderSample(IMediaSample *pMediaSample)
{
  CheckPointer(pMediaSample, E_POINTER);
  CAutoLock lock(&ccLock);

  BYTE *pBuffer = NULL;
  HRESULT hr = pMediaSample->GetPointer(&pBuffer);
  if (FAILED(hr))
    return hr;

  unsigned int bufferSize = pMediaSample->GetActualDataLength();
  assert(frameSize >= bufferSize);

  memcpy(pFrameData, pBuffer, frameSize);
  return S_OK;
}

HRESULT DShowDevice::SampleRenderer::CheckMediaType(const CMediaType *pMt)
{
  CheckPointer(pMt,E_POINTER);
  CAutoLock lock(&ccLock);

  if(*pMt->Type() != MEDIATYPE_Video)
    return VFW_E_INVALID_MEDIA_TYPE;

  GUID subType = *pMt->Subtype();
  if(subType != MEDIASUBTYPE_RGB24 && subType != MEDIASUBTYPE_RGB32)
    return VFW_E_INVALID_MEDIA_TYPE;

  if (pMt->cbFormat == sizeof(VIDEOINFOHEADER))
  {
    VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER *)pMt->pbFormat;
    if (pVih != NULL)
      memcpy(&bitmap, &pVih->bmiHeader, sizeof(BITMAPINFOHEADER));
  }
  else if (pMt->cbFormat == sizeof(VIDEOINFOHEADER2))
  {
    VIDEOINFOHEADER2 *pVih = (VIDEOINFOHEADER2 *)pMt->pbFormat;
    if (pVih != NULL)
      memcpy(&bitmap, &pVih->bmiHeader, sizeof(BITMAPINFOHEADER));  
  }
  else if (pMt->cbFormat > sizeof(VIDEOINFOHEADER))
  {
    VIDEOINFOHEADER *pVih = (VIDEOINFOHEADER *)pMt->pbFormat;
    if (pVih != NULL)
      memcpy(&bitmap, &pVih->bmiHeader, sizeof(BITMAPINFOHEADER));
  }
  else
  {
    assert(false);
    return VFW_E_INVALID_MEDIA_TYPE;
  }

  delete[] pFrameData;
  frameSize = pMt->GetSampleSize();
  pFrameData = new unsigned char[frameSize];

  return S_OK;
}

DShowDevice * DShowDevice::Instance(const char *pDeviceName)
{
  std::string deviceNameList;
  HRESULT hr = GetCaptureDeviceList(deviceNameList);

  if (FAILED(hr) || pDeviceName == NULL)
    return NULL;

  if (deviceNameList.find(pDeviceName) == std::string::npos)
    return NULL;

  static std::map<std::string, DShowDevice *> devices;
  DShowDevice *pDevice = devices[pDeviceName];
  
  if (pDevice == NULL)
  {
    pDevice = new DShowDevice(pDeviceName);
    devices.insert(std::make_pair(pDeviceName, pDevice));
  }

  return pDevice;
}

DShowDevice::DShowDevice(const char *pDeviceName) :
counter(0), hr(S_OK), devName(pDeviceName), grabber(&hr)
{
  pGraphBuilder  = NULL;
  pMediaControl  = NULL;
  pMediaEvent    = NULL;

  pSourceFilter   = NULL;
  pGrabberFilter  = NULL;
  pOutPin = NULL;
}

DShowDevice::~DShowDevice()
{
  pGraphBuilder  = NULL;
  pMediaControl  = NULL;
  pMediaEvent    = NULL;

  pSourceFilter   = NULL;
  pGrabberFilter  = NULL;
  pOutPin = NULL;
}

int DShowDevice::Take()
{
  if (counter == 0)
  {
    PROPAGATE_ERROR(createGraphBuilder());
    PROPAGATE_ERROR(setupCaptureFilter());
    PROPAGATE_ERROR(setupGrabberFilter());
    PROPAGATE_ERROR(connectFilterPins());
  }

  counter++;
  return NO_ERRORS;
}

int DShowDevice::Release()
{
  counter--;

  if (counter == 0)
  {

    if (pMediaControl) pMediaControl->Stop();
    if (pOutPin) hr = pOutPin->Disconnect();

    if (pGraphBuilder) pGraphBuilder.Release();
    if (pMediaControl) pMediaControl.Release();
    if (pMediaEvent) pMediaEvent.Release();

    if (pSourceFilter) pSourceFilter.Release();
    if (pGrabberFilter) pGrabberFilter.Detach();

    if (pOutPin) pOutPin.Release();
  }

  return NO_ERRORS;
}

int DShowDevice::PushImage(const MinImg *pImg)
{
  return NO_SENSE;
}

int DShowDevice::QueryImage(const MinImg *pImg)
{
  CAutoLock(&grabber.ccLock);

  if (counter == 0)
    return BAD_STATE;
  if (pImg == NULL || pImg->pScan0 == NULL)
    return BAD_ARGS;
  if (pImg->format != FMT_UINT || pImg->channelDepth != 1)
    return BAD_ARGS;

  const int ch = grabber.bitmap.biBitCount / 8;
  const int wd = grabber.bitmap.biWidth;
  const int ht = std::abs(grabber.bitmap.biHeight);

  if (pImg->width != wd || pImg->height != ht)
    return BAD_ARGS;
  if (pImg->channels != ch)
    return BAD_ARGS;

  const unsigned char *pData = grabber.pFrameData;
  int stride = (wd * ch + 3) & ~3;

  if (grabber.bitmap.biHeight > 0)
  {
    pData += (grabber.bitmap.biHeight - 1) * stride;
    stride = -stride;
  }

  for (int y = 0; y < ht; y++)
  {
    unsigned const char *pSrcLine = pData + y * stride;
    uint8_t *pDstLine = pImg->pScan0 + y * pImg->stride;
    memcpy(pDstLine, pSrcLine, wd * ch);
  }

  return NO_ERRORS;
}

int DShowDevice::QueryImagePropsEx(MinImg *pImg, ExtImgProps *pProps)
{
  CAutoLock(&grabber.ccLock);
  if (counter == 0) return BAD_STATE;

  if (pImg == NULL)
    return BAD_ARGS;

  pImg->width = grabber.bitmap.biWidth;
  pImg->height = std::abs(grabber.bitmap.biHeight);
  pImg->channels = std::max<int>(1, grabber.bitmap.biBitCount / 8);
  pImg->channelDepth = grabber.bitmap.biBitCount > 1 ? 1 : 0;
  pImg->format = FMT_UINT;

  return NO_ERRORS;
}

int DShowDevice::GetProperty(const char *pKey, char *pValue, int size)
{
  if (pKey == NULL || pValue == NULL)
    return BAD_ARGS;

  try
  {
    HRESULT hr = S_OK;

    if (strcmp(pKey, SP_GAIN) == 0)
    {
      int gain = 0;
      hr = GetGainCam(pSourceFilter, &gain, NULL);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, "%d", gain);
      return NO_ERRORS;
    }
    
    if (strcmp(pKey, SP_GAINAUTO) == 0)
    {
      bool isGainAuto = false;
      hr = GetGainCam(pSourceFilter, NULL, &isGainAuto);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, isGainAuto ? "true" : "false");
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_EXPOSURE) == 0)
    {
      double exposure = 0;
      hr = GetExposureCam(pSourceFilter, &exposure, NULL);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, "%f", exposure);
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_EXPOSUREAUTO) == 0)
    {
      bool isExposureAuto = false;
      hr = GetExposureCam(pSourceFilter, NULL, &isExposureAuto);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, isExposureAuto ? "true" : "false");
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_FOCUS) == 0)
    {
      double focus = 0;
      hr = GetFocusCam(pSourceFilter, &focus, NULL);
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, "%f", focus);
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_FOCUSAUTO) == 0)
    {
      bool isFocusAuto = false;
      hr = GetFocusCam(pSourceFilter, NULL, &isFocusAuto );
      if (FAILED(hr)) throw hr;

      snprintf(pValue, size, isFocusAuto ? "true" : "false");
      return NO_ERRORS;
    }

    int width = 0, height = 0, frameRate = 0;
    hr = GetFormatCam(pOutPin, &width, &height, &frameRate);
    if (FAILED(hr)) throw hr;

    if (strcmp(pKey, SP_FRAMERATE) == 0)
      snprintf(pValue, size, "%d", frameRate);
    else if (strcmp(pKey, SP_FRAMESIZE) == 0)
      snprintf(pValue, size, "%dx%d", width, height);
    else
      return NO_SENSE;

    return NO_ERRORS;
  }
  catch (HRESULT hr)
  {
    if (hr == VFW_E_NO_INTERFACE)
      return NOT_SUPPORTED;
    else
      return INTERNAL_ERROR;
  }
}

int DShowDevice::SetProperty(const char *pKey, const char *pValue)
{
  if (pKey == NULL || pValue == NULL)
    return BAD_ARGS;

  try
  {
    HRESULT hr = S_OK;

    if (strcmp(pKey, SP_GAIN) == 0)
    {
      int gain = atoi(pValue);
      hr = SetGainCam(pSourceFilter, gain, false);
      if (FAILED(hr)) throw hr;
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_GAINAUTO) == 0)
    {
      bool isGainAuto = atob(pValue);
      hr = SetGainCam(pSourceFilter, 0, isGainAuto);
      if (FAILED(hr)) throw hr;
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_EXPOSURE) == 0)
    {
      double exposure = atof(pValue);
      hr = SetExposureCam(pSourceFilter, exposure, false);
      if (FAILED(hr))
        throw hr;
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_EXPOSUREAUTO) == 0)
    {
      bool isExposureAuto = atob(pValue);
      hr = SetExposureCam(pSourceFilter, 1.0, isExposureAuto);
      if (FAILED(hr))
        throw hr;
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_FOCUS) == 0)
    {
      double focus = atof(pValue);
      hr = SetFocusCam(pSourceFilter, focus, false);
      if (FAILED(hr))
        throw hr;
      return NO_ERRORS;
    }

    if (strcmp(pKey, SP_FOCUSAUTO) == 0)
    {
      bool isFocusAuto = atob(pValue);
      hr = SetFocusCam(pSourceFilter, 1.0, isFocusAuto);
      if (FAILED(hr))
        throw hr;
      return NO_ERRORS;
    }

    if (counter > 1)
      return BAD_STATE;

    int width = 0, height = 0, frameRate = 0;
    hr = GetFormatCam(pOutPin, &width, &height, &frameRate);
    if (FAILED(hr))
      throw hr;

    if (strcmp(pKey, SP_FRAMERATE) == 0)
    {
      frameRate = atoi(pValue);
    }
    else if (strcmp(pKey, SP_FRAMESIZE) == 0)
    {
      sscanf(pValue, "%dx%d", &width, &height);
    }
    else if (strcmp(pKey, SP_VIDEOFILEPATH) == 0)
    {
      videoPath = pValue;
    }
    else
    {
      return NO_SENSE;
    }

    PROPAGATE_ERROR(Release());
    PROPAGATE_ERROR(createGraphBuilder());
    PROPAGATE_ERROR(setupGrabberFilter());
    PROPAGATE_ERROR(setupCaptureFilter());

    hr = SetFormatCam(pOutPin, width, height, frameRate);
    if (FAILED(hr))
      throw hr;
    
    PROPAGATE_ERROR(connectFilterPins());
    counter++;

    return NO_ERRORS;
  }
  catch (HRESULT hr)
  {
    if (hr == VFW_E_NO_INTERFACE)
      return NOT_SUPPORTED;
    else
      return INTERNAL_ERROR;
  }}

int DShowDevice::createGraphBuilder()
{
  HRESULT hr = pCaptureBuilder.CreateInstance(CLSID_CaptureGraphBuilder2);
  if (FAILED(hr))
    return INTERNAL_ERROR;

  hr = pGraphBuilder.CreateInstance(CLSID_FilterGraph);
  if (FAILED(hr))
    return INTERNAL_ERROR;

  hr = pCaptureBuilder->SetFiltergraph(pGraphBuilder.GetInterfacePtr());
  if (FAILED(hr))
    return INTERNAL_ERROR;

  hr = pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&pMediaControl.GetInterfacePtr());
  if (FAILED(hr))
    return INTERNAL_ERROR;

  hr = pGraphBuilder->QueryInterface(IID_IMediaEvent, (void **)&pMediaEvent.GetInterfacePtr());
  if (FAILED(hr))
    return INTERNAL_ERROR;

  return NO_ERRORS;
}

int DShowDevice::setupCaptureFilter()
{
  if (pGraphBuilder == NULL)
    return INTERNAL_ERROR;

  HRESULT hr = FindCaptureDevice(pSourceFilter.GetInterfacePtr(), devName.c_str());
  if (FAILED(hr))
    return INTERNAL_ERROR;

  hr = GetCapturePin(pSourceFilter, pOutPin.GetInterfacePtr());
  if (FAILED(hr))
    return INTERNAL_ERROR;


  hr = pGraphBuilder->AddFilter(pSourceFilter, L"Source");
  if (FAILED(hr))
    return INTERNAL_ERROR;

  return NO_ERRORS;
}

int DShowDevice::setupGrabberFilter()
{
  if (pGraphBuilder == NULL)
    return INTERNAL_ERROR;

  HRESULT hr = grabber.QueryInterface(IID_IBaseFilter, (void**)&pGrabberFilter.GetInterfacePtr());
  if (FAILED(hr))
    return INTERNAL_ERROR;

  hr = pGraphBuilder->AddFilter(pGrabberFilter.GetInterfacePtr(), L"Grabber");
  if (FAILED(hr))
    return INTERNAL_ERROR;

  return NO_ERRORS;
}

int DShowDevice::connectFilterPins()
{
  HRESULT hr = pCaptureBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, 
    &MEDIATYPE_Video, pSourceFilter, NULL, pGrabberFilter);
  if (FAILED(hr))
    return INTERNAL_ERROR;

  if (videoPath.length() > 0)
  {
    IBaseFilter *pMux = NULL;
    hr = pCaptureBuilder->SetOutputFileName(&MEDIASUBTYPE_Avi,
      _bstr_t(videoPath.c_str()), &pMux, NULL);

    hr = pCaptureBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, 
      &MEDIATYPE_Video, pSourceFilter, 0, pMux);

    pMux->Release();
  }

  hr = pMediaControl->Run();
  if (FAILED(hr))
    return INTERNAL_ERROR;

  return NO_ERRORS;
}