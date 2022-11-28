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

var vao_Cube;
var vbo_Cube;

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
var textureSamplerUniform;
var texture_marble;
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
        "#version 300 es" +
        "\n" +
        "in vec4 a_position;" +
        "in vec4 a_color;" +
        "in vec3 a_normal;" +
        "in vec2 a_texcoord;" +
        "uniform mat4 u_modelMatrix;" +
        "uniform mat4 u_viewMatrix;" +
        "uniform mat4 u_projectionMatrix;" +
        "uniform vec4 u_lightPosition;" +
        "uniform mediump int u_lightEnabled;" +
        "out vec3 transformedNormals;" +
        "out vec3 lightDirection;" +
        "out vec3 viewerVector;" +
        "out vec4 a_color_out;" +
        "out vec2 a_texcoord_out;" +
        "void main(void)" +
        "{" +
        "if(u_lightEnabled == 1)" +
        "{" +
        "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
        "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
        "transformedNormals = normalMatrix * a_normal;" +
        "lightDirection = vec3(u_lightPosition) - eyeCoordinates.xyz;" + // Swizaling+
        "viewerVector = -eyeCoordinates.xyz;" +
        "}" +
        "gl_Position = u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
        "a_color_out = a_color;" +
        "a_texcoord_out = a_texcoord;" +
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
        "in vec4 a_color_out;" +
        "in vec2 a_texcoord_out;" +

        "uniform vec3 u_la;" +
        "uniform vec3 u_ld;" +
        "uniform vec3 u_ls;" +
        "uniform vec3 u_ka;" +
        "uniform vec3 u_ks;" +
        "uniform vec3 u_kd;" +

        "uniform highp float u_materialShininnes;" +
        "uniform mediump int u_lightEnabled;" +
        "uniform highp sampler2D u_textureSampler;" +

        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "vec3 phong_ads_color;" +
        "vec3 tex;" +
        "tex = vec3(texture(u_textureSampler, a_texcoord_out));" +
        "if(u_lightEnabled == 1)" +
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
        "phong_ads_color = ambiant + diffuse + specular;" +
        "}" +
        "else" +
        "{" +
        "phong_ads_color = vec3(1.0 , 1.0 , 1.0);" +
        "}" +
        "FragColor = vec4(tex * phong_ads_color * vec3(a_color_out), 1.0);" +
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
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.PRJ_ATTRIBUTE_COLOR, "a_color");
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");
    gl.bindAttribLocation(shaderProgramObject, webGLMacros.PRJ_ATTRIBUTE_TEXTURE0, "a_texcoord");

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

    textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "u_textureSampler");

    var cubepcnt = new Float32Array([
        // front             //color-red         //normal-front      //texture-front
        1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,
        -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
        -1.0, -1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0,
        1.0, -1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,

        // right             //color-green       //normal-right      //texture-right
        1.0, 1.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
        1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
        1.0, -1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0,
        1.0, -1.0, -1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.0,

        // back              //color-blue        //normal-back       //texture-back
        -1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0, 0.0,
        1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0,
        1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 0.0, 1.0,
        -1.0, -1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, -1.0, 1.0, 1.0,

        // left              //color-cyan        //normal-left       //texture-back
        -1.0, 1.0, 1.0, 0.0, 1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0,
        -1.0, 1.0, -1.0, 0.0, 1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0,
        -1.0, -1.0, -1.0, 0.0, 1.0, 1.0, -1.0, 0.0, 0.0, 0.0, 1.0,
        -1.0, -1.0, 1.0, 0.0, 1.0, 1.0, -1.0, 0.0, 0.0, 1.0, 1.0,

        // top               //color-magenta     //normal-top        //texture-top
        1.0, 1.0, -1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
        -1.0, 1.0, -1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0,
        -1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0,
        1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0,

        // bottom            //color-yellow      //normal-bottom     //texture-bottom
        1.0, -1.0, -1.0, 1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0,
        -1.0, -1.0, -1.0, 1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0,
        -1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 1.0,
        1.0, -1.0, 1.0, 1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 1.0, 1.0
    ]);


    // VERTEX ARRAY OBJECT
    vao_Cube = gl.createVertexArray();
    gl.bindVertexArray(vao_Cube);
    vbo_Cube = gl.createBuffer();
    gl.bindBuffer(gl.ARRAY_BUFFER, vbo_Cube);

    gl.bufferData(gl.ARRAY_BUFFER, cubepcnt, gl.STATIC_DRAW);


    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_POSITION, 3, gl.FLOAT, false, 11 * 4, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_POSITION);

    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_COLOR, 3, gl.FLOAT, false, 11 * 4, 3 * 4);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_COLOR);

    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_NORMAL, 3, gl.FLOAT, false, 11 * 4, 6 * 4);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_NORMAL);

    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 11 * 4, 9 * 4);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_TEXTURE0);


    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    gl.bindVertexArray(null);

    // Depth Related Changes
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    // TEXTURE RELATED CODE
    texture_marble = gl.createTexture();
    texture_marble.image = new Image();
    texture_marble.image.src = "marble.png";
    texture_marble.image.onload = function () {
        gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, true);
        gl.bindTexture(gl.TEXTURE_2D, texture_marble);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

        gl.texImage2D(gl.TEXTURE_2D,
            0,
            gl.RGBA,
            gl.RGBA,
            gl.UNSIGNED_BYTE,
            texture_marble.image);

        gl.generateMipmap(gl.TEXTURE_2D);
        gl.bindTexture(gl.TEXTURE_2D, null);
    }
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

    var rotationMatrix = mat4.create();
    var scaleMatrix = mat4.create();
    var modelMatrix = mat4.create();
    var viewMatrix = mat4.create();
    var translateMatrix = mat4.create();

    // DRAW Cube
    mat4.translate(translateMatrix, translateMatrix, [0.0, 0.0, -4.0]);
    mat4.rotateX(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateY(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateZ(rotationMatrix, rotationMatrix, degToRad(angleCube));

    mat4.scale(scaleMatrix, scaleMatrix, [0.8, 0.8, 0.8]);

    mat4.multiply(rotationMatrix, rotationMatrix, scaleMatrix);
    mat4.multiply(modelMatrix, translateMatrix, rotationMatrix);

    gl.uniformMatrix4fv(modelMatrixUniform, false, modelMatrix);
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

    gl.activeTexture(gl.TEXTURE0); //
    gl.bindTexture(gl.TEXTURE_2D, texture_marble);
    gl.uniform1i(textureSamplerUniform, 0); //

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

function update() {
    /* CODE */
    angleCube = angleCube + 3.00;
    if (angleCube >= 360.0)
        angleCube = angleCube - 360.0;

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
    if (vbo_Cube) {
        gl.deleteBuffer(vbo_Cube);
        vbo_Cube = null;
    }

    if (vao_Cube) {
        gl.deleteVertexArray(vao_Cube);
        vao_Cube = null;
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
