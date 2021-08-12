/*---------------------------------------------------------*/
/* ------------ Proyecto Final de CGEIHC ------------------*/
/*------------- Semestre 2021-2   -------------------------*/
/*------------- Alumnos: Diana Paola Sanjuan Aldape -------*/
/*-------------			 Jesús Ángel León Díaz ------------*/
/*-------------			 Alan Jonathan Marín Zarate -------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 100.0f, 500.0f));
float MovementSpeed = 1.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, -1.0f, 0.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	crecimiento_aro = 0.0f,
		posi_aroX=0.0f,
		posi_aroZ=0.0f,
		sube_aro = 0.0f,
		caer_objeto = 0.0f,
		recorrido_ola = 0.0f,
		aparicion_ola=0.0f,
		retardo = 0.0f,
		brinca_pez=0.0f,
		recorridoX_pez=0.0f,
		recorridoY_pez=0.0f,
		rotacion_pez=0.0f,
		cargax=0.0f,
		cargay=0.0f,
		//animacion del personaje
		movi_PerX = 0.0f,
		movi_PerY = 0.0f,
		movi_PerZ = 0.0f,
		movi_rota = 0.0f,
		anim1 =1.0f,
		anim2 =0.0f,
		anim3 =0.0f,
		anim4 =0.0f,
		anguloD = 90.0f,
		anguloI = 90.0f,
		posicionPX =0.0f,
		posicionPY =0.0f,
		poliX = 0.0f,
		poliZ = 0.0f,
		Npolivuelta = 0.0f,
		Npolicaminando = 0.5f,
		NpoliIzquierda = 0.0f,
		anguloPoli = 90.0f;

bool	animacion = false,
		sube = false,
		subeA = false,
		creceA=false,
		topeA=false,
		avanza = true,
		sube_pez=true,
		animacion_pez=false,
		animacion_caida=false,
		empieza_onda=false,
		Act_anim1 = true,
		Act_anim2 = false,
		Act_anim3 = false,
		Act_anim4 = false,
		Act_anim5 = false,
		puertaA = false,
		puertaC = false,
		pelotaAB = true,
		pelotaBA = false,
		poliIda = true,
		poliRegreso = false,
		poliRecorrido = false;



//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}




void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	if (Act_anim1) {
		movi_PerZ += 1.0f;
		if (movi_PerZ > 400.0f) {
			Act_anim1 = false;
			Act_anim2 = true;
			Act_anim3 = false;
			Act_anim4 = false;
			Act_anim5 = false;
			anim1 = 0.0f;
			anim2 = 1.0f;
			anim3 = 0.0f;
			anim4 = 0.0f;
		}
	}
	else {
		if (Act_anim2) {
			movi_rota = 90.0f;
			Act_anim1 = false;
			Act_anim2 = false;
			Act_anim3 = false;
			Act_anim4 = false;
			Act_anim5 = true;
			anim1 = 1.0f;
			anim2 = 0.0f;
			anim3 = 0.0f;
			anim4 = 0.0f;
		}
		else {
			if (Act_anim3) {
				movi_PerZ += 2.0f;
				movi_PerY -= 10.0f;
				Act_anim1 = false;
				Act_anim2 = false;
				Act_anim3 = false;
				Act_anim4 = true;
				Act_anim5 = false;
				anim1 = 0.0f;
				anim2 = 0.0f;
				anim3 = 0.0f;
				anim4 = 1.0f;

			}
			else {
				if (Act_anim4) {
					movi_PerY -= 1.0f;
					movi_PerX += 1.0f;
					if (movi_PerY < -30.0f) {
						Act_anim1 = false;
						Act_anim2 = false;
						Act_anim3 = false;
						Act_anim4 = false;
						Act_anim5 = false;
						anim1 = 0.0f;
						anim2 = 0.0f;
						anim3 = 0.0f;
						anim4 = 1.0f;
						posi_aroX = -140.0f;
						posi_aroZ = -100.0f;
						animacion = true;
						empieza_onda = true;
					}
				}
				else {
					if (Act_anim5) {
						movi_PerX += 1.0f;
						movi_PerY -= 0.3f;
						if (movi_PerX > 50.0f) {
							Act_anim1 = false;
							Act_anim2 = false;
							Act_anim3 = true;
							Act_anim4 = false;
							Act_anim5 = false;
							anim1 = 0.0f;
							anim2 = 0.0f;
							anim3 = 1.0f;
							anim4 = 0.0f;
						}

					}
				}
			}
		}
	}

	////////////////////////////*policia*///////////////////////////////////////	
	
	if(poliIda){
		if(poliX < 300.0f){
			//Npolicaminando = 0.5f;
			poliX += 2.0f;
		}
		else{
			anguloPoli = -90.0f;
			//Npolivuelta = 1.0f;
			//Npolicaminando =0.0f;
			poliRegreso = true;
			poliIda = false;
		}
	}
	if(poliRegreso){
		if(poliX > 0.0f){
			//Npolicaminando = 0.5f;
			poliX -=2.0f;
		}
		else{
			anguloPoli = 90.0f;
			//Npolivuelta = 1.0f;
			//Npolicaminando =0.0f;
			poliRegreso = true;
			poliIda = false;
		}
	
	}
	

	/*pelota*/
	if(pelotaAB){
	if(posicionPX < 300.0f){
		posicionPX += 2.0f;	
		if(posicionPX < 150.0f)
			posicionPY +=2.0f;
		else
			posicionPY -=2.0f;
	}
	else{
		pelotaAB = false;
		pelotaBA = true;
	}
	}
	if(pelotaBA){
	if(posicionPX > 0.0f){
		posicionPX -= 2.0f;	
		if(posicionPX > 150.0f)
			posicionPY +=2.0f;
		else
			posicionPY -=2.0f;
	}
	else{
		pelotaAB = true;
		pelotaBA = false;
	}
	}

	/*movimiento de la ola*/
	if (recorrido_ola < 350.0f) {
		recorrido_ola += 1.0f;
		if (aparicion_ola < 6.0f) {
			aparicion_ola += 0.1f;
		}
		if (recorrido_ola > 250.0f) {
			aparicion_ola -= 0.2f;
		}
	}
	else {
		recorrido_ola = 0.0f;
		aparicion_ola = 0.0f;
	}

	// animacion del pez
	if (animacion_pez) {
		cargax = recorridoX_pez;
		cargay = recorridoY_pez;

		if (sube_pez) {
			brinca_pez += 10.0f;
			if (brinca_pez > 70.0f) {
				sube_pez = false;
			}

		}
		else {
			if (rotacion_pez < 180.0f) {
				rotacion_pez += 20.0f;
			}
			else {
				brinca_pez -= 10.0f;
				if (brinca_pez < -20.0f) {
					rotacion_pez = 0.0f;
					sube_pez = true;
					animacion_pez = false;
					animacion = true;
					empieza_onda = true;
					posi_aroX = 0.0f;
					posi_aroZ = 0.0f;
				}
			}
		}
	}


	if (animacion_caida) {
		caer_objeto -= 1.0f;
		if (caer_objeto < -100.0f) {
			animacion = true;
			empieza_onda = true;
		}
	}
	if (puertaA) {
		if (anguloD < 180.0f)
			anguloD += 1.0f;
		if (anguloI > 0.0f)
			anguloI -= 1.0f;
	}
	if (puertaC) {
		if (anguloD > 90.0f)
			anguloD -= 1.0f;
		if (anguloI < 90.0f)
			anguloI += 1.0f;


	}
	if (animacion)
	{
		/*movimiento dela caida */
		if (empieza_onda) {
			subeA = true;
			if (subeA)
				if (sube_aro < 1.0f) {
					sube_aro += 0.01f;
					subeA = false;
					creceA = true;
				}
		}
		if (creceA) {
			crecimiento_aro += 0.01f;
			if (crecimiento_aro > 0.5f)
				topeA = true;
		}
		if (topeA) {
			crecimiento_aro += 0.0001f;
			sube_aro -= 0.1f;
			if (sube_aro < -10.0f) {
				sube_aro = 0.0f;
				crecimiento_aro = 0.0f;
				animacion = false;
				empieza_onda = false;
				subeA = false;
				creceA = false;
				topeA = false;
			}
		}
	}

}

void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model arbolito("resources/objects/arboles/Gledista_Triacanthos.obj");
	Model arbolito2("resources/objects/arboles/Gledista_Triacanthos_2.obj");
	Model arbolito3("resources/objects/arboles/Gledista_Triacanthos_3.obj");
	Model pelota("resources/objects/pelota/pelota.obj");
	Model piso("resources/objects/piso2/Piso2.obj");
	Model piso2("resources/objects/piso/Piso.obj");
	Model casa("resources/objects/Muro/casa7.obj");
	Model muro("resources/objects/Muro/muro.obj");
	Model lago("resources/objects/lago/lago.obj");
	Model reja1("resources/objects/Muro/reja5.obj");
	Model reja2("resources/objects/Muro/reja4.obj");
	Model lambo("resources/objects/Lambo/.obj");
	Model aro("resources/objects/aro/aro.obj");
	Model ola("resources/objects/ola/ola.obj");
	Model pez("resources/objects/Fishes/TropicalFish01.obj");

	ModelAnim policaminando("resources/objects/policia/caminar/caminar.dae");
	policaminando.initShaders(animShader.ID);

	//ModelAnim polivuelta("resources/objects/policia/vuelta/vuelta.dae");
	//polivuelta.initShaders(animShader.ID);

	//ModelAnim poliIzquierda("resources/objects/policia/izquierda/izquierda.dae");
	//poliIzquierda.initShaders(animShader.ID);

	ModelAnim nino("resources/objects/niño/Jumping.dae");
	nino.initShaders(animShader.ID);
	ModelAnim nina("resources/objects/niña/Jumping.dae");
	nina.initShaders(animShader.ID);

	ModelAnim caminar("resources/objects/caminar/caminar.dae");
	caminar.initShaders(animShader.ID);

	ModelAnim caer("resources/objects/caer/caer.dae");
	caer.initShaders(animShader.ID);

	ModelAnim izquierda("resources/objects/izquierda/izquierda.dae");
	izquierda.initShaders(animShader.ID);

	ModelAnim nadar("resources/objects/nadar/nadar.dae");
	nadar.initShaders(animShader.ID);

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Animacion 1
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(100.0f , 0.0f, 500.0f));
		model = glm::scale(model, glm::vec3(0.2f));	
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		nino.Draw(animShader);
			
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f , 0.0f, 500.0f));
		model = glm::scale(model, glm::vec3(0.2f));	
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		nina.Draw(animShader);

		/*policia*/
		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f + poliX, 0.0f, 900.0f + poliZ));
		//model = glm::scale(model, glm::vec3(Npolicaminando));	
		//model = glm::rotate(model, glm::radians(anguloPoli), glm::vec3(0.0f, 1.0f, 0.0f));
		//animShader.setMat4("model", model);
		//policaminando.Draw(animShader);

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f + poliX, 0.0f, 800.0f + poliZ));
		//model = glm::scale(model, glm::vec3(Npolivuelta));	
		//model = glm::rotate(model, glm::radians(anguloPoli), glm::vec3(0.0f, 1.0f, 0.0f));
		//animShader.setMat4("model", model);
		//polivuelta.Draw(animShader);


		//model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f + poliX, 0.0f, 800.0f + poliZ));
		//model = glm::scale(model, glm::vec3(NpoliIzquierda));	
		//model = glm::rotate(model, glm::radians(anguloPoli), glm::vec3(0.0f, 1.0f, 0.0f));
		//animShader.setMat4("model", model);
		//poliIzqueirda.Draw(animShader);



		/*tipo lago*/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f + movi_PerX, movi_PerY, -500.0f + movi_PerZ));
		model = glm::scale(model, glm::vec3(0.1f * anim1));	
		model = glm::rotate(model, glm::radians(movi_rota), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		caminar.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f + movi_PerX, movi_PerY, -500.0f + movi_PerZ));
		model = glm::scale(model, glm::vec3(0.1f * anim4));	
		model = glm::rotate(model, glm::radians(movi_rota), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		nadar.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f + movi_PerX, movi_PerY, -500.0f + movi_PerZ));
		model = glm::scale(model, glm::vec3(0.1f * anim2));	
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		izquierda.Draw(animShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-200.0f + movi_PerX, movi_PerY, -500.0f + movi_PerZ));
		model = glm::scale(model, glm::vec3(0.1f * anim3));	
		model = glm::rotate(model, glm::radians(movi_rota), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		caer.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Animacion 2
		// -------------------------------------------------------------------------------------------------------------------------

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		/*-------------------------------------------------------------------------------------------------------------------------*/
		/*--------------------------------------------------------- Muro ----------------------------------------------------------*/
		/*-------------------------------------------------------------------------------------------------------------------------*/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-195.0f + posicionPX, 45.0f + posicionPY, 500.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		pelota.Draw(staticShader);
		
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posi_aroX, -2.0f + sube_aro, posi_aroZ));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f + crecimiento_aro));
		staticShader.setMat4("model", model);
		aro.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f - recorrido_ola, -10.0f + aparicion_ola, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		ola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f + cargax, -20.0f + brinca_pez, 0.0f + cargay));
		model = glm::rotate(model, glm::radians(-90.0f + rotacion_pez), glm::vec3(1.0f, 0.0f, 0.0));
		model = glm::scale(model, glm::vec3(0.08f));
		staticShader.setMat4("model", model);
		pez.Draw(staticShader);


		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, 210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(13.5f));
		staticShader.setMat4("model", model);
		muro.Draw(staticShader);

		/*/*--------------------------------------------------------- Reja ----------------------------------------------------------*/
		// reja derecha
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, 820.0f));
		model = glm::rotate(model, glm::radians(anguloD), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		reja1.Draw(staticShader);
		// reja izquierda
		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, 800.0f));
		model = glm::rotate(model, glm::radians(anguloI), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		reja2.Draw(staticShader);


		/*	/*-------------------------------------------------------------------------------------------------------------------------*/
			/*------------------------------------------------------- Pisos -----------------------------------------------------------*/
			/*-------------------------------------------------------------------------------------------------------------------------*/

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(600.0f, -0.01f, -500.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(600.0f, -0.02f, 430.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-680.0f, -0.03f, 500.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-680.0f, -0.04f, -450.0f));
		model = glm::scale(model, glm::vec3(15.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		/*-------------------------------------------------------------------------------------------------------------------------*/
		/*--------------------------------------------------------- Casas ---------------------------------------------------------*/
		/*-------------------------------------------------------------------------------------------------------------------------*/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 0.0f, -500.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 0.0f, 430.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-680.0f, 0.0f, 500.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-680.0f, 0.0f, -450.0f));
		model = glm::scale(model, glm::vec3(6.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		casa.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Arbolitos
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, 200.0f));
		model = glm::scale(model, glm::vec3(2.0f));
		staticShader.setMat4("model", model);
		arbolito.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(600.0f, 0.0f, 300.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		arbolito2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, 200.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		arbolito3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 600.0f));
		model = glm::scale(model, glm::vec3(1.5f));
		staticShader.setMat4("model", model);
		arbolito2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, -200.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		arbolito.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-720.0f, 0.0f, 880.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		arbolito3.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 0.0f, -200.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		arbolito2.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-700.0f, 0.0f, -500.0f));
		model = glm::scale(model, glm::vec3(2.5f));
		staticShader.setMat4("model", model);
		arbolito.Draw(staticShader);




		/*-------------------------------------------------------------------------------------------------------------------------*/
		/*-------------------------------------------------------- Lago -----------------------------------------------------------*/
		/*-------------------------------------------------------------------------------------------------------------------------*/

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		model = glm::rotate(model, glm::radians(-40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		lago.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		animacion_pez = true;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		puertaA = true;
		puertaC = false;
	}
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
		puertaA = false;
		puertaC = true;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		caer_objeto = 0.0f;
		crecimiento_aro = 0.0f;
		aparicion_ola = 0.0f;
		recorrido_ola = 0.0f;
		sube_aro = 0.0f;
		subeA = false;
		creceA = false;
		topeA = false;
		animacion_caida = false;
		animacion = false;

		posi_aroX = 0.0f;
		posi_aroZ = 0.0f;
		Act_anim1 = true;
		Act_anim2 = false;
		Act_anim3 = false;
		Act_anim4 = false;
		Act_anim5 = false;

		movi_PerX = 0.0f;
		movi_PerY = 0.0f;
		movi_PerZ = 0.0f;
		movi_rota = 0.0f;
		anim1 = 1.0f;
		anim2 = 0.0f;
		anim3 = 0.0f;
		anim4 = 0.0f;

	}

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		//animacion ^= true;
		animacion_caida ^= true;
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}