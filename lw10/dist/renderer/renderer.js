"use strict";
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const HarmonicsModel_1 = require("../core/HarmonicsModel");
const HarmonicsViewModel_1 = require("../core/HarmonicsViewModel");
const auto_1 = __importDefault(require("chart.js/auto"));
const model = new HarmonicsModel_1.HarmonicsModel();
const vm = new HarmonicsViewModel_1.HarmonicsViewModel(model);
vm.addHarmonic({ amplitude: 3, frequency: -3, phase: 0.3, kind: "sin" });
vm.addHarmonic({ amplitude: 4.38, frequency: 2.25, phase: 1.5, kind: "sin" });
vm.addHarmonic({ amplitude: 1, frequency: 1, phase: 5, kind: "cos" });
/* ссылки на элементы DOM-дерева */
const harmonicsList = document.getElementById("harmonicsList");
const ampInput = document.getElementById("ampInput");
const freqInput = document.getElementById("freqInput");
const phaseInput = document.getElementById("phaseInput");
const sinRadio = document.getElementById("sinRadio");
const cosRadio = document.getElementById("cosRadio");
const selectedLabel = document.getElementById("selectedLabel");
const addBtn = document.getElementById("addBtn");
const deleteBtn = document.getElementById("deleteBtn");
const tabChart = document.getElementById("tabChart");
const tabTable = document.getElementById("tabTable");
const chartView = document.getElementById("chartView");
const tableView = document.getElementById("tableView");
const canvas = document.getElementById("chartCanvas");
const ctx = canvas.getContext("2d");
if (!ctx)
    throw new Error("Canvas 2D context not supported");
let chart = null;
function renderChart() {
    const series = vm.getSeries();
    const data = series.xs.map((x, i) => ({ x, y: series.ys[i] }));
    if (!chart) {
        chart = new auto_1.default(ctx, {
            type: "line",
            data: {
                datasets: [
                    {
                        label: "y(x)",
                        data,
                        borderWidth: 2,
                        pointRadius: 0,
                        tension: 0.15
                    }
                ]
            },
            options: {
                responsive: true,
                maintainAspectRatio: false,
                animation: false,
                parsing: false,
                scales: {
                    x: { type: "linear", title: { display: true, text: "x" } },
                    y: { title: { display: true, text: "y" } }
                }
            }
        });
        return;
    }
    chart.data.datasets[0].data = data;
    chart.update();
}
const modalBackdrop = document.getElementById("modalBackdrop");
const modal = document.getElementById("modal");
const modalAmp = document.getElementById("modalAmp");
const modalFreq = document.getElementById("modalFreq");
const modalPhase = document.getElementById("modalPhase");
const modalSin = document.getElementById("modalSin");
const modalCos = document.getElementById("modalCos");
const modalOk = document.getElementById("modalOk");
const modalCancel = document.getElementById("modalCancel");
const modalPreview = document.getElementById("modalPreview");
const tableBody = document.querySelector("#dataTable tbody");
/* Вспомогательные функции */
function toNumberSafe(value) {
    if (value.trim() === "")
        return null;
    const n = Number(value);
    if (!Number.isFinite(n))
        return null;
    return n;
}
function setActiveTab(which) {
    const chart = which === "chart";
    tabChart.classList.toggle("active", chart);
    tabTable.classList.toggle("active", !chart);
    chartView.classList.toggle("active", chart);
    tableView.classList.toggle("active", !chart);
}
function openModal() {
    modalBackdrop.classList.remove("hidden");
    modal.classList.remove("hidden");
    updateModalPreview();
}
function closeModal() {
    modalBackdrop.classList.add("hidden");
    modal.classList.add("hidden");
}
function currentModalInput() {
    const a = toNumberSafe(modalAmp.value);
    const f = toNumberSafe(modalFreq.value);
    const p = toNumberSafe(modalPhase.value);
    const kind = modalSin.checked ? "sin" : "cos";
    if (a === null || f === null || p === null)
        return null;
    return { amplitude: a, frequency: f, phase: p, kind };
}
function updateModalPreview() {
    const input = currentModalInput();
    if (!input) {
        modalPreview.textContent = "Введите корректные числа";
        return;
    }
    const fn = input.kind === "sin" ? "sin" : "cos";
    const sign = input.phase >= 0 ? "+" : "-";
    const absP = Math.abs(input.phase);
    modalPreview.textContent = `${input.amplitude}*${fn}(${input.frequency}*x ${sign} ${absP})`;
}
/* Обновление интерфейса (отрисовка) */
function render() {
    renderList();
    renderEditor();
    renderChart();
    renderTable();
}
function renderList() {
    const items = vm.getHarmonics();
    const selected = vm.getSelected();
    harmonicsList.innerHTML = "";
    for (const h of items) {
        const opt = document.createElement("option");
        opt.value = h.id;
        opt.textContent = h.toLabel();
        if (selected && selected.id === h.id)
            opt.selected = true;
        harmonicsList.appendChild(opt);
    }
}
function renderEditor() {
    const selected = vm.getSelected();
    const enabled = !!selected;
    ampInput.disabled = !enabled;
    freqInput.disabled = !enabled;
    phaseInput.disabled = !enabled;
    sinRadio.disabled = !enabled;
    cosRadio.disabled = !enabled;
    deleteBtn.disabled = !enabled;
    if (!selected) {
        ampInput.value = "";
        freqInput.value = "";
        phaseInput.value = "";
        sinRadio.checked = false;
        cosRadio.checked = false;
        selectedLabel.textContent = "—";
        return;
    }
    ampInput.value = String(selected.amplitude);
    freqInput.value = String(selected.frequency);
    phaseInput.value = String(selected.phase);
    sinRadio.checked = selected.kind === "sin";
    cosRadio.checked = selected.kind === "cos";
    selectedLabel.textContent = selected.toLabel();
}
function renderTable() {
    const series = vm.getSeries();
    tableBody.innerHTML = "";
    for (let i = 0; i < series.xs.length; i++) {
        const tr = document.createElement("tr");
        const tdX = document.createElement("td");
        const tdY = document.createElement("td");
        tdX.textContent = series.xs[i].toFixed(2).replace(/\.?0+$/, "");
        tdY.textContent = series.ys[i].toFixed(6).replace(/\.?0+$/, "");
        tr.appendChild(tdX);
        tr.appendChild(tdY);
        tableBody.appendChild(tr);
    }
}
/* Связывание UI и логики (обработчики событий) */
vm.on("changed", render);
harmonicsList.addEventListener("change", () => {
    const id = harmonicsList.value || null;
    vm.select(id);
});
ampInput.addEventListener("input", () => {
    const n = toNumberSafe(ampInput.value);
    if (n === null)
        return;
    vm.updateSelected({ amplitude: n });
});
freqInput.addEventListener("input", () => {
    const n = toNumberSafe(freqInput.value);
    if (n === null)
        return;
    vm.updateSelected({ frequency: n });
});
phaseInput.addEventListener("input", () => {
    const n = toNumberSafe(phaseInput.value);
    if (n === null)
        return;
    vm.updateSelected({ phase: n });
});
sinRadio.addEventListener("change", () => {
    if (!sinRadio.checked)
        return;
    vm.updateSelected({ kind: "sin" });
});
cosRadio.addEventListener("change", () => {
    if (!cosRadio.checked)
        return;
    vm.updateSelected({ kind: "cos" });
});
addBtn.addEventListener("click", () => openModal());
deleteBtn.addEventListener("click", () => vm.deleteSelected());
tabChart.addEventListener("click", () => setActiveTab("chart"));
tabTable.addEventListener("click", () => setActiveTab("table"));
modalBackdrop.addEventListener("click", closeModal);
modalCancel.addEventListener("click", closeModal);
modalOk.addEventListener("click", () => {
    const input = currentModalInput();
    if (!input)
        return;
    vm.addHarmonic(input);
    closeModal();
});
modalAmp.addEventListener("input", updateModalPreview);
modalFreq.addEventListener("input", updateModalPreview);
modalPhase.addEventListener("input", updateModalPreview);
modalSin.addEventListener("change", updateModalPreview);
modalCos.addEventListener("change", updateModalPreview);
setActiveTab("chart");
render();
