/********************************************************************************** 
 
 Copyright (C) 2012 Syed Reza Ali (www.syedrezaali.com)
 
 Permission is hereby granted, free of charge, to any person obtaining a copy of
 this software and associated documentation files (the "Software"), to deal in
 the Software without restriction, including without limitation the rights to
 use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the Software is furnished to do
 so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
 **********************************************************************************/

#ifndef CIUI_MINIMAL_SLIDER
#define CIUI_MINIMAL_SLIDER

#include "ciUISlider.h"

class ciUIMinimalSlider : public ciUISlider
{
public:
    ciUIMinimalSlider(float x, float y, float w, float h, float _min, float _max, float _value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;                                         
        rect = new ciUIRectangle(x,y,w,h); 
        autoSize = false; 
        init(w, h, _min, _max, &_value, _name, _size); 		
    }
    
    ciUIMinimalSlider(float w, float h, float _min, float _max, float _value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;                                         
        rect = new ciUIRectangle(0,0,w,h); 
        autoSize = false; 
        init(w, h, _min, _max, &_value, _name, _size); 
    }    
    
    ciUIMinimalSlider(float w, float _min, float _max, float _value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;                                                 
        rect = new ciUIRectangle(0,0,w,0); 
        autoSize = true; 
        init(w, 0, _min, _max, &_value, _name, _size); 
    } 
    
    ciUIMinimalSlider(float x, float y, float w, float h, float _min, float _max, float *_value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;                                         
        rect = new ciUIRectangle(x,y,w,h); 
        autoSize = false; 
        init(w, h, _min, _max, _value, _name, _size); 		
    }
    
    ciUIMinimalSlider(float w, float h, float _min, float _max, float *_value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = true;                                         
        rect = new ciUIRectangle(0,0,w,h); 
        autoSize = false; 
        init(w, h, _min, _max, _value, _name, _size); 
    }        
    
    ciUIMinimalSlider(float w, float _min, float _max, float *_value, string _name, int _size = CI_UI_FONT_SMALL)
    {
        useReference = false;                                                 
        rect = new ciUIRectangle(0,0,w,0); 
        autoSize = true; 
        init(w, 0, _min, _max, _value, _name, _size); 
    }     
    
    void init(float w, float h, float _min, float _max, float *_value, string _name, int _size)
    {
        name = _name; 				
        kind = CI_UI_WIDGET_MINIMALSLIDER;
        
		paddedRect = new ciUIRectangle(-padding, -padding, w+padding*2.0, h+padding);
		paddedRect->setParent(rect);     
        
        draw_fill = true; 
        
        value = *_value;                                               //the widget's value
        
        if(useReference)
        {
            valueRef = _value; 
        }
        else
        {
            valueRef = new float(); 
            *valueRef = value; 
        }
        
		max = _max; 
		min = _min; 
        labelPrecision = 2;
        
		if(value > max)
		{
			value = max; 
		}
		if(value < min)
		{
			value = min; 
		}
		
		value = cinder::lmap<float>(value, min, max, 0.0, 1.0); 
        
        label = new ciUILabel(padding,h*.5,(name+" LABEL"), name, _size); 	        
        label->setDrawBack(true);         
		label->setParent(label); 
		label->setRectParent(rect); 	
        label->setEmbedded(true);        
        increment = .10;         
    }
    
    virtual void drawFill()
    {
        if(draw_fill)
        {			
            cinder::gl::color(color_fill);              
            cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth()*value, rect->getY()+rect->getHeight()));
        }
    }
    
    virtual void drawFillHighlight()
    {
        if(draw_fill_highlight)
        {
            cinder::gl::color(color_fill_highlight);              
            cinder::gl::drawSolidRect(Rectf(rect->getX(), rect->getY(), rect->getX()+rect->getWidth()*value, rect->getY()+rect->getHeight()));                                 
            label->drawString(rect->getX()+rect->getWidth()+padding, label->getRect()->getY()+padding, numToString(getScaledValue(), labelPrecision));         
        }        
    }
    
    virtual void drawOutlineHighlight()
    {
        if(draw_outline_highlight)
        {
            cinder::gl::color(color_outline_highlight);              
            rect->drawOutline();                      
            if(!draw_fill_highlight)
            {
                cinder::gl::color(label->getColorFill());                       
                label->drawString(rect->getX()+rect->getWidth()+padding, label->getRect()->getY()+padding, numToString(getScaledValue(), labelPrecision)); 
            }
        }
    }
    
    void input(float x, float y)
	{
        value = rect->percentInside(x, y).x; 
        
        if(value > 1.0)
        {
            value = 1.0;
        }
        else if(value < 0.0)
        {
            value = 0.0;
        }        
        updateValueRef();        
		updateLabel(); 
	}

    
    void updateLabel()
	{

	}
    
	void setParent(ciUIWidget *_parent)
	{
		parent = _parent;         
		ciUIRectangle *labelrect = label->getRect(); 
        while(labelrect->getWidth() > rect->getWidth())
        {
            string labelstring = label->getLabel();
            string::iterator it;
            it=labelstring.end();
            it--; 
            labelstring.erase (it); 
            label->setLabel(labelstring);
        }            
        if(autoSize || rect->getHeight() < label->getPaddingRect()->getHeight())
        {
            rect->setHeight(label->getPaddingRect()->getHeight());             
        }
        
		float h = labelrect->getHeight(); 
		float ph = rect->getHeight(); 	
        
		labelrect->setY(((int)(ph*.5 - h*.5)));
        labelrect->setX(padding);
		paddedRect->setHeight(rect->getHeight()+padding*2.0);  
        paddedRect->setWidth(rect->getWidth()+padding*2.0);
        updateValueRef();
        updateLabel(); 
	}	
    
protected:    //inherited: ciUIRectangle *rect; ciUIWidget *parent; 
    bool autoSize;     
}; 

#endif
