
class HMInterfaceButton {
	
	public:
	
		uint16_t x;
		uint16_t y;
		uint16_t w;
		uint16_t h;
		String caption;
    String img_name;
    uint16_t img_x;
    uint16_t img_y;
    
		bool large = false;
  
		uint16_t backColor = 0x0000; 
		uint16_t foreColor = 0xFFFF;
  
		uint16_t invBackColor = 0xFFFF;
		uint16_t invForeColor = 0x0000;
		
		HMInterfaceButton(){}
		
		HMInterfaceButton(uint16_t _x, uint16_t _y,uint16_t _w,uint16_t _h,String _caption)
		{
				x = _x; y = _y;w = _w; h = _h;caption = _caption;	
		}

		HMInterfaceButton(uint16_t _x, uint16_t _y,uint16_t _w,uint16_t _h,String _caption,bool _large,
			uint16_t _backColor, uint16_t _foreColor, uint16_t _invBackColor ,uint16_t _invForeColor)
		{
				x = _x; y = _y;w = _w; h = _h;caption = _caption;
				large = _large;
				backColor = _backColor;
				foreColor = _foreColor;
				invBackColor = _invBackColor;
				invForeColor = _invForeColor;	
		}
		
		
		HMInterfaceButton(uint16_t _x, uint16_t _y,uint16_t _w,uint16_t _h,String _caption,bool _large)
		{
				x = _x; y = _y;w = _w; h = _h;caption = _caption;
				large = _large;
		}

    void setImage(String _imgName, uint16_t _xOffset, uint16_t _yOffset) {
      img_name = _imgName;
      img_x = _xOffset;
      img_y = _yOffset;
    }
  
 };

