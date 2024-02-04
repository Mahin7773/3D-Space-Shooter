#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "Light.h"
#include "cube.h"
#include "halfCube.h"
#include "sphere.h"
#include "stb_image.h" //img load korar jnno
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void spaceShip(unsigned int& cubeVAO, unsigned int& bezierVAO, unsigned int& bezierVAO2, unsigned int& bezierVAO3, unsigned int& bezierVAO4, Shader& lightingShader, glm::mat4 alTogether);
void drawBoundary(Shader& lightingShaderWithTexture, glm::mat4 altogether, Cube &floor, Cube &roof, Cube &front, Cube &back, Cube& side1, Cube &side2);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector <float>& coordinates,
    vector <float>& normals,
    vector <int>& indices,
    vector <float>& vertices);
void drawBazierCurveObj(Shader& lightingShader, glm::mat4 model, unsigned int& bezierVAO_, float r, float g, float b, float dr, float  dg, float db);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

//for curves
vector <float> cntrlPoints = {
0.0100, 1.7150, 5.1000,
-0.1500, 1.6900, 5.1000,
-0.2700, 1.6450, 5.1000,
-0.2850, 1.5450, 5.1000,
-0.2800, 1.4200, 5.1000,
-0.2800, 1.2900, 5.1000,
-0.2750, 1.1700, 5.1000,
-0.2800, 1.0550, 5.1000,
};
vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;

vector<float> cntrlPoints2 = {
-0.0050, 1.7800, 5.1000,
-0.2250, 1.5900, 5.1000,
-0.4200, 1.3550, 5.1000,
-0.6400, 1.0000, 5.1000,
-0.7600, 0.7050, 5.1000,
};
vector <float> coordinates2;
vector <float> normals2;
vector <int> indices2;
vector <float> vertices2;

vector <float> cntrlPoints3 = {
-0.0350, 1.7600, 5.1000,
-0.2200, 1.6500, 5.1000,
-0.5000, 1.4950, 5.1000,
-0.7850, 1.3550, 5.1000,
-1.0450, 1.2200, 5.1000,
-1.3850, 1.0350, 5.1000,
-1.6600, 0.8900, 5.1000,
-1.8550, 0.7650, 5.1000,
};
vector <float> coordinates3;
vector <float> normals3;
vector <int> indices3;
vector <float> vertices3;

vector<float> cntrlPoints4{
0.0400, 1.7800, 5.1000,
0.1450, 1.7850, 5.1000,
0.2200, 1.7800, 5.1000,
0.3050, 1.7750, 5.1000,
0.3950, 1.7750, 5.1000,
0.4850, 1.7750, 5.1000,
0.4850, 1.7050, 5.1000,
0.4850, 1.6400, 5.1000,
0.4850, 1.5750, 5.1000,
0.4800, 1.5000, 5.1000,
0.4850, 1.4100, 5.1000,
0.4850, 1.3350, 5.1000,
0.4850, 1.2550, 5.1000,
0.4800, 1.1700, 5.1000,
0.4800, 1.0850, 5.1000,
0.4800, 1.0000, 5.1000,
0.4700, 0.9200, 5.1000,
0.4750, 0.8200, 5.1000,
0.4750, 0.7200, 5.1000,
0.4700, 0.6450, 5.1000,
0.4650, 0.5600, 5.1000,
0.4600, 0.4750, 5.1000,
0.4550, 0.3950, 5.1000,
0.4550, 0.3250, 5.1000,
0.4500, 0.2250, 5.1000,
0.4400, 0.1450, 5.1000,
0.4250, 0.0650, 5.1000,
0.4100, -0.0100, 5.1000,
0.2850, -0.0050, 5.1000,
0.2100, -0.0050, 5.1000,
0.1150, -0.0050, 5.1000,
0.0500, -0.0050, 5.1000,
};
vector <float> coordinates4;
vector <float> normals4;
vector <int> indices4;
vector <float> vertices4;

class point
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;

bool loadBezierCurvePoints = true;
bool showHollowBezier = true;
bool lineMode = false;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;

// ship1 movement
float rotateAngle_X1 = 0.0;
float rotateAngle_Y1 = 0.0;
float rotateAngle_Z1 = 0.0;
float rotateAxis_X1 = 0.0;
float rotateAxis_Y1 = 0.0;
float rotateAxis_Z1 = 1.0;
float translate_X1 = 0.0;
float translate_Y1 = 0.0;
float translate_Z1 = 0.0;
float scale_X1 = 1.0;
float scale_Y1 = 1.0;
float scale_Z1 = 1.0;

// ship2 movement
float rotateAngle_X2 = 0.0;
float rotateAngle_Y2 = 0.0;
float rotateAngle_Z2 = 0.0;
float rotateAxis_X2 = 0.0;
float rotateAxis_Y2 = 0.0;
float rotateAxis_Z2 = 1.0;
float translate_X2 = 0.0;
float translate_Y2 = 0.0;
float translate_Z2 = 0.0;
float scale_X2 = 1.0;
float scale_Y2 = 1.0;
float scale_Z2 = 1.0;

//rotor blade
float r1 = 0.0f;
float r2 = 0.0f;
float rotorSpeed = 7.0f;

//firing
float f1 = 0.0f;
float f2 = 0.0f;
float bulletSpeed = 0.5f;
bool fire1Enable = false;
bool fire2Enable = false;


// camera
Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(30.0f, 15.0f, 0.0f),
    glm::vec3(-30.0f, 15.0f, 0.0f),
    glm::vec3(0.0f, 15.0f, -30.0f)
};
PointLight pointlight1(
    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  //position
    0.8f, 0.8f, 0.8f,    //ambient
    0.8f, 0.8f, 0.8f,       //diffuse
    1.0f, 1.0f, 1.0f,       //specular
    1.0f,                   //k_c
    0.027f,                 //k_l
    0.0028f,                //k_q
    1                       //light number
);
PointLight pointlight2(
    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  //position
    0.8f, 0.8f, 0.8f,    //ambient
    0.8f, 0.8f, 0.8f,       //diffuse
    1.0f, 1.0f, 1.0f,       //specular
    1.0f,                   //k_c
    0.027f,                 //k_l
    0.0028f,                //k_q
    2                       //light number
);
PointLight pointlight3(
    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  //position
    0.8f, 0.8f, 0.8f,    //ambient
    0.8f, 0.8f, 0.8f,       //diffuse
    1.0f, 1.0f, 1.0f,       //specular
    1.0f,                   //k_c
    0.027f,                 //k_l
    0.0028f,                //k_q
    3                       //light number
);

DirectionalLight directionalLight(
    0.5f, -3.0f, -3.0f, //direction
    0.2f, 0.2f, 0.2f,   //ambient
    0.8f, 0.8f, 0.8f,   //diffuse
    1.0f, 1.0f, 1.0f,   //specular
    1                   //number
);
DirectionalLight directionalLight2(
    -0.5f, 3.0f, 3.0f,  //direction
    0.2f, 0.2f, 0.2f,   //ambient
    0.8f, 0.8f, 0.8f,   //diffuse
    1.0f, 1.0f, 1.0f,   //specular
    2                   //number
);

SpotLight spotLight(
    10.0f, 5.0f, 0.0f,   //position
    0.0f, -1.0f, 0.0f,  //direction
    0.8f, 0.8f, 0.8f,   //ambient
    0.8f, 0.8f, 0.8f,   //diffuse
    1.0f, 1.0f, 1.0f,   //specular
    1.0f, 0.027f, 0.0028f,//k_c, k_l, k_q
    15.0f,              //theta for cos_theta
    1                   //number         
);
SpotLight spotLight2(
    -10.0f, 5.0f, 0.0f,   //position
    0.0f, -1.0f, 0.0f,  //direction
    0.8f, 0.8f, 0.8f,   //ambient
    0.8f, 0.8f, 0.8f,   //diffuse
    1.0f, 1.0f, 1.0f,   //specular
    1.0f, 0.027f, 0.0028f,//k_c, k_l, k_q
    15.0f,              //theta for cos_theta
    2                   //number         
);

// light settings
bool pointLight1Toggle = true;
bool pointLight2Toggle = true;
bool pointLight3Toggle = true;
bool pointLight4Toggle = true;
bool pointLight5Toggle = true;
bool directionalLight1Toggle = true;
bool directionalLight2Toggle = true;
bool spotLight1Toggle = true;
bool spotLight2Toggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

int main()
{

    printf("\t1\tToggle Point Light 1\n");
    printf("\t2\tToggle Point Light 2\n");
    printf("\t3\tToggle Point Light 3\n");
    printf("\t6\tToggle Directional Light1\n");
    printf("\t7\tToggle Directional Light2\n");
    printf("\t8\tToggle Spot Light1\n");
    printf("\t9\tToggle Spot Light2\n\n");

    printf("\tT\tShip 1 UP\n");
    printf("\tG\tShip 1 DOWN\n");
    printf("\tH\tShip 1 RIGHT\n");
    printf("\tF\tShip 1 LEFT\n");
    printf("\tI\tShip 2 UP\n");
    printf("\tK\tShip 2 DOWN\n");
    printf("\tL\tShip 2 RIGHT\n");
    printf("\tJ\tShip 2 LEFT\n");
    printf("\tQ\tShoot\n");

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //fwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");


    //load textures here
    // ------------------------------------------------------------------
    string diffuseMapPath2 = "w.jfif";
    string specularMapPath2 = "floor_spec.png";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    string diffuseMapPath3 = "w2.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube3 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "black.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube4 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "food.jpg";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube5 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //boundary textures--------------------------------------------------
    diffuseMapPath3 = "front_M.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube floor = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "front_M.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube roof = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "front_M.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube side1 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "front_M.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube side2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "front.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube front = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "front.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube back = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //ship textures------------------------------------------------------
    diffuseMapPath3 = "front.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube shipBody = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "front.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube shipWings = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    diffuseMapPath3 = "front.png";
    diffMap2 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube shipTail = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //curve vao hollowBazier----------------------------------------------
    unsigned int bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1, coordinates, normals, indices, vertices);
    loadBezierCurvePoints = false;
    showHollowBezier = true;
    unsigned int bezierVAO2 = hollowBezier(cntrlPoints2.data(), ((unsigned int)cntrlPoints2.size() / 3) - 1, coordinates2, normals2, indices2, vertices2);
    loadBezierCurvePoints = false;
    showHollowBezier = true;
    unsigned int bezierVAO3 = hollowBezier(cntrlPoints3.data(), ((unsigned int)cntrlPoints3.size() / 3) - 1, coordinates3, normals3, indices3, vertices3);
    loadBezierCurvePoints = false;
    showHollowBezier = true;
    unsigned int bezierVAO4 = hollowBezier(cntrlPoints4.data(), ((unsigned int)cntrlPoints4.size() / 3) - 1, coordinates4, normals4, indices4, vertices4);
    loadBezierCurvePoints = false;
    showHollowBezier = true;
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    float pyramid_vertices[] = {

        // Triangle 1
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        -1.0,-1.0,1.0,
        0.0,1.0,0.0,

        1.0,-1.0,1.0,
        0.0,0.0,1.0,

        //Triangle 2
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        1.0,-1.0,1.0,
        0.0,0.0,1.0,

        1.0,-1.0,-1.0,
        0.0,1.0,0.0,

        //Triangle 3
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        1.0,-1.0,-1.0,
        0.0,1.0,0.0,

        -1.0,-1.0,-1.0,
        0.0,0.0,1.0,

        //Triangle 4
        0.0,1.0,0.0,
        1.0,0.0,0.0,

        -1.0,-1.0,-1.0,
        0.0,0.0,1.0,

        -1.0,-1.0,1.0,
        0.0,1.0,0.0

    };
    unsigned int pyramid_indices[] = {
        // First triangle
        0, 1, 2,
        // Second triangle
        3, 4, 5,
        // Third triangle
        6, 7, 8,
        // Fourth triangle
        9, 10, 11,
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);
    glBindVertexArray(cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    /*unsigned int pyrVAO, pyrVBO, pyrEBO;
    glGenVertexArrays(1, &pyrVAO);
    glGenBuffers(1, &pyrVBO);
    glGenBuffers(1, &pyrEBO);
    glBindVertexArray(pyrVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pyrVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyrEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);*/


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere1 = Sphere(3.5f, 36, 18, glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, glm::vec3(0.05f, 0.0f, 0.0f));
    Sphere sphere2 = Sphere(3.5f, 36, 18, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, glm::vec3(0.0f, 0.05f, 0.0f));
    Sphere sphere3 = Sphere(3.5f, 36, 18, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, glm::vec3(0.05f, 0.05f, 0.05f));


    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //glEnable(GL_DEPTH_TEST);
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        // activate shader
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        //pointlight4.setUpPointLight(lightingShader);
        // directional light 1
        directionalLight.setUpDirectionalLight(lightingShader);
        // directional light 2
        directionalLight2.setUpDirectionalLight(lightingShader);
        //spotlights
        spotLight.setUpSpotLight(lightingShader);
        spotLight2.setUpSpotLight(lightingShader);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix,
            translateMatrix1, rotateXMatrix1, rotateYMatrix1, rotateZMatrix1, scaleMatrix1,
            translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2,
            model, ship1, ship2, model1, model2, rotateShip, translateShip, translateShip2;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


        //bezier curves here-----------------------------------------------------------
        //drawBazierCurveObj(lightingShader, model, bezierVAO, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);

        //spaceships here--------------------------------------------------------------
        //ship1
        translateMatrix1 = glm::translate(identityMatrix, glm::vec3(translate_X1, translate_Y1, translate_Z1));
        rotateXMatrix1 = glm::rotate(identityMatrix, glm::radians(rotateAngle_X1), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix1 = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y1), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix1 = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z1), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix1 = glm::scale(identityMatrix, glm::vec3(scale_X1, scale_Y1, scale_Z1));
        ship1 = translateMatrix1 * rotateXMatrix1 * rotateYMatrix1 * rotateZMatrix1 * scaleMatrix1;
        translateShip = glm::translate(identityMatrix, glm::vec3(-10.0f, 0.0f, 0.0f));
        model1 = ship1 * translateShip;
        spaceShip(cubeVAO, bezierVAO, bezierVAO2, bezierVAO3, bezierVAO4, lightingShader, model1);

        //ship2
        translateMatrix2 = glm::translate(identityMatrix, glm::vec3(translate_X2, translate_Y2, translate_Z2));
        rotateXMatrix2 = glm::rotate(identityMatrix, glm::radians(rotateAngle_X2), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix2 = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y2), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix2 = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z2), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix2 = glm::scale(identityMatrix, glm::vec3(scale_X2, scale_Y2, scale_Z2));
        ship2 = translateMatrix2 * rotateXMatrix2 * rotateYMatrix2 * rotateZMatrix2 * scaleMatrix2;
        rotateShip = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        translateShip2 = glm::translate(identityMatrix, glm::vec3(10.0f, 0.0f, 0.0f));
        model2 = ship2 * rotateShip * translateShip;
        spaceShip(cubeVAO, bezierVAO, bezierVAO2, bezierVAO3, bezierVAO4, lightingShader, model2);



        //spheres objects here---------------------------------------------------------
        glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, pointLightPositions[0]);
        sphere1.drawSphere(lightingShader, modelForSphere);

        modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, pointLightPositions[1]);
        sphere2.drawSphere(lightingShader, modelForSphere);

        modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, pointLightPositions[2]);
        sphere3.drawSphere(lightingShader, modelForSphere);



        // draw the lamp object(s)-----------------------------------------------------
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights---------------------
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 3; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.5f, 0.5f, 0.0f));//color of the bulb
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //draw spotlights--------------------------------------------------------------
        model = glm::mat4(1.0f);
        model = glm::translate(model, spotLight.position);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.5f, 0.0f, 0.0f));//color of the bulb
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        model = glm::mat4(1.0f);
        model = glm::translate(model, spotLight2.position);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.0f, 0.5f, 0.0f));//color of the bulb
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        //now we draw elements with texture
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        //pointlight4.setUpPointLight(lightingShaderWithTexture);
        // directional light 1
        directionalLight.setUpDirectionalLight(lightingShaderWithTexture);
        // directional light 2
        directionalLight2.setUpDirectionalLight(lightingShaderWithTexture);
        //spotlights
        spotLight.setUpSpotLight(lightingShaderWithTexture);
        spotLight2.setUpSpotLight(lightingShaderWithTexture);

        drawBoundary(lightingShaderWithTexture, model, floor, roof, front, back, side1, side2);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawBazierCurveObj(Shader& lightingShader, glm::mat4 model, unsigned int& bezierVAO_, float r = 1.0f, float g = 1.0f, float b = 1.0f, float dr = 0.8f, float  dg = 0.8f, float db = 0.8f) {
    lightingShader.use();
    lightingShader.setMat4("model", model);
    lightingShader.setVec3("material.emission", glm::vec3(0, 0, 0));
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(dr, dg, db));
    lightingShader.setFloat("material.shininess", 32.0f);

    glBindVertexArray(bezierVAO_);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);

    // unbind VAO
    glBindVertexArray(0);
}

void drawBoundary(Shader& lightingShaderWithTexture, glm::mat4 altogether, Cube &floor, Cube& roof, Cube &front, Cube& back, Cube &side1, Cube& side2)
{
    lightingShaderWithTexture.use();
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix = glm::mat4(1.0f);
    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    //floor
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(100.0, 0.5, 100.0));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-50.0f, -50.0f, -50.0f));
    model = translateMatrix * scaleMatrix; //* altogether;
    floor.drawCubeWithTexture(lightingShaderWithTexture, model);


    //front
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(100., 100., .5));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-50.0, -50.0, -50.0));
    model = translateMatrix * scaleMatrix; //* altogether;
    front.drawCubeWithTexture(lightingShaderWithTexture, model);

    //back
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(100., 100., .5));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-50.0, -50.0, 50.0));
    model = translateMatrix * scaleMatrix; //* altogether;
    back.drawCubeWithTexture(lightingShaderWithTexture, model);

    //sides
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, 100., 100.));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(50.0, -50.0, -50.0));
    model = translateMatrix * scaleMatrix; //* altogether;
    side1.drawCubeWithTexture(lightingShaderWithTexture, model);

    scaleMatrix = glm::scale(identityMatrix, glm::vec3(.5, 100., 100.));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-50.0, -50.0, -50.0));
    model = translateMatrix * scaleMatrix; //* altogether;
    side2.drawCubeWithTexture(lightingShaderWithTexture, model);

    //roof
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(100., 0.5, 100.0));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-50.0, 50.0, -50.0));
    model = translateMatrix * scaleMatrix; //* altogether;
    roof.drawCubeWithTexture(lightingShaderWithTexture, model);
}

void drawPyramid(unsigned int& pyrVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emissive", glm::vec3(0.0f, 0.0f, 0.0f));

    lightingShader.setMat4("model", model);

    glBindVertexArray(pyrVAO);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);
    lightingShader.setVec3("material.emissive", glm::vec3(0.0f, 0.0f, 0.0f));

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void spaceShip(unsigned int& cubeVAO, unsigned int& bezierVAO, unsigned int& bezierVAO2, unsigned int& bezierVAO3, unsigned int& bezierVAO4, Shader& lightingShader, glm::mat4 alTogether)
{
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translateB = glm::mat4(1.0f);
    glm::mat4 translateA = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);
    //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));//rotate prototypes
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));

    //cubes------------------------------------------------------------------------------
    //rotor1
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.5));
    translate = glm::translate(model, glm::vec3(-35.0, 1.0, 2.5));
    rotate = glm::rotate(model, glm::radians(r1), glm::vec3(1.0f, 0.0f, 0.0f));
    r1 += rotorSpeed;
    model = alTogether *  scale * translate * rotate;
    halfCube rotor1 = halfCube();
    rotor1.drawhalfCube2(lightingShader, model, 0.3, 0.3, 0.3);
    //rotor2
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.5));
    translate = glm::translate(model, glm::vec3(-35.0, 1.0, -2.5));
    rotate = glm::rotate(model, glm::radians(r2), glm::vec3(1.0f, 0.0f, 0.0f));
    r2 -= rotorSpeed;
    model = alTogether * scale * translate * rotate;
    halfCube rotor2 = halfCube();
    rotor2.drawhalfCube2(lightingShader, model, 0.3, 0.3, 0.3);
    //rotor3
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.5));
    translate = glm::translate(model, glm::vec3(-43.0, 2.5, 0.5));
    rotate = glm::rotate(model, glm::radians(r1), glm::vec3(1.0f, 0.0f, 0.0f));
    //r1 += rotorSpeed;
    model = alTogether * scale * translate * rotate;
    halfCube rotor3 = halfCube();
    rotor3.drawhalfCube2(lightingShader, model, 0.3, 0.3, 0.3);
    //rotor4
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.5));
    translate = glm::translate(model, glm::vec3(-43.0, 2.5, -0.5));
    rotate = glm::rotate(model, glm::radians(r2), glm::vec3(1.0f, 0.0f, 0.0f));
    //r2 -= rotorSpeed;
    model = alTogether * scale * translate * rotate;
    halfCube rotor4 = halfCube();
    rotor4.drawhalfCube2(lightingShader, model, 0.3, 0.3, 0.3);
    //rotor5
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.5));
    translate = glm::translate(model, glm::vec3(-43.0, -2.5, 0.5));
    rotate = glm::rotate(model, glm::radians(r1), glm::vec3(1.0f, 0.0f, 0.0f));
    //r1 += rotorSpeed;
    model = alTogether * scale * translate * rotate;
    halfCube rotor5 = halfCube();
    rotor5.drawhalfCube2(lightingShader, model, 0.3, 0.3, 0.3);
    //rotor6
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(0.1, 0.1, 0.5));
    translate = glm::translate(model, glm::vec3(-43.0, -2.5, -0.5));
    rotate = glm::rotate(model, glm::radians(r2), glm::vec3(1.0f, 0.0f, 0.0f));
    //r2 -= rotorSpeed;
    model = alTogether * scale * translate * rotate;
    halfCube rotor6 = halfCube();
    rotor6.drawhalfCube2(lightingShader, model, 0.3, 0.3, 0.3);

    //fires
    if (fire1Enable)
    {
        model = glm::mat4(1.0f);
        scale = glm::scale(model, glm::vec3(0.5, 0.1, 0.1));
        translate = glm::translate(model, glm::vec3(f1, 0.0, 0.0));
        f1 += bulletSpeed;
        model = alTogether * scale * translate;
        halfCube bullet1 = halfCube();
        bullet1.drawhalfCube2(lightingShader, model, 1.0, 1.0, 1.0);
    }


    //curves---------------------------------------------------------------------------
    //base
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(3.0, 2.0, 2.0));
    translate = glm::translate(model, glm::vec3(0.4, 0.0, 0.0));
    rotate = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO4, 0.3f, 0.3f, 0.3f, 1.0f, 1.0f, 1.0f);
    //engine1
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.2, 1.0, 1.0));
    translate = glm::translate(model, glm::vec3(-4.0, 0.1, -1.25));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO, 0.86f, 0.2f, 0.14f, 1.0f, 1.0f, 1.0f);
    //engine2
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.2, 1.0, 1.0));
    translate = glm::translate(model, glm::vec3(-4.0, 0.1, 1.25));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO, 0.86f, 0.2f, 0.14f, 1.0f, 1.0f, 1.0f);
    //engine3
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.2, 1.0, 1.0));
    translate = glm::translate(model, glm::vec3(-4.7, 0.25, -0.25));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO, 0.86f, 0.2f, 0.14f, 1.0f, 1.0f, 1.0f);
    //engine4
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.2, 1.0, 1.0));
    translate = glm::translate(model, glm::vec3(-4.7, 0.25, 0.25));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO, 0.86f, 0.2f, 0.14f, 1.0f, 1.0f, 1.0f);
    //engine5
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.2, 1.0, 1.0));
    translate = glm::translate(model, glm::vec3(-4.7, -0.25, 0.25));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO, 0.86f, 0.2f, 0.14f, 1.0f, 1.0f, 1.0f);
    //engine6
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.2, 1.0, 1.0));
    translate = glm::translate(model, glm::vec3(-4.7, -0.25, -0.25));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO, 0.86f, 0.2f, 0.14f, 1.0f, 1.0f, 1.0f);
    //front
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.1, 1.1, 1.1));
    translate = glm::translate(model, glm::vec3(0.0, 0., 0.));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO2, 0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f);
    //tail up
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(1.0, 1.5, 0.1));
    translate = glm::translate(model, glm::vec3(-4.4, 0.65, 0.0));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO2, 0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f);
    //wings
    model = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(3.5, 0.05, 2.5));
    translate = glm::translate(model, glm::vec3(-1.8, -6.0, 0.0));
    rotate = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    model = alTogether * scale * translate * rotate;
    drawBazierCurveObj(lightingShader, model, bezierVAO3, 0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f);

}
long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector <float>& coordinates,
    vector <float>& normals,
    vector <int>& indices,
    vector <float>& vertices)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(-nx);
            normals.push_back(-ny);
            normals.push_back(-nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    //ship1 movement
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) translate_Y1 += 0.1;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) translate_Y1 -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) translate_Z1 += 0.1;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) translate_Z1 -= 0.1;
    //ship2 movement
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y2 += 0.1;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y2 -= 0.1;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_Z2 += 0.1;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_Z2 -= 0.1;

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    //if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    //{
    //    eyeX += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    //{
    //    eyeX -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    //{
    //    eyeZ += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    //{
    //    eyeZ -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    //{
    //    eyeY += 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}
    //if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    //{
    //    eyeY -= 2.5 * deltaTime;
    //    basic_camera.changeEye(eyeX, eyeY, eyeZ);
    //}

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //fires
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        if (fire1Enable)
        {
            fire1Enable = false;
            //printf("false");
        }
        else
        {
            fire1Enable = true;
            f1 = 0.0f;
            //printf("true");
        }
    }

    //if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    //{
    //    if (ambientToggle)
    //    {
    //        pointlight1.turnAmbientOff();
    //        pointlight2.turnAmbientOff();
    //        //pointlight3.turnAmbientOff();
    //        //pointlight4.turnAmbientOff();
    //        directionalLight.turnAmbientOff();
    //        directionalLight2.turnAmbientOff();
    //        spotLight.turnAmbientOff();
    //        ambientToggle = !ambientToggle;
    //    }
    //    else
    //    {
    //        pointlight1.turnAmbientOn();
    //        pointlight2.turnAmbientOn();
    //        //pointlight3.turnAmbientOn();
    //        //pointlight4.turnAmbientOn();
    //        directionalLight.turnAmbientOn();
    //        directionalLight2.turnAmbientOn();
    //        spotLight.turnAmbientOn();
    //        ambientToggle = !ambientToggle;
    //    }
    //}
    //if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    //{
    //    if (diffuseToggle)
    //    {
    //        pointlight1.turnDiffuseOff();
    //        pointlight2.turnDiffuseOff();
    //        //pointlight3.turnDiffuseOff();
    //        //pointlight4.turnDiffuseOff();
    //        directionalLight.turnDiffuseOff();
    //        directionalLight2.turnDiffuseOff();
    //        spotLight.turnDiffuseOff();
    //        diffuseToggle = !diffuseToggle;
    //    }
    //    else
    //    {
    //        pointlight1.turnDiffuseOn();
    //        pointlight2.turnDiffuseOn();
    //        //pointlight3.turnDiffuseOn();
    //        //pointlight4.turnDiffuseOn();
    //        directionalLight.turnDiffuseOn();
    //        directionalLight2.turnDiffuseOn();
    //        spotLight.turnDiffuseOn();
    //        diffuseToggle = !diffuseToggle;
    //    }
    //}
    //if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    //{
    //    if (specularToggle)
    //    {
    //        pointlight1.turnSpecularOff();
    //        pointlight2.turnSpecularOff();
    //        //pointlight3.turnSpecularOff();
    //        //pointlight4.turnSpecularOff();
    //        directionalLight.turnSpecularOff();
    //        directionalLight2.turnSpecularOff();
    //        spotLight.turnSpecularOff();
    //        specularToggle = !specularToggle;
    //    }
    //    else
    //    {
    //        pointlight1.turnSpecularOn();
    //        pointlight2.turnSpecularOn();
    //        //pointlight3.turnSpecularOn();
    //        //pointlight4.turnSpecularOn();
    //        directionalLight.turnSpecularOn();
    //        directionalLight2.turnSpecularOn();
    //        spotLight.turnSpecularOn();
    //        specularToggle = !specularToggle;
    //    }
    //}

    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLight1Toggle)
        {
            pointlight1.turnOff();
            pointLight1Toggle = !pointLight1Toggle;
        }
        else
        {
            pointlight1.turnOn();
            pointLight1Toggle = !pointLight1Toggle;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLight2Toggle)
        {
            pointlight2.turnOff();
            pointLight2Toggle = !pointLight2Toggle;
        }
        else
        {
            pointlight2.turnOn();
            pointLight2Toggle = !pointLight2Toggle;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (pointLight3Toggle)
        {
            pointlight3.turnOff();
            pointLight3Toggle = !pointLight3Toggle;
        }
        else
        {
            pointlight3.turnOn();
            pointLight3Toggle = !pointLight3Toggle;
        }
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (pointLight4Toggle)
        {
            //pointlight4.turnOff();
            pointLight4Toggle = !pointLight4Toggle;
        }
        else
        {
            //pointlight4.turnOn();
            pointLight4Toggle = !pointLight4Toggle;
        }
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (pointLight5Toggle)
        {
            //pointlight5.turnOff();
            pointLight5Toggle = !pointLight5Toggle;
        }
        else
        {
            //pointlight5.turnOn();
            pointLight5Toggle = !pointLight5Toggle;
        }
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (directionalLight1Toggle)
        {
            directionalLight.turnOff();
            directionalLight1Toggle = !directionalLight1Toggle;
        }
        else
        {
            directionalLight.turnOn();
            directionalLight1Toggle = !directionalLight1Toggle;
        }
    }
    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        if (directionalLight2Toggle)
        {
            directionalLight2.turnOff();
            directionalLight2Toggle = !directionalLight2Toggle;
        }
        else
        {
            directionalLight2.turnOn();
            directionalLight2Toggle = !directionalLight2Toggle;
        }
    }

    if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
        if (spotLight1Toggle)
        {
            spotLight.turnOff();
            spotLight1Toggle = !spotLight1Toggle;
        }
        else
        {
            spotLight.turnOn();
            spotLight1Toggle = !spotLight1Toggle;
        }
    }
    if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        if (spotLight2Toggle)
        {
            spotLight2.turnOff();
            spotLight2Toggle = !spotLight2Toggle;
        }
        else
        {
            spotLight2.turnOn();
            spotLight2Toggle = !spotLight2Toggle;
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0); //img er path, w and h and cmp mane koyta channel(rgb naki other)
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data); //0 mipmap level
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}