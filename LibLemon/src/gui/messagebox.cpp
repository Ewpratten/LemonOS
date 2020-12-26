#include <lemon/gui/messagebox.h>

#include <lemon/gui/window.h>
#include <lemon/gui/widgets.h>
#include <lemon/gfx/font.h>

namespace Lemon::GUI
{
	int pressed = 1;
	void OnMessageBoxOKPressed(Lemon::GUI::Button* b){
		pressed = 1;
		b->window->closed = true;
	}
	
	void OnMessageBoxCancelPressed(Lemon::GUI::Button* b){
		pressed = 0;
		b->window->closed = true;
	}

	int DisplayMessageBox(const char* title, const char* message, MsgBoxButtons buttons){
		int width = Graphics::GetTextLength(message) + 10;
		if(width < 220) width = 220;

		Window* win = new Window(title, {width, 80}, 0, WindowType::GUI);
		
		Label* label = new Label(message, {10, 10, 180, 12});
		win->AddWidget(label);

		if(buttons == MsgButtonsOKCancel){
			Button* okBtn = new Button("OK", {-52, 2, 100, 24});
			win->AddWidget(okBtn);
			okBtn->SetLayout(LayoutSize::Fixed, LayoutSize::Fixed, WAlignCentre, WAlignBottom);
			okBtn->OnPress = OnMessageBoxOKPressed;

			Button* cancelBtn = new Button("Cancel", {52, 2, 100, 24});
			win->AddWidget(cancelBtn);
			cancelBtn->SetLayout(LayoutSize::Fixed, LayoutSize::Fixed, WAlignCentre, WAlignBottom);
			cancelBtn->OnPress = OnMessageBoxCancelPressed;
		} else {
			Button* okBtn = new Button("OK", {0, 5, 100, 24});
			win->AddWidget(okBtn);
			okBtn->SetLayout(LayoutSize::Fixed, LayoutSize::Fixed, WAlignCentre, WAlignBottom);
			okBtn->OnPress = OnMessageBoxOKPressed;
		}

		bool paint = true;

		while(!win->closed){
			LemonEvent ev;
			while(win->PollEvent(ev)){
				win->GUIHandleEvent(ev);
				paint = true;
			}

			if(paint){
				win->Paint();
				paint = false;
			}
		}

		delete win;

		return pressed;
	}
}