#include "game.h"
#include <ctime>

#include <iostream>
#include <unistd.h>

Game::Game(unsigned int width, unsigned int height) {
    this->Width = width;
    this->Height = height;
    this->MouseLeft = false;
    this->MouseRight = false;
    wireframe = false;
    this->MousePos = new glm::vec2(0.0f, 0.0f);
    root = new Entity("root");
}

Game::~Game() {
    delete MousePos;
    delete root;
}

#define HOUR_HAND_LENGTH 150.0f
#define MINUTE_HAND_LENGTH 250.0f
#define SECOND_HAND_LENGTH 290.0f

#define HOUR_HAND_WIDTH 10.0f
#define MINUTE_HAND_WIDTH 5.0f
#define SECOND_HAND_WIDTH 2.0f

void Game::Init() {
    // load shaders
    ResourceManager::LoadShader("assets/shaders/sprite_vert_shad.glsl", "assets/shaders/sprite_frag_shad.glsl", nullptr, "sprite");
    ResourceManager::LoadTexture("assets/textures/clock_face.png", true, "face");
    ResourceManager::LoadTexture("assets/textures/clock_hand.png", true, "hand");

    const char* fontPath = "assets/fonts/OCRAEXT.TTF";
    ResourceManager::LoadShader("assets/shaders/text_vert_shad.glsl", "assets/shaders/text_frag_shad.glsl", nullptr, "text");

    MousePos = new glm::vec2(0.0f, 0.0f);

    root->AddChild("clock");
    root->GetChild("clock")->AddComponent(new Sprite(ResourceManager::GetTexture("face"), ResourceManager::GetShader("sprite"), Width, Height, root->GetChild("clock")->GetComponent<Transform>()));

    root->AddChild("date");
    root->GetChild("date")->AddComponent(new Text("", fontPath, ResourceManager::GetShader("text"), 72, this->Width, this->Height, root->GetChild("date")->GetComponent<Transform>()));

    root->AddChild("day");
    root->GetChild("day")->AddComponent(new Text("", fontPath, ResourceManager::GetShader("text"), 50, this->Width, this->Height, root->GetChild("day")->GetComponent<Transform>()));

    root->AddChild("hour_hand");
    root->GetChild("hour_hand")->AddComponent(new Sprite(ResourceManager::GetTexture("hand"), ResourceManager::GetShader("sprite"), Width, Height, root->GetChild("hour_hand")->GetComponent<Transform>()));

    Entity* e = root->GetChild("hour_hand")->Clone();
    e->SetName("minute_hand");
    root->AddChild(e);

    e = root->GetChild("hour_hand")->Clone();
    e->SetName("second_hand");
    root->AddChild(e);

    root->Init();

    // configure game objects
    root->GetChild("clock")->GetComponent<Transform>()->SetPosition(glm::vec2(Width / 2.0f, Height / 2.0f));
    root->GetChild("clock")->GetComponent<Transform>()->SetScale(glm::vec2(Width, Height));

    root->GetChild("hour_hand")->GetComponent<Transform>()->SetPosition(glm::vec2(Width / 2.0f, Height / 2.0f));
    root->GetChild("hour_hand")->GetComponent<Transform>()->SetScale(glm::vec2(HOUR_HAND_WIDTH, HOUR_HAND_LENGTH));
    root->GetChild("hour_hand")->GetComponent<Sprite>()->SetColor(glm::vec4(0.6f, 0.6f, 0.9f, 1.0f));

    root->GetChild("minute_hand")->GetComponent<Transform>()->SetPosition(glm::vec2(Width / 2.0f, Height / 2.0f));
    root->GetChild("minute_hand")->GetComponent<Transform>()->SetScale(glm::vec2(MINUTE_HAND_WIDTH, MINUTE_HAND_LENGTH));
    root->GetChild("minute_hand")->GetComponent<Sprite>()->SetColor(glm::vec4(0.6f, 0.6f, 0.9f, 1.0f));

    root->GetChild("second_hand")->GetComponent<Transform>()->SetPosition(glm::vec2(Width / 2.0f, Height / 2.0f));
    root->GetChild("second_hand")->GetComponent<Transform>()->SetScale(glm::vec2(SECOND_HAND_WIDTH, SECOND_HAND_LENGTH));
    root->GetChild("second_hand")->GetComponent<Sprite>()->SetColor(glm::vec4(0.9f, 0.0f, 0.0f, 1.0f));

    root->GetChild("date")->GetComponent<Transform>()->SetPosition(glm::vec2(120.0, Height / 2.0f));
    root->GetChild("date")->GetComponent<Transform>()->SetScale(glm::vec2(1.0f, 1.0f));

    root->GetChild("date")->GetComponent<Text>()->SetColor(glm::vec3(0.6f, 0.3f, 0.0f));

    root->GetChild("day")->GetComponent<Transform>()->SetPosition(glm::vec2(180, Height / 2.0f + 70));
    root->GetChild("day")->GetComponent<Transform>()->SetScale(glm::vec2(1.0f, 1.0f));

    root->GetChild("day")->GetComponent<Text>()->SetColor(glm::vec3(0.6f, 0.3f, 0.0f));

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to initialize miniaudio engine: " << result << std::endl;
        std::exit(1);
    }

    result = ma_sound_init_from_file(&engine, "assets/sound/gong.wav", 0, NULL, NULL, &sound);
    if (result != MA_SUCCESS) {
        std::cout << "Failed to initialize miniaudio sound: " << result << std::endl;
        std::exit(1);
    }
}
void Game::ProcessInput(float dt) {
    if (this->Keys[GLFW_KEY_SPACE]) {
        wireframe = !wireframe;
        if (wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}

void Game::Update(float dt) {
    root->Update(dt);

    time_t t = time(0);
    struct tm* now = localtime(&t);

    int hour = now->tm_hour;
    int minute = now->tm_min;
    int second = now->tm_sec;

    std::string date = std::to_string(now->tm_mday);
    if (now->tm_mday < 10) {
        date = "0" + date;
    }

    date += "/";
    if (now->tm_mon < 9) {
        date += "0";
    }
    date += std::to_string(now->tm_mon + 1);

    int day_num = now->tm_wday;
    std::string day;
    switch (day_num) {
        case 0:
            day = "SUN";
            break;
        case 1:
            day = "MON";
            break;
        case 2:
            day = "TUE";
            break;
        case 3:
            day = "WED";
            break;
        case 4:
            day = "THU";
            break;
        case 5:
            day = "FRI";
            break;
        case 6:
            day = "SAT";
            break;
    }

    float hour_angle = (hour * 30.0f) + (minute * 0.5f);
    float minute_angle = (minute * 6.0f) + (second * 0.1f);
    float second_angle = second * 6.0f;

    float hour_angle_rad = glm::radians(hour_angle - 90);
    float minute_angle_rad = glm::radians(minute_angle - 90);
    float second_angle_rad = glm::radians(second_angle - 90);

    // transform position of each hand is at the hand's center
    // so we need to offset the position by the half length of the hand
    // and rotate the hand around the center of the clock
    // the clock is at the center of the screen

    root->GetChild("hour_hand")->GetComponent<Transform>()->SetPosition(glm::vec2(Width / 2.0f, Height / 2.0f) + glm::vec2(HOUR_HAND_LENGTH / 3.0f * glm::cos(hour_angle_rad), HOUR_HAND_LENGTH / 3.0f * glm::sin(hour_angle_rad)));
    root->GetChild("hour_hand")->GetComponent<Transform>()->SetRotation(hour_angle);

    root->GetChild("minute_hand")->GetComponent<Transform>()->SetPosition(glm::vec2(Width / 2.0f, Height / 2.0f) + glm::vec2(MINUTE_HAND_LENGTH / 4.0f * glm::cos(minute_angle_rad), MINUTE_HAND_LENGTH / 4.0f * glm::sin(minute_angle_rad)));
    root->GetChild("minute_hand")->GetComponent<Transform>()->SetRotation(minute_angle);

    root->GetChild("second_hand")->GetComponent<Transform>()->SetPosition(glm::vec2(Width / 2.0f, Height / 2.0f) + glm::vec2(SECOND_HAND_LENGTH / 4.0f * glm::cos(second_angle_rad), SECOND_HAND_LENGTH / 4.0f * glm::sin(second_angle_rad)));

    root->GetChild("second_hand")->GetComponent<Transform>()->SetRotation(second_angle);

    root->GetChild("date")->GetComponent<Text>()->SetText(date);
    root->GetChild("day")->GetComponent<Text>()->SetText(day);

    // Play gong.wav every minute
    if (second == 0 && minute == 0 && !played) {
        ma_sound_start(&sound);
        played = true;
    }

    if (second != 0) {
        if (ma_sound_at_end(&sound)) {
            ma_sound_stop(&sound);
            ma_sound_seek_to_pcm_frame(&sound, 0);
        }
        played = false;
    }

    usleep(1000000 - dt * 1000);
}

void Game::Render() {
    root->Render();
}
