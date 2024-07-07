#include <iostream>
#include <cstdlib>
#include <memory>
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Window.H>

class error_DraggleBox : Fl_Box{

public:
	error_DraggleBox(int x, int y, int w, int h, const char* L =0)
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

class error_TitleBar{
public:
        error_TitleBar(Fl_Window* parent) {
		titleName = std::make_unique<error_DraggleBox>(0, -1, parent->w(), 20, "Googler-Error");


		close_button = std::make_unique<Fl_Button>(175, 4, 10, 10, "X");
		close_button->box(FL_NO_BOX);
		close_button->clear_visible_focus();
		close_button->color(FL_BACKGROUND_COLOR);
		close_button->labelcolor(fl_rgb_color(180, 50, 50));
		close_button->callback(close_cb, parent);

	}

private:
	std::unique_ptr<error_DraggleBox> titleName;
	std::unique_ptr<Fl_Button> close_button;

	static void close_cb(Fl_Widget* widget, void* data) {
		Fl_Window* window = (Fl_Window*)data;
		window->hide();
	}
};

class ErrorBox : Fl_Box{
public:
	ErrorBox(int x, int y, int w, int h, const char* L) : Fl_Box(x, y, w, h, L){
		this->box(FL_FLAT_BOX);
		this->labelcolor(FL_WHITE);
		this->color(fl_rgb_color(40, 40, 40));
	}

	void draw() override{
		Fl_Box::draw();
	}
};

int ErrorWindow(std::string errorCommand, std::string system){
	auto window = std::make_unique<Fl_Window>(185, 160, "Googler-Error");
	window->clear_border();
	window->color(fl_rgb_color(50,50,50));

	std::string errorMessage = "I wanted to use \n" +  errorCommand + "\n  on your operating system  \n" + system + "\n but something went wrong.";
	const char *errorMessageCTR = errorMessage.c_str();
	auto errorBox = std::make_unique<ErrorBox>(0, 20, 185, 140, errorMessageCTR);
	auto error_titleBar = std::make_unique<error_TitleBar>(window.get());


	window->end();
	window->show();
	return Fl::run();

}

