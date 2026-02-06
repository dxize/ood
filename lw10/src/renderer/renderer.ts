import { HarmonicsModel } from "../core/HarmonicsModel";
import { HarmonicsViewModel} from "../core/HarmonicsViewModel";
import { HarmonicInput } from "../core/types";
import Chart from "chart.js/auto";


const model = new HarmonicsModel();
const vm = new HarmonicsViewModel(model);

vm.addHarmonic({ amplitude: 3, frequency: -3, phase: 0.3, kind: "sin" });
vm.addHarmonic({ amplitude: 4.38, frequency: 2.25, phase: 1.5, kind: "sin" });
vm.addHarmonic({ amplitude: 1, frequency: 1, phase: 5, kind: "cos" });

/* ссылки на элементы DOM-дерева */
const harmonicsList = document.getElementById("harmonicsList") as HTMLSelectElement;

const ampInput = document.getElementById("ampInput") as HTMLInputElement;
const freqInput = document.getElementById("freqInput") as HTMLInputElement;
const phaseInput = document.getElementById("phaseInput") as HTMLInputElement;
const sinRadio = document.getElementById("sinRadio") as HTMLInputElement;
const cosRadio = document.getElementById("cosRadio") as HTMLInputElement;
const selectedLabel = document.getElementById("selectedLabel") as HTMLDivElement;

const addBtn = document.getElementById("addBtn") as HTMLButtonElement;
const deleteBtn = document.getElementById("deleteBtn") as HTMLButtonElement;

const tabChart = document.getElementById("tabChart") as HTMLButtonElement;
const tabTable = document.getElementById("tabTable") as HTMLButtonElement;
const chartView = document.getElementById("chartView") as HTMLDivElement;
const tableView = document.getElementById("tableView") as HTMLDivElement;

const canvas = document.getElementById("chartCanvas") as HTMLCanvasElement;
const ctx = canvas.getContext("2d");
if (!ctx) throw new Error("Canvas 2D context not supported");

let chart: Chart | null = null;

function renderChart(): void {
  const series = vm.getSeries();

  const data = series.xs.map((x, i) => ({ x, y: series.ys[i] }));

  if (!chart) {
    chart = new Chart(ctx!, {
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

  chart.data.datasets[0].data = data as any;
  chart.update();
}

const modalBackdrop = document.getElementById("modalBackdrop") as HTMLDivElement;
const modal = document.getElementById("modal") as HTMLDivElement;
const modalAmp = document.getElementById("modalAmp") as HTMLInputElement;
const modalFreq = document.getElementById("modalFreq") as HTMLInputElement;
const modalPhase = document.getElementById("modalPhase") as HTMLInputElement;
const modalSin = document.getElementById("modalSin") as HTMLInputElement;
const modalCos = document.getElementById("modalCos") as HTMLInputElement;
const modalOk = document.getElementById("modalOk") as HTMLButtonElement;
const modalCancel = document.getElementById("modalCancel") as HTMLButtonElement;
const modalPreview = document.getElementById("modalPreview") as HTMLDivElement;

const tableBody = (document.querySelector("#dataTable tbody") as HTMLTableSectionElement);

/* Вспомогательные функции */
function toNumberSafe(value: string): number | null {
  if (value.trim() === "") return null;
  const n = Number(value);
  if (!Number.isFinite(n)) return null;
  return n;
}

function setActiveTab(which: "chart" | "table"): void {
  const chart = which === "chart";
  tabChart.classList.toggle("active", chart);
  tabTable.classList.toggle("active", !chart);
  chartView.classList.toggle("active", chart);
  tableView.classList.toggle("active", !chart);
}

function openModal(): void {
  modalBackdrop.classList.remove("hidden");
  modal.classList.remove("hidden");
  updateModalPreview();
}

function closeModal(): void {
  modalBackdrop.classList.add("hidden");
  modal.classList.add("hidden");
}

function currentModalInput(): HarmonicInput | null {
  const a = toNumberSafe(modalAmp.value);
  const f = toNumberSafe(modalFreq.value);
  const p = toNumberSafe(modalPhase.value);
  const kind = modalSin.checked ? "sin" : "cos";
  if (a === null || f === null || p === null) return null;
  return { amplitude: a, frequency: f, phase: p, kind };
}

function updateModalPreview(): void {
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
function render(): void {
  renderList();
  renderEditor();
  renderChart();
  renderTable();
}

function renderList(): void {
  const items = vm.getHarmonics();
  const selected = vm.getSelected();

  harmonicsList.innerHTML = "";
  for (const h of items) {
    const opt = document.createElement("option");
    opt.value = h.id;
    opt.textContent = h.toLabel();
    if (selected && selected.id === h.id) opt.selected = true;
    harmonicsList.appendChild(opt);
  }
}

function renderEditor(): void {
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

function renderTable(): void {
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

/*Связывание UI и логики (обработчики событий)*/

vm.on("changed", render);

harmonicsList.addEventListener("change", () => {
  const id = harmonicsList.value || null;
  vm.select(id);
});

ampInput.addEventListener("input", () => {
  const n = toNumberSafe(ampInput.value);
  if (n === null) return;
  vm.updateSelected({ amplitude: n });
});
freqInput.addEventListener("input", () => {
  const n = toNumberSafe(freqInput.value);
  if (n === null) return;
  vm.updateSelected({ frequency: n });
});
phaseInput.addEventListener("input", () => {
  const n = toNumberSafe(phaseInput.value);
  if (n === null) return;
  vm.updateSelected({ phase: n });
});
sinRadio.addEventListener("change", () => {
  if (!sinRadio.checked) return;
  vm.updateSelected({ kind: "sin" });
});
cosRadio.addEventListener("change", () => {
  if (!cosRadio.checked) return;
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
  if (!input) return;
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
