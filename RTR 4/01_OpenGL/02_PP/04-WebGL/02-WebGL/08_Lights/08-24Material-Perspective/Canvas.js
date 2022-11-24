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


var laUniform;			// light Ambiant
var ldUniform;			// light Diffuse
var lsUniform;			// light Spec
var lighPositionUniform; // light Position

var kaUniform; // material amb
var kdUniform; // mat diff
var ksUniform; // mat specular
var materialShininessUniform;


var lightingEnabledUniform;

var lightAmbiant = [0.1, 0.1, 0.1];
var lightDiffuse = [1.0, 1.0, 1.0];
var lightSpecular = [1.0, 1.0, 1.0];
var lightPosition = [100.0, 100.0, 100.0, 1.0];

var materialAmbiant = [0.0, 0.0, 0.0];
var meterialDeffuse = [1.0, 1.0, 1.0];
var materialSpecular = [1.0, 1.0, 1.0];
var materialShineeness = 120.0;

var bLight = false;
var currentWindowWidth;
var currentWindowHeight;
var angleForRotation = 0.0;
var keyPressed = 0;

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
        "in vec3 a_normal;" +
        "uniform mat4 u_modelMatrix;" +
        "uniform mat4 u_viewMatrix;" +
        "uniform mat4 u_projectionMatrix;" +
        "uniform vec4 u_lightPosition;" +
        "uniform highp int u_lightingEnabled;" +
        "out vec3 transformedNormals;" +
        "out vec3 lightDirection;" +
        "out vec3 viewerVector;" +
        "void main(void)" +
        "{" +
        "if(u_lightingEnabled == 1)" +
        "{" +
        "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
        "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
        "transformedNormals = normalMatrix * a_normal;" +
        "lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;" + // Swizaling
        "viewerVector = -eyeCoordinates.xyz;" +
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
        "in vec3 lightDirection;" +
        "in vec3 viewerVector;" +
        "uniform vec3 u_la;" +
        "uniform vec3 u_ld;" +
        "uniform vec3 u_ls;" +
        "uniform vec3 u_ka;" +
        "uniform vec3 u_ks;" +
        "uniform vec3 u_kd;" +
        "uniform float u_materialShininnes;" +
        "uniform highp int u_lightingEnabled;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "vec3 phong_ads_color;" +
        "if(u_lightingEnabled == 1)" +
        "{" +
        "vec3 ambiant = u_la * u_ka;" +
        "vec3 normalized_transformed_normals = normalize(transformedNormals);" +
        "vec3 normalized_light_direction = normalize(lightDirection);" +
        "vec3 diffuse = u_ld * u_kd * max(dot(normalized_light_direction ,normalized_transformed_normals),0.0);"
        +
        "vec3 reflectionVector = reflect(-normalized_light_direction,normalized_transformed_normals);" +
        "vec3 normalized_view_vector = normalize(viewerVector);" +
        "vec3 specular = u_ls * u_ks * pow(max(dot(reflectionVector , normalized_view_vector), 0.0), u_materialShininnes);"
        +
        "phong_ads_color = ambiant + diffuse +  specular;" +
        "}" +
        "else" +
        "{" +
        "phong_ads_color = vec3(1.0,1.0,1.0);" +
        "}" +
        "FragColor = vec4(phong_ads_color, 1.0);" +
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

    laUniform = gl.getUniformLocation(shaderProgramObject, "u_la");
    ldUniform = gl.getUniformLocation(shaderProgramObject, "u_ld");
    lsUniform = gl.getUniformLocation(shaderProgramObject, "u_ls");
    lighPositionUniform = gl.getUniformLocation(shaderProgramObject, "u_lightPosition");

    kaUniform = gl.getUniformLocation(shaderProgramObject, "u_ka");
    kdUniform = gl.getUniformLocation(shaderProgramObject, "u_kd");
    ksUniform = gl.getUniformLocation(shaderProgramObject, "u_ks");
    materialShininessUniform = gl.getUniformLocation(shaderProgramObject, "u_materialShininnes");


    lightingEnabledUniform = gl.getUniformLocation(shaderProgramObject, "u_lightingEnabled");

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
    var scaleMatrix = mat4.create();
    var modelMatrix = mat4.create();

    // DRAW Sphere
    mat4.scale(scaleMatrix, scaleMatrix, [0.90, 0.90, 0.90]);

    mat4.translate(translateMatrix, translateMatrix, [0.0, 0.0, -6.0]);
    mat4.multiply(modelMatrix, translateMatrix, scaleMatrix);

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform, false, perspectiveProjectionMatrix);

    draw24Sphere();

    gl.useProgram(null);

    // Double Buffering Emulation
    requestAnimationFrame(display, canvas);

    // Update
    update();
}

function update() {
    /* CODE */
    if (keyPressed == 1 || keyPressed == 2 || keyPressed == 3) {

        angleForRotation = angleForRotation + 1.0;
        if (angleForRotation > 360.0)
            angleForRotation = angleForRotation - 360.0;

        // Set Light Zero Position - rotating  light
        var angle = (angleForRotation * Math.PI) / 180.0;
        var x = 10.0 * Math.cos(angle);
        var y = 10.0 * Math.sin(angle);

        if (keyPressed == 1) {
            lightPosition[1] = x;
            lightPosition[2] = y - 6.0;
        }
        if (keyPressed == 2) {
            lightPosition[0] = x;
            lightPosition[2] = y - 6.0;
        }
        if (keyPressed == 3) {
            lightPosition[0] = x;
            lightPosition[1] = y;
            lightPosition[2] = -6.0;
        }
    }
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

        case 88:
            keyPressed = 1;
            angleForRotation = 0.0; // reset
            lightPosition[0] = 0;
            lightPosition[1] = 0;
            lightPosition[2] = 0;
            break;

        case 89:
            keyPressed = 2;
            angleForRotation = 0.0; // reset
            lightPosition[0] = 0;
            lightPosition[1] = 0;
            lightPosition[2] = 0;
            break;

        case 90:
            keyPressed = 3;
            angleForRotation = 0.0; // reset
            lightPosition[0] = 0;
            lightPosition[1] = 0;
            lightPosition[2] = 0;
            break;

        case 76:
            if (bLight == false) {
                bLight = true;
            }
            else {
                bLight = false;
            }
            break;

        default:
            keyPressed = 0;
            lightPositions[0] = 0.0;
            lightPositions[1] = 0.0;
            lightPositions[2] = 4.0;
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

function draw24Sphere() {

    // Code
    currentWindowWidth = canvas.width;
    currentWindowHeight = canvas.height;

    var horDistance = currentWindowWidth / 6;
    var verDistance = currentWindowHeight / 7;

    // ***** 1st sphere on 1st column, emerald *****
    // translationMatrix = vmath::translate(-9.0, 6.0, -21.0); // glTranslatef() is replaced by this line

    gl.viewport(0.0, verDistance * 5, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0215; // r
    materialAmbiant[1] = 0.1745; // g
    materialAmbiant[2] = 0.0215; // b

    // diffuse material
    meterialDeffuse[0] = 0.07568; // r
    meterialDeffuse[1] = 0.61424; // g
    meterialDeffuse[2] = 0.07568; // b

    // specular material
    materialSpecular[0] = 0.633;	// r
    materialSpecular[1] = 0.727811; // g
    materialSpecular[2] = 0.633;	// b


    // shininess
    materialShinniness = 0.6 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 2
    // ***** 2nd sphere on 1st column, jade *****
    //	gl.viewport(0.0, (WINHEIGHT) - (WINHEIGHT) / 4, (currentWindowWidth / 4), (currentWindowHeight / 4));
    // gl.viewport(0.0, 120 * 4.4, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(0.0, verDistance * 4, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.135;	 // r
    materialAmbiant[1] = 0.2225; // g
    materialAmbiant[2] = 0.1575; // b

    // diffuse material
    meterialDeffuse[0] = 0.54; // r
    meterialDeffuse[1] = 0.89; // g
    meterialDeffuse[2] = 0.63; // b

    // specular material
    materialSpecular[0] = 0.316228; // r
    materialSpecular[1] = 0.316228; // g
    materialSpecular[2] = 0.316228; // b

    // shininess
    materialShinniness = 0.1 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // ***** 3rd sphere on 1st column, obsidian *****
    // 3
    // gl.viewport(0.0, (WINHEIGHT) - ((WINHEIGHT) / 2), (currentWindowWidth / 4), (currentWindowHeight / 4));
    // gl.viewport(0.0, verDistance * 4, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(0.0, verDistance * 3, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.05375; // r
    materialAmbiant[1] = 0.05;	  // g
    materialAmbiant[2] = 0.06625; // ba

    // diffuse material
    meterialDeffuse[0] = 0.18275; // r
    meterialDeffuse[1] = 0.17;	  // g
    meterialDeffuse[2] = 0.22525; // b


    // specular material
    materialSpecular[0] = 0.332741; // r
    materialSpecular[1] = 0.328634; // g
    materialSpecular[2] = 0.346435; // b

    // shininess
    materialShinniness = 0.3 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 4
    // ***** 4th sphere on 1st column, pearl *****
    // gl.viewport(0.0, (WINHEIGHT) - ((WINHEIGHT) / 1.3), (currentWindowWidth / 4), (currentWindowHeight / 4));
    // gl.viewport(0.0, verDistance * 3, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(0.0, verDistance * 2, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.25;	  // r
    materialAmbiant[1] = 0.20725; // g
    materialAmbiant[2] = 0.20725; // ba

    // diffuse material
    meterialDeffuse[0] = 1.0;	// r
    meterialDeffuse[1] = 0.829; // g
    meterialDeffuse[2] = 0.829; // b

    // specular material
    materialSpecular[0] = 0.296648; // r
    materialSpecular[1] = 0.296648; // g
    materialSpecular[2] = 0.296648; // b

    // shininess
    materialShinniness = 0.088 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 5
    // ***** 5th sphere on 1st column, ruby *****
    gl.viewport(0.0, verDistance, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.1745;  // r
    materialAmbiant[1] = 0.01175; // g
    materialAmbiant[2] = 0.01175; // ba

    // diffuse material
    meterialDeffuse[0] = 0.61424; // r
    meterialDeffuse[1] = 0.04136; // g
    meterialDeffuse[2] = 0.04136; // b

    // specular material
    materialSpecular[0] = 0.727811; // r
    materialSpecular[1] = 0.626959; // g
    materialSpecular[2] = 0.626959; // b

    // shininess
    materialShinniness = 0.6 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 6
    // ***** 6th sphere on 1st column, turquoise *****
    //	gl.viewport(0.0, 0.0, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(0.0, 0, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.1;	  // r
    materialAmbiant[1] = 0.18725; // g
    materialAmbiant[2] = 0.1745;  // ba

    // diffuse material
    meterialDeffuse[0] = 0.396;	  // r
    meterialDeffuse[1] = 0.74151; // g
    meterialDeffuse[2] = 0.69102; // b


    // specular material
    materialSpecular[0] = 0.297254; // r
    materialSpecular[1] = 0.30829;	// g
    materialSpecular[2] = 0.306678; // b


    // shininess
    materialShinniness = 0.1 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 2nd column, brass *****
    // ambient material
    // 6
    // gl.viewport(425.0, 120 * 5.5, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 1.5, verDistance * 5, (currentWindowWidth / 4), (currentWindowHeight / 4));


    materialAmbiant[0] = 0.329412; // r
    materialAmbiant[1] = 0.223529; // g
    materialAmbiant[2] = 0.027451; // b a

    // diffuse material
    meterialDeffuse[0] = 0.780392; // r
    meterialDeffuse[1] = 0.568627; // g
    meterialDeffuse[2] = 0.113725; // b

    // specular material
    materialSpecular[0] = 0.992157; // r
    materialSpecular[1] = 0.941176; // g
    materialSpecular[2] = 0.807843; // b

    // shininess
    materialShinniness = 0.21794872 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 7
    // ***** 2nd sphere on 2nd column, bronze *****
    // gl.viewport(425.0, 120 * 4.4, (currentWindowWidth / 4), (currentWindowHeight / 4));

    gl.viewport(horDistance * 1.5, verDistance * 4, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.2125; // r
    materialAmbiant[1] = 0.1275; // g
    materialAmbiant[2] = 0.054;	 // b

    // diffuse material
    meterialDeffuse[0] = 0.714;	  // r
    meterialDeffuse[1] = 0.4284;  // g
    meterialDeffuse[2] = 0.18144; // b

    // specular material
    materialSpecular[0] = 0.393548; // r
    materialSpecular[1] = 0.271906; // g
    materialSpecular[2] = 0.166721; // b

    // shininess
    materialShinniness = 0.2 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 8
    // ***** 3rd sphere on 2nd column, chrome *****
    // gl.viewport(425.0, 120 * 3.3, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 1.5, verDistance * 3, (currentWindowWidth / 4), (currentWindowHeight / 4));


    // ambient material
    materialAmbiant[0] = 0.25; // r
    materialAmbiant[1] = 0.25; // g
    materialAmbiant[2] = 0.25; // b
    // diffuse material
    meterialDeffuse[0] = 0.4;  // r
    meterialDeffuse[1] = 0.4;  // g
    meterialDeffuse[2] = 0.4;  // b

    // specular material
    materialSpecular[0] = 0.774597; // r
    materialSpecular[1] = 0.774597; // g
    materialSpecular[2] = 0.774597; // b

    // shininess
    materialShinniness = 0.6 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 9
    // ***** 4th sphere on 2nd column, copper *****
    // gl.viewport(425.0, 120 * 2.2, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 1.5, verDistance * 2, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.19125; // r
    materialAmbiant[1] = 0.0735;  // g
    materialAmbiant[2] = 0.0225;  // ba

    // diffuse material
    meterialDeffuse[0] = 0.7038;  // r
    meterialDeffuse[1] = 0.27048; // g
    meterialDeffuse[2] = 0.0828;  // b

    // specular material
    materialSpecular[0] = 0.256777; // r
    materialSpecular[1] = 0.137622; // g
    materialSpecular[2] = 0.086014; // b

    // shininess
    materialShinniness = 0.1 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 10
    // ***** 5th sphere on 2nd column, gold *****
    //	gl.viewport(425.0, 120 * 1.1, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 1.5, verDistance, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.24725; // r
    materialAmbiant[1] = 0.1995;  // g
    materialAmbiant[2] = 0.0745;  // ba

    // diffuse material
    meterialDeffuse[0] = 0.75164; // r
    meterialDeffuse[1] = 0.60648; // g
    meterialDeffuse[2] = 0.22648; // b

    // specular material
    materialSpecular[0] = 0.628281; // r
    materialSpecular[1] = 0.555802; // g
    materialSpecular[2] = 0.366065; // b

    // shininess
    materialShinniness = 0.4 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 11
    // ***** 6th sphere on 2nd column, silver *****
    // gl.viewport(425.0, 0, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 1.5, 0, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.19225; // r
    materialAmbiant[1] = 0.19225; // g
    materialAmbiant[2] = 0.19225; // ba

    // diffuse material
    meterialDeffuse[0] = 0.50754; // r
    meterialDeffuse[1] = 0.50754; // g
    meterialDeffuse[2] = 0.50754; // b

    // specular material
    materialSpecular[0] = 0.508273; // r
    materialSpecular[1] = 0.508273; // g
    materialSpecular[2] = 0.508273; // b

    // shininess
    materialShinniness = 0.4 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 12
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 3rd column, black *****
    // gl.viewport(850.0, 120 * 5.5, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 3, verDistance * 5, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    // diffuse material
    meterialDeffuse[0] = 0.01; // r
    meterialDeffuse[1] = 0.01; // g
    meterialDeffuse[2] = 0.01; // b

    // specular material
    materialSpecular[0] = 0.50; // r
    materialSpecular[1] = 0.50; // g
    materialSpecular[2] = 0.50; // b

    // shininess
    materialShinniness = 0.25 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // ***** 2nd sphere on 3rd column, cyan *****
    // 13
    gl.viewport(850.0, 120 * 4.4, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 3, verDistance * 4, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.1;  // g
    materialAmbiant[2] = 0.06; // b
    // diffuse material
    meterialDeffuse[0] = 0.0;		 // r
    meterialDeffuse[1] = 0.50980392; // g
    meterialDeffuse[2] = 0.50980392; // b


    // specular material
    materialSpecular[0] = 0.50185078; // r
    materialSpecular[1] = 0.50185078; // g
    materialSpecular[2] = 0.50185078; // b

    // shininess
    materialShinniness = 0.25 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 14
    // ***** 3rd sphere on 2nd column, green *****
    gl.viewport(horDistance * 3, verDistance * 3, (currentWindowWidth / 4), (currentWindowHeight / 4));


    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b
    // diffuse material
    meterialDeffuse[0] = 0.1;  // r
    meterialDeffuse[1] = 0.35; // g
    meterialDeffuse[2] = 0.1;  // b

    // specular material
    materialSpecular[0] = 0.45; // r
    materialSpecular[1] = 0.55; // g
    materialSpecular[2] = 0.45; // b

    // shininess
    materialShinniness = 0.25 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 15
    // ***** 4th sphere on 3rd column, red *****
    // gl.viewport(850.0, 120 * 2.2, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 3, verDistance * 2, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b

    // diffuse material
    meterialDeffuse[0] = 0.5;  // r
    meterialDeffuse[1] = 0.0;  // g
    meterialDeffuse[2] = 0.0;  // b


    // specular material
    materialSpecular[0] = 0.7;	// r
    materialSpecular[1] = 0.6;	// g
    materialSpecular[2] = 0.6;	// b


    // shininess
    materialShinniness = 0.25 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 16
    // ***** 5th sphere on 3rd column, white *****
    // gl.viewport(850.0, 120 * 1.1, (currentWindowWidth / 4), (currentWindowHeight / 4));
    gl.viewport(horDistance * 3, verDistance, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b

    // diffuse material
    meterialDeffuse[0] = 0.55; // r
    meterialDeffuse[1] = 0.55; // g
    meterialDeffuse[2] = 0.55; // b



    // specular material
    materialSpecular[0] = 0.70; // r
    materialSpecular[1] = 0.70; // g
    materialSpecular[2] = 0.70; // b


    // shininess
    materialShinniness = 0.25 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 17
    // ***** 6th sphere on 3rd column, yellow plastic *****
    gl.viewport(horDistance * 3, 0, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b

    // diffuse material
    meterialDeffuse[0] = 0.5;  // r
    meterialDeffuse[1] = 0.5;  // g
    meterialDeffuse[2] = 0.0;  // b

    // specular material
    materialSpecular[0] = 0.60; // r
    materialSpecular[1] = 0.60; // g
    materialSpecular[2] = 0.50; // b

    // shininess
    materialShinniness = 0.25 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 18
    // *******************************************************
    // *******************************************************
    // *******************************************************

    // ***** 1st sphere on 4th column, black *****
    // ambient material
    gl.viewport(horDistance * 4.5, verDistance * 5, (currentWindowWidth / 4), (currentWindowHeight / 4));

    materialAmbiant[0] = 0.02; // r
    materialAmbiant[1] = 0.02; // g
    materialAmbiant[2] = 0.02; // b
    // diffuse material
    meterialDeffuse[0] = 0.01; // r
    meterialDeffuse[1] = 0.01; // g
    meterialDeffuse[2] = 0.01; // b


    // specular material
    materialSpecular[0] = 0.4;	// r
    materialSpecular[1] = 0.4;	// g
    materialSpecular[2] = 0.4;	// b


    // shininess
    materialShinniness = 0.078125 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 19
    // ***** 2nd sphere on 4th column, cyan *****
    gl.viewport(horDistance * 4.5, verDistance * 4, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.05; // b
    // diffuse material
    meterialDeffuse[0] = 0.4;  // r
    meterialDeffuse[1] = 0.5;  // g
    meterialDeffuse[2] = 0.5;  // b

    // specular material
    materialSpecular[0] = 0.04; // r
    materialSpecular[1] = 0.7;	// g
    materialSpecular[2] = 0.7;	// b

    // shininess
    materialShinniness = 0.078125 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 20
    // ***** 3rd sphere on 4th column, green *****
    gl.viewport(horDistance * 4.5, verDistance * 3, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.0;  // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.0;  // b
    // diffuse material
    meterialDeffuse[0] = 0.4;  // r
    meterialDeffuse[1] = 0.5;  // g
    meterialDeffuse[2] = 0.4;  // b

    // specular material
    materialSpecular[0] = 0.04; // r
    materialSpecular[1] = 0.7;	// g
    materialSpecular[2] = 0.04; // b

    // shininess
    materialShinniness = 0.078125 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 21
    // ***** 4th sphere on 4th column, red *****
    gl.viewport(horDistance * 4.5, verDistance * 2, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.05; // r
    materialAmbiant[1] = 0.0;  // g
    materialAmbiant[2] = 0.0;  // b

    // diffuse material
    meterialDeffuse[0] = 0.5;  // r
    meterialDeffuse[1] = 0.4;  // g
    meterialDeffuse[2] = 0.4;  // b

    // specular material
    materialSpecular[0] = 0.7;	// r
    materialSpecular[1] = 0.04; // g
    materialSpecular[2] = 0.04; // b

    // shininess
    materialShinniness = 0.078125 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 22
    // ***** 5th sphere on 4th column, white *****
    gl.viewport(horDistance * 4.5, verDistance, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.05; // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.05; // b

    // diffuse material
    meterialDeffuse[0] = 0.5;  // r
    meterialDeffuse[1] = 0.5;  // g
    meterialDeffuse[2] = 0.5;  // b

    // specular material
    materialSpecular[0] = 0.7;	// r
    materialSpecular[1] = 0.7;	// g
    materialSpecular[2] = 0.7;	// b

    // shininess
    materialShinniness = 0.078125 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();

    // 23
    // ***** 6th sphere on 4th column, yellow rubber *****
    gl.viewport(horDistance * 4.5, 0, (currentWindowWidth / 4), (currentWindowHeight / 4));

    // ambient material
    materialAmbiant[0] = 0.05; // r
    materialAmbiant[1] = 0.05; // g
    materialAmbiant[2] = 0.0;  // b

    // diffuse material
    meterialDeffuse[0] = 0.5;  // r
    meterialDeffuse[1] = 0.5;  // g
    meterialDeffuse[2] = 0.4;  // b

    // specular material
    materialSpecular[0] = 0.7;	// r
    materialSpecular[1] = 0.7;	// g
    materialSpecular[2] = 0.04; // b

    // shininess
    materialShinniness = 0.078125 * 128;

    if (bLight == true) {
        gl.uniform1i(lightingEnabledUniform, 1);

        gl.uniform3fv(laUniform, lightAmbiant); // we can use gl.uniform3f() ,so we can directly pass the values to uniform
        gl.uniform3fv(ldUniform, lightDiffuse);
        gl.uniform3fv(lsUniform, lightSpecular);
        gl.uniform4fv(lighPositionUniform, lightPosition);

        gl.uniform3fv(kaUniform, materialAmbiant);
        gl.uniform3fv(kdUniform, meterialDeffuse);
        gl.uniform3fv(ksUniform, materialSpecular);
        gl.uniform1f(materialShininessUniform, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform, 0);
    }

    // DRAW SPHERE
    sphere.draw();
}

