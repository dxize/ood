import { HarmonicInput, HarmonicKind } from "./types";

export class Harmonic {
  public readonly id: string;
  public amplitude: number;
  public frequency: number;
  public phase: number;
  public kind: HarmonicKind;

  constructor(id: string, input: HarmonicInput) {
    this.id = id;
    this.amplitude = input.amplitude;
    this.frequency = input.frequency;
    this.phase = input.phase;
    this.kind = input.kind;
  }

  evaluate(x: number): number {
    const arg = this.frequency * x + this.phase;
    const base = this.kind === "sin" ? Math.sin(arg) : Math.cos(arg);
    return this.amplitude * base;
  }

  toLabel(): string {
    const a = trimNumber(this.amplitude);
    const f = trimNumber(this.frequency);

    const pNum = this.phase;                 // число
    const sign = pNum >= 0 ? "+" : "-";
    const absP = trimNumber(Math.abs(pNum)); // строка для вывода

    const fn = this.kind === "sin" ? "sin" : "cos";
    return `${a}*${fn}(${f}*x ${sign} ${absP})`;
  }

}

function trimNumber(n: number): string {
  // Красиво печатаем числа, но без “магии”
  const s = n.toFixed(6);
  return s.replace(/\.?0+$/, "");
}
