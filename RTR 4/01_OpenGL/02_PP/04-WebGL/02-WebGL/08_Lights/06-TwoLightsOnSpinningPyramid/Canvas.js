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
var vbo_Pyramid_Normal;

var modelMatrixUniform;
var viewMatrixUniform;
var projectionMatrixUniform;
var perspectiveProjectionMatrix;

var laUniform = [];			 // light Ambiant
var ldUniform = [];			 // light Diffuse
var lsUniform = [];			 // light Spec
var lighPositionUniform = [];  // light Position

var kaUniform;                  // material amb
var kdUniform;                  // mat diff
var ksUniform;                  // mat specular
var materialShininessUniform;   // mat shininess

var lightingEnabledUniform;

var lightAmbiant_One = [0.0, 0.0, 0.0];
var lightAmbiant_Two = [0.0, 0.0, 0.0];

var lightDiffuse_One = [1.0, 0.0, 0.0];
var lightDiffuse_Two = [0.0, 0.0, 1.0];

var lightSpecular_One = [1.0, 0.0, 0.0];
var lightSpecular_Two = [0.0, 0.0, 1.0];

var lightPosition_One = [-2.0, 0.0, 0.0, 1.0]
var lightPosition_Two = [0.0, 0.0, 1.0, 1.0];

var materialAmbiant = [0.0, 0.0, 0.0];
var meterialDiffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShineeness = 50.0;

var bLight = false;

var anglePyramid = 0.0;

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
        "in vec3 a_normal;" +
        "uniform mat4 u_modelMatrix;" +
        "uniform mat4 u_viewMatrix;" +
        "uniform mat4 u_projectionMatrix;" +
        "uniform vec3 u_la[2];" +
        "uniform vec3 u_ld[2];" +
        "uniform vec3 u_ls[2];" +
        "uniform vec4 u_lightPosition[2];" +
        "uniform vec3 u_ka;" +
        "uniform vec3 u_ks;" +
        "uniform vec3 u_kd;" +
        "uniform float u_materialShininnes;" +
        "uniform int u_lightingEnabled;" +
        "out vec3 phong_ads_light;" +
        "void main(void)" +
        "{" +
        "phong_ads_light = vec3(0.0,0.0,0.0);" +
        "if(u_lightingEnabled == 1)" +
        "{" +
        "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
        "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
        "vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
        "vec3 viewerVector = normalize(-eyeCoordinates.xyz);" +
        "vec3 ambiant[2];" +
        "vec3 lightDirection[2];" +
        "vec3 diffuse[2];" +
        "vec3 reflectionVector[2];" +
        "vec3 specular[2];" +
        "for(int i = 0 ; i < 2 ; i++)" +
        "{" +
        "ambiant[i] = u_la[i] * u_ka;" +
        "lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" + // Swizaling+
        "diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i] ,transformedNormals),0.0);" +
        "reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);" +
        "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininnes);"
        +
        "phong_ads_light = phong_ads_light + ambiant[i] + diffuse[i] +  specular[i];" +
        "}" +
        "}" +
        "else" +
        "{" +
        "phong_ads_light = vec3(1.0,1.0,1.0);" +
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
        "in vec3 phong_ads_light;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = vec4(phong_ads_light, 1.0);" +
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

    laUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_la[0]");
    ldUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_ld[0]");
    lsUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_ls[0]");
    lighPositionUniform[0] = gl.getUniformLocation(shaderProgramObject, "u_lightPosition[0]");


    laUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_la[1]");
    ldUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_ld[1]");
    lsUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_ls[1]");
    lighPositionUniform[1] = gl.getUniformLocation(shaderProgramObject, "u_lightPosition[1]");

    kaUniform = gl.getUniformLocation(shaderProgramObject, "u_ka");
    kdUniform = gl.getUniformLocation(shaderProgramObject, "u_kd");
    ksUniform = gl.getUniformLocation(shaderProgramObject, "u_ks");
    materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "u_materialShininnes");


    lightingEnabledUniform = gl.getUniformLocation(shaderProgramObject, "u_lightingEnabled");


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

    var PyramidNormal = new Float32Array([
        0.0, 0.447214, 0.894427, // front-top
        0.0, 0.447214, 0.894427, // front-left
        0.0, 0.447214, 0.894427, // front-right

        0.894427, 0.447214, 0.0, // right-top
        0.894427, 0.447214, 0.0, // right-left
        0.894427, 0.447214, 0.0, // right-right

        0.0, 0.447214, -0.894427, // back-top
        0.0, 0.447214, -0.894427, // back-left
        0.0, 0.447214, -0.894427, // back-right

        -0.894427, 0.447214, 0.0, // left-top
        -0.894427, 0.447214, 0.0, // left-left
        -0.894427, 0.447214, 0.0	 // left-right
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

    vbo_Pyramid_Normal = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Pyramid_Normal);
    gl.bufferData(gl.ARRAY_BUFFER, PyramidNormal, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_NORMAL);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);
    gl.bindVertexArray(null);

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

    var viewMatrix = mat4.create();
    var translateMatrix = mat4.create();
    var rotationMatrix = mat4.create();
    var modelMatrix = mat4.create();

    // DRAW Sphere
    mat4.translate(translateMatrix, translateMatrix, [0.0, 0.0, -4.0]);
    mat4.rotateY(rotationMatrix, rotationMatrix, degToRad(anglePyramid));

    mat4.multiply(modelMatrix, translateMatrix, rotationMatrix);

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform[0], lightAmbiant_One);
        gl.uniform3fv(ldUniform[0], lightDiffuse_One);
        gl.uniform3fv(lsUniform[0], lightSpecular_One);
        gl.uniform4fv(lighPositionUniform[0], lightPosition_One);

        gl.uniform3fv(laUniform[1], lightAmbiant_Two);
        gl.uniform3fv(ldUniform[1], lightDiffuse_Two);
        gl.uniform3fv(lsUniform[1], lightSpecular_Two);
        gl.uniform4fv(lighPositionUniform[1], lightPosition_Two);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDiffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    gl.bindVertexArray(vao_Pyramid);
    gl.drawArrays(gl.TRIANGLES, 0, 12);
    gl.bindVertexArray(null);

    gl.useProgram(null);

    // Double Buffering Emulation
    requestAnimationFrame(display, canvas);

    // Update
    update();
}

function update() {
    /* CODE */
    anglePyramid = anglePyramid + 3.00;
    if (anglePyramid >= 360.0)
        anglePyramid = anglePyramid - 360.0;
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

    if (vbo_Pyramid_Normal) {
        gl.deleteBuffer(vbo_Pyramid_Normal);
        vbo_Pyramid_Normal = null;
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
