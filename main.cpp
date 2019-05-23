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

// 设置初始窗口大小
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

//限制FPS
const float FPS = 60.0f;
float pF = 1.0f / FPS;

// 摄像机初始化
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

//用于移动视角时，记录鼠标移动
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

bool firstMouse = true;
bool mouseButtonDown = false;
bool cursorActive = true;

// 记录时间间隔
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//物体旋转速度
float rotateRate = 2.0f;

//呼出鼠标后，记录第一次点击鼠标的位置
glm::vec2 initPos;
glm::vec2  curPos;

glm::vec2 iMouse;
glm::vec2 iResolution=glm::vec2(SCR_WIDTH,SCR_HEIGHT);
int main()
{
    //初始化，设置信息
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    //创建窗口
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Demo_SK", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //设置窗口调整回调、鼠标移动回调、鼠标滚轮回调、鼠标点击回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // 是否显示鼠标
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    //初始化GLAD，用于访问OpenGL规范接口
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //开启深度测试
    // glEnable(GL_DEPTH_TEST);

    //读取、编译Shader
    Shader ourShader("shader_vs.vs", "GrassyHill.glsl");

    //载入模型
    // Model ourModel(("Model/Plane.dae"));
    MyModel mymodel;

    // 线框模式
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //设置模型初始位置、大小信息
    // model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // // model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    // model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int MyTexture= LoadMyTexture("texture.jpg");
    //渲染
    while (!glfwWindowShouldClose(window))
    {
        //获取时间间隔
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //获取输入
        processInput(window);

        //设置背景颜色
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 启用Shader
        ourShader.use();

        //坐标变换
        // glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // glm::mat4 view = camera.GetViewMatrix();
        // ourShader.setMat4("projection", projection);
        // ourShader.setMat4("view", view);
        // ourShader.setMat4("model", objRotateModel * model);

        //传入摄像机位置
        // ourShader.setVec3("c_Pos", camera.Position);
        ourShader.setFloat("iTime",currentFrame);
        ourShader.setVec2("iMouse",iMouse+curPos);
        ourShader.setVec2("iResolution0",iResolution);
        DrawMyTexture(ourShader.ID, MyTexture);
        // 渲染模型
        // ourModel.Draw(ourShader);
        mymodel.Draw();

        //交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();

        //当刷新间隔小于预设帧间隔时，等待，以此限制帧率
        if (deltaTime < pF)
        {
            Sleep(1000 * (pF - deltaTime));
        }
    }
    
    //释放资源，终止
    glfwTerminate();
    return 0;
}

//检查输入
void processInput(GLFWwindow *window)
{
    //退出窗口
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // 摄像机移动
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    // //按住左Alt键呼出鼠标
    // if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    // {
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    //     cursorActive = true;
    // }

    // //释放左ALT键隐蔽鼠标
    // if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE)
    // {
    //     cursorActive = false;
    //     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    // }
}

//调整并记录窗口大小
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{

    SCR_WIDTH = width;
    SCR_HEIGHT = height;
    iResolution=glm::vec2(SCR_WIDTH,SCR_HEIGHT);
    glViewport(0, 0, width, height);
}

//鼠标移动回调
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    //当鼠标按住拖动时，根据鼠标当前位置计算旋转
    if (cursorActive)
    {
        
        firstMouse = true;
        if (mouseButtonDown)
        {
    
            curPos=glm::vec2((float)xpos-initPos.x,(float)ypos-initPos.y);//-----------------------------------
            //调整不同摄像机位置时的旋转方向
        }
        return;
    }

    //当鼠标隐蔽时，移动鼠标即可变换视角
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

//当鼠标按键按下时，记录鼠标位置
//当释放鼠标按键时，将当前旋转合并至物体的位置矩阵
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        //调整不同摄像机位置时的旋转方向
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

//鼠标滚轮调节视角大小
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

//根据路径加载贴图，并返回贴图ID
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

        //设置贴图重复方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //设置纹理坐标映射方式
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

//绘制MyTexture
void DrawMyTexture(unsigned int ShaderID, unsigned int TextureID)
{
    int i = 5;
    glActiveTexture(GL_TEXTURE0 + i);
    glUniform1i(glGetUniformLocation(ShaderID, "MyTexture"), i);
    glBindTexture(GL_TEXTURE_2D, TextureID);
}