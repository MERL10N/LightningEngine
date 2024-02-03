//
//  Settings.cpp
//  Metal-Tutorial
//
//  Created by Kian Marvi on 23/1/24.
//

#ifdef Settings_h
#include "Settings.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CSettings::CSettings(void)
    : pWindow(nullptr)
    , bFileLoaded(false)
    , filename("")
{
}

/**
 @brief Destructor
 */
CSettings::~CSettings(void)
{
}

/**
 @brief Init this class instance
 */

void CSettings::Init(void)
{
    cSimpleIniA.SetUnicode();
}

/**
 @brief Load the .ini file
 @param filename A std::string const& variable which contains the name of the .ini file
 @return true if the file was successfully loaded, otherwise false
 */

bool CSettings::LoadFile(std::string const& filename)
{
    // first, create a file instance
    SI_Error rc = cSimpleIniA.LoadFile(filename.c_str());
    if (rc != SI_OK)
    {
        // Set default values since we cannot read in the ini file
        // To be added
        bFileLoaded = false;
        return false;
    }

    bFileLoaded = true;
    this->filename = filename;

    return true;
}

/**
 @brief Save to the .ini file
 @return true if the file was successfully saved, otherwise false
 */
bool CSettings::SaveFile(void)
{
    // If .ini file was not loaded, then don't save.
    if (!bFileLoaded)
        return false;

    // first, create a file instance
    SI_Error rc = cSimpleIniA.SaveFile(filename.c_str());
    if (rc != SI_OK)
    {
        // If unable to save the file, then give an error message
        cout << "Unable to save settings to " << filename << endl;
        return false;
    }

    return true;
}

/**
 @brief Update the specifications of the map
 */
void CSettings::UpdateSpecifications(void)
{
    int NUM_TILES_XAXIS = cSimpleIniA.GetIntValue("NumTiles", "NUM_TILES_XAXIS", 32);
    int NUM_TILES_YAXIS = cSimpleIniA.GetIntValue("NumTiles", "NUM_TILES_YAXIS", 24);

    float TILE_WIDTH = 2.0f / NUM_TILES_XAXIS;    // 0.0625f;
    float TILE_HEIGHT = 2.0f / NUM_TILES_YAXIS;    // 0.08333f;

    cSimpleIniA.SetValue("TileSize", "TILE_WIDTH", (std::to_string(TILE_WIDTH)).c_str());
    cSimpleIniA.SetValue("TileSize", "TILE_HEIGHT", (std::to_string(TILE_HEIGHT)).c_str());
}
#endif
