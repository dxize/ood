import { EventEmitter } from "events";
import { Harmonic } from "./Harmonic";
import { HarmonicInput, SampleSeries } from "./types";

export class HarmonicsModel extends EventEmitter {
  private harmonics: Harmonic[] = [];

  list(): Harmonic[] {
    // наружу отдаём копию массива (нельзя заменить сам массив)
    return [...this.harmonics];
  }

  add(input: HarmonicInput): Harmonic {
    const id = cryptoLikeId();
    const h = new Harmonic(id, input);
    this.harmonics.push(h);
    this.emit("changed"); //emit("changed") — “событие произошло”
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

    h.update(patch);

    this.emit("changed");//супер подробные тесты
  }

  sumAt(x: number): number {
    return this.harmonics.reduce((acc, h) => acc + h.evaluate(x), 0);
  }

  buildSeries(xFrom = 0, xTo = 5, step = 0.05): SampleSeries {
    const xs: number[] = [];
    const ys: number[] = [];

    if (step <= 0) step = 0.05;
    if (xTo < xFrom) [xFrom, xTo] = [xTo, xFrom];

    for (let x = xFrom; x <= xTo + 1e-12; x += step) {
      const xr = Math.round(x * 1e12) / 1e12;
      xs.push(xr);
      ys.push(this.sumAt(xr));
    }
    return { xs, ys };
  }
}

function cryptoLikeId(): string {
  return Math.random().toString(16).slice(2) + Math.random().toString(16).slice(2);
}
