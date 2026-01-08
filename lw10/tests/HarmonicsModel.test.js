"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const HarmonicsModel_1 = require("../src/core/HarmonicsModel");
describe("HarmonicsModel", () => {
    test("single sin harmonic evaluate at x=0", () => {
        const m = new HarmonicsModel_1.HarmonicsModel();
        m.add({ amplitude: 2, frequency: 3, phase: 0, kind: "sin" });
        // 2*sin(0)=0
        expect(m.sumAt(0)).toBeCloseTo(0, 10);
    });
    test("single cos harmonic evaluate at x=0", () => {
        const m = new HarmonicsModel_1.HarmonicsModel();
        m.add({ amplitude: 2, frequency: 3, phase: 0, kind: "cos" });
        // 2*cos(0)=2
        expect(m.sumAt(0)).toBeCloseTo(2, 10);
    });
    test("sum of two harmonics equals sum of parts", () => {
        const m = new HarmonicsModel_1.HarmonicsModel();
        m.add({ amplitude: 1, frequency: 1, phase: 0, kind: "sin" });
        m.add({ amplitude: 3, frequency: 2, phase: 0.5, kind: "cos" });
        const x = 1.234;
        const y = m.sumAt(x);
        // посчитаем напрямую
        const y2 = 1 * Math.sin(1 * x + 0) + 3 * Math.cos(2 * x + 0.5);
        expect(y).toBeCloseTo(y2, 10);
    });
    test("buildSeries produces expected x grid", () => {
        const m = new HarmonicsModel_1.HarmonicsModel();
        // пусть будет хотя бы одна гармоника
        m.add({ amplitude: 1, frequency: 1, phase: 0, kind: "sin" });
        const s = m.buildSeries(0, 0.2, 0.05);
        expect(s.xs).toEqual([0, 0.05, 0.1, 0.15, 0.2]);
        expect(s.ys.length).toBe(s.xs.length);
    });
});
