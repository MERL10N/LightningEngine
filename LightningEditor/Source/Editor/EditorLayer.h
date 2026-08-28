//
//  EditorLayer.hpp
//  LightningCore
//
//  Created by Kian Marvi on 8/2/25.
//

#ifndef EditorLayer_hpp
#define EditorLayer_hpp

#include <imgui.h>
#include <array>

class EditorLayer
{
public:
    EditorLayer();
    ~EditorLayer();
    
    void DrawStatsBar();
    void DrawMenuBar();
    
    //template<typename Derived>
   // void DrawContentBrowser(this Derived &&self) { self.DrawContentBrowserImpl(); }
    virtual void DrawContentBrowser() = 0;
    
    inline bool IsWireFrameEnabled() const { return b_EnableWireFrameMode; }

protected:
    std::array<ImTextureID, 4> m_Icons;
private:
    bool b_showAnotherWindow;
    bool b_EnableWireFrameMode;
};

#endif /* EditorLayer_hpp */
