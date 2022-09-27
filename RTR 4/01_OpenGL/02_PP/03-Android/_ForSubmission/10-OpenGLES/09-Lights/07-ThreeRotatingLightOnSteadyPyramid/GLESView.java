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

    private int laUniform[] = new int[3]; // light Ambiant
    private int ldUniform[] = new int[3]; // light Diffuse
    private int lsUniform[] = new int[3]; // light Spec
    private int lightPositionUniform[] = new int[3]; // light Position

    private int kaUniform; // material amb
    private int kdUniform; // material diffuse
    private int ksUniform; // mat specular
    private int materialShininessUniform;

    private int lightingEnabledUniform;
    private int lightingEnabledUniform_PV;

    private float lightAmbiant[][] = new float[][] { { 0.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
    };
    private float lightDiffuse[][] = new float[][] { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f } };
    private float lightSpecular[][] = new float[][] { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f } };
    private float lightPositions[][] = new float[][] { { -2.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f } };

    private float materialAmbiant[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    private float materialDiffuse[] = new float[] { 0.5f, 0.2f, 0.7f, 1.0f };
    private float materialSpecular[] = new float[] { 0.7f, 0.7f, 0.7f, 1.0f };
    private float materialShineeness = 120.0f;

    private Boolean bLight = false;

    private int numVertices;
    private int numElements;

    private float lightAngleOne = 0.0f;
    private float lightAngleTwo = 150.0f;
    private float lightAngleZero = 300.0f;

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
        update();

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
                        "in vec3 a_normal;" +
                        "uniform vec4 u_lightPosition[3];" +
                        "uniform mat4 u_modelMatrix;" +
                        "uniform mat4 u_viewMatrix;" +
                        "uniform mat4 u_projectionMatrix;" +
                        "uniform int u_lightingEnabled;" +
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
                        "uniform int u_lightEnabled_PV;" +
                        "out vec4 FragColor;" +
                        "vec3 phong_ads_light;" +
                        "void main(void)" +
                        "{" +
                        "phong_ads_light = vec3(0.0,0.0,0.0);" +
                        "if(u_lightEnabled_PV == 1)" +
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

        laUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "u_la[0]");
        ldUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld[0]");
        lsUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls[0]");
        lightPositionUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition[0]");

        laUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_la[1]");
        ldUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld[1]");
        lsUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls[1]");
        lightPositionUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition[1]");

        laUniform[2] = GLES32.glGetUniformLocation(shaderProgramObject, "u_la[2]");
        ldUniform[2] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld[2]");
        lsUniform[2] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls[2]");
        lightPositionUniform[2] = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition[2]");

        kaUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ks");
        materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

        lightingEnabledUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");
        lightingEnabledUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject,
                "u_lightEnabled_PV");

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

    void update() {
        lightAngleZero = lightAngleZero + 1.0f;
        if (lightAngleZero > 360.0f)
            lightAngleZero = lightAngleZero - 360.0f;

        lightAngleOne = lightAngleOne + 1.0f;
        if (lightAngleOne > 360.0f)
            lightAngleOne = lightAngleOne - 360.0f;

        lightAngleTwo = lightAngleTwo + 1.0f;
        if (lightAngleTwo > 360.0f)
            lightAngleTwo = lightAngleTwo - 360.0f;
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

        float angle = (lightAngleZero * (float) (Math.PI / 180.0f));
        float x = 5.0f * (float) Math.cos(angle);
        float y = 5.0f * (float) Math.sin(angle);
        lightPositions[0][1] = x;
        lightPositions[0][2] = y;

        // Set Light One Position rotating One Light - Y Rotation
        angle = (lightAngleOne * (float) Math.PI) / 180.0f;
        x = 5.0f * (float) Math.cos(angle);
        y = 5.0f * (float) Math.sin(angle);
        lightPositions[1][0] = x;
        lightPositions[1][2] = y;

        // Set Light Two Position rotating Two Light Z Rotation
        angle = (lightAngleTwo * (float) Math.PI) / 180.0f;
        x = 5.0f * (float) Math.cos(angle);
        y = 5.0f * (float) Math.sin(angle);
        lightPositions[2][0] = x;
        lightPositions[2][1] = y;

        if (bLight == true) {

            GLES32.glUniform1i(lightingEnabledUniform_PV, 1);
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            for (int i = 0; i < 3; i++) {
                GLES32.glUniform3fv(laUniform[i], 1, lightAmbiant[i], 0); // we can use glUniform3f() ,so we can
                                                                          // directly
                                                                          // pass the values to uniform
                GLES32.glUniform3fv(ldUniform[i], 1, lightDiffuse[i], 0);
                GLES32.glUniform3fv(lsUniform[i], 1, lightSpecular[i], 0);
                GLES32.glUniform4fv(lightPositionUniform[i], 1, lightPositions[i], 0);
            }

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShineeness);

        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
            GLES32.glUniform1i(lightingEnabledUniform_PV, 0);

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
