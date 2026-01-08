"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Harmonic = void 0;
class Harmonic {
    constructor(id, input) {
        this.id = id;
        this.amplitude = input.amplitude;
        this.frequency = input.frequency;
        this.phase = input.phase;
        this.kind = input.kind;
    }
    evaluate(x) {
        const arg = this.frequency * x + this.phase;
        const base = this.kind === "sin" ? Math.sin(arg) : Math.cos(arg);
        return this.amplitude * base;
    }
    toLabel() {
        const a = trimNumber(this.amplitude);
        const f = trimNumber(this.frequency);
        const pNum = this.phase; // число
        const sign = pNum >= 0 ? "+" : "-";
        const absP = trimNumber(Math.abs(pNum)); // строка для вывода
        const fn = this.kind === "sin" ? "sin" : "cos";
        return `${a}*${fn}(${f}*x ${sign} ${absP})`;
    }
}
exports.Harmonic = Harmonic;
function trimNumber(n) {
    // Красиво печатаем числа, но без “магии”
    const s = n.toFixed(6);
    return s.replace(/\.?0+$/, "");
}
