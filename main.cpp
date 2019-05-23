#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>

#include <iostream>
#include "MyModel.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void processInput(GLFWwindow *window);
unsigned int LoadMyTexture(const char *myTexturePath);
void DrawMyTexture(unsigned int ShaderID, unsigned int TextureID);

// ���ó�ʼ���ڴ�С
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

//����FPS
const float FPS = 60.0f;
float pF = 1.0f / FPS;

// �������ʼ��
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//�����ƶ��ӽ�ʱ����¼����ƶ�
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;
bool mouseButtonDown = false;
bool cursorActive = true;

// ��¼ʱ����
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//������ת�ٶ�
float rotateRate = 2.0f;

//�������󣬼�¼��һ�ε������λ��
glm::vec2 initPos;
glm::vec2  curPos;

glm::vec2 iMouse;
glm::vec2 iResolution=glm::vec2(SCR_WIDTH,SCR_HEIGHT);
int main()
{
    //��ʼ����������Ϣ
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    //��������
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Demo_SK", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //���ô��ڵ����ص�������ƶ��ص��������ֻص���������ص�
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // �Ƿ���ʾ���
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //��ʼ��GLAD�����ڷ���OpenGL�淶�ӿ�
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //������Ȳ���
    // glEnable(GL_DEPTH_TEST);

    //��ȡ������Shader
    Shader ourShader("shader_vs.vs", "GrassyHill.glsl");

    //����ģ��
    // Model ourModel(("Model/Plane.dae"));
    MyModel mymodel;

    // �߿�ģʽ
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //����ģ�ͳ�ʼλ�á���С��Ϣ
    // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int MyTexture= LoadMyTexture("texture.jpg");
    //��Ⱦ
    while (!glfwWindowShouldClose(window))
    {
        //��ȡʱ����
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //��ȡ����
        processInput(window);

        //���ñ�����ɫ
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // ����Shader
        ourShader.use();

        //����任
        // glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 view = camera.GetViewMatrix();
        // ourShader.setMat4("projection", projection);
        // ourShader.setMat4("view", view);
        // ourShader.setMat4("model", objRotateModel * model);

        //���������λ��
        // ourShader.setVec3("c_Pos", camera.Position);
        ourShader.setFloat("iTime",currentFrame);
        ourShader.setVec2("iMouse",iMouse+curPos);
        ourShader.setVec2("iResolution0",iResolution);
        DrawMyTexture(ourShader.ID, MyTexture);
        // ��Ⱦģ��
        // ourModel.Draw(ourShader);
        mymodel.Draw();

        //��������
        glfwSwapBuffers(window);
        glfwPollEvents();

        //��ˢ�¼��С��Ԥ��֡���ʱ���ȴ����Դ�����֡��
        if (deltaTime < pF)
        {
            Sleep(1000 * (pF - deltaTime));
        }
    }
    
    //�ͷ���Դ����ֹ
    glfwTerminate();
    return 0;
}

//�������
void processInput(GLFWwindow *window)
{
    //�˳�����
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // ������ƶ�
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // //��ס��Alt���������
    // if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    // {
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //     cursorActive = true;
    // }

    // //�ͷ���ALT���������
    // if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    // {
    //     cursorActive = false;
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // }
}

//��������¼���ڴ�С
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    iResolution=glm::vec2(SCR_WIDTH,SCR_HEIGHT);
    glViewport(0, 0, width, height);
}

//����ƶ��ص�
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    //����갴ס�϶�ʱ��������굱ǰλ�ü�����ת
    if (cursorActive)
    {
        
        firstMouse = true;
        if (mouseButtonDown)
        {
    
            curPos=glm::vec2((float)xpos-initPos.x,(float)ypos-initPos.y);//-----------------------------------
            //������ͬ�����λ��ʱ����ת����
        }
        return;
    }

    //���������ʱ���ƶ���꼴�ɱ任�ӽ�
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

//����갴������ʱ����¼���λ��
//���ͷ���갴��ʱ������ǰ��ת�ϲ��������λ�þ���
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        //������ͬ�����λ��ʱ����ת����
        double xpos,ypos;
        glfwGetCursorPos(window,&xpos,&ypos);
        initPos=glm::vec2((float)xpos,(float)ypos);
        mouseButtonDown = true;
    }
    else if (action == GLFW_RELEASE)
    {
        iMouse+=curPos;
        curPos=glm::vec2(0.0f);
        mouseButtonDown = false;
    }
    return;
}

//�����ֵ����ӽǴ�С
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

//����·��������ͼ����������ͼID
unsigned int LoadMyTexture(const char *myTexturePath)
{
    unsigned int texture2;
    glGenTextures(1, &texture2);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(myTexturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        //������ͼ�ظ���ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //������������ӳ�䷽ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture2;
}

//����MyTexture
void DrawMyTexture(unsigned int ShaderID, unsigned int TextureID)
{
    int i = 5;
    glActiveTexture(GL_TEXTURE0 + i);
    glUniform1i(glGetUniformLocation(ShaderID, "MyTexture"), i);
    glBindTexture(GL_TEXTURE_2D, TextureID);
}