#ifndef AVFOUNDATIONUTILS_H_INCLUDED
#define AVFOUNDATIONUTILS_H_INCLUDED

#import <Foundation/Foundation.h>

class ScopedAutoreleasePool
{
private:
  NSAutoreleasePool *pool;
public:
  ScopedAutoreleasePool() : pool([[NSAutoreleasePool alloc] init]) {}
  ~ScopedAutoreleasePool() { [pool drain]; }
};

#endif // AVFOUNDATIONUTILS_H_INCLUDED