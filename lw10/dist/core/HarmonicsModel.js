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
        // наружу отдаём копию массива (нельзя заменить сам массив)
        return [...this.harmonics];
    }
    add(input) {
        const id = cryptoLikeId();
        const h = new Harmonic_1.Harmonic(id, input);
        this.harmonics.push(h);
        this.emit("changed"); //emit("changed") — “событие произошло”
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
        h.update(patch);
        this.emit("changed"); //супер подробные тесты
    }
    sumAt(x) {
        return this.harmonics.reduce((acc, h) => acc + h.evaluate(x), 0);
    }
    buildSeries(xFrom = 0, xTo = 5, step = 0.05) {
        const xs = [];
        const ys = [];
        if (step <= 0)
            step = 0.05;
        if (xTo < xFrom)
            [xFrom, xTo] = [xTo, xFrom];
        for (let x = xFrom; x <= xTo + 1e-12; x += step) {
            const xr = Math.round(x * 1e12) / 1e12;
            xs.push(xr);
            ys.push(this.sumAt(xr));
        }
        return { xs, ys };
    }
}
exports.HarmonicsModel = HarmonicsModel;
function cryptoLikeId() {
    return Math.random().toString(16).slice(2) + Math.random().toString(16).slice(2);
}
