#include "StateWaypointsMenu.h"

using namespace fdm;
using namespace fdm::gui;

void StateWaypointsMenu::closeBtnCallback(void* user)
{
    StateManager* sm = (StateManager*)user;

    StateWaypointsMenu::waypointsMenuOpen = false;
    sm->popState();
}

void StateWaypointsMenu::updateProjection(const glm::ivec2& size, const glm::ivec2& translate2D)
{
    proj2D = glm::ortho(0.f, (float)size.x, (float)size.y, 0.f, -1.f, 1.f);
    proj2D = glm::translate(proj2D, { translate2D.x, translate2D.y, 0 });

    const Shader* textShader = ShaderManager::get("textShader");
    textShader->use();
    glUniformMatrix4fv(glGetUniformLocation(textShader->id(), "P"), 1, GL_FALSE, &proj2D[0][0]);

    const Shader* tex2DShader = ShaderManager::get("tex2DShader");
    tex2DShader->use();
    glUniformMatrix4fv(glGetUniformLocation(tex2DShader->id(), "P"), 1, GL_FALSE, &proj2D[0][0]);

    const Shader* quadShader = ShaderManager::get("quadShader");
    quadShader->use();
    glUniformMatrix4fv(glGetUniformLocation(quadShader->id(), "P"), 1, GL_FALSE, &proj2D[0][0]);
}

void StateWaypointsMenu::openCreateWaypointMenu()
{
    ui = &createUI;
}

// thanks mashpoe for giving me this very cool thingy which you need to make some UI things work
void StateWaypointsMenu::viewportCallbackFunc(void* user, const glm::ivec4& pos, const glm::ivec2& scroll)
{
    GLFWwindow* window = (GLFWwindow*)user;

    int wWidth, wHeight;
    glfwGetWindowSize(window, &wWidth, &wHeight);
    glViewport(pos.x, wHeight - pos.y - pos.w, pos.z, pos.w);

    instanceObj.proj2D = glm::ortho(0.0f, (float)pos.z, (float)pos.w, 0.0f, -1.0f, 1.0f);
    instanceObj.proj2D = glm::translate(instanceObj.proj2D, { scroll.x, scroll.y, 0 });

    const Shader* textShader = ShaderManager::get("textShader");
    textShader->use();
    glUniformMatrix4fv(glGetUniformLocation(textShader->id(), "P"), 1, GL_FALSE, &instanceObj.proj2D[0][0]);

    const Shader* tex2DShader = ShaderManager::get("tex2DShader");
    tex2DShader->use();
    glUniformMatrix4fv(glGetUniformLocation(tex2DShader->id(), "P"), 1, GL_FALSE, &instanceObj.proj2D[0][0]);

    const Shader* quadShader = ShaderManager::get("quadShader");
    quadShader->use();
    glUniformMatrix4fv(glGetUniformLocation(quadShader->id(), "P"), 1, GL_FALSE, &instanceObj.proj2D[0][0]);
}
void StateWaypointsMenu::updateBox(int w, int h)
{
    // std::max the width and height to 1 to prevent some shit from happening
    box.width = std::max(1, w - 15 * 2);
    box.height = std::max(1, h - 15 * 2 - 58);

    box.scrollH = std::max((int)box.elements.size() * 40 + 8 - (int)box.height, 0);

    if (box.scrollH < -box.yScroll)
        box.yScroll = -box.scrollH;

    for (auto& el : box.elements)
        if (WaypointElement* wp = dynamic_cast<WaypointElement*>(el))
            wp->width = box.width - 16 - (box.scrollH == 0 ? 0 : ContentBox::scrollbarWidth - 2);

    closeBtn.width = createWaypointBtn.width = std::max(1, w / 2 - 10 * 2);
}
void StateWaypointsMenu::init(StateManager& s)
{
    int width, height;
    glfwGetWindowSize(s.window, &width, &height);

    qr.shader = ShaderManager::get("quadShader");

    updateProjection({ width, height }, { 0,0 });

    font = { ResourceManager::get("pixelfont.png"), ShaderManager::get("textShader") };

    glClearColor(0, 0, 0, 1);

    qr.init();

    // mainUI
    {
        mainUI = Interface(s.window);
        mainUI.viewportUser = mainUI.window = s.window;
        mainUI.font = &font;
        mainUI.qr = &qr;
        mainUI.viewportCallback = viewportCallbackFunc;

        box = ContentBox{};
        box.offsetX(15);
        box.offsetY(15);
        box.scrollStep = 32;
        box.parent = &mainUI;
        mainUI.addElement(&box);

        createWaypointElems();

        createWaypointBtn = Button{};
        createWaypointBtn.offsetX(16);
        createWaypointBtn.offsetY(-10);
        createWaypointBtn.alignY(ALIGN_BOTTOM);
        createWaypointBtn.text = "Create";
        createWaypointBtn.callback = [](void* s) { StateWaypointsMenu::instanceObj.openCreateWaypointMenu(); };
        mainUI.addElement(&createWaypointBtn);

        closeBtn = Button{};
        closeBtn.offsetX(-16);
        closeBtn.offsetY(-10);
        closeBtn.alignX(ALIGN_RIGHT);
        closeBtn.alignY(ALIGN_BOTTOM);
        closeBtn.text = "Close";
        closeBtn.callback = closeBtnCallback;
        closeBtn.user = &s;
        mainUI.addElement(&closeBtn);
    }

    // createUI
    {
        createUI = Interface(s.window);
        createUI.viewportUser = createUI.window = s.window;
        createUI.font = &font;
        createUI.qr = &qr;
        createUI.viewportCallback = viewportCallbackFunc;

        createBox = ContentBox{};
        createBox.alignX(ALIGN_CENTER_X);
        createBox.alignY(ALIGN_CENTER_Y);
        createBox.scrollStep = 32;
        createBox.width = 200;
        createBox.height = 170;
        createBox.parent = &createUI;
        createUI.addElement(&createBox);

        title = Text{};
        title.alignX(ALIGN_CENTER_X);
        title.offsetY(8);
        title.size = 2;
        title.shadow = true;
        createBox.addElement(&title);
        
        nameInput = TextInput{};
        nameInput.alignX(ALIGN_CENTER_X);
        nameInput.offsetY(30);
        createBox.addElement(&nameInput);

        xInput = NumberInput("X", true);
        xInput.alignX(ALIGN_CENTER_X);
        xInput.offsetY(80);
        xInput.minValue = -100000;
        xInput.maxValue = 100000;
        xInput.text = std::format("{:.1f}", StateGame::instanceObj->player.pos.x);
        createBox.addElement(&xInput);
    }

    ui = &mainUI;
}
void StateWaypointsMenu::update(StateManager& s, double dt)
{
    if (glfwWindowShouldClose(s.window))
    {
        StateWaypointsMenu::waypointsMenuOpen = false;
        s.popState();
    }
    else
        s.states[s.states.size() - 2]->update(s, dt);
}
void StateWaypointsMenu::render(StateManager& s)
{
    int w, h;
    glfwGetWindowSize(s.window, &w, &h);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    s.states[s.states.size() - 2]->render(s);

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    qr.render({ 0.05f, 0.05f, 0.1f, 0.5f }, { 0, 0 }, { w, h });

    ui->render();

    if (updateViewFlag)
    {
        updateViewFlag = false;

        glViewport(0, 0, w, h);

        updateProjection({ w,h }, { 0,0 });

        updateBox(w, h);
    }
}
void StateWaypointsMenu::resume(StateManager& s)
{
    updateViewFlag = true;
}
void StateWaypointsMenu::windowResize(StateManager& s, int w, int h)
{
    updateViewFlag = true;
}
void StateWaypointsMenu::mouseInput(StateManager& s, double xPos, double yPos)
{
    ui->mouseInput(xPos, yPos);
}
void StateWaypointsMenu::scrollInput(StateManager& s, double xOff, double yOff)
{
    ui->scrollInput(xOff, yOff);
}
void StateWaypointsMenu::keyInput(StateManager& s, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        StateWaypointsMenu::waypointsMenuOpen = false;
        s.popState();
    }
    else
        ui->keyInput(key, scancode, action, mods);
}
void StateWaypointsMenu::charInput(StateManager& s, uint32_t codepoint)
{
    ui->charInput(codepoint);
}
void StateWaypointsMenu::mouseButtonInput(StateManager& s, int btn, int action, int mods)
{
    ui->mouseButtonInput(btn, action, mods);
}
void StateWaypointsMenu::close(StateManager& s)
{
    ui->clear();

    if (!box.elements.empty())
    {
        for (auto it = box.elements.begin(); it != box.elements.end(); ++it)
        {
            delete* it;
        }

        box.clear();
    }
}

void StateWaypointsMenu::createWaypointElems()
{
    updateViewFlag = true;
    if (!box.elements.empty())
    {
        for (auto it = box.elements.begin(); it != box.elements.end(); ++it)
        {
            delete* it;
        }

        box.clear();
    }

    if (!StateWaypointsMenu::waypoints.contains(StateWaypointsMenu::curWorld)) return;

    int i = 0;
    for (auto& wp : StateWaypointsMenu::waypoints[StateWaypointsMenu::curWorld])
    {
        WaypointElement* wpEl = new WaypointElement();
        wpEl->coordsText.setText(std::format("X:{:.1f} Y:{:.1f} Z:{:.1f} W:{:.1f}", wp.position.x, wp.position.y, wp.position.z, wp.position.w));
        wpEl->nameText.setText(wp.name);

        wpEl->removeBtn.user = wpEl;
        wpEl->editBtn.user = wpEl;
        wpEl->user = wpEl;

        wpEl->offsetX(8);
        wpEl->offsetY(8 + i * 40);

        box.addElement(wpEl);
        i++;
    }
}

StateWaypointsMenu StateWaypointsMenu::instanceObj = StateWaypointsMenu();