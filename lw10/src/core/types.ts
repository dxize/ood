export type HarmonicKind = "sin" | "cos";

export type HarmonicInput = {
  amplitude: number;
  frequency: number;
  phase: number;
  kind: HarmonicKind;
};

export type SampleSeries = {
  xs: number[];
  ys: number[];
};
