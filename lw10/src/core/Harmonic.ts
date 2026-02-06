import { HarmonicInput, HarmonicKind } from "./types";

export class Harmonic {
  public readonly id: string;

  private amplitude_: number;
  private frequency_: number;
  private phase_: number;
  private kind_: HarmonicKind;

  constructor(id: string, input: HarmonicInput) {
    this.id = id;
    this.amplitude_ = input.amplitude;
    this.frequency_ = input.frequency;
    this.phase_ = input.phase;
    this.kind_ = input.kind;
  }

  get amplitude(): number { 
    return this.amplitude_; 
  }
  get frequency(): number { 
    return this.frequency_; 
  }
  get phase(): number { 
    return this.phase_; 
  }
  get kind(): HarmonicKind { 
    return this.kind_; 
  }

  
  update(patch: Partial<HarmonicInput>): void {
    if (patch.amplitude !== undefined) this.amplitude_ = patch.amplitude;
    if (patch.frequency !== undefined) this.frequency_ = patch.frequency;
    if (patch.phase !== undefined) this.phase_ = patch.phase;
    if (patch.kind !== undefined) this.kind_ = patch.kind;
  }

  evaluate(x: number): number {
    const arg = this.frequency_ * x + this.phase_;
    const base = this.kind_ === "sin" ? Math.sin(arg) : Math.cos(arg);
    return this.amplitude_ * base;
  }

  toLabel(): string {
    const a = trimNumber(this.amplitude_);
    const f = trimNumber(this.frequency_);

    const pNum = this.phase_;
    const sign = pNum >= 0 ? "+" : "-";
    const absP = trimNumber(Math.abs(pNum));

    const fn = this.kind_;
    return `${a}*${fn}(${f}*x ${sign} ${absP})`;
  }
}

function trimNumber(n: number): string {
  const s = n.toFixed(6);
  return s.replace(/\.?0+$/, "");
}
