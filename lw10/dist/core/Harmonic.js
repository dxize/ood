"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Harmonic = void 0;
class Harmonic {
    constructor(id, input) {
        this.id = id;
        this.amplitude_ = input.amplitude;
        this.frequency_ = input.frequency;
        this.phase_ = input.phase;
        this.kind_ = input.kind;
    }
    get amplitude() {
        return this.amplitude_;
    }
    get frequency() {
        return this.frequency_;
    }
    get phase() {
        return this.phase_;
    }
    get kind() {
        return this.kind_;
    }
    update(patch) {
        if (patch.amplitude !== undefined)
            this.amplitude_ = patch.amplitude;
        if (patch.frequency !== undefined)
            this.frequency_ = patch.frequency;
        if (patch.phase !== undefined)
            this.phase_ = patch.phase;
        if (patch.kind !== undefined)
            this.kind_ = patch.kind;
    }
    evaluate(x) {
        const arg = this.frequency_ * x + this.phase_;
        const base = this.kind_ === "sin" ? Math.sin(arg) : Math.cos(arg);
        return this.amplitude_ * base;
    }
    toLabel() {
        const a = trimNumber(this.amplitude_);
        const f = trimNumber(this.frequency_);
        const pNum = this.phase_;
        const sign = pNum >= 0 ? "+" : "-";
        const absP = trimNumber(Math.abs(pNum));
        const fn = this.kind_;
        return `${a}*${fn}(${f}*x ${sign} ${absP})`;
    }
}
exports.Harmonic = Harmonic;
function trimNumber(n) {
    const s = n.toFixed(6);
    return s.replace(/\.?0+$/, "");
}
