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

var vao_Pyramid;
var vbo_Pyramid_Position;
var vbo_Pyramid_Color;

var vao_Cube;
var vbo_Cube_Position;
var vbo_Cube_Color;

var mvpMatrixUniform;
var perspectiveProjectionMatrix;

var anglePyramid = 0.0;
var angleCube = 0.0;

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
        "in vec3 a_color;" +
        "uniform mat4 u_mvpMatrix;" +
        "out vec3 a_color_out;" +
        "void main(void)" +
        "{" +
        "a_color_out = a_color;" +
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
        "in vec3 a_color_out;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = vec4(a_color_out,1.0);" +
        "}"

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
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.PRJ_ATTRIBUTE_COLOR, "a_color");


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

    // 
    var PyramidVertices = new Float32Array([
        // front
        0.0, 1.0, 0.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        // right
        0.0, 1.0, 0.0,
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,

        // back
        0.0, 1.0, 0.0,
        1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,

        // left
        0.0, 1.0, 0.0,
        -1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0
    ]);

    var PyramidColor = new Float32Array([
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,

        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 0.0,

        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0,

        1.0, 0.0, 0.0,
        0.0, 0.0, 1.0,
        0.0, 1.0, 0.0
    ]);

    var CubeVertices = new Float32Array([
        // top
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, 1.0, 1.0,
        1.0, 1.0, 1.0,

        // bottom
        1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        // front
        1.0, 1.0, 1.0,
        -1.0, 1.0, 1.0,
        -1.0, -1.0, 1.0,
        1.0, -1.0, 1.0,

        // back
        1.0, 1.0, -1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,

        // right
        1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,

        // left
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, -1.0,
        -1.0, -1.0, 1.0
    ]);

    var CubeColor = new Float32Array([
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,

        1.0, 0.5, 0.0,
        1.0, 0.5, 0.0,
        1.0, 0.5, 0.0,
        1.0, 0.5, 0.0,

        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,

        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,
        1.0, 1.0, 0.0,

        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,
        0.0, 0.0, 1.0,

        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0,
        1.0, 0.0, 1.0
    ]);

    // VERTEX ARRAY OBJECT
    vao_Pyramid = gl.createVertexArray();
    gl.bindVertexArray(vao_Pyramid);
    vbo_Pyramid_Position = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Pyramid_Position);
    gl.bufferData(gl.ARRAY_BUFFER, PyramidVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    vbo_Cube_Color = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Cube_Color);
    gl.bufferData(gl.ARRAY_BUFFER, PyramidColor, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_COLOR);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    gl.bindVertexArray(null);

    vao_Cube = gl.createVertexArray();
    gl.bindVertexArray(vao_Cube);
    vbo_Cube_Position = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Cube_Position);
    gl.bufferData(gl.ARRAY_BUFFER, CubeVertices, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_POSITION);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    vbo_Color = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Color);
    gl.bufferData(gl.ARRAY_BUFFER, CubeColor, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_COLOR);
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


    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var rotationMatrix = mat4.create();
    var scaleMatrix = mat4.create();

    // DRAW Pyramid
    mat4.translate(modelViewMatrix, modelViewMatrix, [-1.5, 0.0, -6.0]);
    mat4.rotateY(rotationMatrix, rotationMatrix, degToRad(anglePyramid))

    mat4.multiply(modelViewMatrix, modelViewMatrix, rotationMatrix);
    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);

    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    gl.bindVertexArray(vao_Pyramid);
    gl.drawArrays(gl.TRIANGLES, 0, 12);
    gl.bindVertexArray(null);

    // DRAW Cube
    mat4.identity(modelViewMatrix);
    mat4.identity(modelViewProjectionMatrix);
    mat4.identity(rotationMatrix);

    mat4.translate(modelViewMatrix, modelViewMatrix, [1.5, 0.0, -6.0]);
    mat4.rotateX(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateY(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateZ(rotationMatrix, rotationMatrix, degToRad(angleCube));

    mat4.scale(scaleMatrix, scaleMatrix, [0.8, 0.8, 0.8]);


    // mat4.rotateX(rotationMatrix, rotationMatrix, angle)

    mat4.multiply(rotationMatrix, rotationMatrix, scaleMatrix);
    mat4.multiply(modelViewMatrix, modelViewMatrix, rotationMatrix);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    gl.bindVertexArray(vao_Cube);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);
    gl.bindVertexArray(null);

    gl.useProgram(null);

    // Double Buffering Emulation
    requestAnimationFrame(display, canvas);

    // Update
    update();
}

function degToRad(degree) {

    /* CODE */
    return (degree * Math.PI / 180.0);
}

function update() {
    /* CODE */
    anglePyramid = anglePyramid + 3.05;
    if (anglePyramid >= 360.0)
        anglePyramid = anglePyramid - 360.0;

    angleCube = angleCube + 3.05;
    if (angleCube >= 360.0)
        angleCube = angleCube - 360.0;
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
    if (vbo_Cube_Color) {
        gl.deleteBuffer(vbo_Cube_Color);
        vbo_Cube_Color = null;
    }

    if (vbo_Cube_Position) {
        gl.deleteBuffer(vbo_Cube_Position);
        vbo_Cube_Position = null;
    }

    if (vao_Cube) {
        gl.deleteVertexArray(vao_Cube);
        vao_Cube = null;
    }

    if (vbo_Pyramid_Color) {
        gl.deleteBuffer(vbo_Pyramid_Color);
        vbo_Pyramid_Color = null;
    }

    if (vbo_Pyramid_Position) {
        gl.deleteBuffer(vbo_Pyramid_Position);
        vbo_Pyramid_Position = null;
    }

    if (vao_Pyramid) {
        gl.deleteVertexArray(vao_Pyramid);
        vao_Pyramid = null;
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
