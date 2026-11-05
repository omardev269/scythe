// BismIllahIRRahmaanIRRaheem
/* game class decl */

#ifndef SC_GAME_H
#define SC_GAME_H
#include <runtime/engineCore.h>
// runs on startup
void start_game();
// runs per frame
// return false to exit the game
bool update_game();
// runs on exit before the subsystem exit
void end_game();
#endif // !SC_GAME_H