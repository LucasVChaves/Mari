#ifndef _GameController_H_
#define _GameController_H_

#include "Core/SpellBook.h"
#include "types/SensorValues.hpp"

using namespace std;

class GameController
{
    private:
        float timeSincePress;
        SpellBook *spellBook;
    public:
        GameController(SpellBook *spellBook);
        void OnStart();
        void Tick(float ellapsedTime, SensorValues &sensor);
        void OnStop();
        ~GameController();
};

#endif