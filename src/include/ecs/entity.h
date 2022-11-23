#ifndef ENTITY_H
#define ENTITY_H

#include "ecs/component.h"
#include "components/transform.h"
#include "components/sprite.h"
#include "components/text.h"

#include <vector>
#include <algorithm>
#define quote(x) #x

class Entity
{
public:
    Entity(const char* name);
    ~Entity();

    void Init();
    void Update(float dt);
    void Render();
    Entity* Clone();

    void AddComponent(Component* component);
    void RemoveComponent(Component* component);
    
    Entity* AddChild(Entity* child);
    Entity* AddChild(Entity* child, const char* name);
    Entity* AddChild(const char* name);

    Entity* GetChild(const char* name);
    std::vector<Entity*> GetChildren();

    void RemoveChild(Entity* child);
    void RemoveChild(const char* name);

    void SetParent(Entity* parent);
    Entity* GetParent();

    void Destroy();
    std::vector<Component*> GetComponents();
    const char* GetName();
    void SetName(const char* name);

    //generic function to get components
    template <typename T>
    T* GetComponent()
    {

        for (long long unsigned int i = 0; i < m_components.size(); i++)
        {
            if (T* component = dynamic_cast<T*>(m_components[i]))
            {
                return component;
            }
        }
        std::cout << "Component " << quote(T) << " not found" << std::endl;
        return nullptr;
    }

private:
    const char *m_name;
    std::vector<Component*> m_components;
    std::vector<Entity*> m_children;
    glm::vec3 m_positionOffsetFromParent;
    float m_rotationOffsetFromParent;
    Entity* m_parent;
};

#endif