#include "NoMoreLaser.h"
#include "f4se/GameForms.h"
#include "f4se/GameReferences.h"
#include "f4se/GameObjects.h"

namespace NoMoreLaser {

	bool isLoaded = false;

	RelocAddr<uintptr_t> mainLoopFunc(0xd83ec4);

	typedef void(*_hookedMainLoopFunc)(uint64_t rcx);
	RelocAddr<_hookedMainLoopFunc> hookedMainLoopFunc(0x1ba7ba0);

	void setGameLoaded() {
		isLoaded = true;
	}

	void update() {
		if (!isLoaded) {
			return;
		}

		if (!(*g_player)) {
			return;
		}

		PlayerCharacter* pc = (*g_player);

		if (!pc->unkF0) {
			return;
		}

		if (!pc->unkF0->rootNode) {
			return;
		}

		if (!pc->firstPersonSkeleton) {
			return;
		}

		PlayerNodes* pn = (PlayerNodes*)((char*)pc + 0x6e0);

		pn->primaryWandLaserPointer->flags |= 0x1;

	}

	void hookit(uint64_t rcx) {
		update();

		hookedMainLoopFunc(rcx);
		return;
	}

	void hookMain() {

		g_branchTrampoline.Write5Call(mainLoopFunc.GetUIntPtr(), (uintptr_t)&hookit);

	}
}