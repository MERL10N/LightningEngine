//
//  MacApplication.cpp
//  LightningCore
//
//  Created by Kian Marvi on 7/3/25.
//

#include "MacApplication.h"


MacApplication::MacApplication()
{
    m_SharedApplication = NS::Application::sharedApplication();
    m_SharedApplication->setDelegate(&macAppDelegate);
}

void MacApplication::Update()
{
    m_SharedApplication->run();
}

MacApplication::~MacApplication()
{
    m_SharedApplication->release();
}

