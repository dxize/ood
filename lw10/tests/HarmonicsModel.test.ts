import { HarmonicsModel } from "../src/core/HarmonicsModel";

describe("HarmonicsModel", () => {
  test("одна синусоидальная гармоника: значение при x=0", () => {
    const m = new HarmonicsModel();
    m.add({ amplitude: 2, frequency: 3, phase: 0, kind: "sin" });
    // 2*sin(0)=0
    expect(m.sumAt(0)).toBeCloseTo(0, 10);
  });

  test("одна косинусоидальная гармоника: значение при x=0", () => {
    const m = new HarmonicsModel();
    m.add({ amplitude: 2, frequency: 3, phase: 0, kind: "cos" });
    // 2*cos(0)=2
    expect(m.sumAt(0)).toBeCloseTo(2, 10);
  });

  test("сумма двух гармоник равна сумме их вкладов", () => {
    const m = new HarmonicsModel();
    m.add({ amplitude: 1, frequency: 1, phase: 0, kind: "sin" });
    m.add({ amplitude: 3, frequency: 2, phase: 0.5, kind: "cos" });

    const x = 1.234;
    const y = m.sumAt(x);

    const y2 = 1 * Math.sin(1 * x + 0) + 3 * Math.cos(2 * x + 0.5);
    expect(y).toBeCloseTo(y2, 10);
  });

  test("buildSeries формирует ожидаемую сетку x", () => {
    const m = new HarmonicsModel();
    m.add({ amplitude: 1, frequency: 1, phase: 0, kind: "sin" });

    const s = m.buildSeries(0, 0.2, 0.05);
    expect(s.xs).toEqual([0, 0.05, 0.1, 0.15, 0.2]);
    expect(s.ys.length).toBe(s.xs.length);
  });
});
