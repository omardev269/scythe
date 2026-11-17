// BismIllahIRRahmaanIRRaheem
/* program lifetime manager */

#include <core/types.h>
#include <runtime/game.h>

void scythe() {
	start_game();
	while (update_game());
	end_game();
}