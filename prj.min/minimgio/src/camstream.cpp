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

#include <cassert>
#include <cstring>

#include <minutils/minerr.h>
#include <minutils/crossplat.h>

#include "camstream.h"

CamStream::CamStream(Device *pDevice) : pDevice(NULL)
{
  assert(pDevice != NULL);
  if (pDevice == NULL)
    return;

  if (pDevice->Take() < 0)
    return;

  this->pDevice = pDevice;
}

CamStream::~CamStream()
{
  assert(pDevice != NULL);
  if (pDevice == NULL)
    return;

  pDevice->Release();
}

int CamStream::PushImage(const MinImg * /* pImg */)
{
  return NO_SENSE;
}

int CamStream::QueryImage(const MinImg *pImg)
{
  assert(pDevice);
  if (pDevice == NULL)
    return INTERNAL_ERROR;

  return pDevice->QueryImage(pImg);
}

int CamStream::QueryImagePropsEx(MinImg *pImg, ExtImgProps *pProps)
{
  assert(pDevice);
  if (pDevice == NULL)
    return INTERNAL_ERROR;

  return pDevice->QueryImagePropsEx(pImg, pProps);
}

int CamStream::GetProperty(const char *pKey, char *pValue, int size)
{
  assert(pDevice);
  if (pDevice == NULL)
    return INTERNAL_ERROR;

  return pDevice->GetProperty(pKey, pValue, size);
}

int CamStream::SetProperty(const char *pKey, const char *pValue)
{
  assert(pDevice);
  if (pDevice == NULL)
    return INTERNAL_ERROR;

  return pDevice->SetProperty(pKey, pValue);
}
