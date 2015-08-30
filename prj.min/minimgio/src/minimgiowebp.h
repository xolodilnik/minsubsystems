#pragma once

#ifndef MINIMGIO_MINIMGIOWEBP_INCLUDED
#define MINIMGIO_MINIMGIOWEBP_INCLUDED

#include <minimgio/minimgio.h>

int GetWebPPages(const char *pFileName);

int GetWebPPageName(char *pPageName,
                   int pageNameSize,
                   const char *pFileName,
                   int page);

int GetWebPPropsEx(MinImg      *pImg,
                   ExtImgProps *pProps,
                   const char  *pFileName);

int LoadWebP(const MinImg  *pImg,
             const char    *pFileName);

int SaveWebPEx(const char        *pFileName,
               const MinImg      *pImg,
               const ExtImgProps *pProps);

#endif // MINIMGIO_MINIMGIOWEBP_INCLUDED
