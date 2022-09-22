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

    private int vao_Triangle[] = new int[1];
    private int vbo_Position_Triangle[] = new int[1];
    private int vbo_color[] = new int[1];

    private int vao_square[] = new int[1];
    private int vbo_Square_Position[] = new int[1];
    private int vbo_Square_Color[] = new int[1];

    private int mvpMatrixUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

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
                        "in vec3 a_color;" +
                        "uniform mat4 u_mvpMatrix;" +
                        "out vec3 a_color_out;" +
                        "void main(void)" +
                        "{" +
                        "gl_Position = u_mvpMatrix * a_position;" +
                        "a_color_out = a_color;" +
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
                        "in vec3 a_color_out;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "FragColor = vec4(a_color_out, 1.0);" +
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
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATRIBUTE_COLOR, "a_color");

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
        mvpMatrixUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_mvpMatrix");

        final float triangleVertices[] = new float[] {
                0.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f };

        final float triangleColor[] = new float[] {
                1.0f, 0.0f, 0.0f, // RED
                0.0f, 1.0f, 0.0f, // BLUE
                0.0f, 0.0f, 1.0f // GREEN
        };

        final float SquarePosition[] = new float[] {
                1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f };

        final float squareColor[] = new float[] {
                1.0f, 0.0f, 0.0f, // RED
                0.0f, 1.0f, 0.0f, // BLUE
                0.0f, 0.0f, 1.0f, // GREEN
                0.0f, 1.0f, 1.0f // CYAN
        };

        // vao_Triangle
        GLES32.glGenVertexArrays(1, vao_Triangle, 0);
        GLES32.glBindVertexArray(vao_Triangle[0]);

        GLES32.glGenBuffers(1, vbo_Position_Triangle, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Triangle[0]);

        ByteBuffer bytePositionBuffer_Triangle = ByteBuffer.allocateDirect(triangleVertices.length * 4); // 4 = float
                                                                                                         // size
        bytePositionBuffer_Triangle.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer_Triangle = bytePositionBuffer_Triangle.asFloatBuffer();
        positionBuffer_Triangle.put(triangleVertices);
        positionBuffer_Triangle.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangleVertices.length * 4, positionBuffer_Triangle,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glGenBuffers(1, vbo_color, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_color[0]);

        ByteBuffer byteColorBuffer_Triangle = ByteBuffer.allocateDirect(triangleColor.length * 4); // 4 = float size
        byteColorBuffer_Triangle.order(ByteOrder.nativeOrder());
        FloatBuffer colorBuffer_Triangle = byteColorBuffer_Triangle.asFloatBuffer();
        colorBuffer_Triangle.put(triangleColor);
        colorBuffer_Triangle.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangleColor.length * 4, colorBuffer_Triangle,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_COLOR);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // vao_square
        GLES32.glGenVertexArrays(1, vao_square, 0);
        GLES32.glBindVertexArray(vao_square[0]);

        GLES32.glGenBuffers(1, vbo_Square_Position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Square_Position[0]);

        ByteBuffer bytePositionBuffer_Square = ByteBuffer.allocateDirect(SquarePosition.length * 4); // 4 = float size
        bytePositionBuffer_Square.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer_Square = bytePositionBuffer_Square.asFloatBuffer();
        positionBuffer_Square.put(SquarePosition);
        positionBuffer_Square.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, SquarePosition.length * 4, positionBuffer_Square,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glGenBuffers(1, vbo_Square_Color, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Square_Color[0]);

        ByteBuffer byteColorBuffer = ByteBuffer.allocateDirect(squareColor.length * 4); // 4 = float size
        byteColorBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer colorBuffer_Square = byteColorBuffer.asFloatBuffer();
        colorBuffer_Square.put(squareColor);
        colorBuffer_Square.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, squareColor.length * 4, colorBuffer_Square, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_COLOR, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_COLOR);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // Depth Related Changes
        GLES32.glClearDepthf(1.0f);
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

        float modelViewMatrix[] = new float[16];
        Matrix.setIdentityM(modelViewMatrix, 0);

        float modelViewProjectMatrix[] = new float[16];
        Matrix.setIdentityM(modelViewProjectMatrix, 0);

        // Triangle
        Matrix.translateM(modelViewMatrix, 0, -2.0f, 0.0f, -5.0f);

        Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

        GLES32.glBindVertexArray(vao_Triangle[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, 3);

        GLES32.glBindVertexArray(0);

        // Square
        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.setIdentityM(modelViewProjectMatrix, 0);

        Matrix.translateM(modelViewMatrix, 0, 2.0f, 0.0f, -5.0f);

        Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

        GLES32.glBindVertexArray(vao_square[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);
        requestRender();
    }

    private void uninitialize() {
        // code
        if (vbo_Square_Color[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Square_Color, 0);
            vbo_Square_Color[0] = 0;
        }

        if (vbo_Square_Position[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Square_Position, 0);
            vbo_Square_Position[0] = 0;
        }

        if (vao_square[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_square, 0);
            vao_square[0] = 0;
        }

        if (vbo_color[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_color, 0);
            vbo_color[0] = 0;
        }

        if (vbo_Position_Triangle[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Position_Triangle, 0);
            vbo_Position_Triangle[0] = 0;
        }

        if (vao_Triangle[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_Triangle, 0);
            vao_Triangle[0] = 0;
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
