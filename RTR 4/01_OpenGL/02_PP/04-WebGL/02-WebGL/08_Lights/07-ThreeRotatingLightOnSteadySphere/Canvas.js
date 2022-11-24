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

// PER VERTEX RELATED GLOBAL VARIBALES

var shaderProgramObject_PV;

var modelMatrixUniform_PV;
var viewMatrixUniform_PV;
var projectionMatrixUniform_PV;

var laUniform_PV = [];			// light Ambiant
var ldUniform_PV = [];			// light Diffuse
var lsUniform_PV = [];			// light Spec
var lighPositionUniform_PV = []; // light Position

var kaUniform_PV; // material amb
var kdUniform_PV; // mat diff
var ksUniform_PV; // mat specular
var materialShininessUniform_PV;

var lightingEnabledUniform_PV;

// PER FRAGMENT RELATED GLOBAL BARIBALES
var shaderProgramObject_PF;

var modelMatrixUniform_PF;
var viewMatrixUniform_PF;
var projectionMatrixUniform_PF;
var perspectiveProjectionMatrix;

var laUniform_PF = [];			    // light Ambiant
var ldUniform_PF = [];			    // light Diffuse
var lsUniform_PF = [];			    // light Spec
var lighPositionUniform_PF = [];    // light Position

var kaUniform_PF; // material amb
var kdUniform_PF; // mat diff
var ksUniform_PF; // mat specular
var materialShininessUniform_PF;

var lightingEnabledUniform_PF;

var lightAmbiant_One = [0.0, 0.0, 0.0];
var lightDiffuse_One = [1.0, 0.0, 0.0];
var lightSpecular_One = [1.0, 0.0, 0.0];
var lightPosition_One = [-2.0, 0.0, 0.0, 1.0]

var lightAmbiant_Two = [0.0, 0.0, 0.0];
var lightDiffuse_Two = [0.0, 0.0, 1.0];
var lightSpecular_Two = [0.0, 0.0, 1.0];
var lightPosition_Two = [0.0, 0.0, 1.0, 1.0];

var lightAmbiant_Three = [0.0, 0.0, 0.0];
var lightDiffuse_Three = [0.0, 1.0, 0.0];
var lightSpecular_Three = [0.0, 1.0, 0.0];
var lightPosition_Three = [-2.0, 0.0, 0.0, 1.0]

var materialAmbiant = [0.0, 0.0, 0.0];
var meterialDeffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShineeness = 120.0;

var bLight = false;
var isPerFrag = false;
var sphere = null;
var lightAngle = 0.0;

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

    // Per Vertex
    {
        // VERTEX SHADER
        var vertexShaderSourceCode =
            "#version 300 es" +
            "\n" +
            "in vec4 a_position;" +
            "in vec3 a_normal;" +
            "uniform mat4 u_modelMatrix;" +
            "uniform mat4 u_viewMatrix;" +
            "uniform mat4 u_projectionMatrix;" +
            "uniform vec3 u_la[3];" +
            "uniform vec3 u_ld[3];" +
            "uniform vec3 u_ls[3];" +
            "uniform vec4 u_lightPosition[3];" +
            "uniform vec3 u_ka;" +
            "uniform vec3 u_ks;" +
            "uniform vec3 u_kd;" +
            "uniform float u_materialShininnes;" +
            "uniform highp int u_lightingEnabled;" +
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
            "vec3 ambiant[3];" +
            "vec3 lightDirection[3];" +
            "vec3 diffuse[3];" +
            "vec3 reflectionVector[3];" +
            "vec3 specular[3];" +
            "for(int i = 0 ; i < 3 ; i++)" +
            "{" +
            "ambiant[i] = u_la[i] * u_ka;" +
            "lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" + // Swizaling
            "diffuse[i] = u_ld[i] * u_kd * max(dot(lightDirection[i] ,transformedNormals),0.0);" +
            "reflectionVector[i] = reflect(-lightDirection[i],transformedNormals);" +
            "specular[i] = u_ls[i] * u_ks * pow(max(dot(reflectionVector[i], viewerVector), 0.0), u_materialShininnes);" +
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

        laUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "u_la[0]");
        ldUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "u_ld[0]");
        lsUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "u_ls[0]");
        lighPositionUniform_PV[0] = gl.getUniformLocation(shaderProgramObject_PV, "u_lightPosition[0]");

        laUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "u_la[1]");
        ldUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "u_ld[1]");
        lsUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "u_ls[1]");
        lighPositionUniform_PV[1] = gl.getUniformLocation(shaderProgramObject_PV, "u_lightPosition[1]");

        laUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "u_la[2]");
        ldUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "u_ld[2]");
        lsUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "u_ls[2]");
        lighPositionUniform_PV[2] = gl.getUniformLocation(shaderProgramObject_PV, "u_lightPosition[2]");

        kaUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_ka");
        kdUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_kd");
        ksUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_ks");
        materialShininessUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_materialShininnes");


        lightingEnabledUniform_PV = gl.getUniformLocation(shaderProgramObject_PV, "u_lightingEnabled");
    }

    {
        // VERTEX SHADER
        var vertexShaderSourceCode =
            "#version 300 es" +
            "\n" +
            "in vec4 a_position;" +
            "in vec3 a_normal;" +
            "uniform vec4 u_lightPosition[3];" +
            "uniform mat4 u_modelMatrix;" +
            "uniform mat4 u_viewMatrix;" +
            "uniform mat4 u_projectionMatrix;" +
            "uniform highp int u_lightingEnabled;" +
            "out vec3 transformedNormals;" +
            "out vec3 lightDirection[3];" +
            "out vec3 viewerVector;" +
            "void main(void)" +
            "{" +
            "if(u_lightingEnabled == 1)" +
            "{" +
            "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
            "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
            "transformedNormals = normalize(normalMatrix * a_normal);" +
            "viewerVector = normalize(-eyeCoordinates.xyz);" +
            "for(int i = 0 ; i < 3 ; i++)" +
            "{" +
            "lightDirection[i] = normalize(vec3(u_lightPosition[i]) - eyeCoordinates.xyz);" +
            "}" +
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
            "in vec3 transformedNormals;" +
            "in vec3 lightDirection[3];" +
            "in vec3 viewerVector;" +
            "uniform highp vec3 u_la[3];" +
            "uniform vec3 u_ld[3];" +
            "uniform vec3 u_ls[3];" +
            "uniform vec4 u_lightPosition[3];" +
            "uniform vec3 u_ka;" +
            "uniform vec3 u_ks;" +
            "uniform vec3 u_kd;" +
            "uniform float u_materialShininnes;" +
            "uniform highp int u_lightingEnabled;" +
            "out vec4 FragColor;" +
            "vec3 phong_ads_light;" +
            "void main(void)" +
            "{" +
            "phong_ads_light = vec3(0.0,0.0,0.0);" +
            "if(u_lightingEnabled == 1)" +
            "{" +
            "vec3 ambiant[3];" +
            "vec3 diffuse[3];" +
            "vec3 reflectionVector[3];" +
            "vec3 specular[3];" +
            "for(int i = 0 ; i < 3 ; i++)" +
            "{" +
            "ambiant[i] = u_la[i] * u_ka;" +
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
        shaderProgramObject_PF = gl.createProgram();

        gl.attachShader(shaderProgramObject_PF, vertexShaderObject);
        gl.attachShader(shaderProgramObject_PF, fragmentShaderObject);

        // PRELINKING SHADER PROGRAM OBJECT ATTRIBUTE BINDING
        gl.bindAttribLocation(shaderProgramObject_PF, webGLMacros.PRJ_ATTRIBUTE_POSITION, "a_position");
        gl.bindAttribLocation(shaderProgramObject_PF, webGLMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");

        // SHADER PROGRAM LINKING
        gl.linkProgram(shaderProgramObject_PF);

        if (gl.getProgramParameter(shaderProgramObject_PF, gl.LINK_STATUS) == false) {
            var error = gl.getProgramInfoLog(shaderProgramObject_PF);
            if (error.length > 0) {
                //var errorString = "SHADER PROGRAM LINK LOG :" + error;
                alert(errorString);
                uninitialize();
            }
        }

        // POST LINK - GET UNIFORM LOCATION
        modelMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_modelMatrix");
        viewMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_viewMatrix");
        projectionMatrixUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_projectionMatrix");

        laUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "u_la[0]");
        ldUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "u_ld[0]");
        lsUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "u_ls[0]");
        lighPositionUniform_PF[0] = gl.getUniformLocation(shaderProgramObject_PF, "u_lightPosition[0]");

        laUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "u_la[1]");
        ldUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "u_ld[1]");
        lsUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "u_ls[1]");
        lighPositionUniform_PF[1] = gl.getUniformLocation(shaderProgramObject_PF, "u_lightPosition[1]");

        laUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "u_la[2]");
        ldUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "u_ld[2]");
        lsUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "u_ls[2]");
        lighPositionUniform_PF[2] = gl.getUniformLocation(shaderProgramObject_PF, "u_lightPosition[2]");

        kaUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_ka");
        kdUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_kd");
        ksUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_ks");
        materialShininessUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_materialShininnes");


        lightingEnabledUniform_PF = gl.getUniformLocation(shaderProgramObject_PF, "u_lightingEnabled");
    }
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

            var angle = degToRad(lightAngle);
            var x = 5.0 * Math.cos(angle);
            var y = 5.0 * Math.sin(angle);
            lightPosition_One[1] = x;
            lightPosition_One[2] = y - 6.0;

            // Set Light One Position  rotating One Light - Y Rotation
            angle = degToRad(lightAngle);
            x = 5.0 * Math.cos(angle);
            y = 5.0 * Math.sin(angle);
            lightPosition_Two[0] = x;
            lightPosition_Two[2] = y - 6.0;

            // Set Light Two Position rotating Two Light Z Rotation
            angle = degToRad(lightAngle);
            x = 5.0 * Math.cos(angle);
            y = 5.0 * Math.sin(angle);
            lightPosition_Three[0] = x;
            lightPosition_Three[1] = y;
            lightPosition_Three[2] = -6.0;

            gl.uniform1i(lightingEnabledUniform_PF, 1);

            gl.uniform3fv(laUniform_PF[0], lightAmbiant_One);
            gl.uniform3fv(ldUniform_PF[0], lightDiffuse_One);
            gl.uniform3fv(lsUniform_PF[0], lightSpecular_One);
            gl.uniform4fv(lighPositionUniform_PF[0], lightPosition_One);

            gl.uniform3fv(laUniform_PF[1], lightAmbiant_Two);
            gl.uniform3fv(ldUniform_PF[1], lightDiffuse_Two);
            gl.uniform3fv(lsUniform_PF[1], lightSpecular_Two);
            gl.uniform4fv(lighPositionUniform_PF[1], lightPosition_Two);

            gl.uniform3fv(laUniform_PF[2], lightAmbiant_Three);
            gl.uniform3fv(ldUniform_PF[2], lightDiffuse_Three);
            gl.uniform3fv(lsUniform_PF[2], lightSpecular_Three);
            gl.uniform4fv(lighPositionUniform_PF[2], lightPosition_Three);

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
        gl.uniformMatrix4fv(projectionMatrixUniform_PV, false, perspectiveProjectionMatrix);

        if (bLight == true) {
            gl.uniform1i(lightingEnabledUniform_PV, 1);

            var angle = degToRad(lightAngle);
            var x = 5.0 * Math.cos(angle);
            var y = 5.0 * Math.sin(angle);
            lightPosition_One[1] = x;
            lightPosition_One[2] = y - 6.0;

            // Set Light One Position  rotating One Light - Y Rotation
            angle = degToRad(lightAngle);
            x = 5.0 * Math.cos(angle);
            y = 5.0 * Math.sin(angle);
            lightPosition_Two[0] = x;
            lightPosition_Two[2] = y - 6.0;

            // Set Light Two Position rotating Two Light Z Rotation
            angle = degToRad(lightAngle);
            x = 5.0 * Math.cos(angle);
            y = 5.0 * Math.sin(angle);
            lightPosition_Three[0] = x;
            lightPosition_Three[1] = y;
            lightPosition_Three[2] = -6.0;

            gl.uniform3fv(laUniform_PV[0], lightAmbiant_One);
            gl.uniform3fv(ldUniform_PV[0], lightDiffuse_One);
            gl.uniform3fv(lsUniform_PV[0], lightSpecular_One);
            gl.uniform4fv(lighPositionUniform_PV[0], lightPosition_One);

            gl.uniform3fv(laUniform_PV[1], lightAmbiant_Two);
            gl.uniform3fv(ldUniform_PV[1], lightDiffuse_Two);
            gl.uniform3fv(lsUniform_PV[1], lightSpecular_Two);
            gl.uniform4fv(lighPositionUniform_PV[1], lightPosition_Two);

            gl.uniform3fv(laUniform_PV[2], lightAmbiant_Three);
            gl.uniform3fv(ldUniform_PV[2], lightDiffuse_Three);
            gl.uniform3fv(lsUniform_PV[2], lightSpecular_Three);
            gl.uniform4fv(lighPositionUniform_PV[2], lightPosition_Three);

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

    gl.useProgram(null);    // Double Buffering Emulation
    requestAnimationFrame(display, canvas);

    // Update
    update();
}

function update() {
    /* CODE */
    lightAngle = lightAngle + 3.0;
    if (lightAngle > 360.0)
        lightAngle = 0.0;
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

    if (shaderProgramObject_PF) {

        gl.useProgram(shaderProgramObject_PF);
        var shaderObjects = gl.getAttachedShaders(shaderProgramObject_PF);
        for (let i = 0; i < shaderObjects.length; i++) {
            gl.detachShader(shaderProgramObject_PF, shaderObjects[i]);
            gl.deleteShader(shaderObjects[i]);
            shaderObjects[i] = 0;
        }
        delete (shaderObjects);
        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject_PF);
        shaderProgramObject_PF = 0;
    }
}
