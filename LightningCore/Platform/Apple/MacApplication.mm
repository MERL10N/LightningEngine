//
//  MacApplication.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/3/25.
//

#include "MacApplication.h"


MacApplication::MacApplication(unsigned int p_Width, unsigned int p_Height, const char* p_Title)
: macAppDelegate(p_Width, p_Height, p_Title)
{
}


void MacApplication::Update(float p_DeltaTime)
{
    macAppDelegate.Update();
}


MacApplication::~MacApplication()
{
}


