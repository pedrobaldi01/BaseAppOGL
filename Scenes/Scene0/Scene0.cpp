#include "Scene0.h"


Scene0::Scene0()
{
	// Cria a câmera
	pCamera = NULL;
	pCamera = new CCamera(glm::vec3(0.0f, 3.0f, 10.0f));
	pCamera->MovementSpeed = 100.0f;

	// Cria o Timer
	pTimer = NULL;
	pTimer = new CTimer();
	pTimer->Init();

	// Cria gerenciador de impressão de texto na tela
	pText = NULL;
	pText = new CText();
	pText->InitTextManager();

	// Cria os shaders
	pShader = NULL;
	pShader = new CShader();
	pShader->LoadShader("Grid", "Scenes/Common/grid.vert", "Scenes/Common/grid.frag");
	pShader->LoadShader("Axis", "Scenes/Common/axis.vert", "Scenes/Common/axis.frag");
	pShader->LoadShader("Text2D", "Scenes/Common/Text2D.vert", "Scenes/Common/Text2D.frag");

	pShader->LoadShader("SolidColor", "Scenes/Scene0/Solid.vert",
		"Scenes/Scene0/Solid.frag");


	// Cria o gerenciador de grid e axis
	pGridAxis = NULL;
	pGridAxis = new CGridAxis(50.0f);

	// Cor do Framebuffer (Cor de fundo)
	vFramebufferColor = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);

	// Entra no modo GUI
	bActiveGUI = false;

	// Cria o gerenciador de texturas
	pTextures = NULL;
	//pTextures = new CTextures();


	// Inicializar o triangulo
	// Aloca o triangulo na VRAM
	CreateTriangle();
	CreateCube();

}

Scene0::~Scene0()
{
	// Desaloca o triangulo da VRAM
	DestroyTriangle();
	DestroyCube();

	if (pCamera)
	{
		delete pCamera;
		pCamera = NULL;
	}

	if (pShader)
	{
		delete pShader;
		pShader = NULL;
	}

	if (pGridAxis)
	{
		delete pGridAxis;
		pGridAxis = NULL;
	}

	if (pTextures)
	{
		delete pTextures;
		pTextures = NULL;
	}

	if (pTimer)
	{
		delete pTimer;
		pTimer = NULL;
	}

	if (pText)
	{
		delete pText;
		pText = NULL;
	}


}

void Scene0::DrawScene(float deltaTime)
{
	// Update timer (must be called at start of frame)
	pTimer->Update();



	// Clear the color and depth buffers
	glClearColor(vFramebufferColor.r, vFramebufferColor.g, vFramebufferColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Create transformations 
	// Note: In a real application, you would typically calculate the projection and view matrices 
	// once per frame and pass them to your shaders
	glm::mat4 projection = glm::perspective(glm::radians(pCamera->Zoom), ASPECT_RATIO, Z_NEAR, Z_FAR);
	glm::mat4 view = pCamera->GetViewMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw the 3D grid
	pShader->Use("Grid");
	pShader->SetMat4("uProj", projection);
	pShader->SetMat4("uView", view);
	pShader->SetFloat("uGridSpacing", 1.0f);
	pShader->SetFloat("uFadeStart", 18.f);
	pShader->SetFloat("uFadeEnd", 100.f);
	pShader->SetVec3("uCamPos", pCamera->Position);
	pGridAxis->DrawGrid();
	// Draw the 3D axis
	pShader->Use("Axis");
	pShader->SetMat4("uProj", projection);
	pShader->SetMat4("uView", view);
	pGridAxis->DrawAxis();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (INÍCIO)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Desenha o triangulo
	pShader->Use("SolidColor"); // Habilitar o shader para o objeto
	pShader->SetMat4("projection", projection);
	pShader->SetMat4("view", view);

	model = glm::mat4(1.0);
	pShader->SetMat4("model", model);

	DrawTriangle();
	DrawCube();

















	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                               DESENHA OS OBJETOS DA CENA (FIM)
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Draw text on the screen (disable depth test and use orthographic projection)
	glDisable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));

	pShader->Use("Text2D");
	pShader->SetMat4("projection", projection);
	pShader->SetVec3("textColor", 1.0f, 1.0f, 1.0f);

	pText->RenderText(std::format("CamPosition\tx: {0:.2f} \t y: {1:.2f} \t z: {2:.2f}", pCamera->Position.x, pCamera->Position.y, pCamera->Position.z), 5.0f, 100.0f, 0.3f);
	pText->RenderText(std::format("CamFoward\tx: {0:.2f} \t y: {1:.2f} \t z: {2:.2f}", pCamera->Front.x, pCamera->Front.y, pCamera->Front.z), 5.0f, 80.0f, 0.3f);
	pText->RenderText(std::format("[ENTER] GUI mode: {0}", bActiveGUI ? "true" : "false"), 5.0f, 60.0f, 0.3f);

	pText->RenderText(std::format("FPS: {0:.0f}\tDeltatime: {1:.2f}ms\tTimer: {2:.2f}s", pTimer->GetFPS(), pTimer->GetDeltaTime(), pTimer->GetTime() / 1000), 5.0f, 10.0f, 0.3f);
	glEnable(GL_DEPTH_TEST);
}


void Scene0::ProcessSceneInput(GLFWwindow* window, float deltaTime)
{
	InputManager& input = InputManager::GetInstance();

	if (input.IsKeyJustPressed(GLFW_KEY_ENTER))
	{
		bActiveGUI = !bActiveGUI;
		input.SetCursorMode(bActiveGUI ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

		std::cout << "GUI mode: '" << bActiveGUI << "'" << std::endl;
	}

	if (bActiveGUI == false)
	{
		// Camera movement
		if (input.IsKeyPressed(GLFW_KEY_W))
			pCamera->ProcessKeyboard(FORWARD, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_S))
			pCamera->ProcessKeyboard(BACKWARD, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_A))
			pCamera->ProcessKeyboard(LEFT, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_D))
			pCamera->ProcessKeyboard(RIGHT, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_E))
			pCamera->ProcessKeyboard(UP, deltaTime);
		if (input.IsKeyPressed(GLFW_KEY_Q))
			pCamera->ProcessKeyboard(DOWN, deltaTime);

		// Increase/decrease movement speed
		if (input.IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			pCamera->MovementSpeed = SPEED_MULTIPLIER;
		else
			pCamera->MovementSpeed = SPEED;

		// Process mouse movement
		glm::vec2 mouseDelta = input.GetMouseDelta();
		if (mouseDelta.x != 0.0f || mouseDelta.y != 0.0f)
		{
			pCamera->ProcessMouseMovement(mouseDelta.x, mouseDelta.y);
		}

		// Process scroll
		float scrollOffset = input.GetScrollOffset();
		if (scrollOffset != 0.0f)
		{
			pCamera->ProcessMouseScroll(scrollOffset);
		}
	}
	else
	{
		ShowGUI(); // Show Graphics User Interface
	}

}

void Scene0::ShowGUI()
{
	ImGui::Begin("Scene Options:");
	ImGui::Text("Framebuffer Color:");
	ImGui::ColorEdit4(" ", (float*)&vFramebufferColor);
	if (ImGui::Button("Reset Color"))
		vFramebufferColor = glm::vec4(0.1f, 0.1f, 0.15f, 1.0f);
	ImGui::End();
}



void Scene0::CreateTriangle()
{
	float vertices[] =
	{
		//    X     Y     Z		R	   G	  B
			-1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 1.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,
			 0.0f, 2.0f, 0.0f,  1.0f, 0.0f, 0.0f,

			 1.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
			 0.0f, 2.0f,  0.0f, 0.0f, 0.0f, 1.0f,

			 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			-1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
			 0.0f, 2.0f,  0.0f, 0.0f, 1.0f, 0.0f,

		    -1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		    -1.0f,  0.0f,  1.0f, 0.0f, 1.0f, 0.0f,
			 0.0f,  2.0f,  0.0f, 0.0f, 0.0f, 1.0f
	};

	// Cria o VAO
	glGenVertexArrays(1, &triangleVAO);

	// Cria o VBO
	glGenBuffers(1, &triangleVBO);

	// Ativa o VAO
	glBindVertexArray(triangleVAO);

	// Ativa o VBO e aloca os dados do objeto nele (vai para a GPU)
	// Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Define os atributos de cada vértice (VAO)
	// Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	// Posição do vértice (ID = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// Cor do vértice (ID = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	// Desativa o VAO (evita conflitos com outros VAOs)
	glBindVertexArray(0);
}

void Scene0::DrawTriangle()
{
	// Ativa o VAO que representa o objeto a ser desenhado
	glBindVertexArray(triangleVAO);
	// Desenha o objeto
	glDrawArrays(GL_TRIANGLES, 0, 12);
	// Desativa o VAO depois da renderização do objeto
	glBindVertexArray(0);
}

void Scene0::DestroyTriangle()
{
	glDeleteVertexArrays(1, &triangleVAO);
	glDeleteBuffers(1, &triangleVBO);
}

void Scene0::CreateCube()
{
	float vertices[] =
	{
		// Face da frente
		// X     Y     Z     R     G     B
		-1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,

		 1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,

		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		 -1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		 -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,

		 -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		 -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f,

		-1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		 1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		-1.0f,  -1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		 1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		 1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-1.0f,  -1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

	};

	// Cria o VAO
	glGenVertexArrays(1, &cubeVAO);

	// Cria o VBO
	glGenBuffers(1, &cubeVBO);

	// Ativa o VAO
	glBindVertexArray(cubeVAO);

	// Ativa o VBO e aloca os dados do objeto nele (vai para a GPU)
	// Esse VBO está ligado com o VAO
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Define os atributos de cada vértice (VAO)
	// Diz para a GPU como interpretar os dados do VBO
	int stride = 6 * sizeof(float);

	// Posição do vértice (ID = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(0);

	// Cor do vértice (ID = 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Desativa o VAO (evita conflitos com outros VAOs)
	glBindVertexArray(0);
}

void Scene0::DrawCube()
{
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Scene0::DestroyCube()
{
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
}