"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.HarmonicsViewModel = void 0;
const events_1 = require("events");
/**
 * MVVM:
 * - Model: HarmonicsModel (список гармоник, расчёты)
 * - ViewModel: HarmonicsViewModel (выбранная гармоника + команды + события)
 * - View: DOM/Canvas в renderer (подписывается на события VM)
 */
class HarmonicsViewModel extends events_1.EventEmitter {
    constructor(model) {
        super();
        this.selectedId = null;
        this.xFrom = 0;
        this.xTo = 5;
        this.step = 0.05;
        this.model = model;
        this.model.on("changed", () => {
            if (this.selectedId && !this.model.list().some(h => h.id === this.selectedId)) {
                this.selectedId = null;
            }
            this.emit("changed");
        });
    }
    getHarmonics() {
        return this.model.list();
    }
    getSelected() {
        if (!this.selectedId)
            return null;
        return this.model.list().find(h => h.id === this.selectedId) ?? null;
    }
    getSeries() {
        return this.model.buildSeries(this.xFrom, this.xTo, this.step);
    }
    //действия пользователя (Commands) 
    select(id) {
        this.selectedId = id;
        this.emit("changed");
    }
    addHarmonic(input) {
        const created = this.model.add(input);
        this.selectedId = created.id;
        this.emit("changed");
    }
    deleteSelected() {
        if (!this.selectedId)
            return;
        this.model.remove(this.selectedId);
    }
    updateSelected(patch) {
        if (!this.selectedId)
            return;
        this.model.update(this.selectedId, patch);
    }
}
exports.HarmonicsViewModel = HarmonicsViewModel;
