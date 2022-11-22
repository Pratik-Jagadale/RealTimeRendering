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
var perspectiveProjectionMatrix;

var ldUniform; // light direction
var kdUniform; // material diffuse
var lightPositionUniform;
var lightingEnabledUniform;

var lightDiffuse = [1.0, 1.0, 1.0];
var materialDiffuse = [1.0, 1.0, 1.0];
var lightPosition = [0.0, 0.0, 2.0];

var bLight = false;

var sphere = null;

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
        "in vec3 a_normal; " +
        "uniform mat4 u_projectionMatrix;" +
        "uniform mat4 u_viewMatrix;" +
        "uniform mat4 u_modelMatrix;" +
        "uniform vec3 u_ld;" +
        "uniform vec3 u_kd;" +
        "uniform vec4 u_lightPosition;" +
        "uniform highp int u_lightEnabled;" +
        "out vec3 diffuse_light_color;" +
        "void main(void)" +
        "{" +
        "if(u_lightEnabled == 1)" +
        "{" +
        "vec4 eyeCoordinate = u_viewMatrix * u_modelMatrix * a_position;" +
        "mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));" +
        "vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
        "vec3 lightDirection = vec3(normalize(u_lightPosition - eyeCoordinate));" +
        "diffuse_light_color = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);" +
        "}" +
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
        "in vec3 diffuse_light_color;" +
        "uniform highp int u_lightEnabled;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "if(u_lightEnabled == 1)" +
        "{" +
        "FragColor = vec4(diffuse_light_color, 1.0);" +
        "}" +
        "else" +
        "{" +
        "FragColor = vec4(1.0,1.0,1.0,1.0f);" +
        "}" +
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
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");

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

    ldUniform = gl.getUniformLocation(shaderProgramObject, "u_ld");
    kdUniform = gl.getUniformLocation(shaderProgramObject, "u_kd");
    lightPositionUniform = gl.getUniformLocation(shaderProgramObject, "u_lightPosition");
    lightingEnabledUniform = gl.getUniformLocation(shaderProgramObject, "u_lightEnabled");

    sphere = new Mesh();
    makeSphere(sphere, 2.0, 30, 30);

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

    var viewMatrix = mat4.create();
    var translateMatrix = mat4.create();

    // DRAW Sphere
    mat4.translate(translateMatrix, translateMatrix, [0.0, 0.0, -6.0]);

    gl.uniformMatrix4fv(modelMatrixUniform, false, translateMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(kdUniform, materialDiffuse);
        gl.uniform3fv(lightPositionUniform, lightPosition);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW sphere
    sphere.draw();

    gl.useProgram(null);

    // Double Buffering Emulation
    requestAnimationFrame(display, canvas);

    // Update
    update();
}

function update() {
    /* CODE */
}

function degToRad(degree) {

    /* CODE */
    return (degree * Math.PI / 180.0);
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

        case 76:
            if (bLight == false) {
                bLight = true;
            }
            else {
                bLight = false;
            }
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
