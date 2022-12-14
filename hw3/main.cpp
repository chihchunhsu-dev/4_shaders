#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h> /* offsetof */
#include "../GL/glew.h"
#include "../GL/glut.h""
#include "../shader_lib/shader.h"

#include "glm/glm.h"
extern "C"
{
	#include "glm_helper.h"
}

void init(void);
void display(void);
void reshape(int width, int height);
void keyboard(unsigned char key, int x, int y);
void idle(void);

GLuint currentShader = 0;

GLuint flatShader, phongShader, toonShader, gouraudShader;
GLuint VBO;

GLMmodel *XiaoPingGuo;
GLuint texture1, texture2;

GLfloat light_pos[] = { 10.0, 10.0, 0.0 };
GLfloat eyex = 0.0;
GLfloat eyey = 0.0;
GLfloat eyez = 3.0;

GLfloat rotateAngle = 0.0f;

void init(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLuint flatVert = createShader("Shaders/flat.vert", "vertex");
	GLuint flatFrag = createShader("Shaders/flat.frag", "fragment");
	flatShader = createProgram(flatVert, flatFrag);

	GLuint phongVert = createShader("Shaders/phong.vert", "vertex");
	GLuint phongFrag = createShader("Shaders/phong.frag", "fragment");
	phongShader = createProgram(phongVert, phongFrag);

	GLuint toonVert = createShader("Shaders/toon.vert", "vertex");
	GLuint toonFrag = createShader("Shaders/toon.frag", "fragment");
	toonShader = createProgram(toonVert, toonFrag);

	GLuint gouraudVert = createShader("Shaders/gouraud.vert", "vertex");
	GLuint gouraudFrag = createShader("Shaders/gouraud.frag", "fragment");
	gouraudShader = createProgram(gouraudVert, gouraudFrag);

	XiaoPingGuo = glmReadOBJ("Model/apple.obj");

	glmUnitize(XiaoPingGuo);
	glmFacetNormals(XiaoPingGuo);
	glmVertexNormals(XiaoPingGuo, 90.0, GL_FALSE);

	//print_model_info(XiaoPingGuo);

	GLuint numOfFloat = 3 * 11; // number of floats per triangle in VBO
	GLfloat *vtxAttribs = (GLfloat*)malloc(sizeof(GLfloat) * (XiaoPingGuo->numtriangles * numOfFloat) + 1);

	GLfloat		*vertices   = XiaoPingGuo->vertices;
	GLfloat		*normals    = XiaoPingGuo->normals;
	GLfloat		*texcoords  = XiaoPingGuo->texcoords;
	GLMtriangle *triangles  = XiaoPingGuo->triangles;
	GLfloat		*facetnorms = XiaoPingGuo->facetnorms;
	
	for (int i = 0; i < XiaoPingGuo->numtriangles; i++)
	{
		// vertex 1
		vtxAttribs[numOfFloat * i + 0]  = vertices[triangles[i].vindices[0] * 3 + 0];
		vtxAttribs[numOfFloat * i + 1]  = vertices[triangles[i].vindices[0] * 3 + 1];
		vtxAttribs[numOfFloat * i + 2]  = vertices[triangles[i].vindices[0] * 3 + 2];

		vtxAttribs[numOfFloat * i + 3]  = normals[triangles[i].nindices[0] * 3 + 0];
		vtxAttribs[numOfFloat * i + 4]  = normals[triangles[i].nindices[0] * 3 + 1];
		vtxAttribs[numOfFloat * i + 5]  = normals[triangles[i].nindices[0] * 3 + 2];

		vtxAttribs[numOfFloat * i + 6]  = texcoords[triangles[i].tindices[0] * 2 + 0];
		vtxAttribs[numOfFloat * i + 7]  = texcoords[triangles[i].tindices[0] * 2 + 1];

		vtxAttribs[numOfFloat * i + 8]  = facetnorms[triangles[i].findex * 3 + 0];
		vtxAttribs[numOfFloat * i + 9]  = facetnorms[triangles[i].findex * 3 + 1];
		vtxAttribs[numOfFloat * i + 10] = facetnorms[triangles[i].findex * 3 + 2];

		// vertex 2
		vtxAttribs[numOfFloat * i + 11] = vertices[triangles[i].vindices[1] * 3 + 0];
		vtxAttribs[numOfFloat * i + 12] = vertices[triangles[i].vindices[1] * 3 + 1];
		vtxAttribs[numOfFloat * i + 13] = vertices[triangles[i].vindices[1] * 3 + 2];

		vtxAttribs[numOfFloat * i + 14] = normals[triangles[i].nindices[1] * 3 + 0];
		vtxAttribs[numOfFloat * i + 15] = normals[triangles[i].nindices[1] * 3 + 1];
		vtxAttribs[numOfFloat * i + 16] = normals[triangles[i].nindices[1] * 3 + 2];

		vtxAttribs[numOfFloat * i + 17] = texcoords[triangles[i].tindices[1] * 2 + 0];
		vtxAttribs[numOfFloat * i + 18] = texcoords[triangles[i].tindices[1] * 2 + 1];

		vtxAttribs[numOfFloat * i + 19] = facetnorms[triangles[i].findex * 3 + 0];
		vtxAttribs[numOfFloat * i + 20] = facetnorms[triangles[i].findex * 3 + 1];
		vtxAttribs[numOfFloat * i + 21] = facetnorms[triangles[i].findex * 3 + 2];

		// vertex 3
		vtxAttribs[numOfFloat * i + 22] = vertices[triangles[i].vindices[2] * 3 + 0];
		vtxAttribs[numOfFloat * i + 23] = vertices[triangles[i].vindices[2] * 3 + 1];
		vtxAttribs[numOfFloat * i + 24] = vertices[triangles[i].vindices[2] * 3 + 2];

		vtxAttribs[numOfFloat * i + 25] = normals[triangles[i].nindices[2] * 3 + 0];
		vtxAttribs[numOfFloat * i + 26] = normals[triangles[i].nindices[2] * 3 + 1];
		vtxAttribs[numOfFloat * i + 27] = normals[triangles[i].nindices[2] * 3 + 2];

		vtxAttribs[numOfFloat * i + 28] = texcoords[triangles[i].tindices[2] * 2 + 0];
		vtxAttribs[numOfFloat * i + 29] = texcoords[triangles[i].tindices[2] * 2 + 1];

		vtxAttribs[numOfFloat * i + 30] = facetnorms[triangles[i].findex * 3 + 0];
		vtxAttribs[numOfFloat * i + 31] = facetnorms[triangles[i].findex * 3 + 1];
		vtxAttribs[numOfFloat * i + 32] = facetnorms[triangles[i].findex * 3 + 2];
	}
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * XiaoPingGuo->numtriangles * numOfFloat, vtxAttribs, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	texture1 = XiaoPingGuo->textures[0].id;
	texture2 = XiaoPingGuo->textures[1].id;

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 0);
}




void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1e-2, 1e2);
	GLfloat pmat4[16];
	glGetFloatv(GL_PROJECTION_MATRIX, pmat4);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 
	gluLookAt(eyex, eyey, eyez,
			   0.0,  0.0,  0.0,
			   0.0,  1.0,  0.0);

	GLfloat mvmat4_light[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat4_light);
	
	glRotatef(rotateAngle, 0.0f, 1.0f, 0.0f);
	GLfloat mvmat4_apple[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, mvmat4_apple);

	// Make sure glmLoadOBJ work. If it works, remove this line.
	// glmDraw(XiaoPingGuo, GLM_TEXTURE);

	// phong shading
	if (currentShader == 0)
	{
		GLint loc_pmat		  = glGetUniformLocation(phongShader, "u_projection");
		GLint loc_mvmat_light = glGetUniformLocation(phongShader, "u_modelview_light");
		GLint loc_mvmat_apple = glGetUniformLocation(phongShader, "u_modelview_apple");
		GLint loc_lightPos	  = glGetUniformLocation(phongShader, "u_light_pos");
		GLint loc_viewPos	  = glGetUniformLocation(phongShader, "u_view_pos");
		GLint loc_ka		  = glGetUniformLocation(phongShader, "u_ka");
		GLint loc_ks		  = glGetUniformLocation(phongShader, "u_ks");
		GLint loc_shininess   = glGetUniformLocation(phongShader, "u_shininess");
		GLint loc_texture	  = glGetUniformLocation(phongShader, "u_texture");

		glUseProgram(phongShader);
			glUniformMatrix4fv(loc_pmat		  , 1, GL_FALSE, pmat4);
			glUniformMatrix4fv(loc_mvmat_light, 1, GL_FALSE, mvmat4_light);
			glUniformMatrix4fv(loc_mvmat_apple, 1, GL_FALSE, mvmat4_apple);

			glUniform4f(loc_lightPos, light_pos[0], light_pos[1], light_pos[2], 1.0f);
			glUniform4f(loc_viewPos, eyex, eyey, eyez, 1.0f);

			// apple skin
			glUniform3f(loc_ka, XiaoPingGuo->materials[2].ambient[0],
								XiaoPingGuo->materials[2].ambient[1],
								XiaoPingGuo->materials[2].ambient[2]);
			glUniform3f(loc_ks, XiaoPingGuo->materials[2].specular[0],
								XiaoPingGuo->materials[2].specular[1],
								XiaoPingGuo->materials[2].specular[2]);
			glUniform1f(loc_shininess, XiaoPingGuo->materials[2].shininess);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES, 
						 0, 
						 XiaoPingGuo->groups->next->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);

			// apple stem
			glUniform3f(loc_ka, XiaoPingGuo->materials[3].ambient[0],
								XiaoPingGuo->materials[3].ambient[1],
								XiaoPingGuo->materials[3].ambient[2]);
			glUniform3f(loc_ks, XiaoPingGuo->materials[3].specular[0],
								XiaoPingGuo->materials[3].specular[1],
								XiaoPingGuo->materials[3].specular[2]);
			glUniform1f(loc_shininess, XiaoPingGuo->materials[3].shininess);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES, 
						 XiaoPingGuo->groups->next->numtriangles * 3,
						 XiaoPingGuo->groups->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);

		glUseProgram(0);
	}
	// flat shading
	else if (currentShader == 1)
	{
		GLint loc_pmat		  = glGetUniformLocation(flatShader, "u_projection");
		GLint loc_mvmat_light = glGetUniformLocation(flatShader, "u_modelview_light");
		GLint loc_mvmat_apple = glGetUniformLocation(flatShader, "u_modelview_apple");
		GLint loc_lightPos	  = glGetUniformLocation(flatShader, "u_light_pos");
		GLint loc_ka		  = glGetUniformLocation(flatShader, "u_ka");
		GLint loc_texture	  = glGetUniformLocation(flatShader, "u_texture");

		glUseProgram(flatShader);
			glUniformMatrix4fv(loc_pmat		  , 1, GL_FALSE, pmat4);
			glUniformMatrix4fv(loc_mvmat_light, 1, GL_FALSE, mvmat4_light);
			glUniformMatrix4fv(loc_mvmat_apple, 1, GL_FALSE, mvmat4_apple);

			glUniform4f(loc_lightPos, light_pos[0], light_pos[1], light_pos[2], 1.0f);

			// apple skin
			glUniform3f(loc_ka, XiaoPingGuo->materials[2].ambient[0],
								XiaoPingGuo->materials[2].ambient[1],
								XiaoPingGuo->materials[2].ambient[2]);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES, 0, XiaoPingGuo->groups->next->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);

			// apple stem
			glUniform3f(loc_ka, XiaoPingGuo->materials[3].ambient[0],
								XiaoPingGuo->materials[3].ambient[1],
								XiaoPingGuo->materials[3].ambient[2]);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES, 
						 XiaoPingGuo->groups->next->numtriangles * 3,
						 XiaoPingGuo->groups->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glUseProgram(0);
	}
	// toon shading
	else if (currentShader == 2)
	{
		GLint loc_pmat		  = glGetUniformLocation(toonShader, "u_projection");
		GLint loc_mvmat_light = glGetUniformLocation(toonShader, "u_modelview_light");
		GLint loc_mvmat_apple = glGetUniformLocation(toonShader, "u_modelview_apple");
		GLint loc_lightPos	  = glGetUniformLocation(toonShader, "u_light_pos");
		GLint loc_texture	  = glGetUniformLocation(toonShader, "u_texture");

		glUseProgram(toonShader);
			glUniformMatrix4fv(loc_pmat, 1, GL_FALSE, pmat4);
			glUniformMatrix4fv(loc_mvmat_light, 1, GL_FALSE, mvmat4_light);
			glUniformMatrix4fv(loc_mvmat_apple, 1, GL_FALSE, mvmat4_apple);

			glUniform4f(loc_lightPos, light_pos[0], light_pos[1], light_pos[2], 1.0f);

			// apple skin
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES, 0, XiaoPingGuo->groups->next->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);

			// apple stem
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES,
						 XiaoPingGuo->groups->next->numtriangles * 3,
						 XiaoPingGuo->groups->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glUseProgram(0);
	}
	// gouraud shading
	else if (currentShader == 3)
	{
		GLint loc_pmat		  = glGetUniformLocation(gouraudShader, "u_projection");
		GLint loc_mvmat_light = glGetUniformLocation(gouraudShader, "u_modelview_light");
		GLint loc_mvmat_apple = glGetUniformLocation(gouraudShader, "u_modelview_apple");
		GLint loc_lightPos	  = glGetUniformLocation(gouraudShader, "u_light_pos");
		GLint loc_viewPos	  = glGetUniformLocation(gouraudShader, "u_view_pos");
		GLint loc_ka		  = glGetUniformLocation(gouraudShader, "u_ka");
		GLint loc_ks		  = glGetUniformLocation(gouraudShader, "u_ks");
		GLint loc_shininess   = glGetUniformLocation(gouraudShader, "u_shininess");
		GLint loc_texture	  = glGetUniformLocation(gouraudShader, "u_texture");

		glUseProgram(gouraudShader);
			glUniformMatrix4fv(loc_pmat, 1, GL_FALSE, pmat4);
			glUniformMatrix4fv(loc_mvmat_light, 1, GL_FALSE, mvmat4_light);
			glUniformMatrix4fv(loc_mvmat_apple, 1, GL_FALSE, mvmat4_apple);

			glUniform4f(loc_lightPos, light_pos[0], light_pos[1], light_pos[2], 1.0f);
			glUniform4f(loc_viewPos, eyex, eyey, eyez, 1.0f);

			// apple skin
			glUniform3f(loc_ka, XiaoPingGuo->materials[2].ambient[0],
								XiaoPingGuo->materials[2].ambient[1],
								XiaoPingGuo->materials[2].ambient[2]);
			glUniform3f(loc_ks, XiaoPingGuo->materials[2].specular[0],
								XiaoPingGuo->materials[2].specular[1],
								XiaoPingGuo->materials[2].specular[2]);
			glUniform1f(loc_shininess, XiaoPingGuo->materials[2].shininess);
			glBindTexture(GL_TEXTURE_2D, texture1);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES,
						 0,
					 	 XiaoPingGuo->groups->next->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);

			// apple stem
			glUniform3f(loc_ka, XiaoPingGuo->materials[3].ambient[0],
								XiaoPingGuo->materials[3].ambient[1],
								XiaoPingGuo->materials[3].ambient[2]);
			glUniform3f(loc_ks, XiaoPingGuo->materials[3].specular[0],
								XiaoPingGuo->materials[3].specular[1],
								XiaoPingGuo->materials[3].specular[2]);
			glUniform1f(loc_shininess, XiaoPingGuo->materials[3].shininess);
			glBindTexture(GL_TEXTURE_2D, texture2);
			glUniform1i(loc_texture, 0);
			glDrawArrays(GL_TRIANGLES,
						 XiaoPingGuo->groups->next->numtriangles * 3,
						 XiaoPingGuo->groups->numtriangles * 3);
			glBindTexture(GL_TEXTURE_2D, NULL);
		glUseProgram(0);
	}
	else
	{
	}

	rotateAngle += 0.015f;

	glutSwapBuffers();
}




void reshape(int width, int height)
{
	glViewport(0, 0, width, height);
}




void keyboard(unsigned char key, int x, int y)
{
	switch (key)
		{
		case 27:
		{	// esc
			exit(0);
			break;
		}
		case 32:
		{   // space
			if (currentShader == 0)
				currentShader = 1;
			else if (currentShader == 1)
				currentShader = 2;
			else if (currentShader == 2)
				currentShader = 3;
			else if (currentShader == 3)
				currentShader = 0;
			
			break;
		}
		case '1':
		{	// phong shading
			currentShader = 0;
			break;
		}
		case '2':
		{	// flat shading
			currentShader = 1;
			break;
		}
		case '3':
		{	// toon shading
			currentShader = 2;
			break;
		}
		case '4':
		{	// gouraud shading
			currentShader = 3;
			break;
		}
		case 'd':
		{
			eyex += 0.1;
			break;
		}
		case 'a':
		{
			eyex -= 0.1;
			break;
		}
		case 'w':
		{
			eyey += 0.1;
			break;
		}
		case 's':
		{
			eyey -= 0.1;
			break;
		}
		default:
		{
			break;
		}
	}
}




void idle(void)
{
	glutPostRedisplay();
}




int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("OpenGL HW3 - 0456031");
	glutReshapeWindow(512, 512);

	glewInit();

	init();

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	glmDelete(XiaoPingGuo);
	return 0;
}