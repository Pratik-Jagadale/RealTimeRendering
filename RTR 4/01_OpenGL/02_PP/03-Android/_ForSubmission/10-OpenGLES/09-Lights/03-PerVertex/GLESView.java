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
    private int shaderProgramObject;

    private int vao_sphere[] = new int[1];
    private int vbo_sphere_position[] = new int[1];
    private int vbo_sphere_normal[] = new int[1];
    private int vbo_sphere_element[] = new int[1];

    private int modelMatrixUniform;
    private int viewMatrixUniform;
    private int ProjectionMatrixUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private int laUniform;
    private int ldUniform; // light direction
    private int lsUniform; // light Spec
    private int lightPositionUniform;

    private int kaUniform; // material amb
    private int kdUniform; // material diffuse
    private int ksUniform; // mat specular
    private int materialShininessUniform;

    private int lightingEnabledUniform;
    private int lightingEnabledUniform_PV;

    private float lightAmbiant[] = new float[] { 0.1f, 0.1f, 0.1f, 1.0f };
    private float lightDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightSpecular[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightPosition[] = new float[] { 100.0f, 100.0f, 100.0f, 1.0f };

    private float materialAmbiant[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    private float materialDiffuse[] = new float[] { 0.5f, 0.2f, 0.7f, 1.0f };
    private float materialSpecular[] = new float[] { 0.7f, 0.7f, 0.7f, 1.0f };
    private float materialShineeness = 120.0f;

    private Boolean bLight = false;

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

        // Vertex Shader
        final String vertexShaderSourceCode = String.format(
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
                        "uniform int u_lightingEnabled;" +
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

        int vertexShaderObject = GLES32.glCreateShader(GLES32.GL_VERTEX_SHADER);

        GLES32.glShaderSource(vertexShaderObject, vertexShaderSourceCode);

        GLES32.glCompileShader(vertexShaderObject);

        int status[] = new int[1];

        int infoLogLength[] = new int[1];

        String log = null;

        GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(vertexShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(vertexShaderObject);
                System.out.println("PRJ: VERTEX SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Fragment Shader
        final String fragmentShaderSourceCode = String.format(
                "#version 320 es" +
                        "\n" +
                        "precision highp float;" +
                        "in vec3 phong_ads_out;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "FragColor = vec4(phong_ads_out, 1.0);" +
                        "}");

        int fragmentShaderObject = GLES32.glCreateShader(GLES32.GL_FRAGMENT_SHADER);

        GLES32.glShaderSource(fragmentShaderObject, fragmentShaderSourceCode);

        GLES32.glCompileShader(fragmentShaderObject);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_COMPILE_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetShaderiv(fragmentShaderObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(fragmentShaderObject);
                System.out.println("PRJ: FRAGMENT SHADER COMPILATION LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // shader program
        shaderProgramObject = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject, vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject, fragmentShaderObject);

        // Pre-Link
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATTRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATTRIBUTE_NORMAL, "a_normal");

        GLES32.glLinkProgram(shaderProgramObject);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_LINK_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject);
                System.out.println("PRJ: SHADER PROGRAM LINK LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Post Link
        ProjectionMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_projectionMatrix");
        viewMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_viewMatrix");
        modelMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_modelMatrix");

        laUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_la");
        ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld");
        lsUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls");
        lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition");

        kaUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ks");

        materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

        lightingEnabledUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");
        // lightingEnabledUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject,
        // "u_lightEnabled_PV");

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
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        GLES32.glUseProgram(shaderProgramObject);

        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];

        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.setIdentityM(modelMatrix, 0);

        Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -2.0f);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        if (bLight == true) {

            // GLES32.glUniform1i(lightingEnabledUniform_PV, 1);
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lightPositionUniform, 1, lightPosition, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShineeness);

        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
            // GLES32.glUniform1i(lightingEnabledUniform_PV, 0);

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

        if (shaderProgramObject > 0) // means not equal to
        {
            GLES32.glUseProgram(shaderProgramObject);

            int retVal[] = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
            if (retVal[0] > 0) {
                int numAttachedShaders = retVal[0];

                int shaderObjects[] = new int[numAttachedShaders];
                GLES32.glGetAttachedShaders(shaderProgramObject, numAttachedShaders, retVal, 0, shaderObjects, 0);

                for (int i = 0; i < numAttachedShaders; i++) {
                    GLES32.glDetachShader(shaderProgramObject, shaderObjects[i]);
                    GLES32.glDeleteShader(shaderObjects[i]);
                    shaderObjects[i] = 0;
                }
            }

            GLES32.glDeleteProgram(shaderProgramObject);
            shaderProgramObject = 0;

            GLES32.glUseProgram(0);
        }

    }

}
