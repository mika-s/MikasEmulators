mkdir -p build/emscripten/lmc/

emcc \
  libs/imgui/imgui.cpp \
  libs/imgui/imgui_demo.cpp \
  libs/imgui/imgui_draw.cpp \
  libs/imgui/imgui_impl_opengl3.cpp \
  libs/imgui/imgui_impl_sdl_emscripten.cpp \
  libs/imgui/imgui_tables.cpp \
  libs/imgui/imgui_widgets.cpp \
  src/applications/lmc_application/emscripten_runner.cpp \
  src/applications/lmc_application/gui_imgui.cpp \
  src/applications/lmc_application/input_imgui.cpp \
  src/applications/lmc_application/lmc_application.cpp \
  src/applications/lmc_application/lmc_application_session.cpp \
  src/applications/lmc_application/lmc_memory_editor.cpp \
  src/applications/lmc_application/tui_terminal.cpp \
  src/applications/lmc_application/usage.cpp \
  src/applications/lmc_application/states/paused_state.cpp \
  src/applications/lmc_application/states/running_awaiting_input_state.cpp \
  src/applications/lmc_application/states/running_state.cpp \
  src/applications/lmc_application/states/state_context.cpp \
  src/applications/lmc_application/states/stepping_state.cpp \
  src/applications/lmc_application/states/stopped_state.cpp \
  src/chips/trivial/lmc/cpu.cpp \
  src/chips/trivial/lmc/disassembler.cpp \
  src/chips/trivial/lmc/flags.cpp \
  src/chips/trivial/lmc/unrecognized_assembly_exception.cpp \
  src/chips/trivial/lmc/assembler/assembler.cpp \
  src/chips/trivial/lmc/assembler/environment.cpp \
  src/chips/trivial/lmc/assembler/lmc_add.cpp \
  src/chips/trivial/lmc/assembler/lmc_bra.cpp \
  src/chips/trivial/lmc/assembler/lmc_brp.cpp \
  src/chips/trivial/lmc/assembler/lmc_brz.cpp \
  src/chips/trivial/lmc/assembler/lmc_dat.cpp \
  src/chips/trivial/lmc/assembler/lmc_hlt.cpp \
  src/chips/trivial/lmc/assembler/lmc_inp.cpp \
  src/chips/trivial/lmc/assembler/lmc_instruction.cpp \
  src/chips/trivial/lmc/assembler/lmc_integer.cpp \
  src/chips/trivial/lmc/assembler/lmc_label.cpp \
  src/chips/trivial/lmc/assembler/lmc_label_argument.cpp \
  src/chips/trivial/lmc/assembler/lmc_lda.cpp \
  src/chips/trivial/lmc/assembler/lmc_line.cpp \
  src/chips/trivial/lmc/assembler/lmc_operand.cpp \
  src/chips/trivial/lmc/assembler/lmc_otc.cpp \
  src/chips/trivial/lmc/assembler/lmc_out.cpp \
  src/chips/trivial/lmc/assembler/lmc_program.cpp \
  src/chips/trivial/lmc/assembler/lmc_sta.cpp \
  src/chips/trivial/lmc/assembler/lmc_sub.cpp \
  src/chips/trivial/lmc/assembler/scanner.cpp \
  src/chips/trivial/lmc/assembler/token.cpp \
  src/chips/trivial/lmc/instructions/add.cpp \
  src/chips/trivial/lmc/instructions/bra.cpp \
  src/chips/trivial/lmc/instructions/brp.cpp \
  src/chips/trivial/lmc/instructions/brz.cpp \
  src/chips/trivial/lmc/instructions/hlt.cpp \
  src/chips/trivial/lmc/instructions/lda.cpp \
  src/chips/trivial/lmc/instructions/sta.cpp \
  src/chips/trivial/lmc/instructions/sub.cpp \
  src/crosscutting/exceptions/rom_file_not_found_exception.cpp \
  src/crosscutting/exceptions/unrecognized_opcode_exception.cpp \
  src/crosscutting/gui/debugging_panes/debug_log_pane.cpp \
  src/crosscutting/gui/main_panes/terminal_pane.cpp \
  src/crosscutting/logging/logger.cpp \
  src/crosscutting/memory/emulator_memory.cpp \
  src/crosscutting/misc/governor.cpp \
  src/crosscutting/misc/sdl_counter.cpp \
  src/crosscutting/misc/uinteger.cpp \
  src/crosscutting/util/byte_util.cpp \
  src/crosscutting/util/file_util.cpp \
  src/crosscutting/util/string_util.cpp \
  -I src -I src/applications/lmc_application -I src/chips/trivial/lmc -I libs/imgui -I libs/glad/include -I libs/doctest -I ~/include \
  -DFMT_HEADER_ONLY \
  -DDOCTEST_CONFIG_DISABLE \
  -s USE_SDL=2 \
  -s MAX_WEBGL_VERSION=2 \
  -o build/emscripten/lmc/lmc.js \
  -s ASYNCIFY -s 'ASYNCIFY_IMPORTS=[\"copy,paste\"]' -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
  -O2 \
  --preload-file src/applications/lmc_application/emscripten_assets@/

cp src/applications/lmc_application/emscripten_assets/lmc.html build/emscripten/lmc/
