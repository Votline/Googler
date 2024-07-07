#include <memory>
#include <cstdlib>
#include <iostream>
#include <cctype>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

#include "error.cpp"

std::string urlEncode(const std::string& input){
	std::string encodingString = "";
	for(char c : input){
		if(isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~'){
			encodingString += c;
		}
		else{
			char hex[4];
			sprintf(hex, "%%%02X", static_cast<unsigned char>(c));
			encodingString += hex;
		}
	}
	return encodingString;
}

class DraggleBox : Fl_Box{
public:
	DraggleBox(int x, int y, int w, int h, const char* L =0)
	 : Fl_Box(x, y, w, h, L){
		this->box(FL_NO_BOX);
		this->labelcolor(FL_WHITE);
		this->color(FL_BACKGROUND_COLOR);
	}
	int handle(int event) override{
		static int startX, startY, winX, winY;
		switch(event){
			case FL_PUSH:
				if(Fl::event_button() == FL_LEFT_MOUSE){
					startX = Fl::event_x_root();
					startY = Fl::event_y_root();
					winX = window()->x();
					winY = window()->y();
					return 1;
				}
				break;
			case FL_DRAG:
				if(true){
					int dx = Fl::event_x_root() - startX;
					int dy = Fl::event_y_root() - startY;
					window()->position(dx + winX, dy + winY);
					return 1;
				}
					break;
			case FL_RELEASE:
				if(Fl::event_button() == FL_LEFT_MOUSE){
					return 1;
				}
				break;
		}
		return Fl_Box::handle(event);
	}
};


class EntryField : Fl_Input{
public:
	EntryField(int x, int y, int w, int h) : Fl_Input(x, y, w ,h){
		this->box(FL_FLAT_BOX);
		this->textcolor(FL_WHITE);
		this->color(fl_rgb_color(35, 35, 35));

	}

	int handle(int event) override{
		if(event == FL_KEYDOWN){
			if(Fl::event_key() == FL_Enter){
				std::string cursystem, command, url;
				std::string input = this->value();
				command =  "xdg-open ";
				cursystem = "unsupported system ";
				url = "https://www.google.com/search?q=" + urlEncode(input);
				#ifdef __linux__
					command = "xdg-open ";
					input = command + url;
					cursystem = "Linux";
				#elif __unix__
					command = "xdg-open ";
					input = command + url;
					cursystem = "Unix";
				#elif WIN32
					command = "start ";
					input = command + url;
					cursystem = "Windows"
				#elif  __APPLE__
					command = "open ";
					input = command + url;
					cursystem = "macOS";
				#endif
				int status = system(input.c_str());
				if(status != 0){
					ErrorWindow(command, cursystem);
				}
				return 1;
			}
		}
		return Fl_Input::handle(event);
	}
};

class TitleBar{
public:
        TitleBar(Fl_Window* parent) {
		titleName = std::make_unique<DraggleBox>(0, -2, 600, 20, "Googler");

		close_button = std::make_unique<Fl_Button>(590, 4, 10, 10, "X");
		close_button->box(FL_NO_BOX);
		close_button->clear_visible_focus();
		close_button->color(FL_BACKGROUND_COLOR);
		close_button->labelcolor(fl_rgb_color(180, 50, 50));
		close_button->callback(close_cb, parent);
	}
private:
	std::unique_ptr<DraggleBox> titleName;
	std::unique_ptr<Fl_Button> close_button;

	static void close_cb(Fl_Widget* widget, void* data) {
		Fl_Window* window = (Fl_Window*)data;
		window->hide();
	}
};


int main() {
	auto window = std::make_unique<Fl_Window>(600, 55, "Googler");
	window->clear_border(); window->color(fl_rgb_color(50, 50, 50));

	auto titleBar = std::make_unique<TitleBar>(window.get());
        auto entryField = std::make_unique<EntryField>(0, 17, 700, 40);

	int x,y,w,h;
	Fl::screen_xywh(x, y, w, h);

	window->position((w-window->w())/2, h - window->w() - 450);

	window->end();
	window->show();
	return Fl::run();
}

