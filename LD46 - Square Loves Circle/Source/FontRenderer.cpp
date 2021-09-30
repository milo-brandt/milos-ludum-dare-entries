//
//  FontRenderer.cpp
//  Dummy
//
//  Created by Milo Brandt on 4/19/20.
//  Copyright © 2020 Milo Brandt. All rights reserved.
//

#include "FontRenderer.hpp"

namespace FontRenderer{
    void generateDataToCenter(RenderingSegment& segment, std::string const& text, Vector2<float> center, float height, Vector<float, 4> const& color){
        float adjHeight = height/(openSans.maxHeight);
        float length = 0;
        for(char c : text){
            CharacterFormat const& format = openSans[c];
            length += adjHeight * format.width;
        }
        float offset = center[0] - length/2;
        float bottom = center[1] - height/2;
        for(char c : text){
            CharacterFormat const& format = openSans[c];
            float xLeft = offset + adjHeight*format.offsetX;
            float xRight = offset + adjHeight*(format.offsetX + format.rectWidth);
            float yTop = bottom + adjHeight*((int)openSans.maxHeight - format.offsetY);
            float yBottom = bottom + adjHeight*((int)openSans.maxHeight - format.offsetY - format.rectHeight);
            
            float texLeft = (float)format.rectX / openSans.totalWidth;
            float texRight = (float)(format.rectX + format.rectWidth) / openSans.totalWidth;
            float texTop = (float)format.rectY / openSans.totalHeight;
            float texBot = (float)(format.rectY + format.rectHeight) / openSans.totalHeight;

            segment.addRectangle({{xLeft, yBottom},{xRight, yTop}}, {{texLeft, texBot}, {texRight, texTop}}, color);
            
            offset += adjHeight * format.width;
        }
    }
}
