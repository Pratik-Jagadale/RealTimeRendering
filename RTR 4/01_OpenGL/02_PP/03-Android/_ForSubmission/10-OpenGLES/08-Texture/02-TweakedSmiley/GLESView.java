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

/* Texture Related Imports */
import android.graphics.BitmapFactory;
import android.graphics.Bitmap;
import android.opengl.GLUtils;

// Matric Package
import android.opengl.Matrix;

public class GLESView extends GLSurfaceView implements OnDoubleTapListener, OnGestureListener, GLSurfaceView.Renderer {

    // Properties
    private GestureDetector gestureDetector;
    private Context context;
    private int shaderProgramObject;

    private int vao_square[] = new int[1];
    private int vbo_Square_Position[] = new int[1];
    private int vbo_TexCoord_Square[] = new int[1];

    private int mvpMatrixUniform;
    private int textureSamplerUniform;
    private int keyPressUniform;

    private float perspectiveProjectionMatrix[] = new float[16];

    private int texture_smiley[] = new int[1];

    private int singleTap = 0;

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
        singleTap++;
        if (singleTap > 4)
            singleTap = 0;
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
                        "uniform int u_singleTap;" +
                        "out vec4 FragColor;" +
                        "void main(void)" +
                        "{" +
                        "if(u_singleTap == 1)" +
                        "{" +
                        "FragColor = texture(u_textureSampler , a_texcoord_out);" +
                        "}" +
                        "else" +
                        "{" +
                        "FragColor = vec4(1.0,1.0,1.0,1.0);" +
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
        keyPressUniform = GLES32.glGetUniformLocation(shaderProgramObject, "u_singleTap");

        final float SquarePosition[] = new float[] {
                1.0f, 1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f };

        // vao_square
        GLES32.glGenVertexArrays(1, vao_square, 0);
        GLES32.glBindVertexArray(vao_square[0]);

        GLES32.glGenBuffers(1, vbo_Square_Position, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_Square_Position[0]);

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(SquarePosition.length * 4); // 4 = float size
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer positionBuffer_Square = byteBuffer.asFloatBuffer();
        positionBuffer_Square.put(SquarePosition);
        positionBuffer_Square.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, SquarePosition.length * 4, positionBuffer_Square,
                GLES32.GL_STATIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_POSITION, 3, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_POSITION);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glGenBuffers(1, vbo_TexCoord_Square, 0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_TexCoord_Square[0]);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, 4 * 2 * 4, null,
                GLES32.GL_DYNAMIC_DRAW);
        GLES32.glVertexAttribPointer(MyGLESMacros.PRJ_ATRIBUTE_TEXTURE0, 2, GLES32.GL_FLOAT, false, 0, 0);
        GLES32.glEnableVertexAttribArray(MyGLESMacros.PRJ_ATRIBUTE_TEXTURE0);
        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, 0);

        GLES32.glBindVertexArray(0);

        // Depth Related Changes
        GLES32.glClearDepthf(1.0f); // removed because not working in pp
        GLES32.glEnable(GLES32.GL_DEPTH_TEST);
        GLES32.glDepthFunc(GLES32.GL_LEQUAL);
        GLES32.glEnable(GLES32.GL_CULL_FACE);

        // textur Related Code
        GLES32.glEnable(GLES32.GL_TEXTURE_2D);

        texture_smiley[0] = loadGLTexture(R.raw.smiley);

        GLES32.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        // Initialization of Project MNatrix
        Matrix.setIdentityM(perspectiveProjectionMatrix, 0);

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

        Matrix.translateM(modelViewMatrix, 0, 0.0f, 0.0f, -4.0f);

        Matrix.multiplyMM(modelViewProjectMatrix, 0, perspectiveProjectionMatrix, 0, modelViewMatrix, 0);

        GLES32.glUniformMatrix4fv(mvpMatrixUniform, 1, false, modelViewProjectMatrix, 0);

        // texture related
        GLES32.glActiveTexture(GLES32.GL_TEXTURE0); //
        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, texture_smiley[0]);
        GLES32.glUniform1i(textureSamplerUniform, 0); //

        GLES32.glBindVertexArray(vao_square[0]);

        float textCoord[] = new float[8];
        if (singleTap == 1) {
            textCoord[0] = 0.5f;
            textCoord[1] = 0.5f;
            textCoord[2] = 0.0f;
            textCoord[3] = 0.5f;
            textCoord[4] = 0.0f;
            textCoord[5] = 1.0f;
            textCoord[6] = 0.5f;
            textCoord[7] = 1.0f;

            GLES32.glUniform1i(keyPressUniform, 1);
        } else if (singleTap == 2) {
            textCoord[0] = 1.0f;
            textCoord[1] = 0.0f;
            textCoord[2] = 0.0f;
            textCoord[3] = 0.0f;
            textCoord[4] = 0.0f;
            textCoord[5] = 1.0f;
            textCoord[6] = 1.0f;
            textCoord[7] = 1.0f;

            GLES32.glUniform1i(keyPressUniform, 1);
        } else if (singleTap == 3) {
            textCoord[0] = 0.0f;
            textCoord[1] = 0.0f;
            textCoord[2] = 2.0f;
            textCoord[3] = 0.0f;
            textCoord[4] = 2.0f;
            textCoord[5] = 2.0f;
            textCoord[6] = 0.0f;
            textCoord[7] = 2.0f;

            GLES32.glUniform1i(keyPressUniform, 1);
        } else if (singleTap == 4) {
            textCoord[0] = 0.5f;
            textCoord[1] = 0.5f;
            textCoord[2] = 0.5f;
            textCoord[3] = 0.5f;
            textCoord[4] = 0.5f;
            textCoord[5] = 0.5f;
            textCoord[6] = 0.5f;
            textCoord[7] = 0.5f;

            GLES32.glUniform1i(keyPressUniform, 1);
        } else {
            GLES32.glUniform1i(keyPressUniform, 0);
        }

        GLES32.glBindBuffer(GLES32.GL_ARRAY_BUFFER, vbo_TexCoord_Square[0]);

        ByteBuffer byteBuffer = ByteBuffer.allocateDirect(textCoord.length * 4); // 4 = float size
        byteBuffer.order(ByteOrder.nativeOrder());
        FloatBuffer TexCoordBuffer_Square = byteBuffer.asFloatBuffer();
        TexCoordBuffer_Square.put(textCoord);
        TexCoordBuffer_Square.position(0);

        GLES32.glBufferData(GLES32.GL_ARRAY_BUFFER, textCoord.length * 4, TexCoordBuffer_Square,
                GLES32.GL_DYNAMIC_DRAW);

        GLES32.glDrawArrays(GLES32.GL_TRIANGLE_FAN, 0, 4);

        GLES32.glBindVertexArray(0);

        GLES32.glBindTexture(GLES32.GL_TEXTURE_2D, 0);

        GLES32.glUseProgram(0);
        requestRender();
    }

    private void uninitialize() {
        // code
        if (texture_smiley[0] > 0) {
            GLES32.glDeleteTextures(1, texture_smiley, 0);
            texture_smiley[0] = 0;
        }

        if (vbo_TexCoord_Square[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_TexCoord_Square, 0);
            vbo_TexCoord_Square[0] = 0;
        }

        if (vbo_Square_Position[0] > 0) {
            GLES32.glDeleteBuffers(1, vbo_Square_Position, 0);
            vbo_Square_Position[0] = 0;
        }

        if (vao_square[0] > 0) {
            GLES32.glDeleteVertexArrays(1, vao_square, 0);
            vao_square[0] = 0;
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
