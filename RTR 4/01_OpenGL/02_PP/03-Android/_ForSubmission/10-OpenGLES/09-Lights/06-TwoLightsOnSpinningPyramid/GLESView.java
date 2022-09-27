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

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer {

    // Properties
    private GestureDetector gestureDetector;
    private Context context;
    private int shaderProgramObject;

    private int vao_Pyramid[] = new int[1];
    private int vbo_Position_Pyramid[] = new int[1];
    private int vbo_Normal_Pyramid[] = new int[1];

    private int modelMatrixUniform;
    private int viewMatrixUniform;
    private int ProjectionMatrixUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private int laUniform[] = new int[2]; // light Ambiant
    private int ldUniform[] = new int[2]; // light Diffuse
    private int lsUniform[] = new int[2]; // light Spec
    private int lighPositionUniform[] = new int[2]; // light Position

    private int kaUniform; // material amb
    private int kdUniform; // mat diff
    private int ksUniform; // mat specular
    private int materialShininessUniform;

    private int lightingEnabledUniform;

    private Boolean bLight = false;

    private float lightAmbiant[][] = new float[][] { { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 1.0f } };
    private float lightDiffuse[][] = new float[][] { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
    private float lightSpecular[][] = new float[][] { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };
    private float lightPositions[][] = new float[][] { { -2.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } };

    private float materialAmbiant[] = new float[] { 0.0f, 0.0f, 0.0f, 1.0f };
    private float meterialDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float materialSpecular[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float materialShineeness = 50.0f;

    private float anglePyramid = 0.0f;

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
                        "in vec3 phong_ads_light;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
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
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATRIBUTE_NORMAL, "a_normal");

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
        lighPositionUniform[0] = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition[0]");

        laUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_la[1]");
        ldUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld[1]");
        lsUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_ls[1]");
        lighPositionUniform[1] = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition[1]");

        kaUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ka");
        kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
        ksUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ks");
        materialShininessUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_materialShininnes");

        lightingEnabledUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightingEnabled");

        final float PyramidPosition[] = new float[] {
                // front
                0.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,

                // right
                0.0f, 1.0f, 0.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,

                // back
                0.0f, 1.0f, 0.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,

                // left
                0.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f };

        final float PyramidNormal[] = new float[] {
                0.0f, 0.447214f, 0.894427f, // front-top
                0.0f, 0.447214f, 0.894427f, // front-left
                0.0f, 0.447214f, 0.894427f, // front-right

                0.894427f, 0.447214f, 0.0f, // right-top
                0.894427f, 0.447214f, 0.0f, // right-left
                0.894427f, 0.447214f, 0.0f, // right-right

                0.0f, 0.447214f, -0.894427f, // back-top
                0.0f, 0.447214f, -0.894427f, // back-left
                0.0f, 0.447214f, -0.894427f, // back-right

                -0.894427f, 0.447214f, 0.0f, // left-top
                -0.894427f, 0.447214f, 0.0f, // left-left
                -0.894427f, 0.447214f, 0.0f // left-right
        };

        // vao_Pyramid
        GLES32.glGenVertexArrays(1, vao_Pyramid, 0);
        GLES32.glBindVertexArray(vao_Pyramid[0]);

        // Position vbo
        GLES32.glGenBuffers(1, vbo_Position_Pyramid, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Pyramid[0]);

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(PyramidPosition.length * 4); // 4 = float
                                                                                       // size
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer_Pyramid = byteBuffer.asFloatBuffer();
        positionBuffer_Pyramid.put(PyramidPosition);
        positionBuffer_Pyramid.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, PyramidPosition.length * 4, positionBuffer_Pyramid,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // Normal vbo
        GLES32.glGenBuffers(1, vbo_Normal_Pyramid, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Normal_Pyramid[0]);

        byteBuffer = ByteBuffer.allocateDirect(PyramidNormal.length * 4); // 4 = float
                                                                          // size
        byteBuffer.order(ByteOrder.nativeOrder());
        positionBuffer_Pyramid = byteBuffer.asFloatBuffer();
        positionBuffer_Pyramid.put(PyramidNormal);
        positionBuffer_Pyramid.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, PyramidNormal.length * 4, positionBuffer_Pyramid,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_NORMAL, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_NORMAL);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // Depth Related Changes
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);

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

    private void update() {
        anglePyramid = anglePyramid + 2.0f;
        if (anglePyramid >= 360.0f)
            anglePyramid = anglePyramid - 360.0f;

    }

    private void display() {
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        GLES32.glUseProgram(shaderProgramObject);

        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];
        float rotationMatrix[] = new float[16];
        float translateMatrix[] = new float[16];

        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(rotationMatrix, 0);
        Matrix.setIdentityM(translateMatrix, 0);
        Matrix.setIdentityM(viewMatrix, 0);

        // Triangle
        Matrix.translateM(translateMatrix, 0, 0.0f, 0.0f, -4.0f);
        Matrix.rotateM(rotationMatrix, 0, anglePyramid, 0.0f, 1.0f, 0.0f);

        Matrix.multiplyMM(modelMatrix, 0, translateMatrix, 0, rotationMatrix, 0);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform, 1);

            for (int i = 0; i < 2; i++) {
                GLES32.glUniform3fv(laUniform[i], 1, lightAmbiant[i], 0); // we can use glUniform3f() ,so we can
                                                                          // directly
                                                                          // pass the values to uniform
                GLES32.glUniform3fv(ldUniform[i], 1, lightDiffuse[i], 0);
                GLES32.glUniform3fv(lsUniform[i], 1, lightSpecular[i], 0);
                GLES32.glUniform4fv(lighPositionUniform[i], 1, lightPositions[i], 0);
            }

            GLES32.glUniform3fv(kaUniform, 1, materialAmbiant, 0);
            GLES32.glUniform3fv(kdUniform, 1, meterialDiffuse, 0);
            GLES32.glUniform3fv(ksUniform, 1, materialSpecular, 0);
            GLES32.glUniform1f(materialShininessUniform, materialShineeness);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
        }

        GLES32.glBindVertexArray(vao_Pyramid[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 12);

        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);
        requestRender();
    }

    private void uninitialize() {
        // code
        if (vbo_Normal_Pyramid[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Normal_Pyramid, 0);
            vbo_Normal_Pyramid[0] = 0;
        }

        if (vbo_Position_Pyramid[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Position_Pyramid, 0);
            vbo_Position_Pyramid[0] = 0;
        }

        if (vao_Pyramid[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_Pyramid, 0);
            vao_Pyramid[0] = 0;
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
