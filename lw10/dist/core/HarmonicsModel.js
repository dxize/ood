"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.HarmonicsModel = void 0;
const events_1 = require("events");
const Harmonic_1 = require("./Harmonic");
class HarmonicsModel extends events_1.EventEmitter {
    constructor() {
        super(...arguments);
        this.harmonics = [];
    }
    list() {
        // наружу отдаём копию массива (чтобы не ломали напрямую)
        return [...this.harmonics];
    }
    add(input) {
        const id = cryptoLikeId();
        const h = new Harmonic_1.Harmonic(id, input);
        this.harmonics.push(h);
        this.emit("changed");
        return h;
    }
    remove(id) {
        const before = this.harmonics.length;
        this.harmonics = this.harmonics.filter(h => h.id !== id);
        if (this.harmonics.length !== before)
            this.emit("changed");
    }
    update(id, patch) {
        const h = this.harmonics.find(x => x.id === id);
        if (!h)
            return;
        if (patch.amplitude !== undefined)
            h.amplitude = patch.amplitude;
        if (patch.frequency !== undefined)
            h.frequency = patch.frequency;
        if (patch.phase !== undefined)
            h.phase = patch.phase;
        if (patch.kind !== undefined)
            h.kind = patch.kind;
        this.emit("changed");
    }
    sumAt(x) {
        return this.harmonics.reduce((acc, h) => acc + h.evaluate(x), 0);
    }
    buildSeries(xFrom = 0, xTo = 5, step = 0.05) {
        const xs = [];
        const ys = [];
        // защита от плохих параметров
        if (step <= 0)
            step = 0.05;
        if (xTo < xFrom)
            [xFrom, xTo] = [xTo, xFrom];
        // строим в точности как в “табличке”: 0, 0.05, 0.10, ...
        for (let x = xFrom; x <= xTo + 1e-12; x += step) {
            // округление чтобы не было 0.1500000000002
            const xr = Math.round(x * 1e12) / 1e12;
            xs.push(xr);
            ys.push(this.sumAt(xr));
        }
        return { xs, ys };
    }
}
exports.HarmonicsModel = HarmonicsModel;
function cryptoLikeId() {
    // Без внешних зависимостей: достаточно для идентификатора.
    // (Не криптостойко — и не нужно.)
    return Math.random().toString(16).slice(2) + Math.random().toString(16).slice(2);
}
