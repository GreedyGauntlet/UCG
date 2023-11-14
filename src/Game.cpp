#include <Flora.h>
#include <Flora/Core/EntryPoint.h>
#include "GameLayer.h"

namespace Flora {
	class Game : public Application {
	public:
		Game(const WindowProps& props, ApplicationCommandLineArgs args)
			: Application(props, "", args) {
			PushLayer(new UCG::GameLayer());
			GetWindow().SetWindowIcon("assets/templogo.png");
		}

		~Game() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		WindowProps props;
		props.Title = "Untitled Card Game";
		props.Width = 1600;
		props.Height = 900;
		props.Fullscreen = false;
		return new Game(props, args);
	}
}