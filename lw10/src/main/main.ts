import { app, BrowserWindow } from "electron";
import * as path from "path";

function createWindow(): void {
  const win = new BrowserWindow({
    width: 980,
    height: 640,
    webPreferences: {
      nodeIntegration: true, // Разрешает в HTML/JS внутри окна использовать Node.js функции, например require(...)
      contextIsolation: false 
    }
  });

  const indexHtmlPath = path.join(__dirname, "..", "renderer", "index.html");
  win.loadFile(indexHtmlPath);
}

app.whenReady().then(() => {
  createWindow();
});

app.on("window-all-closed", () => {
  app.quit();
});
