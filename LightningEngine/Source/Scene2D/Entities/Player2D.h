//
//  Player2D.hpp
//  LightningEngine
//
//  Created by Kian Marvi on 3/2/24.
//

#ifndef Player2D_h
#define Player2D_h

#include <DesignPatterns/SingletonTemplate.h>

class CPlayer2D : public CSingletonTemplate<CPlayer2D>
{
    friend CSingletonTemplate<CPlayer2D>;
    
    // Init
    bool Init(void);
    
    // PreRender
    void PreRender(void);

    // Render
    void Render(void);

    // PostRender
    void PostRender(void);
    
protected:
    // Constructor
    CPlayer2D(void);

    // Destructor
    virtual ~CPlayer2D(void);

};

#endif /* Player2D_hpp */
