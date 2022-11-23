#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component
{
public:
    Component();
    
    virtual ~Component();

    virtual void Init();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Destroy();

    virtual Component* Clone();
    Entity* GetEntity() const { return m_entity; }
    void SetEntity(Entity* entity) { m_entity = entity; }
    const char* GetName() const { return m_name; }
protected:
    const char* m_name;
    Entity* m_entity;
};

#endif