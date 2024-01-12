const canvas = document.getElementById("canvas");
import EngineRuntime from "./lib.mjs";

function resizeCanvas() {
	canvas.width = canvas.offsetWidth;
	canvas.height = canvas.offsetHeight;
}

window.addEventListener("resize", resizeCanvas);

(async () => {
	const moduleArgs = {
		print: (text) => {
			console.log(text);
		},
		printErr: (text) => {
			console.log(text);
		},
		canvas,
		setStatus: (text) => {
			console.log(text);
		},
	};

	const engine = await EngineRuntime(moduleArgs);
})();
