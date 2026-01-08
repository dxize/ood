const fs = require("fs");
const path = require("path");

function copyFile(src, dst) {
  fs.mkdirSync(path.dirname(dst), { recursive: true });
  fs.copyFileSync(src, dst);
}

const srcDir = path.join(__dirname, "..", "src", "renderer");
const dstDir = path.join(__dirname, "..", "dist", "renderer");

copyFile(path.join(srcDir, "index.html"), path.join(dstDir, "index.html"));
copyFile(path.join(srcDir, "styles.css"), path.join(dstDir, "styles.css"));
copyFile(path.join(srcDir, "boot.js"), path.join(dstDir, "boot.js"));

console.log("Static files copied to dist/renderer");
