// fractal.js - draws a Mandelbrot set with WebGL.
// Usage: <script src="fractal.js"></script> in your HTML (no canvas needed,
// it creates one). If a <canvas id="canvas"> already exists, it uses that.

const vsSource = `
    attribute vec2 a_position;
    void main() {
        gl_Position = vec4(a_position, 0.0, 1.0);
    }
`;


function compileShader(gl, type, source) {
    const shader = gl.createShader(type);
    gl.shaderSource(shader, source);
    gl.compileShader(shader);
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        const log = gl.getShaderInfoLog(shader);
        gl.deleteShader(shader);
        throw new Error("Shader compile error:\n" + log);
    }
    return shader;
}

function createProgram(gl, vsSource, fsSource) {
    const program = gl.createProgram();
    gl.attachShader(program, compileShader(gl, gl.VERTEX_SHADER, vsSource));
    gl.attachShader(program, compileShader(gl, gl.FRAGMENT_SHADER, fsSource));
    gl.linkProgram(program);
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
        throw new Error("Program link error:\n" + gl.getProgramInfoLog(program));
    }
    return program;
}

async function loadText(url) {
    const response = await fetch(url);
    if (!response.ok) {
        throw new Error("Could not load " + url + " (" + response.status + ")");
    }
    return response.text();
}


export async function main(fragment) {
    const fsSource = await loadText(`./fractal/${fragment}`)
    // Get or create the canvas
    let canvas = document.getElementById("canvas");
    if (!canvas) {
        canvas = document.createElement("canvas");
        document.body.style.margin = "0";
        document.body.appendChild(canvas);
    }
    canvas.width = window.innerWidth;
    canvas.height = window.innerHeight;

    const gl = canvas.getContext("webgl");
    if (!gl) {
        alert("WebGL is not supported in this browser.");
        return;
    }

    const program = createProgram(gl, vsSource, fsSource);
    gl.useProgram(program);

    // Fullscreen quad (two triangles as a strip)
    const vertices = new Float32Array([
        -1, -1,
        1, -1,
        -1, 1,
        1, 1,
    ]);
    const buffer = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);

    const posLoc = gl.getAttribLocation(program, "a_position");
    gl.enableVertexAttribArray(posLoc);
    gl.vertexAttribPointer(posLoc, 2, gl.FLOAT, false, 0, 0);

    // Pass the canvas size to the fragment shader
    const resLoc = gl.getUniformLocation(program, "u_resolution");
    const mouseLoc = gl.getUniformLocation(program, "u_mouse");

    function draw() {
        gl.viewport(0, 0, canvas.width, canvas.height);
        gl.uniform2f(resLoc, canvas.width, canvas.height);
        gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
    }
    
    canvas.addEventListener("mousemove", (e) => {
        const rect = canvas.getBoundingClientRect();

        // Scale in case the canvas is displayed at a different size than its resolution
        const x = (e.clientX - rect.left) * (canvas.width / rect.width);
        // Flip Y: the mouse origin is top-left, but gl_FragCoord's origin is bottom-left
        const y = canvas.height - (e.clientY - rect.top) * (canvas.height / rect.height);

        gl.uniform2f(mouseLoc, x, y);
        draw(gl);
    });

    draw(gl);
}
