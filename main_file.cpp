/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE
#define FRAME_RATE 144
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include <vector>
#include <cmath>

#include <table.h>
#include <ball.h>
#include "main_file.h"
#include <cue.h>

float speed_x = 0;
float speed_y = 0;
float speed_z = 10;
float aspectRatio = 1;
float cue_speed = 0;
float cue_angle = 0;

Table* table;
Cue* cue;
std::vector<Ball*> balls;

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void resetBalls() {
	glm::vec2 newPos(0);
	int row = 1,
		col = 1;
	balls[0]->position = glm::vec2(0, 6.0f);
	balls[0]->speed = glm::vec2(0);
	for (int i = 1; i <= 15; i++)
	{
		float spacing = 0.1f * col;
		newPos.y = -row;
		newPos.x = (-float(row - 1) / 2.0f + col) - 1.0f + spacing / 2;
		balls[i]->position = newPos;
		balls[i]->speed = glm::vec2(0);
		if (col == row)
		{
			row++;
			col = 0;
		}
		col++;
	}
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_SPACE) {
			resetBalls();
		}
		if (key == GLFW_KEY_LEFT) speed_x = -PI / 2;
		if (key == GLFW_KEY_RIGHT) speed_x = PI / 2;
		if (key == GLFW_KEY_UP) speed_y = PI / 2;
		if (key == GLFW_KEY_DOWN) speed_y = -PI / 2;
		if (key == GLFW_KEY_A) cue_speed = PI / 2;
		if (key == GLFW_KEY_D) cue_speed = -PI / 2;
		if (key == GLFW_KEY_S) {
			balls[0]->acceleration = glm::vec2(cosf(cue_angle) * 1.0f, sinf(cue_angle) * 1.0f);
		}
		if (key == GLFW_KEY_Q) Ball::radius += +0.05f;
		if (key == GLFW_KEY_Z) Ball::radius += -0.05f;
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_x = 0;
		if (key == GLFW_KEY_RIGHT) speed_x = 0;
		if (key == GLFW_KEY_UP) speed_y = 0;
		if (key == GLFW_KEY_DOWN) speed_y = 0;
		if (key == GLFW_KEY_A) cue_speed = 0;
	}
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	speed_z += yoffset;
	//std::cout << yoffset << std::endl;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	if (height == 0) return;
	aspectRatio = (float)width / (float)height;
	glViewport(0, 0, width, height);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

void draw3Daxis() {
	glColor3f(1.0, 0.0, 0.0); // red x
	glBegin(GL_LINES);
	// x axis

	glVertex3f(-4.0, 0.0f, 0.0f);
	glVertex3f(4.0, 0.0f, 0.0f);

	// arrow
	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, 1.0f, 0.0f);

	glVertex3f(4.0, 0.0f, 0.0f);
	glVertex3f(3.0, -1.0f, 0.0f);
	glEnd();
	glFlush();

	// y axis
	glColor3f(0.0, 1.0, 0.0); // green y
	glBegin(GL_LINES);
	glVertex3f(0.0, -4.0f, 0.0f);
	glVertex3f(0.0, 4.0f, 0.0f);

	// arrow
	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(1.0, 3.0f, 0.0f);

	glVertex3f(0.0, 4.0f, 0.0f);
	glVertex3f(-1.0, 3.0f, 0.0f);
	glEnd();
	glFlush();

	// z axis
	glColor3f(0.0, 0.0, 1.0); // blue z
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0f, -4.0f);
	glVertex3f(0.0, 0.0f, 4.0f);

	// arrow
	glVertex3f(0.0, 0.0f, 4.0f);
	glVertex3f(0.0, 1.0f, 3.0f);

	glVertex3f(0.0, 0.0f, 4.0f);
	glVertex3f(0.0, -1.0f, 3.0f);
	glEnd();
	glFlush();
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {

	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window, windowResizeCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);

	Table::GetShader();
	Ball::GetShader();

	glEnable(GL_COLOR_MATERIAL);

	Table::GetMesh();
	Ball::GetMesh();

	table = new Table(0.995f);
	cue = new Cue(0, 0);
	balls.push_back(new Ball(0, 6.0f));


	glm::vec2 newPos(0);
	int row = 1,
		col = 1;
	for (int i = 1; i <= 15; i++)
	{	
		float spacing = 0.1f * col;
		newPos.y = -row;
		newPos.x = ( - float(row - 1) / 2.0f + col) - 1.0f + spacing/2;
		Ball* ball = new Ball(newPos);
		ball->color = Ball::colors[i - 1];
		balls.push_back(ball);
		if (col == row)
		{
			row++;
			col = 0;
		}
		col++;
	}

}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float angle_x, float angle_y) 
{
	int time = glfwGetTime();
	int frame = 1000 / FRAME_RATE;

	if (time % frame == 0) {
		//************Tutaj umieszczaj kod rysujący obraz******************l
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		float distance = 20.0f;

		float camX = distance * -sinf(angle_x * PI / 4) * cosf(angle_y * PI / 4);
		float camY = distance * -sinf(angle_y * PI / 4);
		float camZ = -distance * cosf(angle_x * PI / 4) * cosf(angle_y * PI / 4);

		glm::mat4 V = glm::lookAt(
			glm::vec3(camX, camY, camZ + speed_z),
			glm::vec3(0, 0, 0),
			glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku

		glm::mat4 P = glm::perspective(50.0f * PI / 180.0f, aspectRatio, 0.01f, 50.0f); //Wylicz macierz rzutowania
		glm::mat4 M = glm::mat4(1.0f);

		//M = glm::rotate(M, angle_x, glm::vec3(0, 1.0f, 0)); //Wylicz macierz modelu
		//M = glm::rotate(M, angle_y, glm::vec3(1.0f, 0.0f, 0.0f)); //Wylicz macierz modelu
		glColor3f(0.0, 1.0, 0.0);
		glm::mat4 tableM = table->Render(V, P, M);


		cue->cue_angle = cue_angle;
		cue->Render(V, P, M);
		cue->position = balls[0]->position + glm::vec2(cosf(cue_angle), sinf(cue_angle));

		//calc collisions
		for (int i = 0; i < balls.size();++i) {
			for (int j = i + 1; j < balls.size(); ++j) {
				if(balls[i]->collides(balls[j])) 
					balls[i]->bounce(balls[j]);
			}
		}

		//move
		for (Ball* ball : balls) {
			ball->update();
			glm::mat4  ballM = ball->Render(V, P, M);

		}

		//reset collisions
		for (int i = 0; i < balls.size(); ++i) {
			for (int j = i + 1; j < balls.size(); ++j) {
				if (!(balls[i]->collides(balls[j])))
					balls[i]->collisionArray[balls[j]->id] = false;
					balls[j]->collisionArray[balls[i]->id] = false;
			}
		}

		//draw3Daxis();
		glUseProgram(0); // use default shader program
		glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
	}
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x = 0; //Aktualny kąt obrotu obiektu
	float angle_y = -4.0f; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	windowResizeCallback(window, 1280, 720);
	double lastTime = glfwGetTime();
	int nbFrames = 0;
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_x += speed_x * glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		angle_y += speed_y * glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
		cue_angle += cue_speed * glfwGetTime();
		glfwSetTime(0); //Zeruj timer
		drawScene(window, angle_x, angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
