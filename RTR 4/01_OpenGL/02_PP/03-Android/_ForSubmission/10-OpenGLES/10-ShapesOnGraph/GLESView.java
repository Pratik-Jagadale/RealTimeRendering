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

    private int vao_triangle[] = new int[1];
    private int vbo_Triangle_Position[] = new int[1];

    private int vao_Square[] = new int[1];
    private int vbo_Square_Position[] = new int[1];

    private int vao_Circle[] = new int[1];;
    private int vbo_Circle_Position[] = new int[1];;

    private int vao_Line[] = new int[1];;
    private int vbo_Line_Position[] = new int[1];;

    private int uniform_Color;

    private int mvpMatrixUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private int count = 0;
    private int bGraph = 1;
    private int bCircle = 2;
    private int bTriangle = 3;
    private int bSquare = 4;

    private float CircleVertexdata[] = new float[1080];
    private int numberOfVerticesOfCircle = 0;

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
                        "uniform mat4 u_mvpMatrix;" +
                        "void main(void)" +
                        "{" +
                        "gl_Position = u_mvpMatrix * a_position;" +
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
                        "uniform vec3 u_Color;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "FragColor = vec4(u_Color , 1.0);" +
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
        uniform_Color = GLES32.glGetUniformLocation(shaderProgramObject, "u_Color");

        final float triangleVertices[] = new float[] {
                0.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f };

        final float SquarePosition[] = new float[] {
                1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f };

        final float LinePosition[] = new float[] {
                0.0f, 1.0f, 0.0f,
                0.0f, -1.0f, 0.0f };

        numberOfVerticesOfCircle = 1080 / 3;
        getCircleVertexData();

        // vao_triangle
        GLES32.glGenVertexArrays(1, vao_triangle, 0);
        GLES32.glBindVertexArray(vao_triangle[0]);

        GLES32.glGenBuffers(1, vbo_Triangle_Position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Triangle_Position[0]);

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(triangleVertices.length * 4); // 4 = float size
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer = byteBuffer.asFloatBuffer();
        positionBuffer.put(triangleVertices);
        positionBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, triangleVertices.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // vao_Square
        GLES32.glGenVertexArrays(1, vao_Square, 0);
        GLES32.glBindVertexArray(vao_Square[0]);

        GLES32.glGenBuffers(1, vbo_Square_Position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Square_Position[0]);

        ByteBuffer byteBuffer_square = ByteBuffer.allocateDirect(SquarePosition.length * 4); // 4 = float size
        byteBuffer_square.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer_square = byteBuffer_square.asFloatBuffer();
        positionBuffer_square.put(SquarePosition);
        positionBuffer_square.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, SquarePosition.length * 4, positionBuffer_square,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // vao_Line
        GLES32.glGenVertexArrays(1, vao_Line, 0);
        GLES32.glBindVertexArray(vao_Line[0]);

        GLES32.glGenBuffers(1, vbo_Line_Position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Line_Position[0]);

        byteBuffer = ByteBuffer.allocateDirect(LinePosition.length * 4); // 4 = float size
        byteBuffer.order(ByteOrder.nativeOrder());
        positionBuffer = byteBuffer.asFloatBuffer();
        positionBuffer.put(LinePosition);
        positionBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, LinePosition.length * 4, positionBuffer, GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // vao_Circle
        GLES32.glGenVertexArrays(1, vao_Circle, 0);
        GLES32.glBindVertexArray(vao_Circle[0]);

        GLES32.glGenBuffers(1, vbo_Circle_Position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Circle_Position[0]);

        byteBuffer = ByteBuffer.allocateDirect(CircleVertexdata.length * 4); // 4 = float size
        byteBuffer.order(ByteOrder.nativeOrder());
        positionBuffer = byteBuffer.asFloatBuffer();
        positionBuffer.put(CircleVertexdata);
        positionBuffer.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, CircleVertexdata.length * 4, positionBuffer,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

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

        // drawGraph();

        // drawSquare();

        drawCircle();

        // drawTriangle();

        GLES32.glUseProgram(0);
        requestRender();
    }

    private void uninitialize() {
        // code
        if (vbo_Line_Position[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Square_Position, 0);
            vbo_Square_Position[0] = 0;
        }

        if (vao_Line[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_Line, 0);
            vao_Line[0] = 0;
        }

        if (vbo_Circle_Position[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Circle_Position, 0);
            vbo_Circle_Position[0] = 0;
        }

        if (vao_Circle[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_Circle, 0);
            vao_Circle[0] = 0;
        }

        if (vao_Square[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_Square, 0);
            vao_Square[0] = 0;
        }

        if (vbo_Triangle_Position[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Triangle_Position, 0);
            vbo_Triangle_Position[0] = 0;
        }

        if (vao_triangle[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_triangle, 0);
            vao_triangle[0] = 0;
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

    private void drawTriangle() {
        float modelViewMatrix[] = new float[16];
        Matrix.setIdentityM(modelViewMatrix, 0);

        float modelViewProjectMatrix[] = new float[16];
        Matrix.setIdentityM(modelViewProjectMatrix, 0);

        // Square
        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.translateM(modelViewMatrix, 0, 0.0f, 0.0f, -4.0f);

        Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

        float ColorArray[] = new float[] { 1.0f, 1.0f, 0.0f };
        GLES32.glBindVertexArray(vao_triangle[0]);

        GLES32.glUniform3fv(uniform_Color, 1, ColorArray, 0);
        GLES32.glDrawArrays(GLES32.GL_LINE_LOOP, 0, 3);

        GLES32.glBindVertexArray(0);

    }

    private void drawSquare() {
        float modelViewMatrix[] = new float[16];
        Matrix.setIdentityM(modelViewMatrix, 0);

        float modelViewProjectMatrix[] = new float[16];
        Matrix.setIdentityM(modelViewProjectMatrix, 0);

        // Square
        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.translateM(modelViewMatrix, 0, 0.0f, 0.0f, -4.0f);

        Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

        float ColorArray[] = new float[] { 1.0f, 1.0f, 0.0f };

        GLES32.glBindVertexArray(vao_Square[0]);

        GLES32.glUniform3fv(uniform_Color, 1, ColorArray, 0);

        GLES32.glDrawArrays(GLES32.GL_LINE_LOOP, 0, 4);

        GLES32.glBindVertexArray(0);

    }

    void drawGraph() {
        float modelViewMatrix[] = new float[16];
        float modelViewProjectMatrix[] = new float[16];
        float translationMatrix[] = new float[16];
        float scaleMatrix[] = new float[16];
        float rotationMatrix[] = new float[16];

        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.setIdentityM(modelViewProjectMatrix, 0);
        Matrix.setIdentityM(translationMatrix, 0);
        Matrix.setIdentityM(scaleMatrix, 0);
        Matrix.setIdentityM(rotationMatrix, 0);

        // Verticle Lines

        Matrix.translateM(translationMatrix, 0, 0.0f, 0.0f, -4.1f);

        Matrix.scaleM(scaleMatrix, 0, 1.0f, 1.5f, 1.0f);

        for (float i = -1.5f; i < 1.5f; i = i + 0.05f) {

            Matrix.setIdentityM(translationMatrix, 0);

            Matrix.translateM(translationMatrix, 0, i, 0.0f, -4.1f);

            Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, scaleMatrix, 0);
            Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

            GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

            float ColorArray[] = new float[] { 0.0f, 0.0f, 1.0f };

            if (i > -0.02 && i < 0.02) {
                ColorArray[1] = 1.0f;
                ColorArray[2] = 0.0f;
            }

            GLES32.glBindVertexArray(vao_Line[0]);

            GLES32.glUniform3fv(uniform_Color, 1, ColorArray, 0);

            GLES32.glDrawArrays(GLES32.GL_LINES, 0, 2);

            GLES32.glBindVertexArray(0);
        }

        // Horizontal Lines

        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.setIdentityM(translationMatrix, 0);
        Matrix.setIdentityM(scaleMatrix, 0);
        Matrix.setIdentityM(rotationMatrix, 0);

        for (float i = -1.5f; i < 1.5f; i = i + 0.05f) {

            Matrix.setIdentityM(modelViewMatrix, 0);
            Matrix.setIdentityM(translationMatrix, 0);
            Matrix.setIdentityM(rotationMatrix, 0);
            Matrix.setIdentityM(scaleMatrix, 0);

            Matrix.rotateM(rotationMatrix, 0, 90.0f, 0.0f, 0.0f, 1.0f);
            Matrix.translateM(translationMatrix, 0, 0, i, -4.1f);
            Matrix.scaleM(scaleMatrix, 0, 1.0f, 1.5f, 1.0f);

            Matrix.multiplyMM(rotationMatrix, 0, rotationMatrix, 0, scaleMatrix, 0);

            Matrix.multiplyMM(modelViewMatrix, 0, translationMatrix, 0, rotationMatrix, 0);
            Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

            GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

            float ColorArray[] = new float[] { 0.0f, 0.0f, 1.0f };
            if (i > -0.02 && i < 0.02) {
                ColorArray[0] = 1.0f;
                ColorArray[2] = 0.0f;
            }

            GLES32.glBindVertexArray(vao_Line[0]);

            GLES32.glUniform3fv(uniform_Color, 1, ColorArray, 0);

            GLES32.glDrawArrays(GLES32.GL_LINES, 0, 2);

            GLES32.glBindVertexArray(0);
        }
    }

    void drawCircle() {
        // Tranformations
        float modelViewMatrix[] = new float[16];
        float modelViewProjectMatrix[] = new float[16];
        float translationMatrix[] = new float[16];

        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.setIdentityM(modelViewProjectMatrix, 0);
        Matrix.setIdentityM(translationMatrix, 0);

        Matrix.translateM(modelViewMatrix, 0, 0.0f, 0.0f, -4.0f);

        Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        float ColorArray[] = new float[] { 1.0f, 1.0f, 0.0f };

        GLES32.glBindVertexArray(vao_Circle[0]);

        GLES32.glUniform3fv(uniform_Color, 1, ColorArray, 0);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLES, 0, numberOfVerticesOfCircle);

        GLES32.glBindVertexArray(0);
    }

    void getCircleVertexData() {

        // LOCAL VARIABLE DECLARTIONS
        float x = 0.0f;
        float y = 0.0f;
        float radius = 2.0f;

        // CODE
        float angle = 0.0f;
        for (int i = 0; i < 1080; i = i + 3) {
            float rAngle = angle * (float) Math.PI / 180.0f;

            System.out.println("PRJ: " + angle + "=" + rAngle);

            x = radius * (float) Math.cos(rAngle);
            y = radius * (float) Math.sin(rAngle);

            CircleVertexdata[i] = x;
            CircleVertexdata[i + 1] = y;
            CircleVertexdata[i + 2] = 0.0f;

            System.out.println("PRJ: " + x + " " + y);
            angle = angle + 1.0f;
        }

        CircleVertexdata[1080 - 3] = CircleVertexdata[0];
        CircleVertexdata[1080 - 2] = CircleVertexdata[1];
        CircleVertexdata[1080 - 1] = CircleVertexdata[2];

    }

}
