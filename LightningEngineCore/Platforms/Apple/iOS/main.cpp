//
//  main.cpp
//  LightningEngine-iOS
//
//  Created by Kian Marvi on 8/4/24.
//

#include "IOSApplication.h"

int main(int argc, char* argv[])
{
    IOSApplication del;
    
    NS::AutoreleasePool* autoReleasePool = NS::AutoreleasePool::alloc()->init();
    UI::ApplicationMain(argc, argv, &del);
    autoReleasePool->release();
    
    return 0;
}
