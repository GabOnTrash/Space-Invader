class RectS  
{  
private:  
   float posX = 0;  
   float posY = 0;  
   float width = 0;  
   float height = 0;  

   float centerX = 0;  
   float centerY = 0;  

   float roundness = 0;  
   float segments = 0;  

   float min = 0;  
   float max = 0;  
   float* var = nullptr;  

   Color color = { 0, 0, 0, 0 };  

public:  
   RectS() {}  
   RectS(float x, float y, float width, float height, float roundness, float segments, float min, float max, float* var, Color color)  
       : posX(x), posY(y), width(width), height(height), roundness(roundness), segments(segments), min(min), max(max), var(var), color(color)  
   {  
   }  

   // Getters  
   float getPosX() const { return posX; }  
   float getPosY() const { return posY; }  
   float getWidth() const { return width; }  
   float getHeight() const { return height; }  

   float getCenterX() const { return centerX; }  
   float getCenterY() const { return centerY; }  

   float getRoundness() const { return roundness; }  
   float getSegments() const { return segments; }  

   float getMin() const { return min; }  
   float getMax() const { return max; }  
   float* getVar() const { return var; }  

   Color getColor() const { return color; }  

   // Setters  
   void setPosX(float value) { posX = value; }  
   void setPosY(float value) { posY = value; }  
   void setWidth(float value) { width = value; }  
   void setHeight(float value) { height = value; }  

   void setCenterX(float value) { centerX = value; }  
   void setCenterY(float value) { centerY = value; }  

   void setRoundness(float value) { roundness = value; }  
   void setSegments(float value) { segments = value; }  

   void setMin(float value) { min = value; }  
   void setMax(float value) { max = value; }  
   void setVar(float* ptr) { var = ptr; }  

   void setColor(Color newColor) { color = newColor; }  
};
