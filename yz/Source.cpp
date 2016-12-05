#include <Windows.h>
#include "yz/decl_begin.hpp"
yz_declare_with_members_begin(button, SpecialButton)
int test;
void test_func() { MessageBoxA(0, "a", "a", 0); }
yz_declare_with_members_end;
#include "yz/decl_end.hpp"

#include "yz/ui_begin.hpp"



/*yz_define_with_members(button, SpecialButton)
{
	self.text = "SpecialButton!";
	self.test = 1;
};*/

yz_object(window, main_form)
{
	self.title = "Main Form";

	yz_property int direction = 1;
	yz_property auto test = [&]() { printf("aaaa\n"); };

	yz_object(SpecialButton, button1)
	{
		test();
		self.test = 100;
	};

	yz_object(button, button2)
	{
		self.x = 200;
		self.y = 100;
		self.text = "button2";
		self.on_click += [&](){
			self.x = self.x + 50; 
			self.y = self.y + 50;
		};
	};

	yz_object(timer, timer1)
	{
		self.interval = 100;
		self.triggered_on_start = true;
		self.repeat = true;
		static int direction = 1;
		self.on_timer += [&](){
			button& button1 = parent["button1"]; 
			if (button1.x > 400)
				direction = -1;
			
			if (button1.x < 0)
				direction = 1;

			button1.x = button1.x + 50 * direction;
		};
	};
};

#include "yz/ui_end.hpp"
#include <yz/app.hpp>

int main()
{
	yz::app app;
	yz::window* w = yz::ui["main_form"];
	yz::SpecialButton* button = yz::ui["main_form"]["button1"];
	button->test_func();
	w->show();
	app.exec();
	return 0;
}