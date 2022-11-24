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

var shaderProgramObject_PV;

var modelMatrixUniform_PV;
var viewMatrixUniform_PV;
var projectionMatrixUniform_PV;
var perspectiveProjectionMatrix_PV;


var laUniform_PV;			// light Ambiant
var ldUniform_PV;			// light Diffuse
var lsUniform_PV;			// light Spec
var lighPositionUniform_PV; // light Position

var kaUniform_PV; // material amb
var kdUniform_PV; // mat diff
var ksUniform_PV; // mat specular
var materialShininessUniform_PV;

var lightingEnabledUniform_PV;

var lightAmbiant = [0.1, 0.1, 0.1];
var lightDiffuse = [1.0, 1.0, 1.0];
var lightSpecular = [1.0, 1.0, 1.0];
var lightPosition = [100.0, 100.0, 100.0, 1.0];

var materialAmbiant = [0.0, 0.0, 0.0];
var meterialDeffuse = [0.5, 0.2, 0.7];
var materialSpecular = [0.7, 0.7, 0.7];
var materialShineeness = 120.0;


var materialDiffuse = [1.0, 1.0, 1.0];

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

    {
        // VERTEX SHADER
        var vertexShaderSourceCode =
            "#version 300 es" +
            "\n" +
            "in vec4 a_position;" +
            "in vec3 a_normal; " +
            "uniform mat4 u_projectionMatrix;" +
            "uniform mat4 u_viewMatrix;" +
            "uniform mat4 u_modelMatrix;" +
            "uniform vec3 u_la;" +
            "uniform vec3 u_ld;" +
            "uniform vec3 u_ls;" +
            "uniform vec4 u_lightPosition;" +
            "uniform vec3 u_ka;" +
            "uniform vec3 u_ks;" +
            "uniform vec3 u_kd;" +
            "uniform float u_materialShininnes;" +
            "uniform mediump int u_lightingEnabled;" +
            "out vec3 phong_ads_out;" +
            "void main(void)" +
            "{" +
            "if(u_lightingEnabled == 1)" +
            "{" +
            "vec3 ambiant = u_la * u_ka;" +
            "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
            "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
            "vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
            "vec3 lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" + // Swizaling+
            "vec3 diffuse = u_ld * u_kd * max(dot(lightDirection ,transformedNormals),0.0);" +
            "vec3 reflectionVector = reflect(-lightDirection,transformedNormals);" +
            "vec3 viewerVector = normalize(-eyeCoordinates.xyz);" +
            "vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , viewerVector), 0.0), u_materialShininnes);"
            +
            "phong_ads_out = ambiant + diffuse +  specular;" +
            "}" +
            "else" +
            "{" +
            "phong_ads_out = vec3(1.0,1.0,1.0);" +
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
            "in vec3 phong_ads_out;" +
            "out vec4 FragColor;" +
            "void main(void)" +
            "{" +
            "FragColor = vec4(phong_ads_out, 1.0);" +
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
        shaderProgramObject_PV = gl.createProgram();

        gl.attachShader(shaderProgramObject_PV, vertexShaderObject);
        gl.attachShader(shaderProgramObject_PV, fragmentShaderObject);

        // PRELINKING SHADER PROGRAM OBJECT ATTRIBUTE BINDING
        gl.bindAttribLocation(shaderProgramObject_PV, webGLMacros.PRJ_ATTRIBUTE_POSITION, "a_position");
        gl.bindAttribLocation(shaderProgramObject_PV, webGLMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");

        // SHADER PROGRAM LINKING
        gl.linkProgram(shaderProgramObject_PV);

        if (gl.getProgramParameter(shaderProgramObject_PV, gl.LINK_STATUS) == false) {
            var error = gl.getProgramInfoLog(shaderProgramObject_PV);
            if (error.length > 0) {
                //var errorString = "SHADER PROGRAM LINK LOG :" + error;
                alert(errorString);
                uninitialize();
            }
        }

        // POST LINK - GET UNIFORM LOCATION
        modelMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_modelMatrix");
        viewMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_viewMatrix");
        projectionMatrixUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_projectionMatrix");

        laUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_la");
        ldUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_ld");
        lsUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_ls");
        lighPositionUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_lightPosition");

        kaUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_ka");
        kdUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_kd");
        ksUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_ks");
        materialShininessUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_materialShininnes");


        lightingEnabledUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_lightingEnabled");
    }
    sphere = new Mesh();
    makeSphere(sphere, 2.0, 30, 30);

    // Depth Related Changes
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // Clear Screen Using Blue Color
    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    perspectiveProjectionMatrix_PV = mat4.create();
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
        perspectiveProjectionMatrix_PV,
        45.0,
        parseFloat(canvas.width) / parseFloat(canvas.height),
        0.1,
        100.0);

}

function display() {

    /* CODE */
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    if (isPerFrag == true) {

        // USE SHADER PROGRAM OBJECT
        gl.useProgram(shaderProgramObject_PF);

        var viewMatrix = mat4.create();
        var translateMatrix = mat4.create();

        // DRAW Sphere
        mat4.translate(translateMatrix, translateMatrix, [0.0, 0.0, -6.0]);

        gl.uniformMatrix4fv(modelMatrixUniform_PF, false, translateMatrix);
        gl.uniformMatrix4fv(viewMatrixUniform_PF, false, viewMatrix);
        gl.uniformMatrix4fv(projectionMatrixUniform_PF, false, perspectiveProjectionMatrix);

        if (bLight == true) {
            gl.uniform1i(lightingEnabledUniform_PF, 1);

            gl.uniform3fv(laUniform_PF, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
            gl.uniform3fv(ldUniform_PF, lightDiffuse);
            gl.uniform3fv(lsUniform_PF, lightSpecular);
            gl.uniform4fv(lighPositionUniform_PF, lightPosition);

            gl.uniform3fv(kaUniform_PF, materialAmbiant);
            gl.uniform3fv(kdUniform_PF, meterialDeffuse);
            gl.uniform3fv(ksUniform_PF, materialSpecular);
            gl.uniform1f(materialShininessUniform_PF, materialShineeness);
        }
        else {
            gl.uniform1i(lightingEnabledUniform_PF, 0);
        }
    }
    else {

        // USE SHADER PROGRAM OBJECT
        gl.useProgram(shaderProgramObject_PV);

        var viewMatrix = mat4.create();
        var translateMatrix = mat4.create();

        // DRAW Sphere
        mat4.translate(translateMatrix, translateMatrix, [0.0, 0.0, -6.0]);

        gl.uniformMatrix4fv(modelMatrixUniform_PV, false, translateMatrix);
        gl.uniformMatrix4fv(viewMatrixUniform_PV, false, viewMatrix);
        gl.uniformMatrix4fv(projectionMatrixUniform_PV, false, perspectiveProjectionMatrix_PV);

        if (bLight == true) {
            gl.uniform1i(lightingEnabledUniform_PV, 1);

            gl.uniform3fv(laUniform_PV, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
            gl.uniform3fv(ldUniform_PV, lightDiffuse);
            gl.uniform3fv(lsUniform_PV, lightSpecular);
            gl.uniform4fv(lighPositionUniform_PV, lightPosition);

            gl.uniform3fv(kaUniform_PV, materialAmbiant);
            gl.uniform3fv(kdUniform_PV, meterialDeffuse);
            gl.uniform3fv(ksUniform_PV, materialSpecular);
            gl.uniform1f(materialShininessUniform_PV, materialShineeness);
        }
        else {
            gl.uniform1i(lightingEnabledUniform_PV, 0);
        }
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

        case 80:
            if (isPerFrag == false) {
                isPerFrag = true;
            }
            else {
                isPerFrag = false;
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

    if (shaderProgramObject_PV) {

        gl.useProgram(shaderProgramObject_PV);
        var shaderObjects = gl.getAttachedShaders(shaderProgramObject_PV);
        for (let i = 0; i < shaderObjects.length; i++) {
            gl.detachShader(shaderProgramObject_PV, shaderObjects[i]);
            gl.deleteShader(shaderObjects[i]);
            shaderObjects[i] = 0;
        }
        delete (shaderObjects);
        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject_PV);
        shaderProgramObject_PV = 0;
    }
}
