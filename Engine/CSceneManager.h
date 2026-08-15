#pragma once
#include "CSceneBase.h"
#include "Scenes/Scene0/Scene0.h"




class CSceneManager : public CSceneBase
{
public:
	CSceneManager();
	~CSceneManager(void);
	void ChangeScene(unsigned int _uiCurrentScene);

	virtual void DrawScene(float deltaTime);
	virtual void ProcessSceneInput(class GLFWwindow* window, float deltaTime);


private:
	// Ponteiro para cenas do jogo
	CSceneBase* Scene;

	// ID da cena
	unsigned int uiCurrentScene;
	int iMenuCurrentScene;

	bool bWireframeMode;
	bool bVsyncEnabled;
};

