// BismIllahIRRahmaanIRRaheem
/* test game */

#include <runtime/game.h>
#include <core/additional_core_definitions.h>

void start_game() {
	const char* m_String = "AlhamdullIllah!";
	void* p_Thing = sc_alloc(strlen(m_String));
	memcpy(p_Thing, m_String, strlen(m_String));
	
}

bool update_game() {
	return true;
}

void end_game() {

}