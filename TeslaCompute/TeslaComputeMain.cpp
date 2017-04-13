
#include"pgl_mesh.h"
#include "TeslaCompute.h"

using std::cout;
using std::string;
using std::ifstream;
using std::ofstream;

glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
float cameraZoom = 3.0f;
float cameraX = 0.0f;


GLfloat eye_world_pos [] = {0.0,0.0,0.0};
GLfloat light_dir [] = {125.0,125.0,0.0};
GLfloat light_color [] = {0.5,0.5,1.0};

string vertex_source;
string fragment_source;
string tcs_shader_source;
string tes_shader_source;


GLuint vs_shader_object;
GLuint fs_shader_object;
GLuint render_prog;

GLuint MVP_Location;
GLuint worldMatrix_loc;

Mesh *p_Mesh;

bool LoadModel()
{
	p_Mesh = new Mesh();
	if(!p_Mesh->LoadMesh("../Content/hheli.obj"))
	{
		printf("Error Initializing Mesh!!!!\n");
		return false;
	}
return true;
}

bool ReadFile(const char* pFileName, string& outFile)
{
    ifstream f(pFileName);
    
    bool ret = false;
    
    if (f.is_open()) {
        string line;
        while (getline(f, line)) {
            outFile.append(line);
            outFile.append("\n");
        }
        
        f.close();
        
        ret = true;
    }
    else {
       cout << "Error Loading file";
    }
    
    return ret;
}



bool InitShaders()
{
	int success;


	ReadFile("ON_shader.vs",vertex_source);
	ReadFile("ON_shader.fs",fragment_source);

	const char* p[1];
	p[0] = vertex_source.c_str();
	GLint Lengths[1] = { (GLint)vertex_source.size() };

	vs_shader_object = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_shader_object,1,p,NULL);
	glCompileShader(vs_shader_object);
	glGetShaderiv(vs_shader_object, GL_COMPILE_STATUS, &success);
	
    if (!success) {
        char InfoLog[1024];
        glGetShaderInfoLog(vs_shader_object, 1024, NULL, InfoLog);
        std::cout <<"Error compiling : '%s'\n" <<InfoLog;
        return false;
    }

	const char* p1[1];
	p1[0] = fragment_source.c_str();
	GLint Lengths1[1] = { (GLint)fragment_source.size() };

	fs_shader_object = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_shader_object,1,p1,NULL);
	glCompileShader(fs_shader_object);
	glGetShaderiv(fs_shader_object, GL_COMPILE_STATUS, &success);
	
    if (!success) {
        char InfoLog[1024];
        glGetShaderInfoLog(fs_shader_object, 1024, NULL, InfoLog);
        std::cout <<"Error compiling : '%s'\n" <<InfoLog;
        return false;
    }


	render_prog = glCreateProgram();
	glAttachShader(render_prog,vs_shader_object);
	glAttachShader(render_prog,fs_shader_object);

	glLinkProgram(render_prog);
	glGetProgramiv(render_prog, GL_LINK_STATUS, &success);
	
    if (!success) {
        char InfoLog[1024];
        glGetProgramInfoLog(render_prog, 1024, NULL, InfoLog);
        std::cout <<"Error Linking: '%s'\n" <<InfoLog;
        return false;
    }

	glValidateProgram(render_prog);
	glGetProgramiv(render_prog, GL_VALIDATE_STATUS, &success);
    if (!success) {
		char InfoLog[1024];
		glGetProgramInfoLog(render_prog, 1024, NULL, InfoLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", InfoLog);
     return false;
    }

	glDetachShader(render_prog, vs_shader_object);
	glDetachShader(render_prog, fs_shader_object);
	glUseProgram(render_prog);

	MVP_Location = glGetUniformLocation(render_prog, "MVP_matrix");
	worldMatrix_loc = glGetUniformLocation(render_prog, "World_matrix");

return true;
}

void Key(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'a' :
		cameraZoom -= 0.1;
		break;
	case 'z':
		cameraZoom +=0.1;
		break;
	case 'b':
		cameraX +=0.1;
		break;
	case 'c':
		cameraX -= 0.1;
	}

}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glUseProgram(render_prog);
	view = glm::lookAt(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,0.0,-5.0),glm::vec3(0.0,1.0,0.0));
	model= glm::translate(glm::mat4(1.0),glm::vec3(0.0,-3.0,-cameraZoom-5));
	model= glm::rotate(model,cameraX,glm::vec3(0.0,1.0,0.0));
	model = glm::scale(model,glm::vec3(0.05,0.05,0.05));
	glm::mat4 MVP = projection*view*model;
	glUniformMatrix4fv(MVP_Location, 1, GL_FALSE , &MVP[0][0]);
	glUniformMatrix4fv(worldMatrix_loc, 1, GL_FALSE , &model[0][0]);
    
	p_Mesh->Render();

	glUseProgram(0);
	glutSwapBuffers();
	glutPostRedisplay();
}

void Clear()
{

glDeleteShader(vs_shader_object);
glDeleteShader(fs_shader_object);
glDeleteProgram(render_prog);
SAFE_DELETE(p_Mesh);

}

void ChangeSize(int w, int h)
{
	glClearColor(0.0,0.0,0.0,1.0);
	glViewport(0,0,w,h);
	projection = glm::perspective(45.0f,w/(float)h,0.1f, 1000.0f);
}

int main(int argc, char* argv[])
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	
	glutInitWindowPosition(400,400);
	glutInitWindowSize(400,400);
	glutCreateWindow("TeslaTessellate");

	GLenum res = glewInit();
    if (res != GLEW_OK) {
       std::cout<<"Error: '%s'\n"<<glewGetErrorString(res);
        return false;
    }

	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);
	glutReshapeFunc(ChangeSize);
	
	InitShaders();
	LoadModel();
	glutMainLoop();

	Clear();
	return 0;
}

