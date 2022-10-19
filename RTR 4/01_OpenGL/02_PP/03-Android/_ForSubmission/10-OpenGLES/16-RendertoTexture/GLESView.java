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

import android.graphics.BitmapFactory; // for imageData
import android.graphics.Bitmap;
import android.opengl.GLUtils;

// Sphere Relate
import java.nio.ShortBuffer;
import java.sql.PreparedStatement;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer {

    // Properties
    private GestureDetector gestureDetector;
    private Context context;
    private int shaderProgramObject;

    private int vao_Cube[] = new int[1];
    private int vbo_Position_Cube[] = new int[1];
    private int vbo_TexCoord_Cube[] = new int[1];

    private int mvpMatrixUniform;
    private int textureSamplerUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private float angleSquare = 0.0f;
    private int winWidth;
    private int winHeight;

    private static final int FBO_WIDTH = 512;
    private static final int FBO_HEIGHT = 512;

    // FBO RELATED GLOBAL VARIABLES
    private int fbo[] = new int[1]; // FRAME BUFFER OBJECT
    private int rbo[] = new int[1]; // RENDER BUFFER OBJECT
    private int fbo_texture[] = new int[1];
    Boolean bBFOResult = false;

    // SPHERE RELATED VARIABLES
    private int shaderProgramObject_sphere;

    private int vao_sphere[] = new int[1];
    private int vbo_sphere_position[] = new int[1];
    private int vbo_sphere_normal[] = new int[1];
    private int vbo_sphere_element[] = new int[1];

    private int modelMatrixUniform_sphere;
    private int viewMatrixUniform_sphere;
    private int ProjectionMatrixUniform_sphere;

    private float perspectiveProjectionMatrix_sphere[] = new float[16];

    private int laUniform_sphere[] = new int[3]; // light Ambiant
    private int ldUniform_sphere[] = new int[3]; // light Diffuse
    private int lsUniform_sphere[] = new int[3]; // light Spec
    private int lightPositionUniform_sphere[] = new int[3]; // light Position

    private int kaUniform_sphere; // material amb
    private int kdUniform_sphere; // material diffuse
    private int ksUniform_sphere; // mat specular
    private int materialShininessUniform_sphere;

    private int lightingEnabledUniform_sphere;

    private float lightAmbiant_sphere[][] = new float[][] { { 0.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f }
    };
    private float lightDiffuse_sphere[][] = new float[][] { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f } };
    private float lightSpecular_sphere[][] = new float[][] { { 1.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 1.0f, 0.0f, 1.0f } };
    private float lightPositions_sphere[][] = new float[][] { { -2.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 1.0f },
            { 0.0f, 0.0f, 0.0f, 1.0f } };

    private float materialAmbiant_sphere[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    private float materialDiffuse_sphere[] = new float[] { 0.5f, 0.2f, 0.7f, 1.0f };
    private float materialSpecular_sphere[] = new float[] { 0.7f, 0.7f, 0.7f, 1.0f };
    private float materialShineeness_sphere = 120.0f;

    private Boolean bLight = false;

    private int numVertices_sphere;
    private int numElements_sphere;

    private float lightAngleOne_sphere = 0.0f;
    private float lightAngleTwo_sphere = 150.0f;
    private float lightAngleZero_sphere = 300.0f;

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
                        "in vec2 a_texcoord;" +
                        "uniform mat4 u_mvpMatrix;" +
                        "out vec2 a_texcoord_out;" +
                        "void main(void)" +
                        "{" +
                        "gl_Position = u_mvpMatrix * a_position;" +
                        "a_texcoord_out = a_texcoord;" +
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
                        "in vec2 a_texcoord_out;" +
                        "uniform highp sampler2D u_textureSampler;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "FragColor = texture(u_textureSampler , a_texcoord_out);" +
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
        GLES32.glBindAttribLocation(shaderProgramObject, MyGLESMacros.PRJ_ATRIBUTE_TEXTURE0, "a_texcoord");

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

        textureSamplerUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_textureSampler");

        final float CubePosition[] = new float[] {
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
                -1.0f, -1.0f, 1.0f };

        final float CubeTexCoord[] = new float[] {
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
                1.0f, 0.0f,

                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
        };

        // vao_Cube
        GLES32.glGenVertexArrays(1, vao_Cube, 0);
        GLES32.glBindVertexArray(vao_Cube[0]);

        // Position vbo
        GLES32.glGenBuffers(1, vbo_Position_Cube, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Position_Cube[0]);

        ByteBuffer bytePositionBuffer_Cube = ByteBuffer.allocateDirect(CubePosition.length * 4); // 4 = float size
        bytePositionBuffer_Cube.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer_Cube = bytePositionBuffer_Cube.asFloatBuffer();
        positionBuffer_Cube.put(CubePosition);
        positionBuffer_Cube.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, CubePosition.length * 4, positionBuffer_Cube,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        // TexCoord vbo
        GLES32.glGenBuffers(1, vbo_TexCoord_Cube, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_TexCoord_Cube[0]);

        bytePositionBuffer_Cube = ByteBuffer.allocateDirect(CubeTexCoord.length * 4); // 4 = float size
        bytePositionBuffer_Cube.order(ByteOrder.nativeOrder());
        positionBuffer_Cube = bytePositionBuffer_Cube.asFloatBuffer();
        positionBuffer_Cube.put(CubeTexCoord);
        positionBuffer_Cube.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, CubeTexCoord.length * 4, positionBuffer_Cube,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_TEXTURE0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // Depth Related Changes
        GLES32.glClearDepthf(1.0f);
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);
        // GLES32.glEnable(GLES32.GL_CULL_FACE);

        // textur Related Code
        GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Initialization of Project MNatrix
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);

        // FBO CODE
        bBFOResult = CreateFBO(FBO_WIDTH, FBO_HEIGHT);

        int iRetVal;
        if (bBFOResult == true) {
            initialize_sphere(512, 510);
            // HERE SHOULD BE ERROR CHECKING
        } else {
            // fprintf(gpFile, "CREATE FBO FAILED...\n");
            System.out.println("PRJ: CREATE FBO FAILED...");
        }

    }

    boolean CreateFBO(int textureWidth, int textureHeight) {
        //
        int maxRenderbufferSize[] = new int[1];

        // CODE
        GLES32.glGetIntegerv(GLES32.GL_MAX_RENDERBUFFER_SIZE, maxRenderbufferSize, 0);

        // CHECK AVAILABLE BUFFER SIZE
        if (maxRenderbufferSize[0] < textureWidth || maxRenderbufferSize[0] < textureHeight) {
            // fprintf(gpFile, "UnSufficient Render Buffer Size...\n");
            System.out.println("PRJ: UnSufficient Render Buffer Size...:");
            return (false);
        }

        // CREATE FRAME BUFFER OBJECT
        GLES32.glGenFramebuffers(1, fbo, 0);
        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, fbo[0]);

        // CREATE RENDER BUFFER OBJECT
        GLES32.glGenRenderbuffers(1, rbo, 0);
        GLES32.glBindRenderbuffer(GLES32.GL_RENDERBUFFER, rbo[0]);

        // WHERE TO KEEP THIS RENDER BUFFER AND WHAT WILL BE THE FORMAT OF RENDER BUFFER
        GLES32.glRenderbufferStorage(
                GLES32.GL_RENDERBUFFER,
                GLES32.GL_DEPTH_COMPONENT16, // NOT RELATED TO DEPTH - BUT 16 MACRO SIZE THIS ONE HENCE USED HERE
                textureWidth,
                textureHeight);

        // CREATE EMPTY TEXTURE
        GLES32.glGenTextures(1, fbo_texture, 0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, fbo_texture[0]);

        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_S, GLES32.GL_CLAMP_TO_EDGE);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_WRAP_T, GLES32.GL_CLAMP_TO_EDGE);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR);

        GLES32.glTexImage2D(
                GLES32.GL_TEXTURE_2D,
                0,
                GLES32.GL_RGB,
                textureWidth,
                textureHeight,
                0,
                GLES32.GL_RGB,
                GLES32.GL_UNSIGNED_SHORT_5_6_5,
                null // NULL - EMPTY TEXTURE
        );

        // GIVE ABOVE TEXTURE TO FBO
        GLES32.glFramebufferTexture2D(
                GLES32.GL_FRAMEBUFFER,
                GLES32.GL_COLOR_ATTACHMENT0,
                GLES32.GL_TEXTURE_2D,
                fbo_texture[0],
                0 // MIPMAP LEVEL
        );

        // GIVE RENDER BUFFER TO FBO
        GLES32.glFramebufferRenderbuffer(
                GLES32.GL_FRAMEBUFFER,
                GLES32.GL_DEPTH_ATTACHMENT,
                GLES32.GL_RENDERBUFFER,
                rbo[0] //
        );

        // CHECK WATHER FBO IS CREATED SUCCESSFULLY OR NOT
        int result = GLES32.glCheckFramebufferStatus(GLES32.GL_FRAMEBUFFER);
        if (result == GLES32.GL_FRAMEBUFFER_COMPLETE) {
            // fprintf(gpFile, "FRAME BUFFER IS NOT COMPLETE...\n");
            System.out.println("PRJ: FRAME BUFFER IS COMPLETE...");
        }

        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0); // IMPLICITEL UNBINDS THE RBO AND TEXTURE BUFFER OBJECT

        return true;
    }

    private int loadGLTexture(int imageResourceID) {

        BitmapFactory.Options options = new BitmapFactory.Options();
        options.inScaled = false;

        Bitmap bitmap = BitmapFactory.decodeResource(context.getResources(), imageResourceID, options);

        int texture[] = new int[1];

        GLES32.glPixelStorei(GLES32.GL_UNPACK_ALIGNMENT, 1);
        GLES32.glGenTextures(1, texture, 0);
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture[0]);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MAG_FILTER, GLES32.GL_LINEAR);
        GLES32.glTexParameteri(GLES32.GL_TEXTURE_2D, GLES32.GL_TEXTURE_MIN_FILTER, GLES32.GL_LINEAR_MIPMAP_LINEAR);

        // create the texture
        GLUtils.texImage2D(GLES32.GL_TEXTURE_2D // target
                , 0 // Mipmap Level
                , bitmap // image data
                , 0 // Border Widtgh
        );

        GLES32.glGenerateMipmap(GLES32.GL_TEXTURE_2D); // Generate MipMap
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0); // unbind texture

        return texture[0];
    }

    private void resize(int width, int height) {
        if (height <= 0)
            height = 1;

        winWidth = width;
        winHeight = height;

        resize_sphere(width, height);

        GLES32.glViewport(0, 0, width, height);

        Matrix.perspectiveM(perspectiveProjectionMatrix, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);

    }

    private void update() {

        angleSquare = angleSquare + 2.0f;
        if (angleSquare >= 360.0f)
            angleSquare = angleSquare - 360.0f;
    }

    private void display() {

        if (bBFOResult == true) {
            display_sphere(FBO_WIDTH, FBO_HEIGHT);
            update_sphere();
        }

        GLES32.glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        resize(winWidth, winHeight);

        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        GLES32.glUseProgram(shaderProgramObject);

        float modelViewMatrix[] = new float[16];
        float modelViewProjectMatrix[] = new float[16];
        float rotationMatrix[] = new float[16];
        float rotationMatrix_X[] = new float[16];
        float rotationMatrix_Y[] = new float[16];
        float rotationMatrix_Z[] = new float[16];
        float translateMatrix[] = new float[16];
        float scaleMatrix[] = new float[16];

        // Square
        Matrix.setIdentityM(modelViewMatrix, 0);
        Matrix.setIdentityM(modelViewProjectMatrix, 0);
        Matrix.setIdentityM(rotationMatrix, 0);
        Matrix.setIdentityM(translateMatrix, 0);

        Matrix.setIdentityM(rotationMatrix_X, 0);
        Matrix.setIdentityM(rotationMatrix_Y, 0);
        Matrix.setIdentityM(rotationMatrix_Z, 0);

        Matrix.translateM(translateMatrix, 0, 0.0f, 0.0f, -4.0f);
        Matrix.scaleM(scaleMatrix, 0, 0.75f, 0.75f, 0.75f);

        Matrix.rotateM(rotationMatrix_X, 0, angleSquare, 1.0f, 0.0f, 0.0f);
        Matrix.rotateM(rotationMatrix_Y, 0, angleSquare, 0.0f, 1.0f, 0.0f);
        Matrix.rotateM(rotationMatrix_Z, 0, angleSquare, 0.0f, 0.0f, 1.0f);

        Matrix.multiplyMM(rotationMatrix_Z, 0, rotationMatrix_Y, 0, rotationMatrix_Z, 0);
        Matrix.multiplyMM(rotationMatrix, 0, rotationMatrix_X, 0, rotationMatrix_Z, 0);

        Matrix.multiplyMM(modelViewMatrix, 0, translateMatrix, 0, rotationMatrix, 0);
        // Matrix.multiplyMM(modelViewMatrix, 0, modelViewMatrix, 0, translateMatrix,
        // 0); // temp

        Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

        // texture related
        GLES32.glActiveTexture(GLES32.GL_TEXTURE0); //
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, fbo_texture[0]);
        GLES32.glUniform1i(textureSamplerUniform, 0); //

        GLES32.glBindVertexArray(vao_Cube[0]);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 4, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 8, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 12, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 16, 4);
        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 20, 4);

        GLES32.glBindVertexArray(0);

        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

        GLES32.glUseProgram(0);
        // requestRender();
    }

    private void initialize_sphere(int width, int height) {

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
                        "uniform mediump int u_lightingEnabled;" +
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
                        "uniform vec3 u_la[3];" +
                        "uniform vec3 u_ld[3];" +
                        "uniform vec3 u_ls[3];" +
                        "uniform vec4 u_lightPosition[3];" +
                        "uniform vec3 u_ka;" +
                        "uniform vec3 u_ks;" +
                        "uniform vec3 u_kd;" +
                        "uniform highp float u_materialShininnes;" +
                        "uniform mediump int u_lightingEnabled;" +
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
        shaderProgramObject_sphere = GLES32.glCreateProgram();

        GLES32.glAttachShader(shaderProgramObject_sphere, vertexShaderObject);
        GLES32.glAttachShader(shaderProgramObject_sphere, fragmentShaderObject);

        // Pre-Link
        GLES32.glBindAttribLocation(shaderProgramObject_sphere, MyGLESMacros.PRJ_ATRIBUTE_POSITION, "a_position");
        GLES32.glBindAttribLocation(shaderProgramObject_sphere, MyGLESMacros.PRJ_ATRIBUTE_NORMAL, "a_normal");

        GLES32.glLinkProgram(shaderProgramObject_sphere);

        status[0] = 0;

        infoLogLength[0] = 0;

        log = null;

        GLES32.glGetProgramiv(shaderProgramObject_sphere, GLES32.GL_LINK_STATUS, status, 0);

        if (status[0] == GLES32.GL_FALSE) {
            GLES32.glGetProgramiv(shaderProgramObject_sphere, GLES32.GL_INFO_LOG_LENGTH, infoLogLength, 0);
            if (infoLogLength[0] > 0) {
                log = GLES32.glGetProgramInfoLog(shaderProgramObject_sphere);
                System.out.println("PRJ: SHADER PROGRAM LINK LOG :" + log);
                uninitialize();
                System.exit(0);
            }
        }

        // Post Link
        ProjectionMatrixUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_projectionMatrix");
        viewMatrixUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_viewMatrix");
        modelMatrixUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_modelMatrix");

        laUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_la[0]");
        ldUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ld[0]");
        lsUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ls[0]");
        lightPositionUniform_sphere[0] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[0]");

        laUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_la[1]");
        ldUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ld[1]");
        lsUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ls[1]");
        lightPositionUniform_sphere[1] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[1]");

        laUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_la[2]");
        ldUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ld[2]");
        lsUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ls[2]");
        lightPositionUniform_sphere[2] = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightPosition[2]");

        kaUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ka");
        kdUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_kd");
        ksUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_ks");
        materialShininessUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere,
                "u_materialShininnes");

        lightingEnabledUniform_sphere = GLES32.glGetUniformLocation(shaderProgramObject_sphere, "u_lightingEnabled");

        // SPHERE DATA
        Sphere sphere = new Sphere();
        float sphere_vertices[] = new float[1146];
        float sphere_normals[] = new float[1146];
        float sphere_textures[] = new float[764];
        short sphere_elements[] = new short[2280];
        sphere.getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);
        numVertices_sphere = sphere.getNumberOfSphereVertices();
        numElements_sphere = sphere.getNumberOfSphereElements();

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

        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION,
                3,
                GLES32.GL_FLOAT,
                false, 0, 0);

        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);

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

        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_NORMAL,
                3,
                GLES32.GL_FLOAT,
                false, 0, 0);

        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_NORMAL);

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

        // Initialization of Project MNatrix
        Matrix.setIdentityM(perspectiveProjectionMatrix_sphere, 0);

        resize_sphere(FBO_WIDTH, FBO_HEIGHT);
    }

    void update_sphere() {
        lightAngleZero_sphere = lightAngleZero_sphere + 1.0f;
        if (lightAngleZero_sphere > 360.0f)
            lightAngleZero_sphere = lightAngleZero_sphere - 360.0f;

        lightAngleOne_sphere = lightAngleOne_sphere + 1.0f;
        if (lightAngleOne_sphere > 360.0f)
            lightAngleOne_sphere = lightAngleOne_sphere - 360.0f;

        lightAngleTwo_sphere = lightAngleTwo_sphere + 1.0f;
        if (lightAngleTwo_sphere > 360.0f)
            lightAngleTwo_sphere = lightAngleTwo_sphere - 360.0f;

        float angle = (lightAngleZero_sphere * (float) (Math.PI / 180.0f));
        float x = 5.0f * (float) Math.cos(angle);
        float y = 5.0f * (float) Math.sin(angle);
        lightPositions_sphere[0][1] = x;
        lightPositions_sphere[0][2] = y;

        // Set Light One Position rotating One Light - Y Rotation
        angle = (lightAngleOne_sphere * (float) Math.PI) / 180.0f;
        x = 5.0f * (float) Math.cos(angle);
        y = 5.0f * (float) Math.sin(angle);
        lightPositions_sphere[1][0] = x;
        lightPositions_sphere[1][2] = y;

        // Set Light Two Position rotating Two Light Z Rotation
        angle = (lightAngleTwo_sphere * (float) Math.PI) / 180.0f;
        x = 5.0f * (float) Math.cos(angle);
        y = 5.0f * (float) Math.sin(angle);
        lightPositions_sphere[2][0] = x;
        lightPositions_sphere[2][1] = y;
    }

    private void resize_sphere(int width, int height) {
        if (height <= 0)
            height = 1;

        GLES32.glViewport(0, 0, width, height);

        Matrix.perspectiveM(perspectiveProjectionMatrix_sphere, 0, 45.0f, (float) width / (float) height, 0.1f, 100.0f);

    }

    private void display_sphere(int textureWidth, int textureHeight) {

        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, fbo[0]);

        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        resize_sphere(textureWidth, textureHeight);

        GLES32.glClear(GLES32.GL_COLOR_BUFFER_BIT | GLES32.GL_DEPTH_BUFFER_BIT);
        GLES32.glUseProgram(shaderProgramObject_sphere);

        float modelMatrix[] = new float[16];
        float viewMatrix[] = new float[16];

        Matrix.setIdentityM(viewMatrix, 0);
        Matrix.setIdentityM(modelMatrix, 0);

        Matrix.translateM(modelMatrix, 0, 0.0f, 0.0f, -2.0f);

        GLES32.glUniformMatrix4fv(ProjectionMatrixUniform_sphere, 1, false, perspectiveProjectionMatrix_sphere, 0);
        GLES32.glUniformMatrix4fv(viewMatrixUniform_sphere, 1, false, viewMatrix, 0);
        GLES32.glUniformMatrix4fv(modelMatrixUniform_sphere, 1, false, modelMatrix, 0);

        if (bLight == true) {

            GLES32.glUniform1i(lightingEnabledUniform_sphere, 1);

            for (int i = 0; i < 3; i++) {
                GLES32.glUniform3fv(laUniform_sphere[i], 1, lightAmbiant_sphere[i], 0); // we can use glUniform3f() ,so
                                                                                        // we can
                // directly
                // pass the values to uniform
                GLES32.glUniform3fv(ldUniform_sphere[i], 1, lightDiffuse_sphere[i], 0);
                GLES32.glUniform3fv(lsUniform_sphere[i], 1, lightSpecular_sphere[i], 0);
                GLES32.glUniform4fv(lightPositionUniform_sphere[i], 1, lightPositions_sphere[i], 0);
            }

            GLES32.glUniform3fv(kaUniform_sphere, 1, materialAmbiant_sphere, 0);
            GLES32.glUniform3fv(kdUniform_sphere, 1, materialDiffuse_sphere, 0);
            GLES32.glUniform3fv(ksUniform_sphere, 1, materialSpecular_sphere, 0);
            GLES32.glUniform1f(materialShininessUniform_sphere, materialShineeness_sphere);

        } else {
            GLES32.glUniform1i(lightingEnabledUniform_sphere, 0);
        }

        // bind vao
        GLES32.glBindVertexArray(vao_sphere[0]);

        // *** draw, either by glDrawTriangles() or glDrawArrays() or glDrawElements()
        GLES32.glBindBuffer(GLES32.GL_ELEMENT_ARRAY_BUFFER, vbo_sphere_element[0]);
        GLES32.glDrawElements(GLES32.GL_TRIANGLES, numElements_sphere,
                GLES32.GL_UNSIGNED_SHORT, 0);

        // unbind vao
        GLES32.glBindVertexArray(0);

        GLES32.glUseProgram(0);

        GLES32.glBindFramebuffer(GLES32.GL_FRAMEBUFFER, 0);
        requestRender();
    }

    private void uninitialize() {
        // code
        // UNINTIALLIZE SPHERE
        uninitialize_sphere();

        if (vbo_TexCoord_Cube[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_TexCoord_Cube, 0);
            vbo_TexCoord_Cube[0] = 0;
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

    private void uninitialize_sphere() {
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

        if (shaderProgramObject_sphere > 0) // means not equal to
        {
            GLES32.glUseProgram(shaderProgramObject_sphere);

            int retVal[] = new int[1];
            GLES32.glGetProgramiv(shaderProgramObject_sphere, GLES32.GL_ATTACHED_SHADERS, retVal, 0);
            if (retVal[0] > 0) {
                int numAttachedShaders = retVal[0];

                int shaderObjects[] = new int[numAttachedShaders];
                GLES32.glGetAttachedShaders(shaderProgramObject_sphere, numAttachedShaders, retVal, 0, shaderObjects,
                        0);

                for (int i = 0; i < numAttachedShaders; i++) {
                    GLES32.glDetachShader(shaderProgramObject_sphere, shaderObjects[i]);
                    GLES32.glDeleteShader(shaderObjects[i]);
                    shaderObjects[i] = 0;
                }
            }

            GLES32.glDeleteProgram(shaderProgramObject_sphere);
            shaderProgramObject = 0;

            GLES32.glUseProgram(0);
        }

    }

}
