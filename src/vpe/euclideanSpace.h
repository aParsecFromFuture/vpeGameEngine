#ifndef EUCLIDEANSPACE_H
#define EUCLIDEANSPACE_H
#define GLM_ENABLE_EXPERIMENTAL
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

namespace vpe{
    const float G = 0.1f;
    
    struct sphere{
        int id;
        float mass;
        float radius;
        glm::vec3 position;
        glm::vec3 velocity;
        glm::vec3 force;
        
        sphere(int, float, float, glm::vec3, glm::vec3);
        void calculatePosition(float);
        void calculateMotion(float);
        void makeForce(glm::vec3);
        void clearForce();
        glm::vec3 getMomentum();
        static float getForceSize(const sphere&, const sphere&);
        static float getDistance(const sphere&, const sphere &);
        static bool isCollision(const sphere&, const sphere&);
        static void calculateElasticCollision(float, sphere&, sphere&);
    };

    class euclideanSpace
    {
        std::vector<sphere> objects;
        int object_count;
        float delta_time;
    public:
        euclideanSpace();
        ~euclideanSpace();
        void addObject(int, float, float, glm::vec3, glm::vec3=glm::vec3(0, 0, 0));
        sphere& getObject(int);
        void setDeltaTime(float);
        void run();
        glm::vec3 getTotalMomentum();
        int getObjectCount();
    };
};

#endif // EUCLIDEANSPACE_H
