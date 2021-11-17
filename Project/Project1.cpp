
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <GL/glew.h> // glew apare inainte de freeglut
#include <GL/freeglut.h> // nu trebuie uitat freeglut.h

#include "loadShaders.h"

#include "glm/glm/glm.hpp"  
#include "glm/glm/gtc/matrix_transform.hpp"
#include "glm/glm/gtx/transform.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "SOIL.h"

using namespace std;

//////////////////////////////////////
// identificatori 
GLuint
VaoId,
VboId,
EboId,
ColorBufferId,
ProgramId,
myMatrixLocation,
viewLocation,
projLocation,
matrRotlLocation,
matrTrans_5Location,
matrScale_5Location,
codColLocation;

GLuint texture, texture_1, texture_2, texture_3, texture_4;

int codCol;
float PI = 3.141592, angle = 0, ang = 0, ang2 = 0;
float tx = 0; float ty = 0;
//int width = 450, height = 300;
float i = 0.0, ii = 0.0, j = -70.0, jj = 0.0, alpha = 0.0, beta = 0.001;

// matrice utilizate
glm::mat4 myMatrix, matrRot, matrTrans_5, matrScale_5, matrTrans1, matrTrans2, matrTrans_11, matrTrans_12,
matrTrans_21, matrTrans_22, matrRot1, matrRot2;

// elemente pentru matricea de vizualizare
float Obsx = 0.0, Obsy = 0.0, Obsz = 800.f;
float Refx = 0.0f, Refy = 0.0f;
float Vx = 0.0;
float xx = 0.0;
glm::mat4 view;

// elemente pentru matricea de proiectie
float width = 450, height = 300, xwmin = -800.f, xwmax = 800, ywmin = -600, ywmax = 600, znear = 0, zfar = 1000, fov = 45;
glm::mat4 projection;

class Car {
public:
	float i, j, ang;

	void forward() {
		//i += 5 ;
		if (ang >= PI / 30) {
			ang -= PI / 1000;
			j += 0.1;
		}
		else if (ang <= -PI / 30) {
			ang += PI / 1000;
			j -= 0.1;
		}
		else
			ang = 0;
	}

	void go_up() {
		if (ang < 0) {
			j += 0.4;
			//i += 3;
			ang += PI / 100;
		}
		else {
			j += 1;
			//i += 5;
			if (ang < PI / 25)
				ang += PI / 100;
		}
	}


	void go_down() {
		if (ang > 0) {
			j -= 0.4;
			//i += 5;
			ang += -PI / 100;
		}
		else {
			j -= 1;
			//i += 5;
			if (ang > -PI / 25)
				ang += -PI / 100;
		}
	}

	void move_right() {

		
		if (j >= -70)
			go_down();
		else
			forward();

		glutPostRedisplay();
	}

	void move_left() {
		
		if (j <= 0)
			go_up();
		else
			forward();

		glutPostRedisplay();
	}

	void drift() {

		if (ang <= PI / 3.5)
			ang += PI / 400;
		i -= 1;
		j += 0.1;

		glutPostRedisplay();
	}

}blue_car, green_car;

void SetCars() {
	blue_car.i = -80.0;
	blue_car.j = 0.0;
	blue_car.ang = 0.0;
	green_car.i = -80.0;
	green_car.j = -70.0;
	green_car.ang = 0.0;
}

void increment(void) {
	i += 5;
	Obsx += 5;
	glutPostRedisplay();
}

void action(void) {

	// same speed
	if (blue_car.i < 500) {
		Obsx += 3;
		green_car.i += 3;
		blue_car.i += 3;
	}
else
	// blue accelereaza
	if (blue_car.i >= 500 && blue_car.i <= 1000) {
		Obsx += 5;
		green_car.i += 4;
		blue_car.i += 5;
	}
	else
		// blue depaseste
		if (blue_car.i >= 1000 && blue_car.i <= 2000) {
			Obsx += 5;
			green_car.i += 4;
			blue_car.i += 5;
			blue_car.move_right();
		}
		else
			// green intra in depasire pe banda stanga
			if (blue_car.i > 2000 && blue_car.i < 3000) {
				Obsx += 6;
				green_car.i += 6;
				blue_car.i += 6;
				green_car.move_left();
			}
			else
				// green trece de blue
				if (blue_car.i >= 3000 && blue_car.i < 5500) {
					Obsx += 6;
					green_car.i += 7;
					blue_car.i += 6;
				}
				else
					// green depaseste blue, trecand in fata sa, pe aceeasi banda
					if (blue_car.i >= 5500 && blue_car.i < 7500) {
						Obsx += 6;
						green_car.i += 6;
						blue_car.i += 6;
						if(blue_car.i < 6000)
						green_car.move_right();
						if (blue_car.i > 6500) {
							i -= 1;
							blue_car.drift();
							green_car.drift();
						}
					}


	glutPostRedisplay();

}


void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(action);
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			glutIdleFunc(action);
		break;
	default:
		break;
	}
}

void displayMatrix()
{
    for (int ii = 0; ii < 4; ii++)
    {
        for (int jj = 0; jj < 4; jj++)
            cout << myMatrix[ii][jj] << "  ";
        cout << endl;
    };
    cout << "\n";

};


void processSpecialKeys(int key, int xx, int yy) {

    switch (key) {
    case GLUT_KEY_LEFT:
        Obsx -= 20;
        break;
    case GLUT_KEY_RIGHT:
        Obsx += 50;
		//i += 20;
        break;
    case GLUT_KEY_UP:
        Obsy += 20;
        break;
    case GLUT_KEY_DOWN:
        Obsy -= 20;
        break;
    }
}

void CreateVBO(void)
{
    // varfurile 
    GLfloat Vertices[] = {

        // coordonate                 // culori				 // coordonate de texturare
       -50.0f,  -100.0f, 0.0f, 1.0f,	1.0f, 0.0f, 0.0f,		0.0f, 0.0f, // stanga jos
       500.0f,  -100.0f, 0.0f, 1.0f,	0.0f, 1.0f, 0.0f,		1.0f, 0.0f,  // dreapta jos
       500.0f,  50.0f, 0.0f, 1.0f,		1.0f, 1.0f, 0.0f,		1.0f, 1.0f, // dreapta sus
       -50.0f,  50.0f, 0.0f, 1.0f,		0.0f, 1.0f, 1.0f,		0.0f, 1.0f,  // stanga sus
		0.0f, -20.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f,
		100.0f, -20.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,		1.0f, 0.0f,
	    100.0f, 50.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,		1.0f, 1.0f,
		0.0f, 50.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,		0.0f, 1.0f

    };

    // indicii pentru varfuri
    GLuint Indices[] = {
      0, 1, 2, // Primul triunghi
      0, 2, 3, //  Al doilea triunghi
	  4, 5, 6, 7
    };

    // se creeaza un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
    glGenVertexArrays(1, &VaoId);
    // se creeaza un buffer nou (atribute)
    glGenBuffers(1, &VboId);
    // se creeaza un buffer nou (indici)
    glGenBuffers(1, &EboId);

    // legarea VAO 
    glBindVertexArray(VaoId);

    // legarea buffer-ului "Array"
    glBindBuffer(GL_ARRAY_BUFFER, VboId);
    // punctele sunt "copiate" in bufferul curent
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    // legarea buffer-ului "Element" (indicii)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EboId);
    // indicii sunt "copiati" in bufferul curent
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    // se activeaza lucrul cu atribute; atributul 0 = pozitie
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

    // se activeaza lucrul cu atribute; atributul 1 = culoare
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(4 * sizeof(GLfloat)));

    // se activeaza lucrul cu atribute; atributul 2 = coordonate de texturare
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
}

void DestroyVBO(void)
{
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &VboId);
    glDeleteBuffers(1, &EboId);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VaoId);

}

void LoadTexture_3(void)
{

	glGenTextures(1, &texture_3);
	glBindTexture(GL_TEXTURE_2D, texture_3);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned char* image = SOIL_load_image("blue_car.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void LoadTexture_4(void)
{

	glGenTextures(1, &texture_4);
	glBindTexture(GL_TEXTURE_2D, texture_4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	unsigned char* image = SOIL_load_image("green_car.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void LoadTexture(void)
{
	
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height;
    unsigned char* image = SOIL_load_image("rata.png", &width, &height, 0, SOIL_LOAD_RGB);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

}

void LoadTexture_1(void)
{
	
	glGenTextures(1, &texture_1);
	glBindTexture(GL_TEXTURE_2D, texture_1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image_1 = SOIL_load_image("tata.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_1);
	
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image_1);
	glBindTexture(GL_TEXTURE_2D, 0);

}


void LoadTexture_2(void)
{
	
	glGenTextures(1, &texture_2);
	glBindTexture(GL_TEXTURE_2D, texture_2);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	int width, height;
	unsigned char* image_2 = SOIL_load_image("koko.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_2);
	glewExperimental = GL_TRUE;
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image_2);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void CreateShaders(void)
{
    ProgramId = LoadShaders("06_04_Shader.vert", "06_04_Shader.frag");
    glUseProgram(ProgramId);
}


void DestroyShaders(void)
{
    glDeleteProgram(ProgramId);
}

void Initialize(void)
{

    myMatrix = glm::mat4(1.0f);
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f); // culoarea de fond a ecranului
}

void LoadTextures(void) {
	LoadTexture();
	LoadTexture_1();
	LoadTexture_2();
	LoadTexture_3();
	LoadTexture_4();
}

void RenderFunction(void)
{

    glClear(GL_COLOR_BUFFER_BIT);

    // se schimba pozitia observatorului
    glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);

    // pozitia punctului de referinta
    Refx = Obsx; Refy = Obsy;
    glm::vec3 PctRef = glm::vec3(Refx, Refy, -1.0f);

    // verticala din planul de vizualizare 
    glm::vec3 Vert = glm::vec3(Vx, 1.0f, 0.0f);
    view = glm::lookAt(Obs, PctRef, Vert);
    //myMatrix = view;
    projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);
    myMatrix = glm::mat4(1.0f);
	CreateVBO();
	

	// Start texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	CreateShaders();

	matrTrans_5 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0));
	myMatrix = projection * view * myMatrix * matrTrans_5;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Road texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_1);
	for (int j = 0; j <= 10; j++) {
		matrTrans_5 = glm::translate(glm::mat4(1.0f), glm::vec3((j + 1) * 545, 0.0, 0.0));
		myMatrix = glm::mat4(1.0f);
		myMatrix = projection * view * myMatrix * matrTrans_5;
		myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
		glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
		glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	// Finish texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_2);
	matrTrans_5 = glm::translate(glm::mat4(1.0f), glm::vec3(12*545, 0.0, 0.0));
	myMatrix = glm::mat4(1.0f);
	myMatrix = projection * view * myMatrix * matrTrans_5;
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glUniform1i(glGetUniformLocation(ProgramId, "myTexture"), 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	
	// Blue car
	matrTrans1 = glm::translate(glm::mat4(1.0f), glm::vec3(blue_car.i, blue_car.j, 0.0));
	matrTrans_11 = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0, 0.0, 0.0));
	matrTrans_12 = glm::translate(glm::mat4(1.0f), glm::vec3(50.0, 0.0, 0.0));
	matrRot1 = glm::rotate(glm::mat4(1.0f), blue_car.ang, glm::vec3(0.0, 0.0, 1.0));
	myMatrix = glm::mat4(1.0f);
	myMatrix = projection * view * myMatrix * matrTrans1 * (matrTrans_12 * matrRot1 * matrTrans_11);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_3);
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glPointSize(20.0);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void*)(24));

	// Green car
	//matrTrans2 = glm::translate(glm::mat4(1.0f), glm::vec3(xx, -70.0, 0.0));
	matrTrans2 = glm::translate(glm::mat4(1.0f), glm::vec3(green_car.i,green_car.j, 0.0));
	matrTrans_21 = glm::translate(glm::mat4(1.0f), glm::vec3(-50.0, -15.0, 0.0));
	matrTrans_22 = glm::translate(glm::mat4(1.0f), glm::vec3(50.0, 15.0, 0.0));
	matrRot2 = glm::rotate(glm::mat4(1.0f), green_car.ang, glm::vec3(0.0, 0.0, 1.0));
	myMatrix = glm::mat4(1.0f);
	myMatrix = projection * view * myMatrix * matrTrans2 * (matrTrans_22 * matrRot2 * matrTrans_21);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_4);
	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	glPointSize(20.0);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, (void*)(24));

    DestroyVBO();
    DestroyShaders();

    glutSwapBuffers();
    glFlush();
}

void Cleanup(void)
{
    DestroyShaders();
    DestroyVBO();
}

int main(int argc, char* argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(1200, 900);
    glutCreateWindow("Utilizarea texturilor");
    glewInit();
    Initialize();
	SetCars();
	LoadTextures();
    glutDisplayFunc(RenderFunction);
    glutIdleFunc(RenderFunction);
	glutMouseFunc(mouse);
    glutSpecialFunc(processSpecialKeys);
    glutCloseFunc(Cleanup);
    glutMainLoop();

}

