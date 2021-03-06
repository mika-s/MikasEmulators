set(SOURCES_8080_CPP
        src/8080/applications/cpm/cpm_application.cpp
        src/8080/applications/cpm/cpm_application_session.cpp
        src/8080/applications/space_invaders/audio.cpp
        src/8080/applications/space_invaders/cpu_io.cpp
        src/8080/applications/space_invaders/gui_imgui.cpp
        src/8080/applications/space_invaders/gui_sdl.cpp
        src/8080/applications/space_invaders/input_imgui.cpp
        src/8080/applications/space_invaders/input_sdl.cpp
        src/8080/applications/space_invaders/space_invaders.cpp
        src/8080/applications/space_invaders/space_invaders_session.cpp
        src/8080/cpu.cpp
        src/8080/debug_container.cpp
        src/8080/disassembler8080.cpp
        src/8080/emulator_memory.cpp
        src/8080/flags.cpp
        src/8080/frontend.cpp
        src/8080/shift_register.cpp
        src/8080/instructions/aci.cpp
        src/8080/instructions/adc.cpp
        src/8080/instructions/add.cpp
        src/8080/instructions/adi.cpp
        src/8080/instructions/ana.cpp
        src/8080/instructions/ani.cpp
        src/8080/instructions/call.cpp
        src/8080/instructions/cc.cpp
        src/8080/instructions/cm.cpp
        src/8080/instructions/cma.cpp
        src/8080/instructions/cmc.cpp
        src/8080/instructions/cmp.cpp
        src/8080/instructions/cnc.cpp
        src/8080/instructions/cnz.cpp
        src/8080/instructions/cp.cpp
        src/8080/instructions/cpe.cpp
        src/8080/instructions/cpi.cpp
        src/8080/instructions/cpo.cpp
        src/8080/instructions/cz.cpp
        src/8080/instructions/daa.cpp
        src/8080/instructions/dad.cpp
        src/8080/instructions/dcr.cpp
        src/8080/instructions/dcx.cpp
        src/8080/instructions/di.cpp
        src/8080/instructions/ei.cpp
        src/8080/instructions/hlt.cpp
        src/8080/instructions/in.cpp
        src/8080/instructions/inr.cpp
        src/8080/instructions/instruction_util.cpp
        src/8080/instructions/inx.cpp
        src/8080/instructions/jc.cpp
        src/8080/instructions/jm.cpp
        src/8080/instructions/jmp.cpp
        src/8080/instructions/jnc.cpp
        src/8080/instructions/jnz.cpp
        src/8080/instructions/jp.cpp
        src/8080/instructions/jpe.cpp
        src/8080/instructions/jpo.cpp
        src/8080/instructions/jz.cpp
        src/8080/instructions/lda.cpp
        src/8080/instructions/ldax.cpp
        src/8080/instructions/lhld.cpp
        src/8080/instructions/lxi.cpp
        src/8080/instructions/mov.cpp
        src/8080/instructions/mvi.cpp
        src/8080/instructions/nop.cpp
        src/8080/instructions/ora.cpp
        src/8080/instructions/ori.cpp
        src/8080/instructions/out.cpp
        src/8080/instructions/pchl.cpp
        src/8080/instructions/pop.cpp
        src/8080/instructions/push.cpp
        src/8080/instructions/ral.cpp
        src/8080/instructions/rar.cpp
        src/8080/instructions/rc.cpp
        src/8080/instructions/ret.cpp
        src/8080/instructions/rlc.cpp
        src/8080/instructions/rm.cpp
        src/8080/instructions/rnc.cpp
        src/8080/instructions/rnz.cpp
        src/8080/instructions/rp.cpp
        src/8080/instructions/rpe.cpp
        src/8080/instructions/rpo.cpp
        src/8080/instructions/rrc.cpp
        src/8080/instructions/rst.cpp
        src/8080/instructions/rz.cpp
        src/8080/instructions/sbb.cpp
        src/8080/instructions/sbi.cpp
        src/8080/instructions/shld.cpp
        src/8080/instructions/sphl.cpp
        src/8080/instructions/sta.cpp
        src/8080/instructions/stax.cpp
        src/8080/instructions/stc.cpp
        src/8080/instructions/sub.cpp
        src/8080/instructions/sui.cpp
        src/8080/instructions/unused.cpp
        src/8080/instructions/xchg.cpp
        src/8080/instructions/xra.cpp
        src/8080/instructions/xri.cpp
        src/8080/instructions/xthl.cpp
        )

set(SOURCES_8080_H
        src/8080/applications/cpm/cpm_application.h
        src/8080/applications/cpm/cpm_application_session.h
        src/8080/applications/space_invaders/audio.h
        src/8080/applications/space_invaders/cpu_io.h
        src/8080/applications/space_invaders/gui.h
        src/8080/applications/space_invaders/gui_imgui.h
        src/8080/applications/space_invaders/gui_sdl.h
        src/8080/applications/space_invaders/input_imgui.h
        src/8080/applications/space_invaders/input_sdl.h
        src/8080/applications/space_invaders/io_request.h
        src/8080/applications/space_invaders/sounds.h
        src/8080/applications/space_invaders/space_invaders.h
        src/8080/applications/space_invaders/space_invaders_session.h
        src/8080/applications/space_invaders/settings.h
        src/8080/applications/space_invaders/interfaces/input.h
        src/8080/applications/space_invaders/interfaces/io_observer.h
        src/8080/cpu.h
        src/8080/debug_container.h
        src/8080/disassembler8080.h
        src/8080/emulator_memory.h
        src/8080/flags.h
        src/8080/frontend.h
        src/8080/run_status.h
        src/8080/shift_register.h
        src/8080/instructions/instruction_util.h
        src/8080/instructions/instructions.h
        src/8080/interfaces/emulator8080.h
        src/8080/interfaces/gui_observer.h
        src/8080/interfaces/in_observer.h
        src/8080/interfaces/out_observer.h
        src/8080/interfaces/session.h
        )
