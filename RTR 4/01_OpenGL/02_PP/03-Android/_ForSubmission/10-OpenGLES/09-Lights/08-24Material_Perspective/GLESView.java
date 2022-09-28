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

    private int laUniform; // light Ambiant
    private int ldUniform; // light Diffuse
    private int lsUniform; // light Spec
    private int lighPositionUniform; // light Position

    private int kaUniform; // material amb
    private int kdUniform; // mat diff
    private int ksUniform; // mat specular
    private int materialShininessUniform;

    private int lightingEnabledUniform;

    private int numVertices;
    private int numElements;

    boolean bLight = false;

    private float lightAmbiant[] = new float[] { 0.0f, 0.0f, 0.0f, 1.0f };
    private float lightDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightSpecular[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightPositions[] = new float[] { 0.0f, 0.0f, 0.0f, 1.0f };

    private float materialAmbiant[] = new float[] { 0.0f, 0.0f, 0.0f, 1.0f };
    private float materialDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float materialSpecular[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float materialShinniness = 120.0f;

    private float angleForRotation = 0.0f;

    private int keyPressed = 0;

    private int currentWindowWidth = 0;
    private int currentWindowHeight = 0;

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
        if (bLight == false) {
            bLight = true;
        } else {
            bLight = false;
        }
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
        keyPressed++;
        if (keyPressed > 3)
            keyPressed = 0;
        angleForRotation = 0.0f; // reset
        lightPositions[0] = 0;
        lightPositions[1] = 0;
        lightPositions[2] = 0;
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
                        "uniform vec4 u_lightPosition;" +
                        "uniform mat4 u_modelMatrix;" +
                        "uniform mat4 u_viewMatrix;" +
                        "uniform mat4 u_projectionMatrix;" +
                        "uniform mediump int u_lightingEnabled;" +
                        "out vec3 transformedNormals;" +
                        "out vec3 lightDirection;" +
                        "out vec3 viewerVector;" +
                        "void main(void)" +
                        "{" +
                        "if(u_lightingEnabled == 1)" +
                        "{" +
                        "vec4 eyeCoordinates = u_viewMatrix * u_modelMatrix * a_position;" +
                        "mat3 normalMatrix = mat3(u_viewMatrix * u_modelMatrix);" +
                        "transformedNormals = normalize(normalMatrix * a_normal);" +
                        "viewerVector = normalize(-eyeCoordinates.xyz);" +
                        "lightDirection = normalize(vec3(u_lightPosition) - eyeCoordinates.xyz);" + // Swizaling
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
                        "in vec3 lightDirection;" +
                        "in vec3 viewerVector;" +
                        "uniform vec3 u_la;" +
                        "uniform vec3 u_ld;" +
                        "uniform vec3 u_ls;" +
                        "uniform vec4 u_lightPosition;" +
                        "uniform vec3 u_ka;" +
                        "uniform vec3 u_ks;" +
                        "uniform vec3 u_kd;" +
                        "uniform float u_materialShininnes;" +
                        "uniform int u_lightingEnabled;" +
                        "out vec4 FragColor;" +
                        "vec3 phong_ads_light;" +
                        "void main(void)" +
                        "{" +
                        "phong_ads_light = vec3(0.0,0.0,0.0);" +
                        "if(u_lightingEnabled == 1)" +
                        "{" +
                        "vec3 ambiant;" +
                        "vec3 diffuse;" +
                        "vec3 reflectionVector;" +
                        "vec3 specular;" +
                        "ambiant = u_la * u_ka;" +
                        "diffuse = u_ld * u_kd * max(dot(lightDirection ,transformedNormals),0.0);" +
                        "reflectionVector = reflect(-lightDirection,transformedNormals);" +
                        "specular = u_ls * u_ks * pow(max(dot(reflectionVector, viewerVector), 0.0), u_materialShininnes);"
                        +
                        "phong_ads_light = phong_ads_light + ambiant + diffuse +  specular;" +
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

        laUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_la");
        ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld");
        lsUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls");
        lighPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition");

        kaUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ks");
        materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

        lightingEnabledUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

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

        currentWindowWidth = width;
        currentWindowHeight = height;

    }

    private void update() {
        if (keyPressed == 1 || keyPressed == 2 || keyPressed == 3) {
            angleForRotation = angleForRotation + 1.0f;
            if (angleForRotation > 360.0f)
                angleForRotation = angleForRotation - 360.0f;

            // Set Light Zero Position - rotating light
            float angle = (angleForRotation * (float) Math.PI) / 180.0f;
            float x = 10.0f * (float) (Math.cos(angle));
            float y = 10.0f * (float) Math.sin(angle);

            if (keyPressed == 1) {
                lightPositions[1] = x;
                lightPositions[2] = y;
            }
            if (keyPressed == 2) {
                lightPositions[0] = x;
                lightPositions[2] = y;
            }
            if (keyPressed == 3) {
                lightPositions[0] = x;
                lightPositions[1] = y;
            }

        }
    }

    private void display() {
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        GLES32.glUseProgram(shaderProgramObject);

        drawSphere();

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

    private void drawSphere() {

        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];

        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.setIdentityM(modelMatrix, 0);

        Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -1.5f);

        int horDistance = currentWindowWidth / 6;
        int verDistance = currentWindowHeight / 4;

        GLES32.glViewport(0, verDistance * 3, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0215f; // r
        materialAmbiant[1] = 0.1745f; // g
        materialAmbiant[2] = 0.0215f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.07568f; // r
        materialDiffuse[1] = 0.61424f; // g
        materialDiffuse[2] = 0.07568f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.633f; // r
        materialSpecular[1] = 0.727811f; // g
        materialSpecular[2] = 0.633f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.6f * 128f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 2
        // ***** 2nd sphere on 1st column, jade *****
        GLES32.glViewport(horDistance, verDistance * 3, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.135f; // r
        materialAmbiant[1] = 0.2225f; // g
        materialAmbiant[2] = 0.1575f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.54f; // r
        materialDiffuse[1] = 0.89f; // g
        materialDiffuse[2] = 0.63f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.316228f; // r
        materialSpecular[1] = 0.316228f; // g
        materialSpecular[2] = 0.316228f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.1f * 128f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 3
        // ***** 3rd sphere on 1st column, obsidian *****
        GLES32.glViewport(horDistance * 2, verDistance * 3, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.05375f; // r
        materialAmbiant[1] = 0.05f; // g
        materialAmbiant[2] = 0.06625f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.18275f; // r
        materialDiffuse[1] = 0.17f; // g
        materialDiffuse[2] = 0.22525f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.332741f; // r
        materialSpecular[1] = 0.328634f; // g
        materialSpecular[2] = 0.346435f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.3f * 128f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 4
        // ***** 4th sphere on 1st column, pearl *****
        GLES32.glViewport(horDistance * 3, verDistance * 3, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.25f; // r
        materialAmbiant[1] = 0.20725f; // g
        materialAmbiant[2] = 0.20725f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 1.0f; // r
        materialDiffuse[1] = 0.829f; // g
        materialDiffuse[2] = 0.829f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.296648f; // r
        materialSpecular[1] = 0.296648f; // g
        materialSpecular[2] = 0.296648f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.088f * 128f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 5
        // ***** 5th sphere on 1st column, ruby *****
        GLES32.glViewport(horDistance * 4, verDistance * 3, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.1745f; // r
        materialAmbiant[1] = 0.01175f; // g
        materialAmbiant[2] = 0.01175f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.61424f; // r
        materialDiffuse[1] = 0.04136f; // g
        materialDiffuse[2] = 0.04136f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.727811f; // r
        materialSpecular[1] = 0.626959f; // g
        materialSpecular[2] = 0.626959f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.6f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 6
        // ***** 6th sphere on 1st column, turquoise *****
        GLES32.glViewport(horDistance * 5, verDistance * 3, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.1f; // r
        materialAmbiant[1] = 0.18725f; // g
        materialAmbiant[2] = 0.1745f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.396f; // r
        materialDiffuse[1] = 0.74151f; // g
        materialDiffuse[2] = 0.69102f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.297254f; // r
        materialSpecular[1] = 0.30829f; // g
        materialSpecular[2] = 0.306678f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.1f * 128f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);
        ////////////////////////////////////////////////////////////////
        // ***** 1st sphere on 2nd column, brass *****
        GLES32.glViewport(0, verDistance * 2, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        materialAmbiant[0] = 0.329412f; // r
        materialAmbiant[1] = 0.223529f; // g
        materialAmbiant[2] = 0.027451f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.780392f; // r
        materialDiffuse[1] = 0.568627f; // g
        materialDiffuse[2] = 0.113725f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.992157f; // r
        materialSpecular[1] = 0.941176f; // g
        materialSpecular[2] = 0.807843f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.21794872f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 2
        // ***** 2nd sphere on 2nd column, bronze *****
        GLES32.glViewport(horDistance, verDistance * 2, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.2125f; // r
        materialAmbiant[1] = 0.1275f; // g
        materialAmbiant[2] = 0.054f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.714f; // r
        materialDiffuse[1] = 0.4284f; // g
        materialDiffuse[2] = 0.18144f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.393548f; // r
        materialSpecular[1] = 0.271906f; // g
        materialSpecular[2] = 0.166721f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.2f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 3
        // ***** 3rd sphere on 2nd column, chrome *****
        GLES32.glViewport(horDistance * 2, verDistance * 2, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.25f; // r
        materialAmbiant[1] = 0.25f; // g
        materialAmbiant[2] = 0.25f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.4f; // r
        materialDiffuse[1] = 0.4f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.774597f; // r
        materialSpecular[1] = 0.774597f; // g
        materialSpecular[2] = 0.774597f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.6f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 5
        // ***** 4th sphere on 2nd column, copper *****
        GLES32.glViewport(horDistance * 3, verDistance * 2, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.19125f; // r
        materialAmbiant[1] = 0.0735f; // g
        materialAmbiant[2] = 0.0225f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.7038f; // r
        materialDiffuse[1] = 0.27048f; // g
        materialDiffuse[2] = 0.0828f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.256777f; // r
        materialSpecular[1] = 0.137622f; // g
        materialSpecular[2] = 0.086014f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.1f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 5
        // ***** 5th sphere on 2nd column, gold *****
        GLES32.glViewport(horDistance * 4, verDistance * 2, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.24725f; // r
        materialAmbiant[1] = 0.1995f; // g
        materialAmbiant[2] = 0.0745f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.75164f; // r
        materialDiffuse[1] = 0.60648f; // g
        materialDiffuse[2] = 0.22648f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.628281f; // r
        materialSpecular[1] = 0.555802f; // g
        materialSpecular[2] = 0.366065f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.4f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 6
        // ***** 6th sphere on 2nd column, silver *****
        GLES32.glViewport(horDistance * 5, verDistance * 2, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.19225f; // r
        materialAmbiant[1] = 0.19225f; // g
        materialAmbiant[2] = 0.19225f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.50754f; // r
        materialDiffuse[1] = 0.50754f; // g
        materialDiffuse[2] = 0.50754f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.508273f; // r
        materialSpecular[1] = 0.508273f; // g
        materialSpecular[2] = 0.508273f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.4f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        /////////////////////////////////////////////// 3 /////////////////////
        // ***** 1st sphere on 3rd column, black *****
        GLES32.glViewport(0, verDistance, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.0f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.01f; // r
        materialDiffuse[1] = 0.01f; // g
        materialDiffuse[2] = 0.01f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.50f; // r
        materialSpecular[1] = 0.50f; // g
        materialSpecular[2] = 0.50f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.25f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 2
        // ***** 2nd sphere on 3rd column, cyan *****
        GLES32.glViewport(horDistance, verDistance, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        /// ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.1f; // g
        materialAmbiant[2] = 0.06f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.0f; // r
        materialDiffuse[1] = 0.50980392f; // g
        materialDiffuse[2] = 0.50980392f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.50185078f; // r
        materialSpecular[1] = 0.50185078f; // g
        materialSpecular[2] = 0.50185078f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.25f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 3
        // ***** 3rd sphere on 2nd column, green *****
        GLES32.glViewport(horDistance * 2, verDistance, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.0f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.1f; // r
        materialDiffuse[1] = 0.35f; // g
        materialDiffuse[2] = 0.1f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.45f; // r
        materialSpecular[1] = 0.55f; // g
        materialSpecular[2] = 0.45f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.25f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 4
        // ***** 4th sphere on 3rd column, red *****
        GLES32.glViewport(horDistance * 3, verDistance, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.0f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.0f; // g
        materialDiffuse[2] = 0.0f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.6f; // g
        materialSpecular[2] = 0.6f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.25f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 5
        // ***** 5th sphere on 3rd column, white *****
        GLES32.glViewport(horDistance * 4, verDistance, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.0f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.55f; // r
        materialDiffuse[1] = 0.55f; // g
        materialDiffuse[2] = 0.55f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.70f; // r
        materialSpecular[1] = 0.70f; // g
        materialSpecular[2] = 0.70f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.25f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 5
        // ***** 6th sphere on 3rd column, yellow plastic *****
        GLES32.glViewport(horDistance * 5, verDistance, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.0f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.0f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.60f; // r
        materialSpecular[1] = 0.60f; // g
        materialSpecular[2] = 0.50f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.25f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // unbind vao
        GLES32.glBindVertexArray(0);
        ////////////////////////////////////////////////////////////////
        // ***** 1st sphere on 4th column, black *****
        GLES32.glViewport(0, 0, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        materialAmbiant[0] = 0.02f; // r
        materialAmbiant[1] = 0.02f; // g
        materialAmbiant[2] = 0.02f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.01f; // r
        materialDiffuse[1] = 0.01f; // g
        materialDiffuse[2] = 0.01f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.4f; // r
        materialSpecular[1] = 0.4f; // g
        materialSpecular[2] = 0.4f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.078125f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 2
        // ***** 2nd sphere on 4th column, cyan *****
        GLES32.glViewport(horDistance, 0, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.05f; // g
        materialAmbiant[2] = 0.05f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.4f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.5f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.04f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.7f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.078125f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 3
        // ***** 3rd sphere on 4th column, green *****
        GLES32.glViewport(horDistance * 2, 0, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.0f; // r
        materialAmbiant[1] = 0.05f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.4f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.04f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.04f;// b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.078125f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 4
        // ***** 4th sphere on 4th column, red *****
        GLES32.glViewport(horDistance * 3, 0, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.05f; // r
        materialAmbiant[1] = 0.0f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.4f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.04f; // g
        materialSpecular[2] = 0.04f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.078125f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 5
        // ***** 5th sphere on 4th column, white *****
        GLES32.glViewport(horDistance * 4, 0, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.05f; // r
        materialAmbiant[1] = 0.05f; // g
        materialAmbiant[2] = 0.05f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.5f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.7f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.078125f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        // 6
        // ***** 6th sphere on 4th column, yellow rubber *****
        GLES32.glViewport(horDistance * 5, 0, currentWindowWidth / 5, currentWindowHeight / 5);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        // ambient material
        materialAmbiant[0] = 0.05f; // r
        materialAmbiant[1] = 0.05f; // g
        materialAmbiant[2] = 0.0f; // b
        materialAmbiant[3] = 1.0f; // a

        // diffuse material
        materialDiffuse[0] = 0.5f; // r
        materialDiffuse[1] = 0.5f; // g
        materialDiffuse[2] = 0.4f; // b
        materialDiffuse[3] = 1.0f; // a

        // specular material
        materialSpecular[0] = 0.7f; // r
        materialSpecular[1] = 0.7f; // g
        materialSpecular[2] = 0.04f; // b
        materialSpecular[3] = 1.0f; // a

        // shininess
        materialShinniness = 0.078125f * 128.0f;

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            GLES32.glUniform3fv(laUniform, 1, lightAmbiant, 0); // we can use glUniform3f() ,so we can directly pass the
                                                                // values
            // to
            // uniform
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(lsUniform, 1, lightSpecular, 0);
            GLES32.glUniform4fv(lighPositionUniform, 1, lightPositions, 0);

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShinniness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

    }

}
