//
//  EditorLayer.hpp
//  LightningCore
//
//  Created by Kian Marvi on 8/2/25.
//

#ifndef EditorLayer_hpp
#define EditorLayer_hpp

class EditorLayer
{
public:
    EditorLayer();
    ~EditorLayer();
    
    void DrawStatsBar();
    void DrawMenuBar();
    void DrawContentBrowser();
    
private:
    bool b_showAnotherWindow;
};

#endif /* EditorLayer_hpp */
