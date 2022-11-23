#include "components/text.h"
#include "ecs/entity.h"

Text::~Text()
{
    delete m_textRenderer;
}

Text::Text(std::string text, const char *fontPath, Shader *shader, int font_size, int screen_width, int screen_height, Transform *transform)
{
    m_text = text;
    m_font_size = font_size;
    screenWidth = screen_width;
    screenHeight = screen_height;
    m_transform = transform;
    isActivated = true;
    m_color = glm::vec3(1.0f, 1.0f, 1.0f);

    m_textRenderer = new TextRenderer(screenWidth, screenHeight, shader);

    m_fontPath = fontPath;
    m_name = "Text";
}

void Text::Init()
{
    m_textRenderer->Load(m_fontPath, m_font_size);
}

void Text::Update(float dt)
{
}

void Text::Render()
{
    if (isActivated)
    {
        m_textRenderer->RenderText(m_text, m_transform->GetPosition().x, m_transform->GetPosition().y, m_transform->GetScale().x, m_color);
    }
}

void Text::Destroy()
{
}

Text *Text::Clone()
{
    Text *text = new Text(m_text, m_fontPath, m_textRenderer->GetShader(), m_font_size, screenWidth, screenHeight, m_entity->GetComponent<Transform>());
    return text;
}

void Text::SetText(std::string text)
{
    m_text = text;
}

void Text::SetFontSize(int font_size)
{
    m_font_size = font_size;
}

void Text::SetColor(glm::vec3 color)
{
    m_color = color;
}

void Text::SetFontPath(const char *fontPath)
{
    m_fontPath = fontPath;
}

TextRenderer *Text::GetTextRenderer()
{
    return m_textRenderer;
}
