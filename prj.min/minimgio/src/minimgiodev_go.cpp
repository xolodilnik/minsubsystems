#include <cstdio>
#include <cstring>

#include <minimgio/minimgio.h>
#include <minimgio/device.h>

#include <minimgapi/minimgapi.h>
#include <minimgapi/imgguard.hpp>

int main(int argc, char *argv[])
{
  char deviceName[4096] = {0};
  int res = GetDeviceList(DSS_CAMERA, deviceName, sizeof(deviceName));
  if (res < 0 || strlen(deviceName) <= 0)
  {
    printf("Cannot get camera devices or no devices installed");
    return -1;
  };

  if (strchr(deviceName, '\n') != NULL)
    *(strchr(deviceName, '\n')) = '\0';

  char deviceURI[4096] = {0};
  res = OpenStream(DSS_CAMERA, deviceName, deviceURI, sizeof(deviceURI));
  if (res < 0)
  {
    printf("Cannot open new device stream");
    return -1;
  }

  DECLARE_GUARDED_MINIMG(frameImage);
  if (GetMinImageFileProps(&frameImage, deviceURI) < 0)
  {
    CloseStream(deviceURI);
    printf("Cannot get frame properties");
    return -1;
  };

  if (AllocMinImage(&frameImage) < 0)
  {
    CloseStream(deviceURI);
    printf("Cannot allocate frame image");
    return -1;
  };

  for (size_t i = 0; i < 100; i++)
  {
    if (LoadMinImage(&frameImage, deviceURI) < 0)
    {
      printf("Cannot get frame #%d", i);
      continue;
    };

    char imageName[4096] = {0};
    sprintf(imageName, "%06d.jpg", i);
    if (SaveMinImage(imageName, &frameImage) < 0)
    {
      printf("Cannot save frame #%d to %s", i, imageName);
      continue;
    }
  }

  if (CloseStream(deviceURI) < 0)
  {
    printf("Error occurred while closing device stream");
    return -1;
  }

  return 0;
}