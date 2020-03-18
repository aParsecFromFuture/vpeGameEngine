#include "euclideanSpace.h"

vpe::sphere::sphere(int n, float m, float r, glm::vec3 p, glm::vec3 v){
    id = n;
    mass = m;
    radius = r;
    position = p;
    velocity = v;
    force = glm::vec3(0, 0, 0);
}
        
void vpe::sphere::calculatePosition(float delta_time){
    position = position + delta_time * velocity;
}
        
void vpe::sphere::calculateMotion(float delta_time){
    velocity = velocity + (delta_time / mass) * force;
}
        
void vpe::sphere::makeForce(glm::vec3 f){
    force = force + f;
}
        
void vpe::sphere::clearForce(){
    force = glm::vec3(0, 0, 0);
}

glm::vec3 vpe::sphere::getMomentum(){
    return mass * velocity;
}
        
float vpe::sphere::getForceSize(const sphere &object1, const sphere &object2){
    float distance = getDistance(object1, object2);
    return G * object1.mass * object2.mass / (distance * distance);
}
        
float vpe::sphere::getDistance(const sphere &object1, const sphere &object2){
    return length(object1.position - object2.position);
}
        
bool vpe::sphere::isCollision(const sphere &collisionObject1, const sphere &collisionObject2){
    return (getDistance(collisionObject1, collisionObject2) < (collisionObject1.radius + collisionObject2.radius))? true : false;
}
        
void vpe::sphere::calculateElasticCollision(float delta_time, sphere &collisionObject1, sphere &collisionObject2){
    float distance, distanceShouldBe, distanceGap, timeBack;
    glm::vec3 toCenter1, toCenter2, toCenterTotal, normalObj1, normalObj2;
            
    distanceShouldBe = collisionObject1.radius + collisionObject2.radius;
    distance = getDistance(collisionObject1, collisionObject2);
    distanceGap = distanceShouldBe - distance;
    
    normalObj1 = glm::normalize(collisionObject1.position - collisionObject2.position);
    normalObj2 = -normalObj1;
            
    toCenter1 = dot(normalObj1, collisionObject1.velocity) * normalObj1;
    toCenter2 = dot(normalObj2, collisionObject2.velocity) * normalObj2;
            
    toCenterTotal = toCenter1 - toCenter2;
            
    timeBack = distanceGap / glm::length(toCenterTotal);

    collisionObject1.calculatePosition(-timeBack);
    collisionObject2.calculatePosition(-timeBack);
    
    float m1, m2, mTotal, mDifference;
    glm::vec3 v1, v2;
            
    m1 = collisionObject1.mass;
    m2 = collisionObject2.mass;
    
    v1 = toCenter1;
    v2 = toCenter2;
    
    collisionObject1.velocity -= toCenter1;
    collisionObject2.velocity -= toCenter2;
    
    mTotal = m1 + m2;
    mDifference = m1 - m2;

    collisionObject2.velocity += ((2 * m1) / mTotal) * v1 - (mDifference / mTotal) * v2;
    collisionObject1.velocity += ((2 * m2) / mTotal) * v2 + (mDifference / mTotal) * v1;
            
    collisionObject1.calculatePosition(timeBack);
    collisionObject2.calculatePosition(timeBack);
}

vpe::euclideanSpace::euclideanSpace()
{
    object_count = 0;
    delta_time = 0.05f;
}

vpe::euclideanSpace::~euclideanSpace()
{
}

void vpe::euclideanSpace::addObject(int id, float m, float r, glm::vec3 p, glm::vec3 v)
{
    objects.push_back({id, m, r, p, v});
    object_count++;
}

vpe::sphere& vpe::euclideanSpace::getObject(int order){
    return objects[order];
}

void vpe::euclideanSpace::setDeltaTime(float t)
{
    delta_time = t;
}

void vpe::euclideanSpace::run()
{
    float forceSize;
    
    for(int from = 0; from < object_count - 1; from++){
        for(int to = from + 1; to < object_count; to++){
            forceSize = sphere::getForceSize(objects[from], objects[to]);
            objects[from].makeForce(forceSize * normalize(objects[to].position - objects[from].position));
            objects[to].makeForce(forceSize * normalize(objects[from].position - objects[to].position));
        }
    }
    
    for(sphere &object : objects){
        object.calculateMotion(delta_time);
        object.calculatePosition(delta_time);
        object.clearForce();
    }
    
    for(int from = 0; from < object_count - 1; from++){
        for(int to = from + 1; to < object_count; to++){
            if(sphere::isCollision(objects[from], objects[to])){
                sphere::calculateElasticCollision(delta_time, objects[from], objects[to]);
            }
        }
    }
}

glm::vec3 vpe::euclideanSpace::getTotalMomentum(){
    glm::vec3 totalMomentum = glm::vec3(0, 0, 0);
    for(sphere object : objects){
        totalMomentum += object.getMomentum();
    }
    return totalMomentum;
}

int vpe::euclideanSpace::getObjectCount(){
    return object_count;
}