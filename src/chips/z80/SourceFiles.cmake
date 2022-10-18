set(SOURCES_Z80_CPP
        src/chips/z80/cpu.cpp
        src/chips/z80/disassemblerZ80.cpp
        src/chips/z80/emulator_memory.cpp
        src/chips/z80/flags.cpp
        src/chips/z80/instructions/adc.cpp
        src/chips/z80/instructions/add.cpp
        src/chips/z80/instructions/and.cpp
        src/chips/z80/instructions/bit.cpp
        src/chips/z80/instructions/call.cpp
        src/chips/z80/instructions/ccf.cpp
        src/chips/z80/instructions/cp.cpp
        src/chips/z80/instructions/cpd.cpp
        src/chips/z80/instructions/cpdr.cpp
        src/chips/z80/instructions/cpi.cpp
        src/chips/z80/instructions/cpir.cpp
        src/chips/z80/instructions/cpl.cpp
        src/chips/z80/instructions/daa.cpp
        src/chips/z80/instructions/dec.cpp
        src/chips/z80/instructions/di.cpp
        src/chips/z80/instructions/djnz.cpp
        src/chips/z80/instructions/ei.cpp
        src/chips/z80/instructions/ex.cpp
        src/chips/z80/instructions/exx.cpp
        src/chips/z80/instructions/halt.cpp
        src/chips/z80/instructions/im.cpp
        src/chips/z80/instructions/in.cpp
        src/chips/z80/instructions/inc.cpp
        src/chips/z80/instructions/ind.cpp
        src/chips/z80/instructions/indr.cpp
        src/chips/z80/instructions/ini.cpp
        src/chips/z80/instructions/inir.cpp
        src/chips/z80/instructions/instruction_util.cpp
        src/chips/z80/instructions/jp.cpp
        src/chips/z80/instructions/jr.cpp
        src/chips/z80/instructions/ld.cpp
        src/chips/z80/instructions/ldd.cpp
        src/chips/z80/instructions/lddr.cpp
        src/chips/z80/instructions/ldi.cpp
        src/chips/z80/instructions/ldir.cpp
        src/chips/z80/instructions/neg.cpp
        src/chips/z80/instructions/nop.cpp
        src/chips/z80/instructions/or.cpp
        src/chips/z80/instructions/out.cpp
        src/chips/z80/instructions/outd.cpp
        src/chips/z80/instructions/outi.cpp
        src/chips/z80/instructions/otdr.cpp
        src/chips/z80/instructions/otir.cpp
        src/chips/z80/instructions/pop.cpp
        src/chips/z80/instructions/push.cpp
        src/chips/z80/instructions/res.cpp
        src/chips/z80/instructions/ret.cpp
        src/chips/z80/instructions/reti.cpp
        src/chips/z80/instructions/retn.cpp
        src/chips/z80/instructions/rl.cpp
        src/chips/z80/instructions/rlc.cpp
        src/chips/z80/instructions/rld.cpp
        src/chips/z80/instructions/rr.cpp
        src/chips/z80/instructions/rrd.cpp
        src/chips/z80/instructions/rrc.cpp
        src/chips/z80/instructions/rst.cpp
        src/chips/z80/instructions/sbc.cpp
        src/chips/z80/instructions/scf.cpp
        src/chips/z80/instructions/set.cpp
        src/chips/z80/instructions/sla.cpp
        src/chips/z80/instructions/sll.cpp
        src/chips/z80/instructions/sra.cpp
        src/chips/z80/instructions/srl.cpp
        src/chips/z80/instructions/sub.cpp
        src/chips/z80/instructions/xor.cpp
        )

set(SOURCES_Z80_H
        src/chips/z80/cpu.h
        src/chips/z80/disassemblerZ80.h
        src/chips/z80/emulator_memory.h
        src/chips/z80/flags.h
        src/chips/z80/interrupt_mode.h
        src/chips/z80/instructions/instruction_util.h
        src/chips/z80/instructions/instructions.h
        src/chips/z80/interfaces/gui_observer.h
        src/chips/z80/interfaces/in_observer.h
        src/chips/z80/interfaces/out_observer.h
        )
