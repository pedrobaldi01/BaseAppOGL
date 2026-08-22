#pragma once
#include "Engine/CSceneBase.h"
#include "Engine/CCamera.h"
#include "Engine/CGridAxis.h"
#include "Engine/CShader.h"
#include "Engine/CTextures.h"
#include "Engine/CTimer.h"
#include "Engine/CText.h"



class Scene0 : public CSceneBase
{
public:
		Scene0();
		~Scene0();

		virtual void DrawScene(float deltaTime);
        virtual void ProcessSceneInput(GLFWwindow* window, float deltaTime);

		void ShowGUI();

		void CreateTriangle();
		void DrawTriangle();
		void DestroyTriangle();

		void CreateCube();
		void DrawCube();
		void DestroyCube();

		void ResetTriangle();
		void ResetCube();

private:

	// Camera
	CCamera* pCamera;

	// Axis
	CGridAxis* pGridAxis;

	// Shader
	CShader* pShader;

	// Textures
	CTextures* pTextures;

	// Timer
	CTimer* pTimer;			

	// Texto
	CText* pText;			

	// Cor do Framebuffer
	glm::vec4 vFramebufferColor;

	bool bActiveGUI;

	// Declara o Vertex Array Object e o Vertex Buffer Object
	GLuint triangleVAO, triangleVBO;
	GLuint cubeVAO, cubeVBO;

	// Variáveis que controlam a translação do cubo através das setas do teclado
	float fPosX, fPosY, fPosZ, fSpeed;

	// Vetor que controla a escala do cubo
	glm::vec3 vScale;

	// Controla a velocidade de rotação da pirâmide
	float fRotSpeed;

	// Habilita ou Desabilita a rotação da pirâmide
	bool bRot;

	// Cor sólida para o cubo
	glm::vec4 vCubeColor;
	bool bCubeColor;
};

