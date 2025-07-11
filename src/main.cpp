#include <cstdint>
#include <cstdio>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <sstream>
#include <vector>

GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path){
	// Create shaders
	GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	// read vertex code from file
	std::string vertex_shader_code;
	std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
	if(vertex_shader_stream.is_open()){
		std::stringstream sstr;
		sstr << vertex_shader_stream.rdbuf();
		vertex_shader_code = sstr.str();
		vertex_shader_stream.close();
	}
	else{
		printf("Impossible to open %s. File does not exist\n", vertex_file_path);
		getchar();
		return 0;
	}

	// read fragment code from file
	std::string fragment_shader_code;
	std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
	if(fragment_shader_stream.is_open()){
		std::stringstream sstr;
		sstr << fragment_shader_stream.rdbuf();
		fragment_shader_code = sstr.str();
		fragment_shader_stream.close();
	}
	else{
		printf("Impossible to open %s. File does not exist\n", fragment_file_path);
		getchar();
		return 0;
	}

	GLint result = GL_FALSE;
	int info_log_length;

	// Compile vertex shader
	printf("Compiling shader: %s\n", vertex_file_path);
	char const* vertex_source_pointer = vertex_shader_code.c_str();
	glShaderSource(vertex_shader_id, 1, &vertex_source_pointer, NULL);
	glCompileShader(vertex_shader_id);

	// Check vertex shader
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0){
		std::vector<char> vertex_shader_error_message(info_log_length+1);
		glGetShaderInfoLog(vertex_shader_id, info_log_length, NULL, &vertex_shader_error_message[0]);
		printf("Error: %s\n", &vertex_shader_error_message[0]);
	}

	// Compile fragment shader
	printf("Compiling shader: %s\n", fragment_file_path);
	char const* fragment_source_pointer = fragment_shader_code.c_str();
	glShaderSource(fragment_shader_id, 1, &fragment_source_pointer, NULL);
	glCompileShader(fragment_shader_id);

	// Check fragment shader
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
	if (info_log_length > 0){
		std::vector<char> fragment_shader_error_message(info_log_length+1);
		glGetShaderInfoLog(fragment_shader_id, info_log_length, NULL, &fragment_shader_error_message[0]);
		printf("%s\n", &fragment_shader_error_message[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint program_id = glCreateProgram();
	glAttachShader(program_id, vertex_shader_id);
	glAttachShader(program_id, fragment_shader_id);
	glLinkProgram(program_id);

	// Check program
	printf("Check program\n");
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
	printf("Check if\n");
	if (info_log_length > 0){
		std::vector<char> program_error_message(info_log_length+1);
		glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);
		printf("%s\n", &program_error_message[0]);
	}
	printf("Detach shaders\n");
	glDetachShader(program_id, vertex_shader_id);
	glDetachShader(program_id, fragment_shader_id);

	printf("Delete shaders\n");
	glDeleteShader(vertex_shader_id);
	glDeleteShader(fragment_shader_id);

	printf("Linking completed\n");

	return program_id;


}

int main(){
	glfwInit();
	GLFWwindow* w = glfwCreateWindow(600, 600, "trigle", NULL, NULL);
	glfwMakeContextCurrent(w);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	float t[] =
	{
		-.5f, -.5f, 0.f,
		0.f, .5f, 0.f,
		.5f, -.5f, 0.f,
	};
	uint32_t v;
	glGenBuffers(1, &v);
	glBindBuffer(GL_ARRAY_BUFFER, v);
	glBufferData(GL_ARRAY_BUFFER, sizeof(t)*sizeof(t)/sizeof(t[0]), &t[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void*)0);
	glEnableVertexAttribArray(0);
	glViewport(0, 0, 600, 600);

	GLuint program_id = load_shaders("shaders/vertex.vert", "shaders/fragment.frag");

	while(!glfwWindowShouldClose(w)){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(program_id);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(w);
		glfwPollEvents();
	}

	return 0;
}
