/* WINDOW RELATED GLOBAL VARIABLES */
var canvas = null;
/** @type {WebGLRenderingContext}/ */
var gl = null;
var bFullscreen = false;
var canvas_original_width;
var canvas_original_height;

/* WEBGL RELATED GLOBAL VARIABLES */
const webGLMacros =
{
    PRJ_ATTRIBUTE_POSITION: 0,
    PRJ_ATTRIBUTE_COLOR: 1,
    PRJ_ATTRIBUTE_NORMAL: 2,
    PRJ_ATTRIBUTE_TEXTURE0: 3,
};

var shaderProgramObject;

var vao_triangle;
var vbo_triangle;

var vao_Square;
var vbo_Square;

var vao_Circle;
var vbo_Circle;

var vao_Line;
var vbo_Line;

var colorUniform;
var mvpMatrixUniform;
var color = [1.0, 1.0, 1.0];
var perspectiveProjectionMatrix;

/* FULLSCREEN RELATED GLOBAL VARIABLES */
var requestAnimationFrame =
    window.requestAnimationFrame ||
    window.mozRequestAnimationFrame ||
    window.webkitAnimationFrame ||
    window.oRequestAnimationFrame ||
    window.msRequestAnimationFrame;


function main() {

    /* CODE */
    canvas = document.getElementById("PRJ");

    if (!canvas)
        console.log("Obtaining Canvas failed.\n");
    else
        console.log("Obtaining Canvas succeeded.\n");

    // backup canvas dimention
    canvas_original_width = canvas.width;
    canvas_original_height = canvas.height;

    // initialize
    initialize();

    // resize
    resize();

    // Display
    display();

    window.addEventListener("keydown", keyDown, false);
    window.addEventListener("click", mouseDown, false);
    window.addEventListener("resize", resize, false);
}

function toggleFullscreen() {

    /* CODE */
    var fullscreen_element =
        document.fullscreenElement ||
        document.mozFullScreenElement ||
        document.webkitFullscreenElement ||
        document.msFullscreenElement ||
        null;

    if (fullscreen_element == null) {
        if (canvas.requestFullscreen)
            canvas.requestFullscreen();

        else if (canvas.mozRequestFullScreen)
            canvas.mozRequestFullScreen();

        else if (canvas.webkitRequestFullscreen)
            canvas.webkitRequestFullscreen();

        else if (canvas.msRequestFullscreen)
            canvas.msRequestFullscreen();

        bFullscreen = true;
    }

    else {
        if (document.exitFullscreen)
            document.exitFullscreen();

        else if (document.mozExitFullScreen)
            document.mozExitFullScreen();

        else if (document.webkitExitFullscreen)
            document.webkitExitFullscreen();

        else if (document.msExitFullscreen)
            document.msExitFullscreen();

        bFullscreen = false;
    }
}

function initialize() {

    /* CODE */
    // Obtain WebGL Context
    gl = canvas.getContext("webgl2");

    if (!gl)
        console.log("Obtaining WebGL2.0 failed.\n");
    else
        console.log("Obtaining WebGL2.0 succeeded.\n");

    // Set ViewPort Width AND Height of Context
    gl.viewportWidth = canvas.width;
    gl.viewportHeigh = canvas.height;

    // VERTEX SHADER
    var vertexShaderSourceCode =
        "#version 300 es" + // WebGL is subset of OpenGL ES 3.0
        "\n" +
        "in vec4 a_position;" +
        "uniform mat4 u_mvpMatrix;" +
        "void main(void)" +
        "{" +
        "gl_Position = u_mvpMatrix * a_position;" +
        "}";

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if (error.length > 0) {
            var errorString = "VERTEX SHADER COMPILATION LOG :" + error;
            alert(errorString);
            uninitialize();
        }
    }

    // FRAGMENT SHADER
    var fragmentShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "precision highp float;" +
        "uniform vec3 u_color;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = vec4(u_color, 1.0);" +
        "}";

    var fragmentShaderObject = gl.createShader(gl.FRAGMENT_SHADER);

    gl.shaderSource(fragmentShaderObject, fragmentShaderSourceCode);

    gl.compileShader(fragmentShaderObject);

    if (gl.getShaderParameter(fragmentShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(fragmentShaderObject);
        if (error.length > 0) {
            var errorString = "FRAGMENT SHADER COMPILATION LOG :" + error;
            alert(errorString);
            uninitialize();
        }
    }

    // SHADER PROGRAM
    shaderProgramObject = gl.createProgram();

    gl.attachShader(shaderProgramObject, vertexShaderObject);
    gl.attachShader(shaderProgramObject, fragmentShaderObject);

    // PRELINKING SHADER PROGRAM OBJECT ATTRIBUTE BINDING
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.PRJ_ATTRIBUTE_POSITION, "a_position");

    // SHADER PROGRAM LINKING
    gl.linkProgram(shaderProgramObject);

    if (gl.getProgramParameter(shaderProgramObject, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject);
        if (error.length > 0) {
            //var errorString = "SHADER PROGRAM LINK LOG :" + error;
            alert(errorString);
            uninitialize();
        }
    }

    // POST LINK - GET UNIFORM LOCATION
    mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_mvpMatrix");
    colorUniform = gl.getUniformLocation(shaderProgramObject, "u_color");

    var squareVertices = new Float32Array([
        1.0, 1.0, 0.0,
        -1.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0
    ]);

    var LineVertices = new Float32Array([
        0.0, 1.0, 0.0,
        0.0, -1.0, 0.0]);


    var triangleVertices = new Float32Array([
        0.0, 1.0, 0.0,
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0
    ]);

    var x = 0;
    var y = 0;
    var radius = 1.0;
    var circlePositionCoords = new Float32Array(1080);

    // Generate Circle Vertices
    for (var i = 0, angle = 0; i < 1080, angle < 360; i = i + 3, angle++) {
        var rAngle = angle * Math.PI / 180.0;

        x = radius * Math.cos(rAngle);
        y = radius * Math.sin(rAngle);

        circlePositionCoords[i] = x;
        circlePositionCoords[i + 1] = y;
        circlePositionCoords[i + 2] = 0.0;
        console.log("[ " + i + "] = " + x + " " + y);
    }

    // Join Last Vertices
    circlePositionCoords[1080 - 3] = circlePositionCoords[0];
    circlePositionCoords[1080 - 2] = circlePositionCoords[1];
    circlePositionCoords[1080 - 1] = circlePositionCoords[2];

    // VERTEX ARRAY OBJECT
    vao_Square = gl.createVertexArray();
    gl.bindVertexArray(vao_Square);

    vbo_Square = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Square);
    gl.bufferData(gl.ARRAY_BUFFER, squareVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // VERTEX ARRAY OBJECT
    vao_triangle = gl.createVertexArray();
    gl.bindVertexArray(vao_triangle);

    vbo_triangle = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_triangle);
    gl.bufferData(gl.ARRAY_BUFFER, triangleVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // CIRCLE VERTEX OBJECT
    vao_Circle = gl.createVertexArray();
    gl.bindVertexArray(vao_Circle);

    vbo_Circle = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Circle);
    gl.bufferData(gl.ARRAY_BUFFER, circlePositionCoords, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // CIRCLE VERTEX OBJECT
    vao_Line = gl.createVertexArray();
    gl.bindVertexArray(vao_Line);

    vbo_Line = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Line);
    gl.bufferData(gl.ARRAY_BUFFER, LineVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // Depth Related Changes
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // Clear Screen Using Blue Color
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    perspectiveProjectionMatrix = mat4.create();
}

function resize() {

    /* CODE */
    if (bFullscreen == true) {
        canvas.width = window.innerWidth;
        canvas.height = window.innerHeight;
    }
    else {
        canvas.width = canvas_original_width;
        canvas.height = canvas_original_height;
    }

    if (canvas.height == 0)
        canvas.height = 1;

    gl.viewport(0, 0, canvas.width, canvas.height);

    mat4.perspective(
        perspectiveProjectionMatrix,
        45.0,
        parseFloat(canvas.width) / parseFloat(canvas.height),
        0.1,
        100.0);

}

function display() {

    /* CODE */
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // USE SHADER PROGRAM OBJECT
    gl.useProgram(shaderProgramObject);

    drawGraph();

    drawTriangle();

    drawSquare();

    drawCircle();

    gl.useProgram(null);

    // Double Buffering Emulation
    requestAnimationFrame(display, canvas);
}

function update() {
    /* CODE */
}

function keyDown(event) {

    /* CODE */
    switch (event.keyCode) {

        case 81:
            uninitialize();
            window.close(); // NOT ALL BROWSER WILL FOLLOW THIS.
            break;

        case 70:
            toggleFullscreen();
            break;
    }
}

function mouseDown() {
    /* CODE */
}

function uninitialize() {
    /* CODE */
    if (vbo_Circle) {
        gl.deleteBuffer(vbo_Circle);
        vbo_Circle = null;
    }

    if (vao_Circle) {
        gl.deleteVertexArray(vao_Circle);
        vao_Circle = null;
    }

    if (vbo_Square) {
        gl.deleteBuffer(vbo_Square);
        vbo_Square = null;
    }

    if (vao_Square) {
        gl.deleteVertexArray(vao_Square);
        vao_Square = null;
    }

    if (shaderProgramObject) {

        gl.useProgram(shaderProgramObject);
        var shaderObjects = gl.getAttachedShaders(shaderProgramObject);
        for (let i = 0; i < shaderObjects.length; i++) {
            gl.detachShader(shaderProgramObject, shaderObjects[i]);
            gl.deleteShader(shaderObjects[i]);
            shaderObjects[i] = 0;
        }
        delete (shaderObjects);
        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject);
        shaderProgramObject = 0;
    }
}

function drawTriangle() {

    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -4.0]);
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
    gl.uniform3f(colorUniform, 1.0, 1.0, 0.0);

    // DRAW TRIANGLE
    gl.bindVertexArray(vao_triangle);
    gl.drawArrays(gl.LINE_LOOP, 0, 3);
    gl.bindVertexArray(null);
}

function drawCircle() {

    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -4.0]);
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
    gl.uniform3f(colorUniform, 1.0, 1.0, 0.0);

    // DRAW TRIANGLE
    gl.bindVertexArray(vao_Circle);
    gl.drawArrays(gl.LINE_STRIP, 0, 360);
    gl.bindVertexArray(null);
}

function drawSquare() {

    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -4.0]);
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);
    gl.uniform3f(colorUniform, 1.0, 1.0, 0.0);

    // DRAW SQUARE
    gl.bindVertexArray(vao_Square);
    gl.drawArrays(gl.LINE_LOOP, 0, 4);
    gl.bindVertexArray(null);
}

function drawGraph() {
    var modelViewMatrix = mat4.create();
    var scaleMatrix = mat4.create();
    var rotateMatrix = mat4.create();
    var translateMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();

    for (var i = -1.5; i < 1.5; i = i + 0.05) {

        scaleMatrix = mat4.create();
        modelViewMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();

        mat4.scale(scaleMatrix, scaleMatrix, [1.0, 1.5, 1.0]);
        mat4.translate(modelViewMatrix, scaleMatrix, [i, 0.0, -4.0]);

        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

        if (i > -0.02 && i < 0.02)
            gl.uniform3f(colorUniform, 0.0, 1.0, 0.0);
        else
            gl.uniform3f(colorUniform, 0.0, 0.0, 1.0);

        // DRAW SQUARE
        gl.bindVertexArray(vao_Line);
        gl.drawArrays(gl.LINES, 0, 2);
        gl.bindVertexArray(null);
    }

    // Horizontal
    for (var i = -1.5; i < 1.5; i = i + 0.05) {

        scaleMatrix = mat4.create();
        modelViewMatrix = mat4.create();
        modelViewProjectionMatrix = mat4.create();
        rotateMatrix = mat4.create();
        translateMatrix = mat4.create();

        var angle = 90.0;

        var radAng = angle * Math.PI / 180.0;

        mat4.scale(scaleMatrix, scaleMatrix, [1.0, 1.5, 1.0]);
        mat4.rotateZ(rotateMatrix, rotateMatrix, radAng);
        mat4.translate(translateMatrix, translateMatrix, [0, i, -4.0]);

        mat4.multiply(rotateMatrix, rotateMatrix, scaleMatrix);
        mat4.multiply(modelViewMatrix, translateMatrix, rotateMatrix);

        mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
        gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

        if (i > -0.02 && i < 0.02)
            gl.uniform3f(colorUniform, 1.0, 0.0, 0.0);
        else
            gl.uniform3f(colorUniform, 0.0, 0.0, 1.0);

        // DRAW SQUARE
        gl.bindVertexArray(vao_Line);
        gl.drawArrays(gl.LINES, 0, 2);
        gl.bindVertexArray(null);
    }
}
