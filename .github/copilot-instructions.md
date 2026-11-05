# Copilot Instructions

## Project Guidelines
- For runtime checking/debug functions, use the same bare-C style as sc_assert, define exit codes, and use Annex-K compliant functions when compiling with MSVC.
- For Windows time implementations, use high-performance C99 code.
- Prefer largeint and largefloat over long long and double to keep 32-bit builds performant and avoid SSE2 or double-precision emulation.
- For D3D11 scene implementation, use SC_SPECIAL_STATIC only for external organization helpers in namespace scythe; comment any new macros and mark code sections by confidence; implement only remaining virtual methods without fixing existing ones.
- For scene objects, keep resource destruction explicitly performed in Dismiss() before deleting the object; do not rely on a RAII destructor for that cleanup.
- For D3D11GraphicsScene shared object/clump state access and validation, use SC_ARM_D11GS_LOCK() at the caller; avoid nested locking in validation helpers.
- For window implementation, lock write phases with std::mutex, leaving read-only phases unlocked even inside complex functions; adapt falx/FLX names to scythe/SC, and guard ImGui integration with a macro.

## Factory Instructions
- When the user requests only a factory, do not implement its backend or expand scope; honor a request for a non-compilable factory.