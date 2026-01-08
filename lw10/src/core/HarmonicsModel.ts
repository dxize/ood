import { EventEmitter } from "events";
import { Harmonic } from "./Harmonic";
import { HarmonicInput, SampleSeries } from "./types";

export class HarmonicsModel extends EventEmitter {
  private harmonics: Harmonic[] = [];

  list(): Harmonic[] {
    // наружу отдаём копию массива (чтобы не ломали напрямую)
    return [...this.harmonics];
  }

  add(input: HarmonicInput): Harmonic {
    const id = cryptoLikeId();
    const h = new Harmonic(id, input);
    this.harmonics.push(h);
    this.emit("changed");
    return h;
  }

  remove(id: string): void {
    const before = this.harmonics.length;
    this.harmonics = this.harmonics.filter(h => h.id !== id);
    if (this.harmonics.length !== before) this.emit("changed");
  }

  update(id: string, patch: Partial<HarmonicInput>): void {
    const h = this.harmonics.find(x => x.id === id);
    if (!h) return;

    if (patch.amplitude !== undefined) h.amplitude = patch.amplitude;
    if (patch.frequency !== undefined) h.frequency = patch.frequency;
    if (patch.phase !== undefined) h.phase = patch.phase;
    if (patch.kind !== undefined) h.kind = patch.kind;

    this.emit("changed");
  }

  sumAt(x: number): number {
    return this.harmonics.reduce((acc, h) => acc + h.evaluate(x), 0);
  }

  buildSeries(xFrom = 0, xTo = 5, step = 0.05): SampleSeries {
    const xs: number[] = [];
    const ys: number[] = [];

    // защита от плохих параметров
    if (step <= 0) step = 0.05;
    if (xTo < xFrom) [xFrom, xTo] = [xTo, xFrom];

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

function cryptoLikeId(): string {
  // Без внешних зависимостей: достаточно для идентификатора.
  // (Не криптостойко — и не нужно.)
  return Math.random().toString(16).slice(2) + Math.random().toString(16).slice(2);
}
