//
//  Settings.hpp
//  Metal-Tutorial
//
//  Created by Kian Marvi on 23/1/24.
//

#ifdef Settings_h
//#define Settings_h

// Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

#include <Windows.h>

// Include GLEW
#ifndef GLEW_STATIC
    #define GLEW_STATIC
    #include <GL/glew.h>
#endif

// Include GLFW
#include <GLFW/glfw3.h>

// #include ini.h
#include "../System/SimpleIni.h"

class CSettings : public CSingletonTemplate<CSettings>
{
    friend CSingletonTemplate<CSettings>;
public:
    enum AXIS
    {
        x = 0,
        y = 1,
        z = 2,
        NUM_AXIS
    };

    enum RESULTS
    {
        NEGATIVE = 0,
        POSITIVE = 1,
        INVALID = 2,
        NUM_RESULTS
    };

    // GLFW Information
    GLFWwindow* pWindow;

    // For storing the settings read in from .ini file
    CSimpleIniA cSimpleIniA;
    
    // Init this class instance
    void Init(void);

    // Load the .ini file
    bool LoadFile(std::string const& filename);

    // Save to the .ini file
    bool SaveFile(void);

    // Update the specifications of the map
    void UpdateSpecifications(void);

protected:
    // Constructor
    CSettings(void);

    // Destructor
    virtual ~CSettings(void);

    // filename of the ini file
    std::string filename;

    // Boolean flag to indicate if the .ini file was successfully loaded in
    bool bFileLoaded;
};

#endif /* Settings_hpp */
