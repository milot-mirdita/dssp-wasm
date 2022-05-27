import { default as createDssp } from './mkdssp.js'
import mkdsspWasm from './mkdssp.wasm'

function dssp(pdb) {
    return new Promise((resolve, reject) => {
        let buffer = "";
        createDssp({
            locateFile: () => mkdsspWasm,
            print: (msg) => buffer += msg + "\n"
        }).then((instance) => {
            instance.FS.writeFile('/pdb.pdb', pdb);
            const err = instance.callMain([
                "/pdb.pdb",
            ]);
            if (err == 0) {
                resolve(buffer)
            } else {
                reject(err)
            }
        });
    })
}

function parseSecStruct(dssp) {
    const lines = dssp.split('\n');
    const dsspMapping = {
        'H' : 'H',
        'G' : 'H',
        'E' : 'E',
        'B' : 'E',
        'I' : 'C',
        'T' : 'C',
        'S' : 'C',
        ' ' : 'C',
    };
    let start = false;
    let ss = "";
    for (let i = 0; i < lines.length; i++) {
        const line = lines[i];
        if (start == false && line.startsWith("  #")) {
            start = true;
            continue;
        }

        if (start == false) {
            continue;
        }

        if (line.length < 17) {
            continue;
        }
        ss += dsspMapping[line.substr(16, 1)];
    }
    return ss;
}

export { dssp, parseSecStruct };

