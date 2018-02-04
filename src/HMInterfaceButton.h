
class Point {
public:
	Point(void) : x(0), y(0) {}
	Point(int16_t x, int16_t y ) : x(x), y(y) {}
	int16_t x, y;
};

class Rect {
public:
	Rect(void) : x(0), y(0), w(0),h(0) {}
	Rect(int16_t x, int16_t y, int16_t w, int16_t h) : x(x), y(y), w(w), h(h) {}
	int16_t x, y, w,h;
};

class Size {
public:
	Size(void) : w(0), h(0) {}
	Size(int16_t w, int16_t h ) : w(w), h(h) {}
	int16_t w, h;
};

class HMInterfaceButton {

	public:

		Rect frame;
		String caption;
    String img_name;

		Point imgPosition;
    Point textOffset;

		bool large = false;

		uint16_t backColor = 0x0000;
		uint16_t foreColor = 0xFFFF;

		uint16_t invBackColor = 0xFFFF;
		uint16_t invForeColor = 0x0000;

		HMInterfaceButton(){}

		HMInterfaceButton(Rect _frame,String _caption)
		{
				frame = _frame;
				caption = _caption;
		}

		HMInterfaceButton(Rect _frame,String _caption,bool _large,
			uint16_t _backColor, uint16_t _foreColor, uint16_t _invBackColor ,uint16_t _invForeColor)
		{
				frame = _frame;
				caption = _caption;
				large = _large;
				backColor = _backColor;
				foreColor = _foreColor;
				invBackColor = _invBackColor;
				invForeColor = _invForeColor;
		}


		HMInterfaceButton(Rect _frame,String _caption,bool _large)
		{
				frame = _frame;
				caption = _caption;
				large = _large;
		}

    void setImage(String _imgName, Point offset) {
      img_name = _imgName;
			imgPosition = offset;
    }

 };
