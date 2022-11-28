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


const FBO_WIDTH = 512;
const FBO_HEIGHT = 512;

var shaderProgramObject;

var vao_Cube;
var vbo_Cube_Position;
var vbo_Cube_Texcoord;

var mvpMatrixUniform;
var perspectiveProjectionMatrix;

var angleCube = 0.0;

var textureSamplerUniform;
var texture_kundali;

var fbo; // FRAME BUFFER OBJECT
var rbo; // RENDER BUFFER OBJECT
var fbo_texture;
var bBFOResult = false;


// Sphere Related Variables
var shaderProgramObject_Sphere;

var modelMatrixUniform_Sphere;
var viewMatrixUniform_Sphere;
var projectionMatrixUniform_Sphere;
var perspectiveProjectionMatrix_Sphere;

var laUniform_Sphere = [];			    // light Ambiant
var ldUniform_Sphere = [];			    // light Diffuse
var lsUniform_Sphere = [];			    // light Spec
var lighPositionUniform_Sphere = [];    // light Position

var kaUniform_Sphere; // material amb
var kdUniform_Sphere; // mat diff
var ksUniform_Sphere; // mat specular
var materialShininessUniform_Sphere;

var lightingEnabledUniform_Sphere;

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

    // VERTEX SHADER
    var vertexShaderSourceCode =
        "#version 300 es" +
        "\n" +
        "in vec4 a_position;" +
        "in vec2 a_texcoord;" +
        "uniform mat4 u_mvpMatrix;" +
        "out vec2 a_texcoord_out;" +
        "void main(void)" +
        "{" +
        "gl_Position = u_mvpMatrix * a_position;" +
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
        "in vec2 a_texcoord_out;" +
        "uniform highp sampler2D u_textureSampler;" +
        "out vec4 FragColor;" +
        "void main(void)" +
        "{" +
        "FragColor = texture(u_textureSampler , a_texcoord_out);" +
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
    mvpMatrixUniform = gl.getUniformLocation(shaderProgramObject, "u_mvpMatrix");
    textureSamplerUniform = gl.getUniformLocation(shaderProgramObject, "u_textureSampler");


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

    var CubeTexcoord = new Float32Array([
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0,

        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0,
        1.0, 0.0
    ]);

    // VERTEX ARRAY OBJECT
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
    gl.bufferData(gl.ARRAY_BUFFER, CubeTexcoord, gl.STATIC_DRAW);
    gl.vertexAttribPointer(webGLMacros.PRJ_ATTRIBUTE_TEXTURE0, 2, gl.FLOAT, false, 0, 0);
    gl.enableVertexAttribArray(webGLMacros.PRJ_ATTRIBUTE_TEXTURE0);
    gl.bindBuffer(gl.ARRAY_BUFFER, null);

    gl.bindVertexArray(null);

    // Depth Related Changes
    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    //gl.enable(gl.TEXTURE_2D);

    // Clear Screen Using Blue Color
    gl.clearColor(1.0, 1.0, 1.0, 1.0);

    perspectiveProjectionMatrix = mat4.create();

    bBFOResult = CreateFBO(FBO_WIDTH, FBO_HEIGHT);

    var iRetVal;
    if (bBFOResult == true) {
        iRetVal = initialize_sphere(FBO_WIDTH, FBO_HEIGHT);
        //    HERE SHOULD BE ERROR CHECKING
    }
    else {
        //fprintf(gpFile, "CREATE FBO FAILED...\n");
    }

}

function initialize_sphere(width, height) {
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
        "gl_Position= u_projectionMatrix * u_viewMatrix * u_modelMatrix * a_position;" +
        "}";

    var vertexShaderObject = gl.createShader(gl.VERTEX_SHADER);

    gl.shaderSource(vertexShaderObject, vertexShaderSourceCode);

    gl.compileShader(vertexShaderObject);

    if (gl.getShaderParameter(vertexShaderObject, gl.COMPILE_STATUS) == false) {
        var error = gl.getShaderInfoLog(vertexShaderObject);
        if (error.length > 0) {
            var errorString = "VERTEX SHADER COMPILATION LOG :" + error;
            alert(errorString);
            uninitialize_sphere();
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
            uninitialize_sphere();
        }
    }

    // SHADER PROGRAM
    shaderProgramObject_Sphere = gl.createProgram();

    gl.attachShader(shaderProgramObject_Sphere, vertexShaderObject);
    gl.attachShader(shaderProgramObject_Sphere, fragmentShaderObject);

    // PRELINKING SHADER PROGRAM OBJECT ATTRIBUTE BINDING
    gl.bindAttribLocation(shaderProgramObject_Sphere, webGLMacros.PRJ_ATTRIBUTE_POSITION, "a_position");
    gl.bindAttribLocation(shaderProgramObject_Sphere, webGLMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");

    // SHADER PROGRAM LINKING
    gl.linkProgram(shaderProgramObject_Sphere);

    if (gl.getProgramParameter(shaderProgramObject_Sphere, gl.LINK_STATUS) == false) {
        var error = gl.getProgramInfoLog(shaderProgramObject_Sphere);
        if (error.length > 0) {
            //var errorString = "SHADER PROGRAM LINK LOG :" + error;
            alert(errorString);
            uninitialize_sphere();
        }
    }

    // POST LINK - GET UNIFORM LOCATION
    modelMatrixUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_modelMatrix");
    viewMatrixUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_viewMatrix");
    projectionMatrixUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_projectionMatrix");

    laUniform_Sphere[0] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_la[0]");
    ldUniform_Sphere[0] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ld[0]");
    lsUniform_Sphere[0] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ls[0]");
    lighPositionUniform_Sphere[0] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_lightPosition[0]");

    laUniform_Sphere[1] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_la[1]");
    ldUniform_Sphere[1] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ld[1]");
    lsUniform_Sphere[1] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ls[1]");
    lighPositionUniform_Sphere[1] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_lightPosition[1]");

    laUniform_Sphere[2] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_la[2]");
    ldUniform_Sphere[2] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ld[2]");
    lsUniform_Sphere[2] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ls[2]");
    lighPositionUniform_Sphere[2] = gl.getUniformLocation(shaderProgramObject_Sphere, "u_lightPosition[2]");

    kaUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ka");
    kdUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_kd");
    ksUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_ks");
    materialShininessUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_materialShininnes");

    lightingEnabledUniform_Sphere = gl.getUniformLocation(shaderProgramObject_Sphere, "u_lightingEnabled");

    gl.clearDepth(1.0);
    gl.enable(gl.DEPTH_TEST);
    gl.depthFunc(gl.LEQUAL);

    sphere = new Mesh();
    makeSphere(sphere, 2.0, 30, 30);

    perspectiveProjectionMatrix_Sphere = mat4.create();

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    resize_Sphere(FBO_WIDTH, FBO_HEIGHT);

}

function CreateFBO(textureWidth, textureHeight) {
    //
    var maxRenderbufferSize;

    // CODE
    maxRenderbufferSize = gl.getParameter(gl.MAX_RENDERBUFFER_SIZE);

    // CHECK AVAILABLE BUFFER SIZE
    if (maxRenderbufferSize < textureWidth || maxRenderbufferSize < textureHeight) {
        //fprintf(gpFile, "UnSufficient Render Buffer Size...\n");
        return (false);
    }

    // CREATE FRAME BUFFER OBJECT
    fbo = gl.createFramebuffer();
    gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);

    // CREATE RENDER BUFFER OBJECT
    rbo = gl.createRenderbuffer();
    gl.bindRenderbuffer(gl.RENDERBUFFER, rbo);

    // WHERE TO KEEP THIS RENDER BUFFER AND WHAT WILL BE THE FORMAT OF RENDER BUFFER
    gl.renderbufferStorage(
        gl.RENDERBUFFER,
        gl.DEPTH_COMPONENT16, // NOT RELATED TO DEPTH - BUT 16 MACRO SIZE THIS ONE HENCE USED HERE
        textureWidth,
        textureHeight);

    // CREATE EMPTY TEXTURE
    fbo_texture = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D, fbo_texture);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);

    var texture = new ImageData(textureWidth, textureHeight)

    gl.texImage2D(gl.TEXTURE_2D,
        0,
        gl.RGB,
        gl.RGB,
        gl.UNSIGNED_SHORT_5_6_5,
        texture
    );


    // GIVE ABOVE TEXTURE TO FBO
    gl.framebufferTexture2D(
        gl.FRAMEBUFFER,
        gl.COLOR_ATTACHMENT0,
        gl.TEXTURE_2D,
        fbo_texture,
        0 // MIPMAP LEVEL
    );

    // GIVE RENDER BUFFER TO FBO
    gl.framebufferRenderbuffer(
        gl.FRAMEBUFFER,
        gl.DEPTH_ATTACHMENT,
        gl.RENDERBUFFER,
        rbo //
    );

    // CHECK WATHER FBO IS CREATED SUCCESSFULLY OR NOT
    var result = gl.checkFramebufferStatus(gl.FRAMEBUFFER);
    if (result != gl.FRAMEBUFFER_COMPLETE) {
        // fprintf(gpFile, "FRAME BUFFER IS NOT COMPLETE...\n");
    }

    gl.bindFramebuffer(gl.FRAMEBUFFER, null); // IMPLICITEL UNBINDS THE RBO AND TEXTURE BUFFER OBJECT

    return true;
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


    resize_Sphere(canvas.width, canvas.height);

    mat4.perspective(
        perspectiveProjectionMatrix,
        45.0,
        parseFloat(canvas.width) / parseFloat(canvas.height),
        0.1,
        100.0);

}

function resize_Sphere(width, height) {

    /* CODE */
    if (height == 0)
        height = 1;

    gl.viewport(0, 0, width, height);

    mat4.perspective(
        perspectiveProjectionMatrix,
        45.0,
        parseFloat(width) / parseFloat(height),
        0.1,
        100.0);

}

function display() {

    /* CODE */
    if (bBFOResult == true) {
        display_sphere(FBO_WIDTH, FBO_HEIGHT);
    }

    gl.clearColor(1.0, 1.0, 1.0, 1.0);
    resize();
    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // USE SHADER PROGRAM OBJECT

    gl.useProgram(shaderProgramObject);

    var modelViewMatrix = mat4.create();
    var modelViewProjectionMatrix = mat4.create();
    var rotationMatrix = mat4.create();
    var scaleMatrix = mat4.create();

    // DRAW Cube
    mat4.identity(modelViewMatrix);
    mat4.identity(modelViewProjectionMatrix);
    mat4.identity(rotationMatrix);

    mat4.translate(modelViewMatrix, modelViewMatrix, [0.0, 0.0, -4.0]);
    mat4.rotateX(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateY(rotationMatrix, rotationMatrix, degToRad(angleCube));
    mat4.rotateZ(rotationMatrix, rotationMatrix, degToRad(angleCube));

    mat4.scale(scaleMatrix, scaleMatrix, [0.8, 0.8, 0.8]);

    mat4.multiply(rotationMatrix, rotationMatrix, scaleMatrix);
    mat4.multiply(modelViewMatrix, modelViewMatrix, rotationMatrix);

    mat4.multiply(modelViewProjectionMatrix, perspectiveProjectionMatrix, modelViewMatrix);
    gl.uniformMatrix4fv(mvpMatrixUniform, false, modelViewProjectionMatrix);

    gl.activeTexture(gl.TEXTURE0); //
    gl.bindTexture(gl.TEXTURE_2D, fbo_texture);
    gl.uniform1i(textureSamplerUniform, 0); //

    gl.bindVertexArray(vao_Cube);
    gl.drawArrays(gl.TRIANGLE_FAN, 0, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 4, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 8, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 12, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 16, 4);
    gl.drawArrays(gl.TRIANGLE_FAN, 20, 4);
    gl.bindVertexArray(null);

    gl.bindTexture(gl.TEXTURE_2D, null);

    gl.useProgram(null);

    // Update
    update();
}

function display_sphere(textureWidth, textureHeight) {

    /* CODE */
    gl.bindFramebuffer(gl.FRAMEBUFFER, fbo);

    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    resize_Sphere(textureWidth, textureHeight);

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // USE SHADER PROGRAM OBJECT
    gl.useProgram(shaderProgramObject_Sphere);

    var viewMatrix = mat4.create();
    var translateMatrix = mat4.create();

    // DRAW Sphere
    mat4.translate(translateMatrix, translateMatrix, [0.0, 0.0, -6.0]);

    gl.uniformMatrix4fv(modelMatrixUniform_Sphere, false, translateMatrix);
    gl.uniformMatrix4fv(viewMatrixUniform_Sphere, false, viewMatrix);
    gl.uniformMatrix4fv(projectionMatrixUniform_Sphere, false, perspectiveProjectionMatrix);


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

        gl.uniform1i(lightingEnabledUniform_Sphere, 1);

        gl.uniform3fv(laUniform_Sphere[0], lightAmbiant_One);
        gl.uniform3fv(ldUniform_Sphere[0], lightDiffuse_One);
        gl.uniform3fv(lsUniform_Sphere[0], lightSpecular_One);
        gl.uniform4fv(lighPositionUniform_Sphere[0], lightPosition_One);

        gl.uniform3fv(laUniform_Sphere[1], lightAmbiant_Two);
        gl.uniform3fv(ldUniform_Sphere[1], lightDiffuse_Two);
        gl.uniform3fv(lsUniform_Sphere[1], lightSpecular_Two);
        gl.uniform4fv(lighPositionUniform_Sphere[1], lightPosition_Two);

        gl.uniform3fv(laUniform_Sphere[2], lightAmbiant_Three);
        gl.uniform3fv(ldUniform_Sphere[2], lightDiffuse_Three);
        gl.uniform3fv(lsUniform_Sphere[2], lightSpecular_Three);
        gl.uniform4fv(lighPositionUniform_Sphere[2], lightPosition_Three);

        gl.uniform3fv(kaUniform_Sphere, materialAmbiant);
        gl.uniform3fv(kdUniform_Sphere, meterialDeffuse);
        gl.uniform3fv(ksUniform_Sphere, materialSpecular);
        gl.uniform1f(materialShininessUniform_Sphere, materialShineeness);
    }
    else {
        gl.uniform1i(lightingEnabledUniform_Sphere, 0);
    }

    // DRAW sphere
    sphere.draw();

    gl.useProgram(null);    // Double Buffering Emulation

    requestAnimationFrame(display, canvas);

    // Update
    update_sphere();
    gl.bindFramebuffer(gl.FRAMEBUFFER, null);
}

function update_sphere() {
    /* CODE */
    lightAngle = lightAngle + 1.0;
    if (lightAngle > 360.0)
        lightAngle = 0.0;
}

function update() {
    /* CODE */
    angleCube = angleCube + 1.00;
    if (angleCube >= 360.0)
        angleCube = angleCube - 360.0;
}

function uninitialize() {
    /* CODE */
    uninitialize_sphere();


    if (vbo_Cube_Texcoord) {
        gl.deleteBuffer(vbo_Cube_Texcoord);
        vbo_Cube_Texcoord = null;
    }

    if (vbo_Cube_Position) {
        gl.deleteBuffer(vbo_Cube_Position);
        vbo_Cube_Position = null;
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

function uninitialize_sphere() {
    /* CODE */

    if (sphere) {
        sphere.deallocate();
        sphere = null;
    }

    if (shaderProgramObject_Sphere) {

        gl.useProgram(shaderProgramObject_Sphere);
        var shaderObjects = gl.getAttachedShaders(shaderProgramObject_Sphere);
        for (let i = 0; i < shaderObjects.length; i++) {
            gl.detachShader(shaderProgramObject_Sphere, shaderObjects[i]);
            gl.deleteShader(shaderObjects[i]);
            shaderObjects[i] = 0;
        }
        delete (shaderObjects);
        gl.useProgram(null);
        gl.deleteProgram(shaderProgramObject_Sphere);
        shaderProgramObject_Sphere = 0;
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

        case 76:
            if (bLight == false) {
                bLight = true;

            }
            else {
                bLight = false;
            }
            break;

        case 70:
            toggleFullscreen();
            break;
    }
}

function mouseDown() {
    /* CODE */
}
