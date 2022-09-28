package com.pratik.window;

import android.opengl.GLSurfaceView;
import javax.microedition.khronos.opengles.GL10;

import javax.microedition.khronos.egl.EGLConfig;
import android.opengl.GLES32;

import android.content.Context;

import android.graphics.Color;

import android.view.Gravity;

// Event Related Packages
import android.view.GestureDetector;
import android.view.GestureDetector.OnDoubleTapListener;
import android.view.GestureDetector.OnGestureListener;
import android.view.MotionEvent;

import java.nio.ByteBuffer; // nio = native I/O or non blocking I/O (asyncronous)
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

// Matric Package
import android.opengl.Matrix;

// Sphere Relate
import java.nio.ShortBuffer;
import java.sql.PreparedStatement;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer {

    // Properties
    private GestureDetector gestureDetector;
    private Context context;
    private int shaderProgramObject_PV;
    private int shaderProgramObject_PF;

    private int vao_sphere[] = new int[1];
    private int vbo_sphere_position[] = new int[1];
    private int vbo_sphere_normal[] = new int[1];
    private int vbo_sphere_element[] = new int[1];

    private int modelMatrixUniform;
    private int viewMatrixUniform;
    private int ProjectionMatrixUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private int laUniform_PV;
    private int ldUniform_PV; // light direction
    private int lsUniform_PV; // light Spec
    private int lightPositionUniform_PV;

    private int kaUniform_PV; // material amb
    private int kdUniform_PV; // material diffuse
    private int ksUniform_PV; // mat specular
    private int materialShininessUniform_PV;

    private int lightingEnabledUniform_PV;
    private int lightingEnabledUniform_PV_PV;

    private int modelMatrixUniform_PF;
    private int viewMatrixUniform_PF;
    private int ProjectionMatrixUniform_PF;

    private int laUnifor_PF;
    private int ldUniform_PF; // light direction
    private int lsUniform_PF; // light Spec
    private int lightPositionUniform_PF;

    private int kaUniform_PF; // material amb
    private int kdUniform_PF; // material diffuse
    private int ksUniform_PF; // mat specular
    private int materialShininessUniform_PF;

    private int lightingEnabledUniform_PF;
    private int lightingEnabledUniform_PF_F;

    private float lightAmbiant[] = new float[] { 0.1f, 0.1f, 0.1f, 1.0f };
    private float lightDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightSpecular[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightPosition[] = new float[] { 100.0f, 100.0f, 100.0f, 1.0f };

    private float materialAmbiant[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    private float materialDiffuse[] = new float[] { 0.5f, 0.2f, 0.7f, 1.0f };
    private float materialSpecular[] = new float[] { 0.7f, 0.7f, 0.7f, 1.0f };
    private float materialShineeness = 120.0f;

    private Boolean bLight = false;
    private Boolean PerFragOperations = false;

    private int numVertices;
    private int numElements;

    // Beahviours
    GLESView(Context _context) {
        super(_context);

        context = _context;

        setEGLContextClientVersion(3);

        setRenderer(this);

        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);

        // Gesture Related Code
        gestureDetector = new GestureDetector(context, this, null, false);
        gestureDetector.setOnDoubleTapListener(this);
    }

    // Three Methods of GLSurfaceView.Renderer
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        String glesVersion = gl.glGetString(GL10.GL_VERSION);
        System.out.println("PRJ: " + glesVersion);

        String renderer = gl.glGetString(GL10.GL_RENDERER);
        System.out.println("PRJ: " + renderer);

        String glslVersion = gl.glGetString(GLES32.GL_SHADING_LANGUAGE_VERSION);
        System.out.println("PRJ: " + glslVersion);

        initialize();
    }

    @Override
    public void onSurfaceChanged(GL10 unUsed, int width, int height) {
        resize(width, height);
    }

    @Override
    public void onDrawFrame(GL10 unUsed) {
        // onDrawFrame should be consider as GameLoop
        display();
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        // Code
        if (!gestureDetector.onTouchEvent(e))
            super.onTouchEvent(e);

        return true;
    }

    // Three methods from OnDoubleTapListaner Interface
    @Override
    public boolean onDoubleTap(MotionEvent e) {
        if (PerFragOperations == false)
            PerFragOperations = true;
        else
            PerFragOperations = false;
        return true;
    }

    @Override
    public boolean onDoubleTapEvent(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onSingleTapConfirmed(MotionEvent e) {

        if (bLight == true) {
            bLight = false;
        } else {
            bLight = true;
        }
        return true;
    }

    // Six Methods of onGestureListaner
    @Override
    public boolean onDown(MotionEvent e) {
        return true;
    }

    @Override
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX, float velocityY) {
        return true;
    }

    @Override
    public void onLongPress(MotionEvent e) {
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        uninitialize();
        System.exit(0);
        return true;
    }

    @Override
    public void onShowPress(MotionEvent e) {
    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return true;
    }

    // Custom Private Function
    private void initialize() {

        // ============================= PerVertex
        // ================================================================
        // Vertex Shader
        final String vertexShaderSourceCode_PV = String.format(
                "#version 320 es" +
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
                        "}");

        int vertexShaderObject_PV = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        GLES32.glShaderSource(vertexShaderObject_PV, vertexShaderSourceCode_PV);

        GLES32.glCompileShader(vertexShaderObject_PV);

        int status[] = new int[1];

        int infoLogLength[] = new int[1];

        String log = null;

        GLES32.glGetShaderiv(vertexShaderObject_PV, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(vertexShaderObject_PV, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(vertexShaderObject_PV);
                System.out.println("PRJ: VERTEX SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader
        final String fragmentShaderSourceCode_PV = String.format(
                "#version 320 es" +
                        "\n" +
                        "precision highp float;" +
                        "in vec3 phong_ads_out;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "FragColor = vec4(phong_ads_out, 1.0);" +
                        "}");

        int fragmentShaderObject_PV = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        GLES32.glShaderSource(fragmentShaderObject_PV, fragmentShaderSourceCode_PV);

        GLES32.glCompileShader(fragmentShaderObject_PV);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetShaderiv(fragmentShaderObject_PV, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(fragmentShaderObject_PV, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(fragmentShaderObject_PV);
                System.out.println("PRJ: FRAGMENT SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // shader program
        shaderProgramObject_PV = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_PV, vertexShaderObject_PV);
        GLES32.glAttachShader(shaderProgramObject_PV, fragmentShaderObject_PV);

        // Pre-Link
        GLES32.glBindAttribLocation(shaderProgramObject_PV, MyGLESMacros.PRJ_ATTRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject_PV, MyGLESMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");

        GLES32.glLinkProgram(shaderProgramObject_PV);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetProgramiv(shaderProgramObject_PV, GLES32.GL_LINK_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject_PV, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject_PV);
                System.out.println("PRJ: SHADER PROGRAM LINK LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Post Link
        ProjectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_projectionMatrix");
        viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_viewMatrix");
        modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_modelMatrix");

        laUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_la");
        ldUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_ld");
        lsUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_ls");
        lightPositionUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_lightPosition");

        kaUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_ka");
        kdUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_kd");
        ksUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_ks");

        materialShininessUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_materialShininnes");

        lightingEnabledUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject_PV, "u_lightingEnabled");
        // ==================================================================================================================;
        // =============================================PerFragment====================================

        // Vertex Shader
        final String vertexShaderSourceCode_PF = String.format(
                "#version 320 es" +
                        "\n" +
                        "in vec4 a_position;" +
                        "in vec3 a_normal;" +
                        "uniform mat4 u_modelMatrix;" +
                        "uniform mat4 u_viewMatrix;" +
                        "uniform mat4 u_projectionMatrix;" +
                        "uniform vec4 u_lightPosition;" +
                        "uniform int u_lightingEnabled;" +
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
                        "}");

        int vertexShaderObject_PF = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        GLES32.glShaderSource(vertexShaderObject_PF, vertexShaderSourceCode_PF);

        GLES32.glCompileShader(vertexShaderObject_PF);

        log = null;

        GLES32.glGetShaderiv(vertexShaderObject_PF, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(vertexShaderObject_PF, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(vertexShaderObject_PF);
                System.out.println("PRJ: VERTEX SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader
        final String fragmentShaderSourceCode_PF = String.format(
                "#version 320 es" +
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
                        "uniform int u_lightEnabled_PV;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "vec3 phong_ads_color;" +
                        "if(u_lightEnabled_PV == 1)" +
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
                        "}");

        int fragmentShaderObject_PF = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        GLES32.glShaderSource(fragmentShaderObject_PF, fragmentShaderSourceCode_PF);

        GLES32.glCompileShader(fragmentShaderObject_PF);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetShaderiv(fragmentShaderObject_PF, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(fragmentShaderObject_PF, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(fragmentShaderObject_PF);
                System.out.println("PRJ: FRAGMENT SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // shader program
        shaderProgramObject_PF = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_PF, vertexShaderObject_PF);
        GLES32.glAttachShader(shaderProgramObject_PF, fragmentShaderObject_PF);

        // Pre-Link
        GLES32.glBindAttribLocation(shaderProgramObject_PF, MyGLESMacros.PRJ_ATTRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject_PF, MyGLESMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");

        GLES32.glLinkProgram(shaderProgramObject_PF);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetProgramiv(shaderProgramObject_PF, GLES32.GL_LINK_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject_PF, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject_PF);
                System.out.println("PRJ: SHADER PROGRAM LINK LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Post Link
        ProjectionMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_projectionMatrix");
        viewMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_viewMatrix");
        modelMatrixUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_modelMatrix");

        laUnifor_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_la");
        ldUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ld");
        lsUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ls");
        lightPositionUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_lightPosition");

        kaUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ka");
        kdUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_kd");
        ksUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_ks");

        materialShininessUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_materialShininnes");

        lightingEnabledUniform_PF = GLES32.glGetUniformLocation(shaderProgramObject_PF, "u_lightingEnabled");
        lightingEnabledUniform_PF_F = GLES32.glGetUniformLocation(shaderProgramObject_PF,
                "u_lightEnabled_PV");

        // =========================================================================

        // SPHERE DATA
        Sphere sphere = new Sphere();
        float sphere_vertices[] = new float[1146];
        float sphere_normals[] = new float[1146];
        float sphere_textures[] = new float[764];
        short sphere_elements[] = new short[2280];
        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        numVertices = sphere.getNumberOfSphereVertices();
        numElements = sphere.getNumberOfSphereElements();

        // vao
        GLES32.glGenVertexArrays(1, vao_sphere, 0);
        GLES32.glBindVertexArray(vao_sphere[0]);

        // position vbo
        GLES32.glGenBuffers(1, vbo_sphere_position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_position[0]);

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(sphere_vertices.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_vertices);
        verticesBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                sphere_vertices.length * 4,
                verticesBuffer,
                GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATTRIBUTE_POSITION,
                3,
                GLES32.GL_FLOAT,
                false, 0, 0);

        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATTRIBUTE_POSITION);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // normal vbo
        GLES32.glGenBuffers(1, vbo_sphere_normal, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_sphere_normal[0]);

        byteBuffer = ByteBuffer.allocateDirect(sphere_normals.length * 4);
        byteBuffer.order(ByteOrder.nativeOrder());
        verticesBuffer = byteBuffer.asFloatBuffer();
        verticesBuffer.put(sphere_normals);
        verticesBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER,
                sphere_normals.length * 4,
                verticesBuffer,
                GLES32.GL_STATIC_DRAW);

        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATTRIBUTE_NORMAL,
                3,
                GLES32.GL_FLOAT,
                false, 0, 0);

        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATTRIBUTE_NORMAL);

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // element vbo
        GLES32.glGenBuffers(1, vbo_sphere_element, 0);
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);

        byteBuffer = ByteBuffer.allocateDirect(sphere_elements.length * 2);
        byteBuffer.order(ByteOrder.nativeOrder());
        ShortBuffer elementsBuffer = byteBuffer.asShortBuffer();
        elementsBuffer.put(sphere_elements);
        elementsBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ELEMENT_ARRAY_BUFFER,
                sphere_elements.length * 2,
                elementsBuffer,
                GLES32.GL_STATIC_DRAW);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);
        // Depth Related Changes
        GLES32.glClearDepthf(1.0f); // removed because not working in pp
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);
        GLES32.glEnable(GLES32.GL_CULL_FACE);

        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Initialization of Project MNatrix
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);

    }

    private void resize(int width, int height) {
        if (height <= 0)
            height = 1;

        GLES32.glViewport(0, 0, width, height);

        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);

    }

    private void display() {
        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];

        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.setIdentityM(modelMatrix, 0);

        Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -2.0f);

        if (PerFragOperations == false) {
            GLES32.glUseProgram(shaderProgramObject_PV);

            if (bLight == true) {

                // GLES32.glUniform1i(lightingEnabledUniform_PV_PV, 1);
                GLES32.glUniform1i(lightingEnabledUniform_PV, 1);

                GLES32.glUniform3fv(laUniform_PV, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly
                                                                       // pass
                                                                       // the
                                                                       // values to
                // uniform
                GLES32.glUniform3fv(ldUniform_PV, 1, lightDiffuse, 0);
                GLES32.glUniform3fv(lsUniform_PV, 1, lightSpecular, 0);
                GLES32.glUniform4fv(lightPositionUniform_PV, 1, lightPosition, 0);

                GLES32.glUniform3fv(kaUniform_PV, 1, materialAmbiant, 0);
                GLES32.glUniform3fv(kdUniform_PV, 1, materialDiffuse, 0);
                GLES32.glUniform3fv(ksUniform_PV, 1, materialSpecular, 0);
                GLES32.glUniform1f(materialShininessUniform_PV, materialShineeness);

            } else {
                GLES32.glUniform1i(lightingEnabledUniform_PV, 0);
                // GLES32.glUniform1i(lightingEnabledUniform_PV_PV, 0);

            }

            GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
            GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
            GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        } else {
            GLES32.glUseProgram(shaderProgramObject_PF);

            if (bLight == true) {

                GLES32.glUniform1i(lightingEnabledUniform_PF_F, 1);
                GLES32.glUniform1i(lightingEnabledUniform_PF, 1);

                GLES32.glUniform3fv(laUnifor_PF, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly
                                                                      // pass the
                                                                      // values to
                // uniform
                GLES32.glUniform3fv(ldUniform_PF, 1, lightDiffuse, 0);
                GLES32.glUniform3fv(lsUniform_PF, 1, lightSpecular, 0);
                GLES32.glUniform4fv(lightPositionUniform_PF, 1, lightPosition, 0);

                GLES32.glUniform3fv(kaUniform_PF, 1, materialAmbiant, 0);
                GLES32.glUniform3fv(kdUniform_PF, 1, materialDiffuse, 0);
                GLES32.glUniform3fv(ksUniform_PF, 1, materialSpecular, 0);
                GLES32.glUniform1f(materialShininessUniform_PF, materialShineeness);

            } else {
                GLES32.glUniform1i(lightingEnabledUniform_PF, 0);
                GLES32.glUniform1i(lightingEnabledUniform_PF_F, 0);
            }

            GLES32.glUniformMatrix4fv(ProjectionMatrixUniform_PF, 1, false, perspectiveProjectionMatrix, 0);
            GLES32.glUniformMatrix4fv(viewMatrixUniform_PF, 1, false, viewMatrix, 0);
            GLES32.glUniformMatrix4fv(modelMatrixUniform_PF, 1, false, modelMatrix, 0);

        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);
        requestRender();
    }

    private void uninitialize() {
        // code
        // destroy element vbo
        if (vbo_sphere_element[0] != 0) {
            GLES32.glDeleteBuffers(1, vbo_sphere_element, 0);
            vbo_sphere_element[0] = 0;
        }

        if (vbo_sphere_normal[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_sphere_normal, 0);
            vbo_sphere_normal[0] = 0;
        }

        if (vbo_sphere_position[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_sphere_position, 0);
            vbo_sphere_position[0] = 0;
        }

        if (vao_sphere[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_sphere, 0);
            vao_sphere[0] = 0;
        }

        if (shaderProgramObject_PV > 0) // means not equal to
        {
            GLES32.glUseProgram(shaderProgramObject_PV);

            int retVal[] = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject_PV, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
            if (retVal[0] > 0) {
                int numAttachedShaders = retVal[0];

                int shaderObjects[] = new int[numAttachedShaders];
                GLES32.glGetAttachedShaders(shaderProgramObject_PV, numAttachedShaders, retVal, 0, shaderObjects, 0);

                for (int i = 0; i < numAttachedShaders; i++) {
                    GLES32.glDetachShader(shaderProgramObject_PV, shaderObjects[i]);
                    GLES32.glDeleteShader(shaderObjects[i]);
                    shaderObjects[i] = 0;
                }
            }

            GLES32.glDeleteProgram(shaderProgramObject_PV);
            shaderProgramObject_PV = 0;

            GLES32.glUseProgram(0);
        }

    }

}
