#include "include/Tools.h"

int main() {
    Tool::generateInstance();
    MainMenuHandler::getInstance()->menuHandler();
    Tool::deleteInstance();
    return 0;
}