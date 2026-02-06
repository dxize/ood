import { EventEmitter } from "events";
import { HarmonicsModel } from "./HarmonicsModel";
import { HarmonicInput, HarmonicKind, SampleSeries } from "./types";
import { Harmonic } from "./Harmonic";

/**
 * MVVM:
 * - Model: HarmonicsModel (список гармоник, расчёты)
 * - ViewModel: HarmonicsViewModel (выбранная гармоника + команды + события)
 * - View: DOM/Canvas в renderer (подписывается на события VM)
 */
export class HarmonicsViewModel extends EventEmitter {
  private readonly model: HarmonicsModel;
  private selectedId: string | null = null;

  private xFrom = 0;
  private xTo = 5;
  private step = 0.05;

  constructor(model: HarmonicsModel) {
    super();
    this.model = model;

    this.model.on("changed", () => {
      if (this.selectedId && !this.model.list().some(h => h.id === this.selectedId)) {
        this.selectedId = null;
      }
      this.emit("changed");
    });
  }


  getHarmonics(): Harmonic[] {
    return this.model.list();
  }

  getSelected(): Harmonic | null {
    if (!this.selectedId) return null;
    return this.model.list().find(h => h.id === this.selectedId) ?? null;
  }

  getSeries(): SampleSeries {
    return this.model.buildSeries(this.xFrom, this.xTo, this.step);
  }

  //действия пользователя (Commands) 

  select(id: string | null): void {
    this.selectedId = id;
    this.emit("changed");
  }

  addHarmonic(input: HarmonicInput): void {
    const created = this.model.add(input);
    this.selectedId = created.id;
    this.emit("changed");
  }

  deleteSelected(): void {
    if (!this.selectedId) return;
    this.model.remove(this.selectedId);
  }

  updateSelected(patch: Partial<HarmonicInput>): void {
    if (!this.selectedId) return;
    this.model.update(this.selectedId, patch);
  }
}
