//
//  MacApplication.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/3/25.
//

#include "MacApplication.h"


MacApplication::MacApplication(unsigned int p_Width, unsigned int p_Height, const char* p_Title)
: macAppDelegate(p_Width, p_Height, p_Title),
  m_SharedApplication(NS::Application::sharedApplication())
{
   macAppDelegate.SetPreferredFramesPerSecond(120.f);
   m_SharedApplication->setDelegate(&macAppDelegate);
}


void MacApplication::Update(float p_DeltaTime)
{
    m_SharedApplication->run();
}


MacApplication::~MacApplication()
{
    m_SharedApplication->release();
}


