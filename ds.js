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
    "RAND",
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
    "RAND",
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

const toNumberHex = (num) => {
    const text = num.toString(16).toUpperCase();
    if (text.length == 1) {
        return "0" + text;
    }
    return text;
};

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

class StringFile {
    constructor () {
        /** @type {Array<string>} */
        this.texts = [];
    }
    write (str) {
        this.texts.push(str);
    }
    getText () {
        return this.texts.join("");
    }
}

/**
 * processFileContent
 * @param {Uint8Array} data 
 * @returns {string}
 */
const disassembleFileContent = (data) => {
    const f = new StringFile();
    const encoder = new TextEncoder();
    let pc = 0;
    while (pc < data.length) {
        f.write(`${toAddress((pc >> 8) & 0xFF, pc & 0xFF)}: `);
        const op = INSTRUCTION_NAMES[ data[ pc ] ];
        pc += 1;
        f.write(`${op}`);
        if (INST_ZERO_PARAM.includes(op)) {
            // do nothing
        } else if (INST_ONE_REG_PARAM.includes(op)) {
            const reg = data[ pc ];
            pc += 1;
            f.write(` ${toRegister(reg)}`);
        } else if (INST_ONE_ADDR_PARAM.includes(op)) {
            const haddr = data[ pc ];
            pc += 1;
            const laddr = data[ pc ];
            pc += 1;
            f.write(` ${toAddress(haddr, laddr)}`);
        } else if (INST_TWO_PARAM.includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            const param2 = data[ pc ];
            pc += 1;
            if ([ "STOR" ].includes(op)) {
                f.write(` ${toNumber(param1)}`);
            } else {
                f.write(` ${toRegister(param1)}`);
            }
            f.write(` ${toRegister(param2)}`);
        } else if ([ "TILE" ].includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            f.write(` ${toNumber(param1)}`);
            for (let i = 0; i < 8; i++) {
                const d = data[ pc ];
                pc += 1;
                f.write(` 0x${toNumberHex(d)}`);
            }
        } else if ([ "DTXT" ].includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            f.write(` ${toRegister(param1)}`);
            const [ pos, text ] = readString(data, pc);
            pc = pos;
            f.write(` "${toProgramString(text)}"`);
        } else if ([ "DSEL" ].includes(op)) {
            const param1 = data[ pc ];
            pc += 1;
            f.write(` ${toRegister(param1)}`);
            const [ pos, options ] = readOptions(data, pc);
            pc = pos;
            for (const text of options) {
                f.write(` "${toProgramString(text)}"`);
            }
        }
        f.write("\n");
    }
    return f.getText();
};

const __main__ = async () => {
    const file_content = await Deno.readFile(Deno.args[ 0 ]);
    const data = disassembleFileContent(file_content);
    const encoded = new TextEncoder();
    await Deno.writeFile("./a.txt", encoded.encode(data));
};

if (import.meta.main) {
    __main__();
}
