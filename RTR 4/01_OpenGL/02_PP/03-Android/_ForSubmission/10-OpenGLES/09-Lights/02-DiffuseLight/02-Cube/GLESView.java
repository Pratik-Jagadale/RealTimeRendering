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

    private int vao_Cube[] = new int[1];
    private int vbo_Position_Cube[] = new int[1];
    private int vbo_Normal_Cube[] = new int[1];

    private int modelMatrixUniform;
    private int viewMatrixUniform;
    private int ProjectionMatrixUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private int ldUniform; // light direction
    private int kdUniform; // material diffuse
    private int lightPositionUniform;
    private int lightingEnabledUniform;

    private int lightingEnabledUniform_PV;

    private float lightDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float materialDiffuse[] = new float[] { 1.0f, 1.0f, 1.0f, 1.0f };
    private float lightPosition[] = new float[] { 0.0f, 0.0f, 2.0f, 1.0f };

    private Boolean bLight = false;

    private float angleCube = 0.0f;

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
                        "in vec3 a_normal; " +
                        "uniform mat4 u_projectionMatrix;" +
                        "uniform mat4 u_viewMatrix;" +
                        "uniform mat4 u_modelMatrix;" +
                        "uniform vec3 u_ld;" +
                        "uniform vec3 u_kd;" +
                        "uniform vec4 u_lightPosition;" +
                        "uniform int u_lightEnabled_PV;" +
                        "out vec3 diffuse_light_color;" +
                        "void main(void)" +
                        "{" +
                        "if(u_lightEnabled_PV == 1)" +
                        "{" +
                        "vec4 eyeCoordinate = u_viewMatrix * u_modelMatrix * a_position;" +
                        "mat3 normalMatrix = mat3(transpose(inverse(u_viewMatrix * u_modelMatrix)));" +
                        "vec3 transformedNormals = normalize(normalMatrix * a_normal);" +
                        "vec3 lightDirection = vec3(normalize(u_lightPosition - eyeCoordinate));" +
                        "diffuse_light_color = u_ld * u_kd * max(dot(lightDirection, transformedNormals), 0.0);" +
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
                        "in vec3 diffuse_light_color;" +
                        "uniform highp int u_lightEnabled;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "if(u_lightEnabled == 1)" +
                        "{" +
                        "FragColor = vec4(diffuse_light_color, 1.0);" +
                        "}" +
                        "else" +
                        "{" +
                        "FragColor = vec4(1.0f,1.0f,1.0f,1.0f);" +
                        "}" +
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

        ldUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_ld");
        kdUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_kd");
        lightPositionUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightPosition");
        lightingEnabledUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightEnabled");

        lightingEnabledUniform_PV = GLES32.glGetUniformLocation(shaderProgramObject, "u_lightEnabled_PV");

        final float cubePosition[] = new float[] {
                // top
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,

                // bottom
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,

                // front
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,

                // back
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                // right
                1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,

                // left
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f
        };

        final float cubeNormal[] = new float[] {
                // top surface
                0.0f, 1.0f, 0.0f, // top-right of top
                0.0f, 1.0f, 0.0f, // top-left of top
                0.0f, 1.0f, 0.0f, // bottom-left of top
                0.0f, 1.0f, 0.0f, // bottom-right of top

                // bottom surface
                0.0f, -1.0f, 0.0f, // top-right of bottom
                0.0f, -1.0f, 0.0f, // top-left of bottom
                0.0f, -1.0f, 0.0f, // bottom-left of bottom
                0.0f, -1.0f, 0.0f, // bottom-right of bottom

                // front surface
                0.0f, 0.0f, 1.0f, // top-right of front
                0.0f, 0.0f, 1.0f, // top-left of front
                0.0f, 0.0f, 1.0f, // bottom-left of front
                0.0f, 0.0f, 1.0f, // bottom-right of front

                // back surface
                0.0f, 0.0f, -1.0f, // top-right of back
                0.0f, 0.0f, -1.0f, // top-left of back
                0.0f, 0.0f, -1.0f, // bottom-left of back
                0.0f, 0.0f, -1.0f, // bottom-right of back

                // right surface
                1.0f, 0.0f, 0.0f, // top-right of right
                1.0f, 0.0f, 0.0f, // top-left of right
                1.0f, 0.0f, 0.0f, // bottom-left of right
                1.0f, 0.0f, 0.0f, // bottom-right of right

                // left surface
                -1.0f, 0.0f, 0.0f, // top-right of left
                -1.0f, 0.0f, 0.0f, // top-left of left
                -1.0f, 0.0f, 0.0f, // bottom-left of left
                -1.0f, 0.0f, 0.0f // bottom-right of left

        };

        // vao_Cube
        GLES32.glGenVertexArrays(1, vao_Cube, 0);
        GLES32.glBindVertexArray(vao_Cube[0]);

        // Position vbo
        GLES32.glGenBuffers(1, vbo_Position_Cube, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Cube[0]);

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(cubePosition.length * 4); // 4 = float
                                                                                    // size
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer_Pyramid = byteBuffer.asFloatBuffer();
        positionBuffer_Pyramid.put(cubePosition);
        positionBuffer_Pyramid.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubePosition.length * 4, positionBuffer_Pyramid,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // Normal vbo
        GLES32.glGenBuffers(1, vbo_Normal_Cube, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Normal_Cube[0]);

        byteBuffer = ByteBuffer.allocateDirect(cubeNormal.length * 4); // 4 = float
                                                                       // size
        byteBuffer.order(ByteOrder.nativeOrder());
        positionBuffer_Pyramid = byteBuffer.asFloatBuffer();
        positionBuffer_Pyramid.put(cubeNormal);
        positionBuffer_Pyramid.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, cubeNormal.length * 4, positionBuffer_Pyramid,
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
        angleCube = angleCube + 2.0f;
        if (angleCube >= 360.0f)
            angleCube = angleCube - 360.0f;

    }

    private void display() {
        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);

        GLES32.glUseProgram(shaderProgramObject);

        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];
        float rotationMatrix[] = new float[16];
        float rotationMatrix_X[] = new float[16];
        float rotationMatrix_Y[] = new float[16];
        float rotationMatrix_Z[] = new float[16];
        float translateMatrix[] = new float[16];

        Matrix.setIdentityM(modelMatrix, 0);
        Matrix.setIdentityM(rotationMatrix, 0);
        Matrix.setIdentityM(translateMatrix, 0);
        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.setIdentityM(rotationMatrix_X, 0);
        Matrix.setIdentityM(rotationMatrix_Y, 0);
        Matrix.setIdentityM(rotationMatrix_Z, 0);

        // Triangle
        Matrix.translateM(translateMatrix, 0, 0.0f, 0.0f, -6.0f);
        Matrix.rotateM(rotationMatrix_X, 0, angleCube, 1.0f, 0.0f, 0.0f);
        Matrix.rotateM(rotationMatrix_Y, 0, angleCube, 0.0f, 1.0f, 0.0f);
        Matrix.rotateM(rotationMatrix_Z, 0, angleCube, 0.0f, 0.0f, 1.0f);

        Matrix.multiplyMM(rotationMatrix_Y, 0, rotationMatrix_Y, 0, rotationMatrix_Z, 0);
        Matrix.multiplyMM(rotationMatrix, 0, rotationMatrix_X, 0, rotationMatrix_Y, 0);
        Matrix.multiplyMM(modelMatrix, 0, translateMatrix, 0, rotationMatrix, 0);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform, 1, false, perspectiveProjectionMatrix, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform, 1, false, modelMatrix, 0);

        if (bLight == true) {
            GLES32.glUniform1i(lightingEnabledUniform_PV, 1);
            GLES32.glUniform1i(lightingEnabledUniform, 1);
            GLES32.glUniform3fv(ldUniform, 1, lightDiffuse, 0);
            GLES32.glUniform3fv(kdUniform, 1, materialDiffuse, 0);
            GLES32.glUniform4fv(lightPositionUniform, 1, lightPosition, 0);
        } else {
            GLES32.glUniform1i(lightingEnabledUniform, 0);
            GLES32.glUniform1i(lightingEnabledUniform_PV, 0);

        }

        GLES32.glBindVertexArray(vao_Cube[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);
        requestRender();
    }

    private void uninitialize() {
        // code
        if (vbo_Normal_Cube[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Normal_Cube, 0);
            vbo_Normal_Cube[0] = 0;
        }

        if (vbo_Position_Cube[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Position_Cube, 0);
            vbo_Position_Cube[0] = 0;
        }

        if (vao_Cube[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_Cube, 0);
            vao_Cube[0] = 0;
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
