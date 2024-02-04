//
//  pointLight.h
//  test
//
//  Created by Nazirul Hasan on 22/9/23.
//

#ifndef Light_h
#define Light_h

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shader.h"

class PointLight {
public:
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    int lightNumber; //for number of light sources

    PointLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, int num) {

        position = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        lightNumber = num;
    }
    void setUpPointLight(Shader& lightingShader)
    {
        lightingShader.use();

        if (lightNumber == 1) {
            lightingShader.setVec3("pointLights[0].position", position);//0 means light source 0
            lightingShader.setVec3("pointLights[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[0].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[0].k_c", k_c);
            lightingShader.setFloat("pointLights[0].k_l", k_l);
            lightingShader.setFloat("pointLights[0].k_q", k_q);
        }

        else if (lightNumber == 2)
        {
            lightingShader.setVec3("pointLights[1].position", position);
            lightingShader.setVec3("pointLights[1].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[1].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[1].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[1].k_c", k_c);
            lightingShader.setFloat("pointLights[1].k_l", k_l);
            lightingShader.setFloat("pointLights[1].k_q", k_q);
        }
        else if (lightNumber == 3)
        {
            lightingShader.setVec3("pointLights[2].position", position);
            lightingShader.setVec3("pointLights[2].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[2].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[2].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[2].k_c", k_c);
            lightingShader.setFloat("pointLights[2].k_l", k_l);
            lightingShader.setFloat("pointLights[2].k_q", k_q);
        }
        else
        {
            lightingShader.setVec3("pointLights[3].position", position);
            lightingShader.setVec3("pointLights[3].ambient", ambientOn * ambient);
            lightingShader.setVec3("pointLights[3].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("pointLights[3].specular", specularOn * specular);
            lightingShader.setFloat("pointLights[3].k_c", k_c);
            lightingShader.setFloat("pointLights[3].k_l", k_l);
            lightingShader.setFloat("pointLights[3].k_q", k_q);
        }
    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};




class DirectionalLight {
public:
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    int lightNumber; //for number of light sources

    DirectionalLight(float posX, float posY, float posZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, int num) {

        direction = glm::vec3(posX, posY, posZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        lightNumber = num;
    }
    void setUpDirectionalLight(Shader& lightingShader)
    {
        lightingShader.use();
        if (lightNumber == 1) {
            lightingShader.setVec3("directionalLight[0].direction", direction);//0 means light source 0
            lightingShader.setVec3("directionalLight[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("directionalLight[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("directionalLight[0].specular", specularOn * specular);
        }
        else
        {
            lightingShader.setVec3("directionalLight[1].direction", direction);//0 means light source 0
            lightingShader.setVec3("directionalLight[1].ambient", ambientOn * ambient);
            lightingShader.setVec3("directionalLight[1].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("directionalLight[1].specular", specularOn * specular);
        }
    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};


class SpotLight {
public:
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float k_c;
    float k_l;
    float k_q;
    float cos_theta;
    int lightNumber; //for number of light sources

    SpotLight(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic, float theta, int num) {

        position = glm::vec3(posX, posY, posZ);
        direction = glm::vec3(dirX, dirY, dirZ);
        ambient = glm::vec3(ambR, ambG, ambB);
        diffuse = glm::vec3(diffR, diffG, diffB);
        specular = glm::vec3(specR, specG, specB);
        k_c = constant;
        k_l = linear;
        k_q = quadratic;
        cos_theta = glm::cos(glm::radians(theta));
        lightNumber = num;
    }
    void setUpSpotLight(Shader& lightingShader)
    {
        lightingShader.use();

        if (lightNumber == 1) {
            lightingShader.setVec3("spotLight[0].position", position);
            lightingShader.setVec3("spotLight[0].direction", direction);
            lightingShader.setVec3("spotLight[0].ambient", ambientOn * ambient);
            lightingShader.setVec3("spotLight[0].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("spotLight[0].specular", specularOn * specular);
            lightingShader.setFloat("spotLight[0].k_c", k_c);
            lightingShader.setFloat("spotLight[0].k_l", k_l);
            lightingShader.setFloat("spotLight[0].k_q", k_q);
            lightingShader.setFloat("spotLight[0].cos_theta", cos_theta);
        }
        else
        {
            lightingShader.setVec3("spotLight[1].position", position);
            lightingShader.setVec3("spotLight[1].direction", direction);
            lightingShader.setVec3("spotLight[1].ambient", ambientOn * ambient);
            lightingShader.setVec3("spotLight[1].diffuse", diffuseOn * diffuse);
            lightingShader.setVec3("spotLight[1].specular", specularOn * specular);
            lightingShader.setFloat("spotLight[1].k_c", k_c);
            lightingShader.setFloat("spotLight[1].k_l", k_l);
            lightingShader.setFloat("spotLight[1].k_q", k_q);
            lightingShader.setFloat("spotLight[1].cos_theta", cos_theta);
        }

    }
    void turnOff()
    {
        ambientOn = 0.0;
        diffuseOn = 0.0;
        specularOn = 0.0;
    }
    void turnOn()
    {
        ambientOn = 1.0;
        diffuseOn = 1.0;
        specularOn = 1.0;
    }
    void turnAmbientOn()
    {
        ambientOn = 1.0;
    }
    void turnAmbientOff()
    {
        ambientOn = 0.0;
    }
    void turnDiffuseOn()
    {
        diffuseOn = 1.0;
    }
    void turnDiffuseOff()
    {
        diffuseOn = 0.0;
    }
    void turnSpecularOn()
    {
        specularOn = 1.0;
    }
    void turnSpecularOff()
    {
        specularOn = 0.0;
    }
private:
    float ambientOn = 1.0;
    float diffuseOn = 1.0;
    float specularOn = 1.0;
};

#endif /* Light_h */
