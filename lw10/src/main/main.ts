import { app, BrowserWindow } from "electron";
import * as path from "path";

function createWindow(): void {
  const win = new BrowserWindow({
    width: 980,
    height: 640,
    webPreferences: {
      // Для учебного проекта так проще: renderer может require(...) скомпилированные TS-модули.
      nodeIntegration: true,
      contextIsolation: false
    }
  });

  const indexHtmlPath = path.join(__dirname, "..", "renderer", "index.html");
  win.loadFile(indexHtmlPath);
}

app.whenReady().then(() => {
  createWindow();

  app.on("activate", () => {
    if (BrowserWindow.getAllWindows().length === 0) createWindow();
  });
});

app.on("window-all-closed", () => {
  if (process.platform !== "darwin") app.quit();
});
