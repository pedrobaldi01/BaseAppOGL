#include "CSceneManager.h"

// Construtor
	
CSceneManager::CSceneManager()
{
	this->Scene = NULL;

	bWireframeMode = false; 
	bVsyncEnabled = false;

	// Inicializa ponteiros das cenas	
	// Cria a cena principal
	uiCurrentScene = 0;
	iMenuCurrentScene = uiCurrentScene;
	ChangeScene(uiCurrentScene);
}


// Destrutor
CSceneManager::~CSceneManager(void)
{
	if (this->Scene)
	{
		delete this->Scene;
		this->Scene = NULL;
	}
}

void CSceneManager::DrawScene(float deltaTime)
{
	this->Scene->DrawScene(deltaTime);
}

void CSceneManager::ProcessSceneInput(GLFWwindow* window, float deltaTime)
{
	InputManager& input = InputManager::GetInstance();

	// Close window on ESC
	if (input.IsKeyJustPressed(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	


	// Toggle cursor mode with TAB or VSync
	if (input.IsKeyJustPressed(GLFW_KEY_V))
	{
		bVsyncEnabled = !bVsyncEnabled;
		glfwSwapInterval(bVsyncEnabled);

		std::cout << "Vsync: '" << bVsyncEnabled << "'" << std::endl;
	}

	if (input.IsKeyJustPressed(GLFW_KEY_CAPS_LOCK))
	{
		// Toggle wireframe mode
		bWireframeMode = !bWireframeMode;

		std::cout << "Wireframe: '" << bWireframeMode << "'" << std::endl;
	}
	if (bWireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // Fill mode
	

	if (input.IsKeyJustPressed(GLFW_KEY_0))
		ChangeScene(0);
	





	// Processa input da cena atual
	Scene->ProcessSceneInput(window, deltaTime);
}


void CSceneManager::ChangeScene(unsigned int _uiCurrentScene)
{
#ifdef _WIN32
	std::system("cls");
#else
	std::cout << "\033[2J\033[H";
#endif

	// Disable Cursor for all scenes
	InputManager& input = InputManager::GetInstance();
	input.SetCursorMode(GLFW_CURSOR_DISABLED);

	this->uiCurrentScene = _uiCurrentScene;

	if (this->Scene)
	{
		delete this->Scene;
		this->Scene = NULL;
	}


	switch (uiCurrentScene)
	{
	case 0:
		this->Scene = new Scene0();
		break;

	

	}
}

