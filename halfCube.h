#ifndef halfCube_h
#define halfCube_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class halfCube {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    halfCube()
    {
        setUphalfCubeVertexDataAndConfigureVertexAttribute(); //lab 2 er moto halfCube
    }

    halfCube(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUphalfCubeVertexDataAndConfigureVertexAttribute(); //lab 3 er mto halfCube
    }

    halfCube(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUphalfCubeVertexDataAndConfigureVertexAttribute(); //texture
    }

    // destructor
    ~halfCube()
    {
        glDeleteVertexArrays(1, &halfCubeVAO);
        glDeleteVertexArrays(1, &lighthalfCubeVAO);
        glDeleteVertexArrays(1, &lightTexhalfCubeVAO);
        glDeleteBuffers(1, &halfCubeVBO);
        glDeleteBuffers(1, &halfCubeEBO);
    }

    void drawhalfCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0); //koto tomo texture
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexhalfCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawhalfCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lighthalfCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawhalfCube1(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(halfCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
    void drawhalfCube2(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
        lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
        lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setFloat("material.shininess", 32.0f);
        lightingShader.setVec3("material.emissive", glm::vec3(0.0f, 0.0f, 0.0f));

        lightingShader.setMat4("model", model);

        glBindVertexArray(lighthalfCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int halfCubeVAO;
    unsigned int lighthalfCubeVAO;
    unsigned int lightTexhalfCubeVAO;
    unsigned int halfCubeVBO;
    unsigned int halfCubeEBO;

    void setUphalfCubeVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float halfCube_vertices[] = {
            // positions      // normals         // texture
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
            0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

            -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
            -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

            0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
            0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
            0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
        };
        unsigned int halfCube_indices[] = {
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

        glGenVertexArrays(1, &halfCubeVAO);
        glGenVertexArrays(1, &lighthalfCubeVAO);
        glGenVertexArrays(1, &lightTexhalfCubeVAO);
        glGenBuffers(1, &halfCubeVBO);
        glGenBuffers(1, &halfCubeEBO);


        glBindVertexArray(lightTexhalfCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, halfCubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(halfCube_vertices), halfCube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, halfCubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(halfCube_indices), halfCube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lighthalfCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, halfCubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, halfCubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(halfCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, halfCubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, halfCubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};


#endif /* halfCube_h */
