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

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;
var colorUniform;

var perspectiveProjectionMatrix;
var sphere = null;

var MODEL_VIEW_MATRIX_STACK = 32;
var matrixStack = new Array(32);
var matrixStackTop = -1;

var shoulder = 0;
var elbow = 0;
var palm = 0;

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
        "#version 300 es" +
        "\n" +
        "in vec4 a_position;" +
        "uniform mat4 u_modelMatrix;" +
        "uniform mat4 u_viewMatrix;" +
        "uniform mat4 u_projectionMatrix;" +
        "void main(void)" +
        "{" +
        "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
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
        "FragColor = vec4(1.0,1.0,1.0, 1.0);" +
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
    modelMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_modelMatrix");
    viewMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_viewMatrix");
    projectionMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_projectionMatrix");
    colorUniform = gl.getUniformLocation(shaderProgramObject, "u_color");

    sphere = new Mesh();
    makeSphere(sphere, 2.0, 30, 30);

    // Depth Related Changes
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    perspectiveProjectionMatrix = mat4.create();

    initializeMatrixStack();
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

    var modelMatrix = mat4.create();
    var viewMatrix = mat4.create();
    var rotationMatrix = mat4.create();
    var scaleMatrix = mat4.create();
    var translationMatrix = mat4.create();

    // SUN  
    mat4.lookAt(viewMatrix, [0, 0, 6], [0, 0, 0], [0, 1, 0]);


    mat4.translate(translationMatrix, translationMatrix, [0.0, 0.0, -8.0]);
    mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
    translationMatrix = mat4.create();

    pushMatrix(modelMatrix);
    {
        mat4.rotateZ(rotationMatrix, rotationMatrix, shoulder);
        mat4.translate(translationMatrix, translationMatrix, [2.0, 0.0, 0.0]);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix);

        pushMatrix(modelMatrix);
        {
            mat4.scale(scaleMatrix, scaleMatrix, [2.0, 0.5, 1.0]);
            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
            gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
            gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);
            // gl.uni

            sphere.draw();

            modelMatrix = mat4.create();
        }
        modelMatrix = mat4.clone(popMatrix());

        // Make Identity
        rotationMatrix = mat4.create();
        scaleMatrix = mat4.create();
        translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [1.0, 0.0, 0.0]);
        mat4.rotateZ(rotationMatrix, rotationMatrix, elbow);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix);

        translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [1.0, 0.0, 0.0]);
        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        pushMatrix(modelMatrix);
        {
            mat4.scale(scaleMatrix, scaleMatrix, [2.0, 0.5, 1.0]);
            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
            gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
            gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

            sphere.draw();

            modelMatrix = mat4.create();
        }
        modelMatrix = mat4.clone(popMatrix());

        // Make Identity
        rotationMatrix = mat4.create();
        scaleMatrix = mat4.create();
        translationMatrix = mat4.create();

        mat4.translate(translationMatrix, translationMatrix, [1.0, 0.0, 0.0]);
        mat4.rotateZ(rotationMatrix, rotationMatrix, palm);

        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);
        mat4.multiply(modelMatrix, modelMatrix, rotationMatrix);

        translationMatrix = mat4.create();
        mat4.translate(translationMatrix, translationMatrix, [0.4, 0.0, 0.0]);
        mat4.multiply(modelMatrix, modelMatrix, translationMatrix);

        pushMatrix(modelMatrix);
        {
            mat4.scale(scaleMatrix, scaleMatrix, [1.0, 0.35, 1.0]);
            mat4.multiply(modelMatrix, modelMatrix, scaleMatrix);

            gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
            gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
            gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

            sphere.draw();

            modelMatrix = mat4.create();
        }
        modelMatrix = mat4.clone(popMatrix());
    }
    modelMatrix = mat4.clone(popMatrix());

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

        case 68:
            day = (day + 0.07) % 360;
            moon = (moon + 0.15) % 360;
            break;

        case 89:
            day = (day + 0.1) % 360;
            year = (year + 0.05) % 360;
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

    if (sphere) {
        sphere.deallocate();
        sphere = null;
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


function initializeMatrixStack() {
    // code

    matrixStackTop = 0;
    for (var i = matrixStackTop; i < MODEL_VIEW_MATRIX_STACK; i++)
        matrixStack[i] = new Array(16);

}

function pushMatrix(matrix) {

    //	Code
    if (matrixStackTop > (MODEL_VIEW_MATRIX_STACK - 1)) {
        console.log("ERROR - EXCEEDED MATRIX STACK LIMIT:\n");
        uninitialize();
    }

    matrixStack[matrixStackTop] = mat4.clone(matrix);
    matrixStackTop++;
}

function popMatrix() {
    // variable declartions
    var matrix = mat4.create();

    // code

    if (matrixStackTop < 0) {
        console.log("ERROR : MATRIX STACK EMPTY!\n");
        uninitialize();
    }

    matrixStack[matrixStackTop] = mat4.create();
    matrixStackTop--;

    matrix = mat4.clone(matrixStack[matrixStackTop]);

    return (matrix);
}
