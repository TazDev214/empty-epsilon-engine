#include <i18n.h>
#include "engine.h"
#include "mainMenus.h"
#include "main.h"
#include "preferenceManager.h"
#include "epsilonServer.h"
#include "playerInfo.h"
#include "gameGlobalInfo.h"
#include "spaceObjects/spaceship.h"
#include "menus/serverCreationScreen.h"
#include "menus/optionsMenu.h"
#include "menus/tutorialMenu.h"
#include "menus/serverBrowseMenu.h"
#include "screens/gm/gameMasterScreen.h"
#include "screenComponents/rotatingModelView.h"

#include "gui/gui2_image.h"
#include "gui/gui2_label.h"
#include "gui/gui2_button.h"
#include "gui/gui2_textentry.h"
#include "gui/gui2_scrolltext.h"

class DebugAllModelView : public GuiCanvas
{
public:
    DebugAllModelView()
    {
        new GuiOverlay(this, "", colorConfig.background);
        (new GuiOverlay(this, "", glm::u8vec4{255,255,255,255}))->setTextureTiled("gui/background/crosses.png");

        std::vector<string> names = ModelData::getModelDataNames();
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("transport_"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("artifact"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("SensorBuoyMK"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.startswith("space_station_"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name == "ammo_box"; }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name == "shield_generator"; }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Blue"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Green"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Grey"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("Red"); }), names.end());
        names.erase(std::remove_if(names.begin(), names.end(), [](const string& name) { return name.endswith("White"); }), names.end());
        int col_count = sqrtf(names.size()) + 1;
        int row_count = ceil(names.size() / col_count) + 1;
        int x = 0;
        int y = 0;
        float w = 1600 / col_count;
        float h = 900 / row_count;
        for(string name : names)
        {
            (new GuiRotatingModelView(this, "", ModelData::getModel(name)))->setPosition(x * w, y * h, sp::Alignment::TopLeft)->setSize(w, h);
            x++;
            if (x == col_count)
            {
                x = 0;
                y++;
            }
        }
    }
};

MainMenu::MainMenu()
{
    new GuiOverlay(this, "", colorConfig.background);
    (new GuiOverlay(this, "", glm::u8vec4{255,255,255,255}))->setTextureTiled("gui/background/crosses.png");
    (new GuiLabel(this, "VEYRA_STAR_WARS", "Star Wars", 80))
        ->setPosition(0, 140, sp::Alignment::TopCenter)->setSize(800, 100);
    (new GuiLabel(this, "VEYRA_TITLE", "Der Veyra-Korridor", 48))
        ->setPosition(0, 250, sp::Alignment::TopCenter)->setSize(850, 70);
    (new GuiLabel(this, "VEYRA_BUILD", "LAN-Prototyp 0.1 | Empty Epsilon 2024.12.8", 20))
        ->setPosition(0, 335, sp::Alignment::TopCenter)->setSize(850, 30);
    (new GuiLabel(this, "VEYRA_CREDITS", "Ein Projekt von Okko", 25))
        ->setPosition(0, 390, sp::Alignment::TopCenter)->setSize(800, 35);
    (new GuiLabel(this, "VEYRA_AI_CREDITS", "Mit Codex und ChatGPT", 22))
        ->setPosition(0, 430, sp::Alignment::TopCenter)->setSize(800, 30);
    (new GuiLabel(this, "VEYRA_MODEL_LOG", "Modellnachweise: MODELLE-ARBEITSLOG.md", 16))
        ->setPosition(0, 470, sp::Alignment::TopCenter)->setSize(800, 25);

    (new GuiLabel(this, "", tr("mainMenu", "Your name:"), 30))->setAlignment(sp::Alignment::CenterLeft)->setPosition({50, -400}, sp::Alignment::BottomLeft)->setSize(300, 50);
    (new GuiTextEntry(this, "USERNAME", PreferencesManager::get("username")))->callback([](string text) {
        PreferencesManager::set("username", text);
    })->setPosition({50, -350}, sp::Alignment::BottomLeft)->setSize(300, 50);

    (new GuiButton(this, "START_SERVER", tr("mainMenu", "Start server"), [this]() {
        new ServerSetupScreen();
        destroy();
    }))->setPosition({50, -230}, sp::Alignment::BottomLeft)->setSize(300, 50);

    (new GuiButton(this, "START_CLIENT", tr("mainMenu", "Start client"), [this]() {
        new ServerBrowserMenu();
        destroy();
    }))->setPosition({50, -170}, sp::Alignment::BottomLeft)->setSize(300, 50);

    (new GuiButton(this, "OPEN_OPTIONS", tr("mainMenu", "Options"), [this]() {
        new OptionsMenu();
        destroy();
    }))->setPosition({50, -110}, sp::Alignment::BottomLeft)->setSize(300, 50);

    (new GuiButton(this, "QUIT", tr("mainMenu", "Quit"), []() {
        engine->shutdown();
    }))->setPosition({50, -50}, sp::Alignment::BottomLeft)->setSize(300, 50);

    (new GuiButton(this, "VEYRA_OPEN_CREDITS", "Credits", [this]() {
        auto credits = new GuiOverlay(this, "VEYRA_CREDITS_OVERLAY", colorConfig.background);
        credits->setSize(GuiElement::GuiSizeMax, GuiElement::GuiSizeMax);
        auto text = new GuiScrollText(credits, "VEYRA_CREDITS_TEXT", "Star Wars - Der Veyra-Korridor\n\nProjekt: Okko\nEntwicklung mit Codex und ChatGPT\nModellnachweise: MODELLE-ARBEITSLOG.md\n\nEmpty Epsilon - urspruengliche Mitwirkende:\nDaid\ngcask\nNallath\nXansta\nStarryWisdom\nInteresting John\nMuerte (FR)\naBlueShadow (DE)\nMatthew Pablo\nAlexandr Zhelanov\nJoe Baxter-Webb\nneocrey\nFoxSynergy\nAngryfly (turbosquid.com)\nSolCommand (http://solcommand.blogspot.com/)\nTokka (http://bekeen.de/)\nMarty Lewis (MadKat)\nSerge Wroclawski\nDennis Shelton\nVolgClawtooth\nDaniel Loftis\nDavid Concepcion\nPhilippe Bruylant\nRalf Leichter\nLee McDonough (Flea)\nMickael Houet\n\nSeriousProton: Daid und Mitwirkende.\nWeitere Urheber-/Lizenzangaben: LICENSE-EmptyEpsilon.txt und SCHIFFSMODELL-KATALOG.md.");
        text->setPosition(0, 70, sp::Alignment::TopCenter)->setSize(900, 600);
        (new GuiButton(credits, "VEYRA_CREDITS_CLOSE", "Zurueck", [credits]() {
            credits->destroy();
        }))->setPosition(0, -40, sp::Alignment::BottomCenter)->setSize(300, 50);
    }))->setPosition({370, -50}, sp::Alignment::BottomLeft)->setSize(300, 50);

    if (PreferencesManager::get("instance_name") != "")
    {
        (new GuiLabel(this, "", PreferencesManager::get("instance_name"), 25))->setAlignment(sp::Alignment::CenterLeft)->setPosition(20, 20, sp::Alignment::TopLeft)->setSize(0, 18);
    }


}
