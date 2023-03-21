const INSTRUCTION_NAMES = [
    "NOPE",
    "EXIT",
    "WKEY",
    "GKEY",
    "TICK",
    "SAVE",
    "LOAD",
    "TILE",
    "STOR",
    "MOVE",
    "ADDI",
    "SUBI",
    "MULI",
    "DIVI",
    "MODI",
    "INVI",
    "JUMP",
    "JPEZ",
    "JPGZ",
    "JPLZ",
    "CALL",
    "RETN",
    "DTIL",
    "DNUM",
    "DTXT",
    "DSEL",
];
const INST_ZERO_PARAM = [
    "NOPE",
    "EXIT",
    "WKEY",
    "GKEY",
    "TICK",
    "SAVE",
    "LOAD",
    "INVI",
    "RETN",
];
const INST_ONE_REG_PARAM = [
    "ADDI",
    "SUBI",
    "MULI",
    "DIVI",
    "MODI",
];
const INST_ONE_ADDR_PARAM = [
    "JUMP",
    "JPEZ",
    "JPGZ",
    "JPLZ",
    "CALL",
];
const INST_TWO_PARAM = [
    "STOR",
    "MOVE",
    "DTIL",
    "DNUM",
];

const toAddress = (hnum, lnum) => {
    const num = (hnum << 8) | lnum;
    let text = num.toString(16);
    while (text.length < 4) {
        text = "0" + text;
    }
    return "l" + text;
};

const toRegister = (num) => {
    if (num === 0) {
        return "r0";
    } else if (num < 5) {
        return `rc${num - 1}`;
    } else if (num < 112) {
        let text = num.toString(10);
        while (text.length < 3) {
            text = "0" + text;
        }
        return "r" + text;
    } else if (num < 128) {
        return `rs${num - 112}`;
    } else {
        let text = (num - 128).toString(10);
        while (text.length < 3) {
            text = "0" + text;
        }
        return "*r" + text;
    }
};

const toNumber = (num) => { return num.toString(10); };

const toNumberHex = (num) => { return num.toString(16).toUpperCase(); };

const toProgramString = (text) => {
    return text.replaceAll("\\", "\\\\")
        .replaceAll("\'", "\\\'")
        .replaceAll("\"", "\\\"");
};

/**
 * 
 * @param {Uint8Array} data 
 * @param {number} offset 
 */
const readString = (data, offset) => {
    let size = 0;
    while (data[ offset + size ] != 0) {
        size += 1;
    }
    const decoder = new TextDecoder();
    const text = decoder.decode(data.slice(offset, offset + size));
    return [ offset + size + 1, text ];
};

const readOptions = (data, initOffset) => {
    let offset = initOffset;
    const options = [];
    while (data[ offset ] != 0) {
        const [ off, text ] = readString(data, offset);
        options.push(text);
        offset = off;
    }
    return [ offset + 1, options ];
};

const processFile = async (file, out) => {
    const data = await Deno.readFile(file);
    const f = await Deno.open(out, { write: true, create: true });
    const encoder = new TextEncoder();
    let pc = 0;
    while (pc < data.length) {
        await f.write(encoder.encode(`${toAddress((pc >> 8) & 0xFF, pc & 0xFF)}: `));
        const op = INSTRUCTION_NAMES[ data[ pc ] ];
        pc += 1;
        await f.write(encoder.encode(`${op}`));
        if (INST_ZERO_PARAM.includes(op)) {
            // do nothing
        } else if (INST_ONE_REG_PARAM.includes(op)) {
            const reg = data[ pc ];
            pc += 1;
            await f.write(encoder.encode(` ${toRegister(reg)}`));
        } else if (INST_ONE_ADDR_PARAM.includes(op)) {
            const haddr = data[ pc ];
            pc += 1;
            const laddr = data[ pc ];
            pc += 1;
            await f.write(encoder.encode(` ${toAddress(haddr, laddr)}`));
        } else if (INST_TWO_PARAM.includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            const param2 = data[ pc ];
            pc += 1;
            if ([ "STOR" ].includes(op)) {
                await f.write(encoder.encode(` ${toNumber(param1)}`));
            } else {
                await f.write(encoder.encode(` ${toRegister(param1)}`));
            }
            if ([ "DTIL" ].includes(op)) {
                await f.write(encoder.encode(` ${toNumber(param2)}`));
            } else {
                await f.write(encoder.encode(` ${toRegister(param2)}`));
            }
        } else if ([ "TILE" ].includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            await f.write(encoder.encode(` ${toNumber(param1)}`));
            for (let i = 0; i < 8; i++) {
                const d = data[ pc ];
                pc += 1;
                await f.write(encoder.encode(` 0x${toNumberHex(d)}`));
            }
        } else if ([ "DTXT" ].includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            await f.write(encoder.encode(` ${toRegister(param1)}`));
            const [ pos, text ] = readString(data, pc);
            pc = pos;
            await f.write(encoder.encode(` "${toProgramString(text)}"`));
        } else if ([ "DSEL" ].includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            await f.write(encoder.encode(` ${toRegister(param1)}`));
            const [ pos, options ] = readOptions(data, pc);
            pc = pos;
            for (const text of options) {
                await f.write(encoder.encode(` "${toProgramString(text)}"`));
            }
        }
        await f.write(encoder.encode("\n"));
    }
    f.close();
};

const __main__ = async () => {
    await processFile(Deno.args[ 0 ], "./a.txt");
};

if (import.meta.main) {
    __main__();
}
