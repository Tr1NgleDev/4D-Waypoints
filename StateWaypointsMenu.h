#pragma once

#include <4dm.h>

using namespace fdm;
using namespace fdm::gui;

#include "WaypointElement.h"
#include "NumberInput.h"

struct Waypoint
{
    std::string name;
    glm::vec4 position;

    Waypoint(const std::string& name = "", const glm::vec4& position = { 0,0,0,0 }) : name(name), position(position) {}
};

inline static void to_json(nlohmann::json& j, const Waypoint& wp)
{
    j = nlohmann::json
    {
        {"name", wp.name},
        {"position", {wp.position.x, wp.position.y, wp.position.z, wp.position.w}}
    };
}
inline static void from_json(const nlohmann::json& j, Waypoint& wp)
{
    wp.name = j.at("name").get<std::string>();
    auto position = j.at("position");
    wp.position.x = position.at(0).get<float>();
    wp.position.y = position.at(1).get<float>();
    wp.position.z = position.at(2).get<float>();
    wp.position.w = position.at(3).get<float>();
}
inline static void to_json(nlohmann::json& j, const std::vector<Waypoint>& waypoints)
{
    for (const auto& wp : waypoints)
    {
        j.push_back(wp);
    }
}
inline static void from_json(const nlohmann::json& j, std::vector<Waypoint>& waypoints)
{
    for (const nlohmann::json& wp : j)
    {
        Waypoint waypoint{ };
        from_json(wp, waypoint);
        waypoints.emplace_back(waypoint);
    }
}

class StateWaypointsMenu : public State
{
public:
    inline static std::unordered_map<std::string, std::vector<Waypoint>> waypoints; // server address (or "singleplayer/{world name}") -> waypoint data
    inline static bool showWaypoints; // in settings or through a keybind
    inline static bool showCoords; // in settings or through a keybind
    inline static float maxScale; // in settings
    inline static float maxScaleDistance; // in settings
    
    inline static std::string curWorld;
    
    inline static bool waypointsMenuOpen;

    static StateWaypointsMenu instanceObj;

    QuadRenderer qr;
    FontRenderer font;

    glm::mat4 proj2D;
    bool updateViewFlag = true;

    Interface mainUI;
    Interface createUI;
    Interface* ui;

    ContentBox box;
    Button createWaypointBtn;
    Button closeBtn;

    ContentBox createBox;

    Text title;
    TextInput nameInput;

    NumberInput xInput;
    NumberInput yInput;
    NumberInput zInput;
    NumberInput wInput;

    Button createBtn;
    Button closeBtn2;

    StateWaypointsMenu() {}

    static void closeBtnCallback(void* user);
    void openCreateWaypointMenu();

    void updateProjection(const glm::ivec2& size, const glm::ivec2& translate2D);

    // thanks mashpoe for giving me this very cool thingy which you need to make some UI things work
    static void viewportCallbackFunc(void* user, const glm::ivec4& pos, const glm::ivec2& scroll);

    void updateBox(int w, int h);
    void createWaypointElems();

    void init(StateManager& s) override;
    void update(StateManager& s, double dt) override;
    void render(StateManager& s) override;
    void resume(StateManager& s) override;
    void windowResize(StateManager& s, int w, int h) override;
    void mouseInput(StateManager& s, double xPos, double yPos) override;
    void scrollInput(StateManager& s, double xOff, double yOff) override;
    void keyInput(StateManager& s, int key, int scancode, int action, int mods) override;
    void charInput(StateManager& s, uint32_t codepoint) override;
    void mouseButtonInput(StateManager& s, int btn, int action, int mods) override;
    void close(StateManager& s) override;
};