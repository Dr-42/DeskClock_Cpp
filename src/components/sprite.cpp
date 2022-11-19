#include "components/sprite.h"
#include "ecs/entity.h"

Sprite::~Sprite()
{
    delete m_spriteRenderer;
}

Sprite::Sprite(Texture2D* texture, glm::vec4 color, Shader* shader, int screen_width, int screen_height, Transform* transform)
{
    m_texture = texture;
    m_color = color;
    m_shader = shader;
    m_transform = transform;
    m_size = m_transform->GetScale();
    screenWidth = screen_width;
    screenHeight = screen_height;
    m_spriteRenderer = new SpriteRenderer(m_shader);
    isActivated = true;
    m_name = "Sprite";
}

Sprite::Sprite(Texture2D* texture, Shader* shader, int screen_width, int screen_height, Transform* transform)
{
    m_texture = texture;
    m_color = glm::vec4(1.0f);
    m_shader = shader;
    m_transform = transform;
    m_size = m_transform->GetScale();
    screenWidth = screen_width;
    screenHeight = screen_height;
    m_spriteRenderer = new SpriteRenderer(m_shader);
    isActivated = true;
    m_name = "Sprite";
}

void Sprite::Init()
{
    m_ConfigureShader();
}

void Sprite::Update(float dt)
{
    if(isActivated)
    {
        m_spriteRenderer->DrawSprite(m_texture, m_transform->GetPosition(), m_transform->GetScale(), m_transform->GetRotation(), m_color);
    }

}

void Sprite::Destroy()
{

}

Sprite* Sprite::Clone()
{
    Sprite* sprite = new Sprite(m_texture, m_color, m_shader, screenWidth, screenHeight, m_entity->GetComponent<Transform>());
    return sprite;
}

void Sprite::SetTexture(const char* texturePath)
{
    ResourceManager::LoadTexture(texturePath, true, "face");
    std::cout << "Loaded texture: " << texturePath << std::endl;
}

void Sprite::SetTexture(Texture2D* texture)
{
    m_texture = texture;
}

void Sprite::SetColor(glm::vec4 color)
{
    m_color = color;
}

void Sprite::SetShader(Shader* shader, glm::vec2 size)
{
    m_shader = shader;
    m_size = size;
}


Texture2D* Sprite::GetTexture()
{
    return m_texture;
}

SpriteRenderer* Sprite::GetSpriteRenderer()
{
    return m_spriteRenderer;
}

Shader* Sprite::GetShader()
{
    return m_shader;
}

void Sprite::m_ConfigureShader()
{
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->screenWidth), 
        static_cast<float>(this->screenHeight), 0.0f, -1.0f, 1.0f);
    this->m_shader->Use().SetInteger("image", 0);
    this->m_shader->SetMatrix4("projection", projection);
}