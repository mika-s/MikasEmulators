set(SOURCES_Z80_CPP
        src/z80/applications/cpm/cpm_application.cpp
        src/z80/applications/cpm/cpm_application_session.cpp
        src/z80/applications/pacman/gui.cpp
        src/z80/applications/pacman/gui_imgui.cpp
        src/z80/applications/pacman/gui_sdl.cpp
        src/z80/applications/pacman/input_imgui.cpp
        src/z80/applications/pacman/input_sdl.cpp
        src/z80/applications/pacman/memory_mapped_io.cpp
        src/z80/applications/pacman/pacman.cpp
        src/z80/applications/pacman/pacman_session.cpp
        src/z80/cpu.cpp
        src/z80/disassemblerZ80.cpp
        src/z80/emulator_memory.cpp
        src/z80/flags.cpp
        src/z80/frontend.cpp
        src/z80/instructions/adc.cpp
        src/z80/instructions/add.cpp
        src/z80/instructions/and.cpp
        src/z80/instructions/bit.cpp
        src/z80/instructions/call.cpp
        src/z80/instructions/ccf.cpp
        src/z80/instructions/cp.cpp
        src/z80/instructions/cpd.cpp
        src/z80/instructions/cpdr.cpp
        src/z80/instructions/cpi.cpp
        src/z80/instructions/cpir.cpp
        src/z80/instructions/cpl.cpp
        src/z80/instructions/daa.cpp
        src/z80/instructions/dec.cpp
        src/z80/instructions/di.cpp
        src/z80/instructions/djnz.cpp
        src/z80/instructions/ei.cpp
        src/z80/instructions/ex.cpp
        src/z80/instructions/exx.cpp
        src/z80/instructions/halt.cpp
        src/z80/instructions/im.cpp
        src/z80/instructions/in.cpp
        src/z80/instructions/inc.cpp
        src/z80/instructions/ind.cpp
        src/z80/instructions/indr.cpp
        src/z80/instructions/ini.cpp
        src/z80/instructions/inir.cpp
        src/z80/instructions/instruction_util.cpp
        src/z80/instructions/jp.cpp
        src/z80/instructions/jr.cpp
        src/z80/instructions/ld.cpp
        src/z80/instructions/ldd.cpp
        src/z80/instructions/lddr.cpp
        src/z80/instructions/ldi.cpp
        src/z80/instructions/ldir.cpp
        src/z80/instructions/neg.cpp
        src/z80/instructions/nop.cpp
        src/z80/instructions/or.cpp
        src/z80/instructions/out.cpp
        src/z80/instructions/outd.cpp
        src/z80/instructions/outi.cpp
        src/z80/instructions/otdr.cpp
        src/z80/instructions/otir.cpp
        src/z80/instructions/pop.cpp
        src/z80/instructions/push.cpp
        src/z80/instructions/res.cpp
        src/z80/instructions/ret.cpp
        src/z80/instructions/reti.cpp
        src/z80/instructions/retn.cpp
        src/z80/instructions/rl.cpp
        src/z80/instructions/rlc.cpp
        src/z80/instructions/rld.cpp
        src/z80/instructions/rr.cpp
        src/z80/instructions/rrd.cpp
        src/z80/instructions/rrc.cpp
        src/z80/instructions/rst.cpp
        src/z80/instructions/sbc.cpp
        src/z80/instructions/scf.cpp
        src/z80/instructions/set.cpp
        src/z80/instructions/sla.cpp
        src/z80/instructions/sll.cpp
        src/z80/instructions/sra.cpp
        src/z80/instructions/srl.cpp
        src/z80/instructions/sub.cpp
        src/z80/instructions/xor.cpp
        )

set(SOURCES_Z80_H
        src/z80/applications/cpm/cpm_application.h
        src/z80/applications/cpm/cpm_application_session.h
        src/z80/applications/pacman/gui.h
        src/z80/applications/pacman/gui_imgui.h
        src/z80/applications/pacman/gui_sdl.h
        src/z80/applications/pacman/input_imgui.h
        src/z80/applications/pacman/input_sdl.h
        src/z80/applications/pacman/io_request.h
        src/z80/applications/pacman/memory_mapped_io.h
        src/z80/applications/pacman/pacman.h
        src/z80/applications/pacman/pacman_session.h
        src/z80/applications/pacman/settings.h
        src/z80/applications/pacman/interfaces/input.h
        src/z80/applications/pacman/interfaces/io_observer.h
        src/z80/cpu.h
        src/z80/disassemblerZ80.h
        src/z80/emulator_memory.h
        src/z80/flags.h
        src/z80/frontend.h
        src/z80/interrupt_mode.h
        src/z80/run_status.h
        src/z80/instructions/instruction_util.h
        src/z80/instructions/instructions.h
        src/z80/interfaces/emulatorZ80.h
        src/z80/interfaces/gui_observer.h
        src/z80/interfaces/in_observer.h
        src/z80/interfaces/memory_observer.h
        src/z80/interfaces/out_observer.h
        src/z80/interfaces/session.h
        )
